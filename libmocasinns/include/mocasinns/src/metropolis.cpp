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

#include <iterator>
#include <cmath>

// Includes for boost accumulators
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

namespace ba = boost::accumulators;

#include "../details/metropolis/vector_accumulator.hpp"
#include "../exceptions/iterator_range_exception.hpp"

/*! \fn AUTO_TEMPLATE_2
  \tparam Observator \concept{Observator} If no observator is given, a default observator which measures the energy is used.
  \tparam TemperatureType \concept{InverseTemperatureType}
  \param beta Inverse temperature at which the simulation is performed.
  \returns Vector containing the single measurements performed
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator, bool rejection_free>
template<class Observator, class TemperatureType>
std::vector<typename Observator::observable_type> Mocasinns::Metropolis<ConfigurationType, Step, RandomNumberGenerator, rejection_free>::do_metropolis_simulation(const TemperatureType& beta)
{
  // Check the concept of the observator
  BOOST_CONCEPT_ASSERT((Concepts::ObservatorConcept<Observator,ConfigurationType>));
  // Check the concept of the observable
  BOOST_CONCEPT_ASSERT((Concepts::ObservableConcept<typename Observator::observable_type>));

  // Call the accumulator function using the VectorAccumulator
  Details::Metropolis::VectorAccumulator<typename Observator::observable_type> measurements_accumulator;
  do_metropolis_simulation<Observator>(beta, measurements_accumulator);

  // Return the plain data
  return measurements_accumulator.internal_vector;
}

/*! \fn AUTO_TEMPLATE_2 
  \tparam Observator \concept{Observator} If no observator is given, a default observator which measures the energy is used.
  \tparam InputIterator \conceptiterator{InverseTemperatureType}
  \param first_beta Iterator pointing to the first inverse temperature that is calculated
  \param last_beta Iterator pointing on position after the last inverse temperature that is calculated
  \returns Vector containing the vectors of measurments performed for each temperature. (First index: inverse temperature, second index: measurment number)
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator, bool rejection_free>
template<class Observator, class InputIterator>
std::vector<std::vector<typename Observator::observable_type> > Mocasinns::Metropolis<ConfigurationType, Step, RandomNumberGenerator, rejection_free>::do_metropolis_simulation(InputIterator first_beta, InputIterator last_beta)
{
  // Check the concept of the observator
  BOOST_CONCEPT_ASSERT((Concepts::ObservatorConcept<Observator,ConfigurationType>));
  // Check the concept of the observable
  BOOST_CONCEPT_ASSERT((Concepts::ObservableConcept<typename Observator::observable_type>));

  std::vector<std::vector<typename Observator::observable_type> > results;
  for (InputIterator beta = first_beta; beta != last_beta; ++beta)
  {
    results.push_back(do_metropolis_simulation(*beta));
    if (this->is_terminating) break;
  }
}  

/*! \fn AUTO_TEMPLATE_2
  \tparam Observator \concept{Observator} If no observator is given, a default observator which measures the energy is used.
  \tparam TemperatureType \concept{InverseTemperatureType}
  \tparam Accumulator \concept{Accumulator}
  \param beta Inverse temperature at which the simulation is performed
  \param measurement_accumulator Reference to the accumulator that stores the simulation results
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator, bool rejection_free>
template<class Observator, class Accumulator, class TemperatureType>
void Mocasinns::Metropolis<ConfigurationType,Step,RandomNumberGenerator,rejection_free>::do_metropolis_simulation(const TemperatureType& beta, Accumulator& measurement_accumulator)
{
  // Check the concept of the observator
  BOOST_CONCEPT_ASSERT((Concepts::ObservatorConcept<Observator,ConfigurationType>));
  // Check the concept of the observable
  BOOST_CONCEPT_ASSERT((Concepts::ObservableConcept<typename Observator::observable_type>));
  // Check the concept of the accumulator
  BOOST_CONCEPT_ASSERT((Concepts::AccumulatorConcept<Accumulator, typename Observator::observable_type>));

 // Perform the relaxation steps
  do_metropolis_steps(simulation_parameters.relaxation_steps, beta);
  
  // For each measurement, perform the steps, invoke the signal handler, take the measurement and check for posix signals
  for (unsigned int m = 0; m < simulation_parameters.measurement_number; ++m)
  {
    // Do the steps
    do_metropolis_steps(simulation_parameters.steps_between_measurement, beta);

    // If the right number of measurments was performed, call the signal handler
    if (m / simulation_parameters.measurement_number == 0)
      signal_handler_measurement(this);

    // Observe and check for posix signals
    measurement_accumulator(Observator::observe(this->configuration_space));
    if (this->check_for_posix_signal()) return;
  }
}

/*! \fn AUTO_TEMPLATE_2
  \tparam Observator \concept{Observator} If no observator is given, a default observator which measures the energy is used.
  \tparam AccumulatorIterator \conceptiterator{Accumulator}
  \tparam InverseTemperatureIterator \conceptiterator{InverseTemperatureType}
  \param beta_begin Iterator pointing to the first inverse temperature that is calculated.
  \param beta_end Iterator pointing on position after the last inverse temperature that is calculated.
  \param measurement_accumulator_begin Iterator pointing to the first accumulator that gathers the data for the first inverse temperature.
  \param measurement_accumulator_end Iterator pointing one position after the last accumulator that gathers the data for the last inverse temperature. The number of accumulators must match the number of inverse temperatures.
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator, bool rejection_free>
template<class Observator, class AccumulatorIterator, class InverseTemperatureIterator>
void Mocasinns::Metropolis<ConfigurationType,Step,RandomNumberGenerator,rejection_free>::do_metropolis_simulation(InverseTemperatureIterator beta_begin, InverseTemperatureIterator beta_end, AccumulatorIterator measurement_accumulator_begin, AccumulatorIterator measurement_accumulator_end)
{  
  // Check the concept of the observator
  BOOST_CONCEPT_ASSERT((Concepts::ObservatorConcept<Observator,ConfigurationType>));
  // Check the concept of the observable
  BOOST_CONCEPT_ASSERT((Concepts::ObservableConcept<typename Observator::observable_type>));
  // Check the concept of the accumulator
  BOOST_CONCEPT_ASSERT((Concepts::AccumulatorConcept<typename std::iterator_traits<AccumulatorIterator>::value_type, typename Observator::observable_type>));

  // Check that the number of inverse temperatures and the number of accumulators matches
  if (std::distance(beta_begin, beta_end) != std::distance(measurement_accumulator_begin, measurement_accumulator_end))
    throw Exceptions::IteratorRangeException("The range of given inverse temperatures and accumulators must have the same size.")

  InverseTemperatureIterator beta_iterator = beta_begin;
  AccumulatorIterator measurement_accumulator_iterator = measurement_accumulator_begin;
  for (; beta_iterator != beta_end; ++beta_iterator, ++measurement_accumulator_iterator)
  {
    do_metropolis_simulation(*beta_iterator, *measurement_accumulator_iterator);
    if (this->is_terminating) break;
  }
}

#endif
