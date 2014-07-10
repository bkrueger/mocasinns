#ifndef MOCASINNS_DETAILS_PARALLEL_TEMPERING_INVERSE_TEMPERATURE_OPTIMIZATION_EQUAL_ACCEPTANCE_PROBABILITIES_HPP
#define MOCASINNS_DETAILS_PARALLEL_TEMPERING_INVERSE_TEMPERATURE_OPTIMIZATION_EQUAL_ACCEPTANCE_PROBABILITIES_HPP

#include "inverse_temperature_optimization_base.hpp"

namespace Mocasinns
{
  namespace Details
  {
    namespace ParallelTempering
    {
      //! Structure for use in inverse temperature optimization algorithms for parallel tempering. Weights only the last range of inverse temperatures obtained in the optimization.
      struct WeightOnlyLast
      {
	//! Static function to calculate the weights.
	static std::vector<double> calculate_weights(const std::vector< std::vector<double> >& acceptance_probabilities_optimization)
	{
	  std::vector<double> weights(acceptance_probabilities_optimization.size(), 0.0);
	  weights[weights.size() - 1] = 1.0;
	  return weights;
	}
      };

      //! Structure for use in inverse temperature optimization algorithms for parallel tempering. Weights the inverse temperatures ranges according to the worst acceptance probability obtained in the optimization.
      struct WeightWorstAcceptance
      {
	//! Static function to calculate the weights.
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

      //! Structure for use in inverse temperature optimization algorithms for parallel tempering. Weights the inverse temperatures ranges statistically correct with the assumption that the acceptance probabilities are independent.
      struct WeightIndependentAcceptance
      {	
	//! Static function to calculate the weights.
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

      //! Class for optimizing the inverse temperature so that the acceptance probabilities for temperature exchange steps is approximatly equal.
      /*!
       * \details The optimization procedure starts with an user-provided initial inverse temperature range \f$\beta_0^{(0)}, \dots, \beta_{n-1}^{(0)}\f$.
       * Each step \f$(m)\f$ consists of measuring the the ratio \f$a_i^{(m)}\f$ of executed temperature exchanges \f$\beta_{i-1}^{(m)} \leftrightarrow \beta_i^{(m)}\f$ and updating the inverse temperature according to
       * \f[
       *   \begin{split}
       * 	   \beta_0^{(m+1)} &= \beta_0^{(m)} \\
       * 	   \beta _i^{(m+1)} &= \beta_{i-1}^{(m+1)} + \lambda^{(m)}a_i^{(m)}\left(\beta_{i}^{(m)} - \beta_{i-1}^{(m)}\right)
       * 	 \end{split}
       * \f]
       *
       * The resulting temperatures are then weighted according to
       * \f[
       *   \beta_i = \sum{k = 1}^m w^{(k)} \beta_i^{(k)}
       * \f]
       * where \f$ w^{(k)} \f$ are weights that depend on the acceptance probabilities of the single optimization runs \f$(k)\f$, whose sum is 1.
       *
       * There are the following three possibilities for calculating the weights provided by the library, which can be selected by providing the right class for the template parameter \c WeightCalculator:
       * - \c WeightOnlyLast: Just use the last range of inverse temperatures obtained in the optimization procedures. This equals choosing \f$ w^{(k)} = \delta_{k,m}\f$.
       * - \c WeightWorstAcceptance: Weight each temperature range obtained in the simulation with the worst acceptance rate obtained with this temperature range. This means choosing \f$w^{(k)} = \mathrm{min}_i a_i^{(k)}\f$.
       * - \c WeightIndependentAcceptance: Weight each temperature range statistically correct using the assumption that the acceptance probabilities are independent. This means choosing \f$w^{(k)} \left[ \sum_{i=1}^n (a_i^{(k)})^2 \right]^{-1/2}\f$.
       *
       * \tparam ParallelTemperingType Type of the Parallel Tempering simulation for which the inverse temperatures should be updated.
       * \tparam InverseTemperatureType Type of the inverse temperatures
       * \tparam WeightCalculator Structure defining a static function calculate_weights taking the measured acceptance probabilities for calculating the weights of the inverse temperatures obtained in the simulation. The default behaviour is to use only the last range of inverse temperatures obtained in the optimization.
       */
      template <class ParallelTemperingType, class InverseTemperatureType, class WeightCalculator = WeightOnlyLast>
      class InverseTemperatureOptimizationEqualAcceptanceProbabilities : public InverseTemperatureOptimizationBase<ParallelTemperingType, InverseTemperatureType>
      {
      public:
	typedef InverseTemperatureOptimizationBase<ParallelTemperingType, InverseTemperatureType> Base;
	typedef typename Base::ReplicaExchangeLog ReplicaExchangeLog;
	typedef typename Base::InverseTemperatureHistogram InverseTemperatureHistogram;
	
	//! Constructor for the optimization class
	/*!
	 * \param optimize_simulation A parallel tempering simulation that is used for the optimization. The optimization works on the configurations provided by the simulation.
	 * \param opt_steps Number of optimization iterations to perform
	 */
	InverseTemperatureOptimizationEqualAcceptanceProbabilities(ParallelTemperingType* optimize_simulation, unsigned int opt_steps)
	  : Base(optimize_simulation, opt_steps) { }

	//! Function for actually optimizing the inverse temperatures
	/*!
	 * \details The resulting inverse temperatures are stored again in the given range, so you must not use constant iterators as parameters.
	 * \tparam InverseTemperatureIterator Iterator for a conatiner of inverse temperatures of type InverseTemperatureType
	 * \param inverse_temperatures_begin Begin of the inverse temperature range to optimize
	 * \param inverse_temperatures_end End of the inverse temperature range to optimize
	 */
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
