/*!
 * \file metropolis_parallel.cpp
 * \brief Implementation of the libMoCaSinns Parallel Metropolis template interface
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_METROPOLIS_PARALLEL_HPP

#include <cmath>
#include <omp.h>

namespace Mocasinns
{

/*!
  \tparam Observable Class with static function Observable::observe(ConfigurationType*) taking a pointer to the simulation and returning the value of a arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
  \tparam TemperatureType Type of the inverse temperature, there must be an operator* defined this class and the energy type of the configuration.
  \param beta Inverse temperature at which the simulation is performed.
  \returns Vector containing the single measurements performed
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class Observable, class TemperatureType>
std::vector<typename Observable::observable_type> MetropolisParallel<ConfigurationType, Step, RandomNumberGenerator>::do_parallel_metropolis_simulation(const TemperatureType& beta)
{
  // Call the accumulator function using the VectorAccumulator
  Details::Metropolis::VectorAccumulator<typename Observable::observable_type> measurements_accumulator;
  do_parallel_metropolis_simulation<Observable>(beta, measurements_accumulator);

  // Return the plain data
  return measurements_accumulator.internal_vector;
}

/*!  
  \tparam Observable Class with static function Observable::observe(ConfigurationType*) taking a pointer to the simulation and returning the value of a arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
  \tparam InputIterator Type of the iterator that iterates the different temperatures that will be considered
  \tparam TemperatureType Type of the inverse temperature, there must be an operator* defined this class and the energy type of the configuration.
  \param beta_begin Iterator pointing to the first inverse temperature that is calculated
  \param beta_end Iterator pointing on position after the last inverse temperature that is calculated
  \returns Vector containing the vectors of measurments performed for each temperature. (First index: inverse temperature, second index: measurment number)
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class Observable, class InputIterator>
std::vector<std::vector<typename Observable::observable_type> > MetropolisParallel<ConfigurationType, Step, RandomNumberGenerator>::do_parallel_metropolis_simulation(InputIterator first_beta, InputIterator last_beta)
{
  std::vector<std::vector<typename Observable::observable_type> > results;
  for (InputIterator beta = first_beta; beta != last_beta; ++beta)
  {
    results.push_back(do_parallel_metropolis_simulation(*beta));
    if (this->is_terminating) break;
  }
}  

/*!
 \tparam Observable Class with static function Observable::observe(ConfigurationType*) taking a pointer to the simulation and returning the value of an arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
 \tparam Accumulator Class that accepts the observable in operator() and gathers the required informations about the observables (e.g. boost::accumulator)
 \tparam TemperatureType Type of the inverse temperature, there must be an operator* defined this class and the energy type of the configuration.
 \param beta Inverse temperature at which the simulation is performed
 \param measurement_accumulator Reference to the accumulator that stores the simulation results
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class Observable, class Accumulator, class TemperatureType>
void MetropolisParallel<ConfigurationType,Step,RandomNumberGenerator>::do_parallel_metropolis_simulation(const TemperatureType& beta, Accumulator& measurement_accumulator)
{
  // Create a metropolis-simulation for each run
  std::vector<Metropolis<ConfigurationType, Step, RandomNumberGenerator>* > run_simulations;
  for (unsigned int run = 0; run < simulation_parameters.run_number; ++run)
  {
    ConfigurationType* copied_configuration = new ConfigurationType(*(this->get_config_space()));
    run_simulations.push_back(new Metropolis<ConfigurationType, Step, RandomNumberGenerator>(simulation_parameters, copied_configuration));
    run_simulations[run]->set_random_seed(this->get_random_seed() + run);
  }
  
  // Perform a parallel for-loop for the different runs
  // The signal handlers and the simulation parameters need not to be shared, because class members are allways shared
#pragma omp parallel for shared(run_simulations) shared(measurement_accumulator) shared(beta)
  for (unsigned int run = 0; run < simulation_parameters.run_number; ++run)
  {
    // Perform the relaxation steps
    run_simulations[run]->do_metropolis_steps(simulation_parameters.relaxation_steps, beta);

    // For each measurement, perform the steps, invoke the signal handler, take the measurement and check for posix signals
    for (unsigned int m = 0; m < simulation_parameters.measurement_number && !this->check_for_posix_signal(); ++m)
    {
      run_simulations[run]->do_metropolis_steps(simulation_parameters.steps_between_measurement, beta);

 #pragma omp critical
      {
	signal_handler_measurement(this);
	measurement_accumulator(Observable::observe(run_simulations[run]->get_config_space()));
      }
    }
    
    // Call the signal handler for run finishing
#pragma omp critical
    {
      if (!this->is_terminating)
	signal_handler_run(this);
    }
  }

  // Delete the created configurations and simulations
  for (unsigned int run = 0; run < simulation_parameters.run_number; ++run)
  {
    delete run_simulations[run]->get_config_space();
    delete run_simulations[run];
  }
}

/*!
 \tparam Observable Class with static function Observable::observe(ConfigurationType*) taking a pointer to the simulation and returning the value of an arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
 \tparam AccumulatorIterator Iterator of a container of a class that accepts the observable in operator() and gathers the required informations about the observables (e.g. boost::accumulator)
 \tparam InverseTemperatureIterator  Iterator of a container of a type of the inverse temperature, there must be an operator* defined this class and the energy type of the configuration.
 \param beta_begin Iterator pointing to the first inverse temperature that is calculated
 \param beta_end Iterator pointing on position after the last inverse temperature that is calculated
 \param measurement_accumulator_begin Iterator pointing to the first accumulator that calculates the data for the first inverse temperature.
 \param measurement_accumulator_end Iterator pointing one position after the last accumulator that calculates the data for the last inverse temperature
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class Observable, class AccumulatorIterator, class InverseTemperatureIterator>
void MetropolisParallel<ConfigurationType,Step,RandomNumberGenerator>::do_parallel_metropolis_simulation(InverseTemperatureIterator beta_begin, InverseTemperatureIterator beta_end, AccumulatorIterator measurement_accumulator_begin, AccumulatorIterator measurement_accumulator_end)
{  
  InverseTemperatureIterator beta_iterator = beta_begin;
  AccumulatorIterator measurement_accumulator_iterator = measurement_accumulator_begin;
  for (; beta_iterator != beta_end; ++beta_iterator, ++measurement_accumulator_iterator)
  {
    do_parallel_metropolis_simulation(*beta_iterator, *measurement_accumulator_iterator);
    if (this->is_terminating) break;
  }
}

template <class ConfigurationType, class Step, class RandomNumberGenerator>
void MetropolisParallel<ConfigurationType, Step, RandomNumberGenerator>::load_serialize(std::istream& input_stream)
{
  boost::archive::text_iarchive input_archive(input_stream);
  input_archive >> (*this);
}
template <class ConfigurationType, class Step, class RandomNumberGenerator>
void MetropolisParallel<ConfigurationType, Step, RandomNumberGenerator>::load_serialize(const char* filename)
{
  std::ifstream input_filestream(filename);
  load_serialize(input_filestream);
  input_filestream.close();
}
template <class ConfigurationType, class Step, class RandomNumberGenerator>
void MetropolisParallel<ConfigurationType, Step, RandomNumberGenerator>::save_serialize(std::ostream& output_stream) const
{
  boost::archive::text_oarchive output_archive(output_stream);
  output_archive << (*this);
}
template <class ConfigurationType, class Step, class RandomNumberGenerator>
void MetropolisParallel<ConfigurationType, Step, RandomNumberGenerator>::save_serialize(const char* filename) const
{
  std::ofstream output_filestream(filename);
  save_serialize(output_filestream);
  output_filestream.close();
}

} // of namespace Mocasinns

#endif
