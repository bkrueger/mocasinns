/*!
 * \file parallel_tempering.cpp
 * \brief Implementation of the libMoCaSinns parallel tempering template interface
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_PARALLEL_TEMPERING_HPP

#include <cassert>
#include <iterator>
#include <omp.h>

namespace Mocasinns
{
/*!
    \tparam ConfigurationPointerIterator Iterator of a range of pointers to configurations to work on
    \param params Parameters of the simulation
    \param configuration_pointers_begin Begin of the range of pointers to configurations to work on
    \param configuration_pointers_end End of the range of pointers to configurations to work on
  */    
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class ConfigurationPointerIterator>
  ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::ParallelTempering(const Parameters& params, ConfigurationPointerIterator configuration_pointers_begin, ConfigurationPointerIterator configuration_pointers_end) 
    : simulation_parameters(params),
      replica_exchange_direction(),
      replica_exchange_log_rejected(std::distance(configuration_pointers_begin, configuration_pointers_end) - 1, 0),
      replica_exchange_log_executed(std::distance(configuration_pointers_begin, configuration_pointers_end) - 1, 0),
      inverse_temperature_histogram_up(std::distance(configuration_pointers_begin, configuration_pointers_end), 0),
      inverse_temperature_histogram_down(std::distance(configuration_pointers_begin, configuration_pointers_end), 0)
  {
    // Go through all configurations, store the pointers and set up a Metropolis simulation
    for (ConfigurationPointerIterator configuration_pointers_it = configuration_pointers_begin;
	 configuration_pointers_it != configuration_pointers_end; ++configuration_pointers_it)
    {
      metropolis_simulations.push_back(MetropolisType(params, *configuration_pointers_it));
    }
  }
  
  /*!
    \details This function calls the more general function with variable number of steps per inverse temperature. The range of inverse temperatures must have the same size as the given configurations, otherwise an error occurs.
    \tparam TemperatureTypeIterator Iterator to a range of inverse temperatures. It must be possible to calculate an inner product of an inverse temperature and the energy of the system.
    \param number Number of Metropolis steps that will be performed for all inverse temperatures
    \param inverse_temperatures_begin Begin of the range of inverse temperatures
    \param inverse_temperatures_end End of the range of inverse temperatures.
  */
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class TemperatureTypeIterator>
  void ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::
  do_parallel_tempering_steps(const StepNumberType& number, 
			      TemperatureTypeIterator inverse_temperatures_begin, 
			      TemperatureTypeIterator inverse_temperatures_end)
  {  
    // Check the range of temperatures
    check_temperature_range(inverse_temperatures_begin, inverse_temperatures_end);
    
    // Create a vector and call the generalized function
    std::vector<StepNumberType> numbers(metropolis_simulations.size(), number);
    do_parallel_tempering_steps(numbers.begin(), numbers.end(), inverse_temperatures_begin, inverse_temperatures_end);
  }
  /*!
    \details This function calls the Metropolis::do_metropolis_steps function for all instances and temperatures. The range of inverse temperatures and the range of step numbers must have the same size as the given configurations, otherwise an error occurs.
    \tparam NumberIterator Iterator to a range of numbers of steps to be performed for the corresponding temperature.
    \tparam TemperatureTypeIterator Iterator to a range of inverse temperatures. It must be possible to calculate an inner product of an inverse temperature and the energy of the system.
    \param number Number of Metropolis steps that will be performed for all inverse temperatures
    \param inverse_temperatures_begin Begin of the range of inverse temperatures
    \param inverse_temperatures_end End of the range of inverse temperatures.
  */
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class NumberIterator, class TemperatureTypeIterator>
  void ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::
  do_parallel_tempering_steps(NumberIterator numbers_begin, 
			      NumberIterator numbers_end, 
			      TemperatureTypeIterator inverse_temperatures_begin, 
			      TemperatureTypeIterator inverse_temperatures_end)
  {  
    // Check the range of temperatures
    check_temperature_range(inverse_temperatures_begin, inverse_temperatures_end);

    // Fill a vector with the numbers and the inverse temperatures
    // Using the iterators can cause problems in the parallelization of the code
    typedef typename std::iterator_traits<NumberIterator>::value_type NumberType;
    typedef typename std::iterator_traits<TemperatureTypeIterator>::value_type TemperatureType;
    std::vector<NumberType> numbers(numbers_begin, numbers_end);
    std::vector<TemperatureType> inverse_temperatures(inverse_temperatures_begin, inverse_temperatures_end);
    
    // Do for each temperatures the Metropolis steps
    omp_set_num_threads(simulation_parameters.process_number);
#pragma omp parallel for
    for (unsigned int i = 0; i < metropolis_simulations.size(); ++i)
    {
      metropolis_simulations[i].do_metropolis_steps(numbers[i], inverse_temperatures[i]);
    }
  }

  /*!
    \details Two neighbouring temperatures are selected randomly and the acceptance probability of an temperature exchange is calculated (the index of the lower inverse temperature is the return value). The exchange is executed with this acceptance probability
    \tparam TemperatureTypeIterator Iterator to a range of inverse temperatures. It must be possible to calculate an inner product of an inverse temperature and the energy of the system.
    \param inverse_temperatures_begin Begin of the range of inverse temperatures
    \param inverse_temperatures_end End of the range of inverse temperatures.
    \returns 0 if the exchange was rejected and i if there was an exchange between the configuration at (i-1) and the configuration at i.
   */
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class TemperatureTypeIterator>
  unsigned int ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::do_replica_exchange(TemperatureTypeIterator inverse_temperatures_begin, 
													  TemperatureTypeIterator inverse_temperatures_end)
  {
    // Check the range of temperatures
    check_temperature_range(inverse_temperatures_begin, inverse_temperatures_end);

    // Select randomly the index of the temperatures to exchange
    unsigned int exchange_index = this->rng->random_uint32(1, metropolis_simulations.size() - 1);

    // Get the temperatures of the exchange indices
    TemperatureTypeIterator inverse_temperature_iterator_1 = inverse_temperatures_begin;
    std::advance(inverse_temperature_iterator_1, exchange_index - 1);
    TemperatureTypeIterator inverse_temperature_iterator_2 = inverse_temperatures_begin;
    std::advance(inverse_temperature_iterator_2, exchange_index);

    // Calculate the acceptance probability
    double acceptance_probability = exp((*inverse_temperature_iterator_1 - *inverse_temperature_iterator_2) * 
  					(metropolis_simulations[exchange_index - 1].get_config_space()->energy() - metropolis_simulations[exchange_index].get_config_space()->energy()));

    // Write the inverse temperature log
    for (unsigned int b = 0; b < metropolis_simulations.size(); ++b)
    {
      typename std::map<ConfigurationType*, bool>::const_iterator direction = replica_exchange_direction.find(metropolis_simulations[b].get_config_space());
      if (direction != replica_exchange_direction.end())
      {
	if (direction->second == false)
	  inverse_temperature_histogram_up[b] += 1;
	else
	  inverse_temperature_histogram_down[b] += 1;
      }
    }

    // Do the step with the acceptance probability
    if (this->rng->random_double() < acceptance_probability)
    {
      ConfigurationType* temp(metropolis_simulations[exchange_index - 1].get_config_space());

      // Write the replica exchange log
      replica_exchange_log_executed[exchange_index - 1] += 1;

      // Test whether the direction of replicas has changed
      if (exchange_index == 1)
	replica_exchange_direction[metropolis_simulations[exchange_index].get_config_space()] = false;
      else if (exchange_index == metropolis_simulations.size() - 1)
	replica_exchange_direction[metropolis_simulations[exchange_index - 1].get_config_space()] = true;

      // Exchange the configurations in the metropolis simulations
      metropolis_simulations[exchange_index - 1].set_config_space(metropolis_simulations[exchange_index].get_config_space());
      metropolis_simulations[exchange_index].set_config_space(temp);

      return exchange_index;
    }
    else
    {
      // Write the replica exchange logs for index (exchange_index - 1)
      replica_exchange_log_rejected[exchange_index - 1] += 1;

      return 0;
    }
  }

  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class Observator, class TemperatureTypeIterator>
  std::vector<std::vector<typename Observator::observable_type> > 
  ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::
  do_parallel_tempering_simulation(TemperatureTypeIterator inverse_temperatures_begin, 
				   TemperatureTypeIterator inverse_temperatures_end)
  {
    // Check the range of temperatures
    check_temperature_range(inverse_temperatures_begin, inverse_temperatures_end);

    // Typedef for the accumulator
    typedef Details::Metropolis::VectorAccumulator<typename Observator::observable_type> VectorAccumulatorType;

    // Call the accumulator function using the VectorAccumulator
    VectorAccumulatorType measurements_accumulator;
    std::vector<VectorAccumulatorType> result_accumulators = do_parallel_tempering_simulation<Observator>(measurements_accumulator, inverse_temperatures_begin, inverse_temperatures_end);
    
    // Construct a vector of the plain data and return it
    std::vector< std::vector<typename Observator::observable_type> > result;
    for (unsigned int i = 0; i < result_accumulators.size(); ++i)
      result.push_back(result_accumulators[i].internal_vector);
    return result;
  }

  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class Observator, class Accumulator, class TemperatureTypeIterator>
  std::vector<Accumulator> ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::
  do_parallel_tempering_simulation(const Accumulator& measurement_accumulator,
				   TemperatureTypeIterator inverse_temperatures_begin, 
				   TemperatureTypeIterator inverse_temperatures_end)
  {
    // Check the range of temperatures
    check_temperature_range(inverse_temperatures_begin, inverse_temperatures_end);

    // Copy the accumulators and call the overloaded function
    std::vector<Accumulator> accumulators(metropolis_simulations.size(), measurement_accumulator);
    do_parallel_tempering_simulation(accumulators.begin(), accumulators.end(), inverse_temperatures_begin, inverse_temperatures_end);
    return accumulators;
  }

  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class Observator, class AccumulatorIterator, class TemperatureTypeIterator>
  void ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::
  do_parallel_tempering_simulation(AccumulatorIterator measurement_accumulators_begin, 
				   AccumulatorIterator measurement_accumulators_end,
				   TemperatureTypeIterator inverse_temperatures_begin, 
				   TemperatureTypeIterator inverse_temperatures_end)
  {
    // Check the range of temperatures
    check_temperature_range(inverse_temperatures_begin, inverse_temperatures_end);
    // Assert that the size of the measurment accumulators matches the size of the inverse temperatures
    assert(std::distance(measurement_accumulators_begin, measurement_accumulators_end) == 
	   std::distance(inverse_temperatures_begin, inverse_temperatures_end));

    // Do the relaxation steps
    do_parallel_tempering_steps(simulation_parameters.relaxation_steps, inverse_temperatures_begin, inverse_temperatures_end);

    // For each measurement, perform the steps, invoke the signal handler, take the measurement and check for posix signals
    for (unsigned int m = 0; m < simulation_parameters.measurement_number; ++m)
    {
      // Accumulate into the accumulators (Measure)
      AccumulatorIterator measurement_accumulator_it = measurement_accumulators_begin;
      for (unsigned int i = 0; i < metropolis_simulations.size(); ++i)
      {
  	(*measurement_accumulator_it)(Observator::observe(metropolis_simulations[i].get_config_space()));
  	measurement_accumulator_it++;
      }
      // Call the measurement handler
      signal_handler_measurement(this);

      // Calculate the number of replica exchanges
      unsigned int replica_exchange_number = simulation_parameters.steps_between_measurement / simulation_parameters.steps_between_replica_exchange;

      // Do the replica exchanges and the steps
      for (unsigned int r = 0; r < replica_exchange_number; ++r)
      {
	// Replica exchange and signal handler
  	do_replica_exchange(inverse_temperatures_begin, inverse_temperatures_end);
	signal_handler_replica_exchange(this);

	// Do the relaxation steps afterwards
  	do_parallel_tempering_steps(simulation_parameters.steps_between_replica_exchange, inverse_temperatures_begin, inverse_temperatures_end);
      }

      // Check for POSIX
      if (this->check_for_posix_signal()) return;
    }
  }

  template <class ConfigurationType, class Step, class RandomNumberGenerator>
  void ParallelTempering<ConfigurationType, Step, RandomNumberGenerator>::replica_exchanges_reset()
  {
    for (unsigned int i = 0; i < metropolis_simulations.size() - 1; ++i)
    {
      replica_exchange_log_executed[i] = 0;
      replica_exchange_log_rejected[i] = 0;
    }
  }

  template <class ConfigurationType, class Step, class RandomNumberGenerator>
  void ParallelTempering<ConfigurationType, Step, RandomNumberGenerator>::inverse_temperature_histograms_reset()
  {
    for (unsigned int i = 0; i < metropolis_simulations.size(); ++i)
    {
      inverse_temperature_histogram_up[i] = 0;
      inverse_temperature_histogram_down[i] = 0;
    }
  }
  
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class TemperatureTypeIterator>
  void ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::check_temperature_range(TemperatureTypeIterator inverse_temperatures_begin, 
												    TemperatureTypeIterator inverse_temperatures_end)
  {
    assert(std::distance(inverse_temperatures_begin, inverse_temperatures_end) == static_cast<int>(metropolis_simulations.size()));
  }
}
#endif
