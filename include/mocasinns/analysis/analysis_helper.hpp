#ifndef MOCASINNS_ANALYSIS_ANALYSIS_HELPER_HPP
#define MOCASINNS_ANALYSIS_ANALYSIS_HELPER_HPP

// Boost accumulator
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>

namespace ba = boost::accumulators;

namespace Mocasinns
{
  namespace Analysis
  {
    //! Class providing some common helper function for the analysis classes
    template <class Observable>
    class AnalysisHelper
    {
    public: 
      //! Identity function
      static Observable identity(const Observable& x) { return x; }

      //! Function calculating the mean of an observable range
      template <class ObservableIterator>
      static Observable observable_range_mean(ObservableIterator observables_begin, ObservableIterator observables_end)
      {
	// Define a accumulator for accumulating all observables in the bin
	ba::accumulator_set<Observable, ba::stats<ba::tag::mean> > accumulator;
	
	// Accumulate
	for (ObservableIterator it = observables_begin; it != observables_end; ++it) accumulator(*it);

	// Evaluate the accumulator
	return ba::mean(accumulator);
      }

      //! Function calculating the variance of an observable range
      template <class ObservableIterator>
      static Observable observable_range_variance(ObservableIterator observables_begin, ObservableIterator observables_end)
      {
	// Define a accumulator for accumulating all observables in the bin
	ba::accumulator_set<Observable, ba::stats<ba::tag::variance> > accumulator;
	
	// Accumulate
	for (ObservableIterator it = observables_begin; it != observables_end; ++it) accumulator(*it);

	// Evaluate the accumulator
	return ba::variance(accumulator);
      }
  
      //! Function calculating the variance of an observable range
      template <class ObservableIterator>
      static Observable observable_range_standard_deviation(ObservableIterator observables_begin, ObservableIterator observables_end)
      {
	return sqrt(observable_range_variance(observables_begin, observables_end));
      }
    };
  }
}

#endif
