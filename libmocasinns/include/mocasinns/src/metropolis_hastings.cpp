/*!
 * \file metropolis_hastings.cpp
 * \brief Implementation of the Metropolis-Hastings algorithm for sampling with different probability distributions
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_METROPOLIS_HASTINGS_HPP

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

namespace Mocasinns
{
 
/*!
  \tparam Observator Class with static function Observator::observe(ConfigurationType*) taking a pointer to the simulation and returning the value of a arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
  \tparam AcceptanceProbabilityFunctor Class that defines "double operator()(EnergyType delta_E, EnergyType actual_energy)" returning the acceptance probability of a step with energy difference delta_e and actual_energy being the energy of the state before the step. If \f$P(E(\omega))\f$ is the probability distribution to be sampled, the class must return 
  \f[
    A(\omega_1 \rightarrow \omega_2) = \frac{P(E(\omega_1) + \Delta E(\omega_1 \rightarrow \omega_2)}{P(E(\omega_1)}
  \f]
  \param acceptance_probability_functor Object of type AcceptanceProbabilityFunctor that is used to calculate the acceptance probability.
  \returns Vector containing the single measurements performed
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class Observator, class AcceptanceProbabilityFunctor>
std::vector<typename Observator::observable_type> MetropolisHastings<ConfigurationType, Step, RandomNumberGenerator>::do_metropolis_hastings_simulation(const AcceptanceProbabilityFunctor& acceptance_probability_functor)
{
  // Check the concept of the observator
  BOOST_CONCEPT_ASSERT((Concepts::ObservatorConcept<Observator,ConfigurationType>));
  // Check the concept of the observable
  BOOST_CONCEPT_ASSERT((Concepts::ObservableConcept<typename Observator::observable_type>));

  // Call the accumulator function using the VectorAccumulator
  Details::Metropolis::VectorAccumulator<typename Observator::observable_type> measurements_accumulator;
  do_metropolis_hastings_simulation<Observator>(acceptance_probability_functor, measurements_accumulator);

  // Return the plain data
  return measurements_accumulator.internal_vector;
}

/*!
 \tparam Observator Class with static function Observator::observe(ConfigurationType*) taking a pointer to the simulation and returning the value of an arbitrary observable. The class must contain a typedef ::observable_type classifying the return type of the functor.
 \tparam Accumulator Class that accepts the observable in operator() and gathers the required informations about the observables (e.g. boost::accumulator)
 \param acceptance_probability_functor Class object that defines a "double operator(EnergyType delta_E, EnergyType actual_energy)" returning the acceptance probability of the step.
 \param measurement_accumulator Reference to the accumulator that stores the simulation results
*/
template<class ConfigurationType, class Step, class RandomNumberGenerator>
template<class Observator, class Accumulator, class AcceptanceProbabilityFunctor>
void MetropolisHastings<ConfigurationType,Step,RandomNumberGenerator>::do_metropolis_hastings_simulation(const AcceptanceProbabilityFunctor& acceptance_probability_functor, Accumulator& measurement_accumulator)
{
  // Check the concept of the observator
  BOOST_CONCEPT_ASSERT((Concepts::ObservatorConcept<Observator,ConfigurationType>));
  // Check the concept of the observable
  BOOST_CONCEPT_ASSERT((Concepts::ObservableConcept<typename Observator::observable_type>));
  // Check the concept of the accumulator
  BOOST_CONCEPT_ASSERT((Concepts::AccumulatorConcept<Accumulator, typename Observator::observable_type>));

  // Create the Acceptance probability parameter
  typedef BOOST_TYPEOF(&ConfigurationType::energy) energy_function_type;
  typedef typename boost::function_types::result_type<energy_function_type>::type EnergyType;
  AcceptanceProbabilityParameters<EnergyType, AcceptanceProbabilityFunctor> acceptance_probability_parameters;
  acceptance_probability_parameters.actual_energy = this->configuration_space->energy();
  acceptance_probability_parameters.acceptance_probability_functor = acceptance_probability_functor;

  // Perform the relaxation steps
  do_metropolis_hastings_steps(simulation_parameters.relaxation_steps, acceptance_probability_parameters);
  
  // For each measurement, perform the steps, invoke the signal handler, take the measurement and check for posix signals
  for (unsigned int m = 0; m < simulation_parameters.measurement_number; ++m)
  {
    // Do the steps
    do_metropolis_hastings_steps(simulation_parameters.steps_between_measurement, acceptance_probability_parameters);

    // If the right number of measurments was performed, call the signal handler
    if (m / simulation_parameters.measurement_number == 0)
      signal_handler_measurement(this);

    // Observe and check for posix signals
    measurement_accumulator(Observator::observe(this->configuration_space));
    if (this->check_for_posix_signal()) return;
  }
}

} // of namespace Mocasinns

#endif
