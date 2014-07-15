#ifndef MOCASINNS_ANALYSIS_AUTOCORRELATION_HPP
#define MOCASINNS_ANALYSIS_AUTOCORRELATION_HPP

// STL Headers
#include <vector>
#include <cassert>
#include <algorithm>

// Boost accumulator
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>

#include "analysis_helper.hpp"

namespace ba = boost::accumulators;

namespace Mocasinns
{
  namespace Analysis
  {
    //! Class for calculating autocorrelation functions of observables
    /*!      
      \tparam Observable \concept{ObservableType}
    */
    template <class Observable>
    class Autocorrelation
    {
    public:
      /*! Static function to calculate the normalized autocorrelation function of a range of observables 
	\details The unnormalized autocorrelation \f$ C(t) \f$ function of the observables \f$ x_i \f$ with \f$ 1 \leq 0 \leq N \f$ is defined as
	\f[
	  C(t) := \frac{1}{N - t}\sum{i = 0}^N x_i x_{i+t} - \overline{x}^2
	\f]
	The returned normalized autocorrelation function \f$ c(t) \f$ is defined as \f$ c(t) := C(t) / \mathrm{Var}(x)\f$
	\tparam ObservableIterator Iterator over the range of observables to take into account
	\param observables_begin Iterator to the begin of the observable range
	\param observables_end Iterator to the end of the observable range
	\returns Vector of observables, at index t the normalized autocorrelation function at time t is stored
      */
      template <class ObservableIterator>
      static std::vector<Observable> autocorrelation_function(ObservableIterator observables_begin, 
							      ObservableIterator observables_end)
      {
	unsigned int observables_size = std::distance(observables_begin, observables_end);
	std::vector<Observable> result(observables_size);

	// Define an accumulator for calculating the mean
	ba::accumulator_set<Observable, ba::stats<ba::tag::mean, ba::tag::variance> > mean_variance_accumulator;
	ObservableIterator mean_variance_iterator = observables_begin;

	// Calculate the autocorrelation functions for the single times
	for (unsigned int t = 0; t < observables_size; ++t)
	{
	  // Accumulator for accumulating means for this time
	  ba::accumulator_set<Observable, ba::stats<ba::tag::mean> > time_accumulator;
	  
	  // Define iterators in distance t
	  ObservableIterator time_observables_it_1 = observables_begin;
	  ObservableIterator time_observables_it_2 = observables_begin; std::advance(time_observables_it_2, t);
	  // Iterate and accumulate
	  for (; time_observables_it_2 != observables_end; ++time_observables_it_1, ++time_observables_it_2)
	    time_accumulator((*time_observables_it_1) * (*time_observables_it_2));
	  
	  // Store the correlation in results
	  result[t] = ba::mean(time_accumulator);

	  // Accumulate the mean
	  mean_variance_accumulator(*mean_variance_iterator);
	  ++mean_variance_iterator;
	}

	// Calculate the mean and the veriance
	Observable observables_mean = ba::mean(mean_variance_accumulator);
	Observable observables_variance = ba::variance(mean_variance_accumulator);

	// Normalize the autocorrelation function
	for (unsigned int t = 0; t < observables_size; ++t)
	  result[t] = (result[t] - observables_mean*observables_mean) / observables_variance;

	// Return the result
	return result;
      }

      /*! Static function to calculate the normalized autocorrelation function of a range of observables taken in several independent runs.
	\details If the different runs have different length, the autocorrelation function is only calculated from 0 to the minimal length of all given runs.
	\tparam ObservableIterator Iterator over the range of observables to take into account
	\param vector_observables_begin Vector of iterators to the begin of the independent observable ranges
	\param vector_observables_end Vector of iterators to the end of the independent observable ranges. Must have the same length as vector_observables_begin
	\returns Pair of two vector of observables, the first entry stores at index t the mean, the second entry stores at index t the standard error of the normalized autocorrelation function at time t.
      */
      template <class ObservableIterator>
      static std::pair<std::vector<Observable>, std::vector<Observable> > autocorrelation_function(std::vector<ObservableIterator> vector_observables_begin, 
												   std::vector<ObservableIterator> vector_observables_end)
      {
	// Get the number of independent observations
	assert((vector_observables_begin.size() == vector_observables_end.size()) && (vector_observables_begin.size() != 0));
	unsigned int independent_observations_number = vector_observables_begin.size();

	// Create a vector storing all autocorrelation functions
	std::vector<std::vector<Observable> > autocorrelation_functions(independent_observations_number);
	for (unsigned int i = 0; i < independent_observations_number; ++i)
	  autocorrelation_functions[i] = autocorrelation_function(vector_observables_begin[i], vector_observables_end[i]);

	// Get the maximal time
	typename std::vector<Observable>::size_type maximal_time = autocorrelation_functions[0].size();
	for (unsigned int i = 1; i < independent_observations_number; ++i)
	  maximal_time = std::min(maximal_time, autocorrelation_functions[i].size());

	// Create the result
	std::pair<std::vector<Observable>, std::vector<Observable> > result(std::vector<Observable>(maximal_time, 0.0), std::vector<Observable>(maximal_time, 0.0));

	// Go through the times and calculate the mean and the error of the autocorrelation functions
	for (unsigned int t = 0; t < maximal_time; ++t)
	{
	  // Accumulate for mean and variance
	  ba::accumulator_set<Observable, ba::stats<ba::tag::mean, ba::tag::variance> > mean_variance_accumulator;
	  for (unsigned int i = 0; i < independent_observations_number; ++i)
	    mean_variance_accumulator(autocorrelation_functions[i][t]);

	  // Extract mean and standard error
	  (result.first)[t] = ba::mean(mean_variance_accumulator);
	  (result.second)[t] = sqrt(ba::variance(mean_variance_accumulator) / (independent_observations_number - 1)) ;
	}

	return result;
      }
    };
  }
}

#endif
