#ifndef MOCASINNS_DETAILS_PARALLEL_TEMPERING_INVERSE_TEMPERATURE_OPTIMIZATION_KATZGRABER_HPP
#define MOCASINNS_DETAILS_PARALLEL_TEMPERING_INVERSE_TEMPERATURE_OPTIMIZATION_KATZGRABER_HPP

#include "inverse_temperature_optimization_base.hpp"

namespace Mocasinns
{
  namespace Details
  {
    namespace ParallelTempering
    {
      template <class ParallelTemperingType, class InverseTemperatureType>
      class InverseTemperatureOptimizationKatzgraber : public InverseTemperatureOptimizationKatzgraberBase<ParallelTemperingType, InverseTemperatureType>
      {
      public:
	typedef InverseTemperatureOptimizationKatzgraberBase<ParallelTemperingType, InverseTemperatureType> Base;
	typedef typename Base::ReplicaExchangeLog ReplicaExchangeLog;
	typedef typename Base::InverseTemperatureHistogram InverseTemperatureHistogram;
	
	InverseTemperatureOptimizationKatzgraber(ParallelTemperingType* optimize_simulation, unsigned int opt_steps) 
	  : Base(optimize_simulation, opt_steps) { }

	//! Function for optimizing the inverse temperatures, can be used in the signal handlers.
	//! Uses the method suggested by H.G. Katzgraber et al. (2006)
	template <class InverseTemperatureIterator>
	void optimize(InverseTemperatureIterator inverse_temperatures_begin,
		      InverseTemperatureIterator inverse_temperatures_end)
	{
	  // Initialise with the given temperature range
	  this->initialize_inverse_temperatures(inverse_temperatures_begin, inverse_temperatures_end);

	  // Do the number of given recursions
	  for (unsigned int r = 0; r < this->optimization_steps; ++r)
	  {
	    optimize_step();
	    std::cout << "Recursion: " << r << std::endl;
	    for (unsigned int b = 0; b < replica_number; ++b)
	      std::cout << inverse_temperatures.back()[b] << std::endl;
	  }
	
	  std::copy(inverse_temperatures.back().begin(), inverse_temperatures.back().end(), inverse_temperatures_begin);
	}
	
	
      protected:
	//! Does a single temperature optimization step using the method suggested by H.G. Katzgraber et al. (2006)
	void optimize_step()
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
      };
    }
  }
}

#endif
