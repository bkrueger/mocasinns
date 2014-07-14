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
  \tparam Observator Class with static function Observator::observe(ConfigurationType*) taking a pointer to the simulation and returning the value of a arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
  \tparam TemperatureType Type of the inverse temperature, there must be an operator* defined this class and the energy type of the configuration.
  \param beta Inverse temperature at which the simulation is performed.
  \returns Vector containing the single measurements performed
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class Observator, class TemperatureType>
std::vector<typename Observator::observable_type> MetropolisParallel<ConfigurationType, Step, RandomNumberGenerator>::do_parallel_metropolis_simulation(const TemperatureType& beta)
{
  // Check the concept of the observator
  BOOST_CONCEPT_ASSERT((Concepts::ObservatorConcept<Observator,ConfigurationType>));
  // Check the concept of the observable
  BOOST_CONCEPT_ASSERT((Concepts::ObservableConcept<typename Observator::observable_type>));

  // Call the accumulator function using the VectorAccumulator
  Details::Metropolis::VectorAccumulator<typename Observator::observable_type> measurements_accumulator;
  do_parallel_metropolis_simulation<Observator>(beta, measurements_accumulator);

  // Return the plain data
  return measurements_accumulator.internal_vector;
}

/*!  
  \tparam Observator Class with static function Observator::observe(ConfigurationType*) taking a pointer to the simulation and returning the value of a arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
  \tparam InputIterator Type of the iterator that iterates the different temperatures that will be considered
  \tparam TemperatureType Type of the inverse temperature, there must be an operator* defined this class and the energy type of the configuration.
  \param beta_begin Iterator pointing to the first inverse temperature that is calculated
  \param beta_end Iterator pointing on position after the last inverse temperature that is calculated
  \returns Vector containing the vectors of measurments performed for each temperature. (First index: inverse temperature, second index: measurment number)
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class Observator, class InputIterator>
std::vector<std::vector<typename Observator::observable_type> > MetropolisParallel<ConfigurationType, Step, RandomNumberGenerator>::do_parallel_metropolis_simulation(InputIterator first_beta, InputIterator last_beta)
{
  // Check the concept of the observator
  BOOST_CONCEPT_ASSERT((Concepts::ObservatorConcept<Observator,ConfigurationType>));
  // Check the concept of the observable
  BOOST_CONCEPT_ASSERT((Concepts::ObservableConcept<typename Observator::observable_type>));

  std::vector<std::vector<typename Observator::observable_type> > results;
  for (InputIterator beta = first_beta; beta != last_beta; ++beta)
  {
    results.push_back(do_parallel_metropolis_simulation(*beta));
    if (this->is_terminating) break;
  }
}  

/*!
 \tparam Observator Class with static function Observator::observe(ConfigurationType*) taking a pointer to the simulation and returning the value of an arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
 \tparam Accumulator Class that accepts the observable in operator() and gathers the required informations about the observables (e.g. boost::accumulator)
 \tparam TemperatureType Type of the inverse temperature, there must be an operator* defined this class and the energy type of the configuration.
 \param beta Inverse temperature at which the simulation is performed
 \param measurement_accumulator Reference to the accumulator that stores the simulation results
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class Observator, class Accumulator, class TemperatureType>
void MetropolisParallel<ConfigurationType,Step,RandomNumberGenerator>::do_parallel_metropolis_simulation(const TemperatureType& beta, Accumulator& measurement_accumulator)
{
  // Check the concept of the observator
  BOOST_CONCEPT_ASSERT((Concepts::ObservatorConcept<Observator,ConfigurationType>));
  // Check the concept of the observable
  BOOST_CONCEPT_ASSERT((Concepts::ObservableConcept<typename Observator::observable_type>));
  // Check the concept of the accumulator
  BOOST_CONCEPT_ASSERT((Concepts::AccumulatorConcept<Accumulator, typename Observator::observable_type>));

  // Perform a parallel for-loop for the different runs
  // The signal handlers and the simulation parameters need not to be shared, because class members are allways shared
  omp_set_num_threads(simulation_parameters.process_number);
#pragma omp parallel for shared(measurement_accumulator) shared(beta)
  for (unsigned int run = 0; run < simulation_parameters.run_number; ++run)
  {
    // Forward declare a pointer for the configuration and the simulation
    ConfigurationType* copied_configuration;
    Metropolis<ConfigurationType, Step, RandomNumberGenerator>* run_simulation;
#pragma omp critical
    {
      // Copy a configuration from the initial one
      copied_configuration = new ConfigurationType(*(this->get_config_space()));
      // Create the new configuration
      run_simulation = new Metropolis<ConfigurationType, Step, RandomNumberGenerator>(simulation_parameters, copied_configuration);
    }
    
    // Set the seed of the simulation to the seed of this simulation plus the run number
    run_simulation->set_random_seed(this->get_random_seed() + run);

    // Perform the relaxation steps
    run_simulation->do_metropolis_steps(simulation_parameters.relaxation_steps, beta);

    // For each measurement, perform the steps, invoke the signal handler, take the measurement and check for posix signals
    for (unsigned int m = 0; m < simulation_parameters.measurement_number && !this->check_for_posix_signal(); ++m)
    {
      run_simulation->do_metropolis_steps(simulation_parameters.steps_between_measurement, beta);

 #pragma omp critical
      {
	signal_handler_measurement(this);
	measurement_accumulator(Observator::observe(run_simulation->get_config_space()));
      }
    }
    
#pragma omp critical
    {
      // Call the signal handler for run finishing
      if (!this->is_terminating)
	signal_handler_run(this);

      // Delete the created configuration and the simulation
      delete run_simulation->get_config_space();
      delete run_simulation;
    }
  }
}

/*!
 \tparam Observator Class with static function Observator::observe(ConfigurationType*) taking a pointer to the simulation and returning the value of an arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
 \tparam AccumulatorIterator Iterator of a container of a class that accepts the observable in operator() and gathers the required informations about the observables (e.g. boost::accumulator)
 \tparam InverseTemperatureIterator  Iterator of a container of a type of the inverse temperature, there must be an operator* defined this class and the energy type of the configuration.
 \param beta_begin Iterator pointing to the first inverse temperature that is calculated
 \param beta_end Iterator pointing on position after the last inverse temperature that is calculated
 \param measurement_accumulator_begin Iterator pointing to the first accumulator that calculates the data for the first inverse temperature.
 \param measurement_accumulator_end Iterator pointing one position after the last accumulator that calculates the data for the last inverse temperature
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class Observator, class AccumulatorIterator, class InverseTemperatureIterator>
void MetropolisParallel<ConfigurationType,Step,RandomNumberGenerator>::do_parallel_metropolis_simulation(InverseTemperatureIterator beta_begin, InverseTemperatureIterator beta_end, AccumulatorIterator measurement_accumulator_begin, AccumulatorIterator measurement_accumulator_end)
{  
  // Check the concept of the observator
  BOOST_CONCEPT_ASSERT((Concepts::ObservatorConcept<Observator,ConfigurationType>));
  // Check the concept of the observable
  BOOST_CONCEPT_ASSERT((Concepts::ObservableConcept<typename Observator::observable_type>));  
  // Check the concept of the accumulator
  BOOST_CONCEPT_ASSERT((Concepts::AccumulatorConcept<typename std::iterator_traits<AccumulatorIterator>::value_type, typename Observator::observable_type>));

  InverseTemperatureIterator beta_iterator = beta_begin;
  AccumulatorIterator measurement_accumulator_iterator = measurement_accumulator_begin;
  for (; beta_iterator != beta_end; ++beta_iterator, ++measurement_accumulator_iterator)
  {
    do_parallel_metropolis_simulation<Observator>(*beta_iterator, *measurement_accumulator_iterator);
    if (this->is_terminating) break;
  }
}

} // of namespace Mocasinns

#endif
