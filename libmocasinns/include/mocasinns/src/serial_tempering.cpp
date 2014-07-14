/*!
 * \file parallel_tempering.cpp
 * \brief Implementation of the libMoCaSinns parallel tempering template interface
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_SERIAL_TEMPERING_HPP

#include <cassert>
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
  SerialTempering<ConfigurationType, StepType, RandomNumberGenerator>::SerialTempering(const Parameters& params, ConfigurationPointerIterator configuration_pointers_begin, ConfigurationPointerIterator configuration_pointers_end) : simulation_parameters(params)
  {
    // Go through all configurations, store the pointers and set up a Metropolis simulation
    for (ConfigurationPointerIterator configuration_pointers_it = configuration_pointers_begin;
	 configuration_pointers_it != configuration_pointers_end; ++configuration_pointers_it)
    {
      configuration_pointers.push_back(*configuration_pointers_it);
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
  void SerialTempering<ConfigurationType, StepType, RandomNumberGenerator>::do_serial_tempering_steps(const StepNumberType& number, 
  												      TemperatureTypeIterator inverse_temperatures_begin, 
  												      TemperatureTypeIterator inverse_temperatures_end)
  {  
    // Check the range of temperatures
    check_temperature_range(inverse_temperatures_begin, inverse_temperatures_end);
    
    // Create a vector and call the generalized function
    std::vector<StepNumberType> numbers(configuration_pointers.size(), number);
    do_serial_tempering_steps(numbers.begin(), numbers.end(), inverse_temperatures_begin, inverse_temperatures_end);
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
  void SerialTempering<ConfigurationType, StepType, RandomNumberGenerator>::do_serial_tempering_steps(NumberIterator numbers_begin, 
  												      NumberIterator, 
  												      TemperatureTypeIterator inverse_temperatures_begin, 
  												      TemperatureTypeIterator inverse_temperatures_end)
  {  
    // Check the range of temperatures
    check_temperature_range(inverse_temperatures_begin, inverse_temperatures_end);
    
    // Do for each temperatures the Metropolis steps
    NumberIterator number_it = numbers_begin;
    TemperatureTypeIterator inverse_temperature_it = inverse_temperatures_begin;
    for (unsigned int i = 0; i < configuration_pointers.size(); ++i)
    {
      metropolis_simulations[i].do_metropolis_steps(*number_it, *inverse_temperature_it);
      number_it++;
      inverse_temperature_it++;
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
  unsigned int SerialTempering<ConfigurationType, StepType, RandomNumberGenerator>::do_replica_exchange(TemperatureTypeIterator inverse_temperatures_begin, 
  												       TemperatureTypeIterator inverse_temperatures_end)
  {
    // Check the range of temperatures
    check_temperature_range(inverse_temperatures_begin, inverse_temperatures_end);

    // Select randomly the index of the temperatures to exchange
    unsigned int exchange_index = this->rng->random_int32(1, configuration_pointers.size() - 1);

    // Get the temperatures of the exchange indices
    TemperatureTypeIterator inverse_temperature_iterator_1 = inverse_temperatures_begin;
    std::advance(inverse_temperature_iterator_1, exchange_index - 1);
    TemperatureTypeIterator inverse_temperature_iterator_2 = inverse_temperatures_begin;
    std::advance(inverse_temperature_iterator_2, exchange_index);

    // Calculate the acceptance probability
    double acceptance_probability = exp((*inverse_temperature_iterator_1 - *inverse_temperature_iterator_2) * 
  					(configuration_pointers[exchange_index - 1]->energy() - configuration_pointers[exchange_index]->energy()));

    // Do the step with the acceptance probability
    if (this->rng->random_double() < acceptance_probability)
    {
      ConfigurationType* temp(configuration_pointers[exchange_index - 1]);
      // Exchange the configuration pointers
      configuration_pointers[exchange_index - 1] = configuration_pointers[exchange_index];
      configuration_pointers[exchange_index] = temp;
      // Exchange the configurations in the metropolis simulations
      metropolis_simulations[exchange_index - 1].set_config_space(configuration_pointers[exchange_index - 1]);
      metropolis_simulations[exchange_index].set_config_space(configuration_pointers[exchange_index]);
      replica_exchange_log[exchange_index] += 1;
      return exchange_index;
    }
    else
    {
      replica_exchange_log[0] += 1;
      return 0;
    }
  }

  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class Observator, class TemperatureTypeIterator>
  std::vector<std::vector<typename Observator::observable_type> > 
  SerialTempering<ConfigurationType, StepType, RandomNumberGenerator>::do_serial_tempering_simulation(TemperatureTypeIterator inverse_temperatures_begin, 
  												      TemperatureTypeIterator inverse_temperatures_end)
  {
    // Check the range of temperatures
    check_temperature_range(inverse_temperatures_begin, inverse_temperatures_end);

    // Typedef for the accumulator
    typedef Details::Metropolis::VectorAccumulator<typename Observator::observable_type> VectorAccumulatorType;

    // Call the accumulator function using the VectorAccumulator
    VectorAccumulatorType measurements_accumulator;
    std::vector<VectorAccumulatorType> result_accumulators = do_serial_tempering_simulation<Observator>(measurements_accumulator, inverse_temperatures_begin, inverse_temperatures_end);
    
    // Construct a vector of the plain data and return it
    std::vector< std::vector<typename Observator::observable_type> > result;
    for (unsigned int i = 0; i < result_accumulators.size(); ++i)
      result.push_back(result_accumulators[i].internal_vector);
    return result;
  }

  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class Observator, class Accumulator, class TemperatureTypeIterator>
  std::vector<Accumulator> SerialTempering<ConfigurationType, StepType, RandomNumberGenerator>::do_serial_tempering_simulation(const Accumulator& measurement_accumulator,
  															       TemperatureTypeIterator inverse_temperatures_begin, 
  															       TemperatureTypeIterator inverse_temperatures_end)
  {
    // Check the range of temperatures
    check_temperature_range(inverse_temperatures_begin, inverse_temperatures_end);

    // Copy the accumulators and call the overloaded function
    std::vector<Accumulator> accumulators(configuration_pointers.size(), measurement_accumulator);
    do_serial_tempering_simulation(accumulators.begin(), accumulators.end(), inverse_temperatures_begin, inverse_temperatures_end);
    return accumulators;
  }

  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class Observator, class AccumulatorIterator, class TemperatureTypeIterator>
  void SerialTempering<ConfigurationType, StepType, RandomNumberGenerator>::do_serial_tempering_simulation(AccumulatorIterator measurement_accumulators_begin, 
													   AccumulatorIterator measurement_accumulators_end,
													   TemperatureTypeIterator inverse_temperatures_begin, 
													   TemperatureTypeIterator inverse_temperatures_end)
  {
    // Log the start of the simulation
    this->simulation_start_log();

    // Check the range of temperatures
    check_temperature_range(inverse_temperatures_begin, inverse_temperatures_end);
    // Assert that the size of the measurment accumulators matches the size of the inverse temperatures
    assert(std::distance(measurement_accumulators_begin, measurement_accumulators_end) == 
	   std::distance(inverse_temperatures_begin, inverse_temperatures_end));
    // Set up the replica exchange log
    replica_exchange_log = std::vector<unsigned int>(configuration_pointers.size(), 0);

    // Do the relaxation steps
    do_serial_tempering_steps(simulation_parameters.relaxation_steps, inverse_temperatures_begin, inverse_temperatures_end);

    // For each measurement, perform the steps, invoke the signal handler, take the measurement and check for posix signals
    for (unsigned int m = 0; m < simulation_parameters.measurement_number; ++m)
    {
      // Call the measurement handler
      signal_handler_measurement(this);

      // Calculate the number of replica exchanges
      unsigned int replica_exchange_number = simulation_parameters.steps_between_measurement / simulation_parameters.steps_between_replica_exchange;

      // Do the steps and the replica exchanges
      for (unsigned int r = 0; r < replica_exchange_number - 1; ++r)
      {
  	do_serial_tempering_steps(simulation_parameters.steps_between_replica_exchange, inverse_temperatures_begin, inverse_temperatures_end);
  	do_replica_exchange(inverse_temperatures_begin, inverse_temperatures_end);
      }

      // Do the last number of steps
      do_serial_tempering_steps(simulation_parameters.steps_between_replica_exchange, inverse_temperatures_begin, inverse_temperatures_end);
      // Accumulate into the accumulators
      AccumulatorIterator measurement_accumulator_it = measurement_accumulators_begin;
      for (unsigned int i = 0; i < configuration_pointers.size(); ++i)
      {
  	(*measurement_accumulator_it)(Observator::observe(configuration_pointers[i]));
  	measurement_accumulator_it++;
      }
      // Do the replica exchange afterwards
      do_replica_exchange(inverse_temperatures_begin, inverse_temperatures_end);

      // Check for POSIX
      if (this->check_for_posix_signal()) return;
    }
  }
  
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class TemperatureTypeIterator>
  void SerialTempering<ConfigurationType, StepType, RandomNumberGenerator>::check_temperature_range(TemperatureTypeIterator inverse_temperatures_begin, 
												    TemperatureTypeIterator inverse_temperatures_end)
  {
    assert(std::distance(inverse_temperatures_begin, inverse_temperatures_end) == static_cast<int>(configuration_pointers.size()));
  }

} // of namespace Mocasinns

#endif
