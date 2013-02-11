/*!
 * \file metropolis.cpp
 * \brief Implementation of the libMoCaSinns template interface
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Johannes F. Knauf
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_METROPOLIS_HPP

#include <cmath>

#include "details/metropolis/vector_accumulator.hpp"

namespace Mocasinns
{

/*!
 * \param num_steps Number of Metropolis steps that will be performed
 * \param beta Inverse temperature that will be used for calculation of the acceptance probability of the Metropolis steps.
 */
template <class ConfigurationType, class Step, class RandomNumberGenerator>
template <class TemperatureType>
void Metropolis<ConfigurationType, Step, RandomNumberGenerator>::do_metropolis_steps(const uint32_t& num_steps, const TemperatureType& beta = 0)
{
  for (uint32_t i = 0; i < num_steps; i++)
  {
    Step next_step = this->configuration_space->propose_step(this->rng);

    if (next_step.is_executable())
    {
      // One does need to define this in advance, because the Temperature Type need not to be comparable to an double 0.0
      double beta_times_delta_E = beta*next_step.delta_E();
      double selection_probability_factor = next_step.selection_probability_factor();
      double random_accept = this->rng->random_double();
      if (beta_times_delta_E <= -log(selection_probability_factor) || random_accept < (1.0/selection_probability_factor)*exp(-beta_times_delta_E))
      {
	next_step.execute();
      }
    }
  }
}

/*!
  \tparam Observable Class with static function Observable::observe(ConfigurationType*) taking a pointer to the simulation and returning the value of a arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
  \tparam TemperatureType Type of the inverse temperature, there must be an operator* defined this class and the energy type of the configuration.
  \param parameters Parameters to use for the simulation
  \param beta Inverse temperature at which the simulation is performed.
  \returns Vector containing the single measurements performed
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class Observable, class TemperatureType>
std::vector<typename Observable::observable_type> Metropolis<ConfigurationType, Step, RandomNumberGenerator>::do_metropolis_simulation(const Parameters& parameters, const TemperatureType& beta)
{
  // Call the accumulator function using the VectorAccumulator
  Details::Metropolis::VectorAccumulator<typename Observable::observable_type> measurements_accumulator;
  do_metropolis_simulation<Observable>(parameters, beta, measurements_accumulator);

  // Return the plain data
  return measurements_accumulator.internal_vector;
}

/*!  
  \tparam Observable Class with static function Observable::observe(ConfigurationType*) taking a pointer to the simulation and returning the value of a arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
  \tparam InputIterator Type of the iterator that iterates the different temperatures that will be considered
  \tparam TemperatureType Type of the inverse temperature, there must be an operator* defined this class and the energy type of the configuration.
  \param parameters Parameters to use for the simulation
  \param beta_begin Iterator pointing to the first inverse temperature that is calculated
  \param beta_end Iterator pointing on position after the last inverse temperature that is calculated
  \returns Vector containing the vectors of measurments performed for each temperature. (First index: inverse temperature, second index: measurment number)
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class Observable, class InputIterator>
std::vector<std::vector<typename Observable::observable_type> > Metropolis<ConfigurationType, Step, RandomNumberGenerator>::do_metropolis_simulation(const Parameters& parameters, InputIterator first_beta, InputIterator last_beta)
{
  std::vector<std::vector<typename Observable::observable_type> > results;
  for (InputIterator beta = first_beta; beta != last_beta; ++beta)
  {
    results.push_back(do_metropolis_simulation(parameters, *beta));
    if (this->is_terminating) break;
  }
}  

/*!
 \tparam Observable Class with static function Observable::observe(ConfigurationType*) taking a pointer to the simulation and returning the value of an arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
 \tparam Accumulator Class that accepts the observable in operator() and gathers the required informations about the observables (e.g. boost::accumulator)
 \tparam TemperatureType Type of the inverse temperature, there must be an operator* defined this class and the energy type of the configuration.
 \param parameters Parameters to use for the simulation
 \param beta Inverse temperature at which the simulation is performed
 \param measurement_accumulator Reference to the accumulator that stores the simulation results
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class Observable, class Accumulator, class TemperatureType>
void Metropolis<ConfigurationType,Step,RandomNumberGenerator>::do_metropolis_simulation(const Parameters& parameters, const TemperatureType& beta, Accumulator& measurement_accumulator)
{
 // Perform the relaxation steps
  do_metropolis_steps(parameters.relaxation_steps, beta);
  
  // For each measurement, perform the steps, invoke the signal handler, take the measurement and check for posix signals
  for (unsigned int m = 0; m < parameters.measurement_number; ++m)
  {
    do_metropolis_steps(parameters.steps_between_measurement, beta);
    signal_handler_measurement(this);
    measurement_accumulator(Observable::observe(this->configuration_space));
    if (this->check_for_posix_signal()) return;
  }
}

/*!
 \tparam Observable Class with static function Observable::observe(ConfigurationType*) taking a pointer to the simulation and returning the value of an arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
 \tparam AccumulatorIterator Iterator of a container of a class that accepts the observable in operator() and gathers the required informations about the observables (e.g. boost::accumulator)
 \tparam InverseTemperatureIterator  Iterator of a container of a type of the inverse temperature, there must be an operator* defined this class and the energy type of the configuration.
 \param parameters Parameters to use for the simulation
 \param beta_begin Iterator pointing to the first inverse temperature that is calculated
 \param beta_end Iterator pointing on position after the last inverse temperature that is calculated
 \param measurement_accumulator_begin Iterator pointing to the first accumulator that calculates the data for the first inverse temperature.
 \param measurement_accumulator_end Iterator pointing one position after the last accumulator that calculates the data for the last inverse temperature
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class Observable, class AccumulatorIterator, class InverseTemperatureIterator>
void Metropolis<ConfigurationType,Step,RandomNumberGenerator>::do_metropolis_simulation(const Parameters& parameters, InverseTemperatureIterator beta_begin, InverseTemperatureIterator beta_end, AccumulatorIterator measurement_accumulator_begin, AccumulatorIterator measurement_accumulator_end)
{  
  InverseTemperatureIterator beta_iterator = beta_begin;
  AccumulatorIterator measurement_accumulator_iterator = measurement_accumulator_begin;
  for (; beta_iterator != beta_end; ++beta_iterator, ++measurement_accumulator_iterator)
  {
    do_metropolis_simulation(parameters, *beta_iterator, *measurement_accumulator_iterator);
    if (this->is_terminating) break;
  }
}

template <class ConfigurationType, class Step, class RandomNumberGenerator>
void Metropolis<ConfigurationType, Step, RandomNumberGenerator>::load_serialize(std::istream& input_stream)
{
  boost::archive::text_iarchive input_archive(input_stream);
  input_archive >> (*this);
}
template <class ConfigurationType, class Step, class RandomNumberGenerator>
void Metropolis<ConfigurationType, Step, RandomNumberGenerator>::load_serialize(const char* filename)
{
  std::ifstream input_filestream(filename);
  load_serialize(input_filestream);
  input_filestream.close();
}
template <class ConfigurationType, class Step, class RandomNumberGenerator>
void Metropolis<ConfigurationType, Step, RandomNumberGenerator>::save_serialize(std::ostream& output_stream) const
{
  boost::archive::text_oarchive output_archive(output_stream);
  output_archive << (*this);
}
template <class ConfigurationType, class Step, class RandomNumberGenerator>
void Metropolis<ConfigurationType, Step, RandomNumberGenerator>::save_serialize(const char* filename) const
{
  std::ofstream output_filestream(filename);
  save_serialize(output_filestream);
  output_filestream.close();
}

} // of namespace Mocasinns

#endif
