#ifndef MOCASINNS_DETAILS_PARALLEL_TEMPERING_INVERSE_TEMPERATURE_OPTIMIZATION_EQUAL_ACCEPTANCE_PROBABILITIES_HPP
#define MOCASINNS_DETAILS_PARALLEL_TEMPERING_INVERSE_TEMPERATURE_OPTIMIZATION_EQUAL_ACCEPTANCE_PROBABILITIES_HPP

#include "inverse_temperature_optimization_base.hpp"

namespace Mocasinns
{
  namespace Details
  {
    namespace ParallelTempering
    {
      struct WeightOnlyLast
      {
	static std::vector<double> calculate_weights(const std::vector< std::vector<double> >& acceptance_probabilities_optimization)
	{
	  std::vector<double> weights(acceptance_probabilities_optimization.size(), 0.0);
	  weights[weights.size() - 1] = 1.0;
	  return weights;
	}
      };

      struct WeightWorstAcceptance
      {
	static std::vector<double> calculate_weights(const std::vector< std::vector<double> >& acceptance_probabilities_optimization)
	{
	  // Abbreviation for the number of optimization steps
	  unsigned int optimization_steps = acceptance_probabilities_optimization.size();

	  // Calculate the weights
	  std::vector<double> weights(optimization_steps, 0.0);
	  double weights_sum = 0.0;
	  for (unsigned int r = 0; r < optimization_steps; ++r)
	  {
	    weights[r] = *std::min_element(acceptance_probabilities_optimization[r].begin(), acceptance_probabilities_optimization[r].end());
	    weights_sum += weights[r];
	  }
	  
	  // Normalize the weights
	  for (unsigned int r = 0; r < optimization_steps; ++r) weights[r] /= weights_sum;

	  return weights;
	}
      };

      struct WeightIndependentAcceptance
      {
	static std::vector<double> calculate_weights(const std::vector< std::vector<double> >& acceptance_probabilities_optimization)
	{
	  // Abbreviation for the number of optimization steps
	  unsigned int optimization_steps = acceptance_probabilities_optimization.size();

	  // Calculate the weights
	  std::vector<double> weights(optimization_steps, 0.0);
	  double weights_sum = 0.0;
	  for (unsigned int r = 0; r < optimization_steps; ++r)
	  {
	    double sigma_squared = 0.0;
	    for (unsigned int i = 0; i < acceptance_probabilities_optimization[r].size(); ++i)
	      sigma_squared += 1.0/pow(acceptance_probabilities_optimization[r][i], 2);
	  
	    weights[r] = 1.0 / sqrt(sigma_squared);
	    weights_sum += weights[r];
	  }
	  
	  // Normalize the weights
	  for (unsigned int r = 0; r < optimization_steps; ++r) weights[r] /= weights_sum;

	  return weights;
	}
      };

      template <class ParallelTemperingType, class InverseTemperatureType, class WeightCalculator = WeightOnlyLast>
      class InverseTemperatureOptimizationEqualAcceptanceProbabilities : public InverseTemperatureOptimizationBase<ParallelTemperingType, InverseTemperatureType>
      {
      public:
	typedef InverseTemperatureOptimizationBase<ParallelTemperingType, InverseTemperatureType> Base;
	typedef typename Base::ReplicaExchangeLog ReplicaExchangeLog;
	typedef typename Base::InverseTemperatureHistogram InverseTemperatureHistogram;
	
	InverseTemperatureOptimizationEqualAcceptanceProbabilities(ParallelTemperingType* optimize_simulation, unsigned int opt_steps)
	  : Base(optimize_simulation, opt_steps) { }

	//! Function for optimizing the inverse temperatures, can be used in the signal handlers.
	//! Uses the method suggested by B.A. Berg (2004, p. 304) without weights.
	template <class InverseTemperatureIterator>
	void optimize(InverseTemperatureIterator inverse_temperatures_begin,
		      InverseTemperatureIterator inverse_temperatures_end)
	{
	  // Initialise with the given temperature range
	  this->initialize_inverse_temperatures(inverse_temperatures_begin, inverse_temperatures_end);

	  // Do the number of given recursions
	  for (unsigned int r = 0; r < this->optimization_steps; ++r) optimize_step();

	  // Calculate the weights
	  std::vector<double> weights = WeightCalculator::calculate_weights(this->acceptance_probabilities);

	  // Write the resulting temperatures into the given iterator range
	  InverseTemperatureIterator inverse_temperature_it = inverse_temperatures_begin;
	  for (unsigned int i = 0; i < this->replica_number; ++i)
	  {
	    // Weight the inverse temperature of the actual replica
	    (*inverse_temperature_it) = 0.0;
	    for (unsigned int r = 0; r < this->optimization_steps; ++r)
	      (*inverse_temperature_it) += weights[r] * this->inverse_temperatures[r][i];

	    // Increase the iterator
	    ++inverse_temperature_it;
	  }
    	}
	

      protected:
	//! Does a single temperature optimization step using the method suggested by B.A. Berg (2004, p. 304)
	void optimize_step()
	{
	  // Find the position of the current inverse temperatures
	  unsigned int current_beta = this->inverse_temperatures.size() - 1;
	  
	  // Measure and log the acceptance probabilities
	  this->measure_acceptance_probabilities();
	  this->log_acceptance_probabilities();
	  
	  // Calculate the correction constant lambda
	  double lambda_denominator = 0.0;
	  for (unsigned int i = 0; i < (this->replica_number) - 1; ++i)
	    lambda_denominator += this->acceptance_probabilities[current_beta][i] * (this->inverse_temperatures[current_beta][i+1] - this->inverse_temperatures[current_beta][i]);
	  double lambda = (this->inverse_temperatures[current_beta].back() - this->inverse_temperatures[current_beta].front()) / lambda_denominator;
	  
	  // Update the temperatures
	  this->inverse_temperatures.push_back(std::vector<InverseTemperatureType>(this->replica_number, 0.0));
	  this->inverse_temperatures[current_beta + 1][0] = this->inverse_temperatures[current_beta][0];
	  for (unsigned int i = 1; i < this->replica_number; ++i)
	  {
	    this->inverse_temperatures[current_beta + 1][i] = this->inverse_temperatures[current_beta + 1][i - 1]
	      + lambda * this->acceptance_probabilities[current_beta][i - 1] * (this->inverse_temperatures[current_beta][i] - this->inverse_temperatures[current_beta][i - 1]);
	  }
	}
      };
    }
  }
}

#endif
