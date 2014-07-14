#ifndef MOCASINNS_ANALYSIS_BINNING_ANALYSIS_HPP
#define MOCASINNS_ANALYSIS_BINNING_ANALYSIS_HPP

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
    //! Class for doing a binning analysis of a number of data points.
    /*!
      \details The functionality of the binning analysis is encapsulated in the static function analyse.
      The values given by an iterator range are gathered in bins of the given bin length \f$ b \f$.
      Then the averages of the single bins
      \f[ 
        \langle \mathcal O \rangle_n := \frac{1}{b} \sum{i = 0}^b \mathcal O_{n*b + i}
      \f] 
      are calculated. The average of the given function \f$ f \f$ applied to the observables is then calculated as
      \f[ 
        \overline{f} = \frac{1}{b} \sum_n f(\langle \mathcal O \rangle_n)
      \f] 
      the standard error is calculated using
      \f[ 
        \Delta \overline{f} = \sqrt{ \frac{1}{b(b-1)} \sum_n \left[ f(\langle \mathcal O \rangle_n) - \overline{f} \right]^2}
      \f] 
      
      \tparam Observable \concept{ObservableType}
    */
    template <class Observable>
    class BinningAnalysis
    {
    public:
      //! Type of the analysis result
      typedef std::pair<Observable, Observable> result_type;

      /*! 
	\brief Static function to execute the analysis
	\tparam ObservableIterator Iterator over the range of observables to take into account
	\param observables_begin Iterator to the begin of the observable range
	\param observables_end Iterator to the end of the observable range
	\param bin_size Number of observables that are binned together
	\param function_of_observable Function taking an Observable and returning an Observable to be applied to every bin. If no function is given, the mean and the error of the observables is calculated.
	\param observables_reduce Function taking a range of observables and returning one reduced observable (e.g. the mean or the variance of the range). The default is to calculate the mean of the range.
	\returns Pair of Observables, the first entry is the mean of the function and the second entry is the error of the mean of the function.
      */
      template <class ObservableIterator>
      static result_type analyse(ObservableIterator observables_begin, 
				 ObservableIterator observables_end, 
				 unsigned int bin_size, 
				 Observable (*function_of_observable)(const Observable&) = AnalysisHelper<Observable>::identity, 
				 Observable (*observables_reduce)(ObservableIterator, ObservableIterator) = AnalysisHelper<Observable>::observable_range_mean)
      {
	int observables_size = std::distance(observables_begin, observables_end);
	unsigned int bin_number = observables_size / bin_size;

	// Define the accumulator accumulating the single bootstrap resamplings
	ba::accumulator_set<Observable, ba::stats<ba::tag::mean, ba::tag::variance> > binning_accumulator;

	for (unsigned int bin = 0; bin < bin_number; ++bin)
	{
	  // Determine the begin and the end of the bin
	  ObservableIterator bin_begin = observables_begin; std::advance(bin_begin, bin*bin_size);
	  ObservableIterator bin_end = observables_begin; if (bin == bin_number - 1) bin_end = observables_end; else std::advance(bin_end, (bin + 1)*bin_size);

	  // Reduce the observables
	  binning_accumulator(function_of_observable(observables_reduce(bin_begin, bin_end)));
	}

	return result_type(ba::mean(binning_accumulator), sqrt(ba::variance(binning_accumulator)/(bin_number - 1)));
      }
    };
  }
}

#endif
