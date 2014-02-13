#ifdef MOCASINNS_PARALLEL_TEMPERING_HPP

namespace Mocasinns
{
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class InverseTemperatureType> 
  void ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::InverseTemperatureOptimization<InverseTemperatureType>::optimize_berg(unsigned int recursion_number)
  {
    // Do the number of given recursions
    for (unsigned int r = 0; r < recursion_number; ++r)
    {
      optimize_berg_step();
    }
    
    // Measure and log the acceptance probabilities
    measure_acceptance_probabilities();
    log_acceptance_probabilities();
  }

  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class InverseTemperatureType>
  void ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::InverseTemperatureOptimization<InverseTemperatureType>::optimize_berg_weights_worst_acceptance(unsigned int recursion_number)
  {
    optimize_berg(recursion_number);

    // Calculate the weights from the acceptance probabilities
    std::vector<double> weights(recursion_number, 0.0);
    double weights_sum = 0.0;
    for (unsigned int r = 0; r < recursion_number; ++r)
    {
      weights[r] = *std::min_element(acceptance_probabilities[r].begin(), acceptance_probabilities[r].end());
      weights_sum += weights[r];
    }

    // Write the resulting temperatures into the vector
    inverse_temperatures.push_back(std::vector<InverseTemperatureType>(replica_number, 0.0));
    for (unsigned int i = 0; i < replica_number; ++i)
    {
      for (unsigned int r = 0; r < recursion_number; ++r)
	inverse_temperatures[recursion_number][i] += weights[r] * inverse_temperatures[r][i] / weights_sum;
    }

    // Measure and log the acceptance probabilities
    measure_acceptance_probabilities();
    log_acceptance_probabilities();
  }

  template <class ConfigurationType, class StepType, class RandomNumberGenerator> 
  template <class InverseTemperatureType>
  void ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::InverseTemperatureOptimization<InverseTemperatureType>::optimize_berg_weights_independent_acceptance(unsigned int recursion_number)
  {
    optimize_berg(recursion_number);

    // Calculate the weights from the acceptance probabilities
    std::vector<double> weights(recursion_number, 0.0);
    double weights_sum = 0.0;
    for (unsigned int r = 0; r < recursion_number; ++r)
    {
      double sigma_squared = 0.0;
      for (unsigned int i = 0; i < replica_number - 1; ++i)
	sigma_squared += 1.0/pow(acceptance_probabilities[r][i], 2);

      weights[r] = 1.0 / sqrt(sigma_squared);
      weights_sum += weights[r];
    }

    // Write the resulting temperatures into the vector
    inverse_temperatures.push_back(std::vector<InverseTemperatureType>(replica_number, 0.0));
    for (unsigned int i = 0; i < replica_number; ++i)
    {
      for (unsigned int r = 0; r < recursion_number; ++r)
	inverse_temperatures[recursion_number][i] += weights[r] * inverse_temperatures[r][i] / weights_sum;
    }

    // Measure and log the acceptance probabilities
    measure_acceptance_probabilities();
    log_acceptance_probabilities();
  }

  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class InverseTemperatureType>
  void ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::InverseTemperatureOptimization<InverseTemperatureType>::optimize_katzgraber(unsigned int recursion_number)
  {
    // Do the number of given recursions
    for (unsigned int r = 0; r < recursion_number; ++r)
    {
      optimize_katzgraber_step();
      std::cout << "Recursion: " << r << std::endl;
      for (unsigned int b = 0; b < replica_number; ++b)
	std::cout << inverse_temperatures.back()[b] << std::endl;
    }
    
    // Measure and log the acceptance probabilities
    measure_acceptance_probabilities();
    log_acceptance_probabilities();
  }

  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class InverseTemperatureType>
  void ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::InverseTemperatureOptimization<InverseTemperatureType>::measure_acceptance_probabilities()
  {
    unsigned int current_beta = inverse_temperatures.size() - 1;

    while (*std::min_element(simulation->get_replica_exchanges_executed().begin(), simulation->get_replica_exchanges_executed().end()) == 0)
    {
      // Do a parallel tempering simulation
      set_parameters_optimize();
      simulation->do_parallel_tempering_simulation(inverse_temperatures[current_beta].begin(), inverse_temperatures[current_beta].end());
      unset_parameters_optimize();
    }
  }

  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class InverseTemperatureType>
  void ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::InverseTemperatureOptimization<InverseTemperatureType>::log_acceptance_probabilities()
  {
    // Log the acceptance probability
    acceptance_probabilities.push_back(std::vector<double>(replica_number - 1, 0.0));							    
    ReplicaExchangeLog replica_exchanges_executed = simulation->get_replica_exchanges_executed();
    ReplicaExchangeLog replica_exchanges_rejected = simulation->get_replica_exchanges_rejected();
    for (unsigned int r = 0; r < replica_number - 1; ++r)
    {
      acceptance_probabilities[acceptance_probabilities.size() - 1][r] = 
	static_cast<double>(replica_exchanges_executed[r]) / static_cast<double>(replica_exchanges_executed[r] + replica_exchanges_rejected[r]);
    }
    
    // Reset the replica exchanges
    simulation->replica_exchanges_reset();
  }

  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class InverseTemperatureType>
  void ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::InverseTemperatureOptimization<InverseTemperatureType>::optimize_berg_step()
  {
    // Find the position of the current inverse temperatures
    unsigned int current_beta = inverse_temperatures.size() - 1;

    // Measure and log the acceptance probabilities
    measure_acceptance_probabilities();
    log_acceptance_probabilities();
    
    // Calculate the correction constant lambda
    double lambda_denominator = 0.0;
    for (unsigned int i = 0; i < replica_number - 1; ++i)
    {
      lambda_denominator += acceptance_probabilities[current_beta][i] * (inverse_temperatures[current_beta][i+1] - inverse_temperatures[current_beta][i]);
    }
    double lambda = (inverse_temperatures[current_beta].back() - inverse_temperatures[current_beta].front()) / lambda_denominator;
    
    // Update the temperatures
    inverse_temperatures.push_back(std::vector<InverseTemperatureType>(replica_number, 0.0));
    inverse_temperatures[current_beta + 1][0] = inverse_temperatures[current_beta][0];
    for (unsigned int i = 1; i < replica_number; ++i)
    {
      inverse_temperatures[current_beta + 1][i] = inverse_temperatures[current_beta + 1][i - 1]
	+ lambda * acceptance_probabilities[current_beta][i - 1] * (inverse_temperatures[current_beta][i] - inverse_temperatures[current_beta][i - 1]);
    }
  }

  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class InverseTemperatureType>
  void ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::InverseTemperatureOptimization<InverseTemperatureType>::optimize_katzgraber_step()
  {
    // Find the position of the current inverse temperatures
    unsigned int current_beta = inverse_temperatures.size() - 1;

    // Define the derivative of the f-histogram
    std::vector<double> f_histogram_derivative(replica_number, -1.0);
      
    // Do parallel tempering simulations as long as the f_histogram_derivative has negative values
    while (*std::min_element(f_histogram_derivative.begin(), f_histogram_derivative.end()) < 0.0)
    {
      // Do a parallel tempering simulation
      set_parameters_optimize();
      simulation->do_parallel_tempering_simulation(inverse_temperatures[current_beta].begin(), inverse_temperatures[current_beta].end());
      unset_parameters_optimize();

      // Calculate the f-histogram
      const InverseTemperatureHistogram& temperature_histogram_up = simulation->get_inverse_temperature_histogram_up();
      const InverseTemperatureHistogram& temperature_histogram_down = simulation->get_inverse_temperature_histogram_down();
      std::vector<double> f_histogram(replica_number, 0.0);
      for (unsigned int b = 0; b < replica_number; ++b)
	f_histogram[b] = static_cast<double>(temperature_histogram_down[b]) / static_cast<double>(temperature_histogram_up[b] + temperature_histogram_down[b]);

      // Calculate the derivatives of the f-histogram
      f_histogram_derivative[0] = (f_histogram[1] - f_histogram[0])/(inverse_temperatures[current_beta][1] - inverse_temperatures[current_beta][0]);
      for (unsigned int b = 1; b < replica_number - 1; ++b)
	f_histogram_derivative[b] = (f_histogram[b+1] - f_histogram[b-1])/(inverse_temperatures[current_beta][b+1] - inverse_temperatures[current_beta][b-1]);
      f_histogram_derivative[replica_number - 1] = (f_histogram[replica_number - 1] - f_histogram[replica_number - 2])
	/ (inverse_temperatures[current_beta][replica_number - 1] - inverse_temperatures[current_beta][replica_number - 2]);
    }
    
    // Log the acceptance probabilities
    log_acceptance_probabilities();

    // Calculate the partial integrals
    std::vector<double> partial_integrals(replica_number - 1, 0.0);
    double partial_integrals_sum = 0.0;
    for (unsigned int b = 0; b < replica_number - 1; ++b)
    {
      double delta_beta = inverse_temperatures[current_beta][b+1] - inverse_temperatures[current_beta][b];
      double lambda = (f_histogram_derivative[b+1] - f_histogram_derivative[b]) / delta_beta;
      partial_integrals[b] = (2.0 / 3.0 * lambda * sqrt(delta_beta)) * (pow(f_histogram_derivative[b+1], 1.5) - pow(f_histogram_derivative[b], 1.5));
      partial_integrals_sum += partial_integrals[b];
    }
    double normalization_constant = 1.0 / partial_integrals_sum;
    // Sum up the partial integrals
    std::vector<double> partial_integrals_summed(replica_number, 0.0);;
    partial_integrals_summed[0] = 0.0;
    for (unsigned int b = 0; b < replica_number - 1; ++b)
      partial_integrals_summed[b+1] = partial_integrals_summed[b] + normalization_constant * partial_integrals[b];

    // Calculate the new inverse temperatures
    inverse_temperatures.push_back(std::vector<InverseTemperatureType>(replica_number, 0.0));
    inverse_temperatures[current_beta + 1][0] = inverse_temperatures[current_beta][0];
    inverse_temperatures[current_beta + 1][replica_number - 1] = inverse_temperatures[current_beta][replica_number - 1];
    for (unsigned int b_new = 1; b_new < replica_number - 1; ++b_new)
    {
      // Value at the right hand side of the integral equation
      double rhs = static_cast<double>(b_new) / static_cast<double>(replica_number - 1);
      
      // Find the old index where the partial integral is smaller then the desired value and bigger at the next index
      unsigned int b_old = -1;
      for (unsigned int b = 0; b < replica_number - 1; ++b)
      {
	if (partial_integrals_summed[b] < rhs && partial_integrals_summed[b+1] >= rhs)
	{
	  b_old = b;
	  break;
	}
      }
	
      // Use a simple linear regression to find the new temperature
      // inverse_temperatures[current_beta + 1][b_new] = inverse_temperatures[current_beta][b_old] + (inverse_temperatures[current_beta][b_old+1] - inverse_temperatures[current_beta][b_old]) * (rhs - partial_integrals_summed[b_old]) / (partial_integrals_summed[b_old+1] - partial_integrals_summed[b_old]);
   
      // Quantities in the integral
      double delta_beta = inverse_temperatures[current_beta][b_old+1] - inverse_temperatures[current_beta][b_old];
      double lambda = (f_histogram_derivative[b_old+1] - f_histogram_derivative[b_old]) / delta_beta;
      
      // Calculate the new temperature
      inverse_temperatures[current_beta + 1][b_new] = inverse_temperatures[current_beta][b_old] 
        - f_histogram_derivative[b_old] / lambda
        + (1.0 / lambda) * pow((3.0 * lambda * sqrt(delta_beta) * (rhs - partial_integrals_summed[b_old])) / (2.0 * normalization_constant) + pow(f_histogram_derivative[b_old], 1.5), 2.0/3.0);
    }
    
    // Reset the inverse temperature histograms
    simulation->inverse_temperature_histograms_reset();
  }
}
#endif
