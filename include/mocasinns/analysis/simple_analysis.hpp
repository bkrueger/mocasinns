#ifndef MOCASINNS_ANALYSIS_SIMPLE_ANALYSIS_HPP
#define MOCASINNS_ANALYSIS_SIMPLE_ANALYSIS_HPP

#include "analysis_helper.hpp"

namespace ba = boost::accumulators;

namespace Mocasinns
{
  namespace Analysis
  {
    //! Class for doing a simple analysis of a number of data points.
    /*!
      \details The functionality of the binning analysis is encapsulated in the static function analyse.
      The \f$ N \f$ values given by an iterator, the average of a function \f$ f \f$ applied to the observables is calculated using
      \f[ 
        \overline{f} = f\left( \frac{1}{N} \sum_{i = 1}{N} x_i \right) 
      \f] 
      The error \f$ \Delta f \f$ cannot be calculated correctly using this method, therefor the analysis returns 0.
      
      \tparam Observable \concept{ObservableType}
    */
    template <class Observable>
    class SimpleAnalysis
    {
    public:
      //! Type of the analysis result
      typedef std::pair<Observable, Observable> result_type;

      /*! 
	\brief Static function to execute the analysis
	\tparam ObservableIterator Iterator over the range of observables to take into account
	\param observables_begin Iterator to the begin of the observable range
	\param observables_end Iterator to the end of the observable range
	\param function_of_observable Function taking an Observable and returning an Observable to be applied to every bin. If no function is given, the mean and the error of the observables is calculated.
	\param observables_reduce Function taking a range of observables and returning one reduced observable (e.g. the mean or the variance of the range). The default is to calculate the mean of the range.
	\returns Pair of Observables, the first entry is the mean of the function and the second entry is the error of the mean of the function.
      */
      template <class ObservableIterator>
      static result_type analyse(ObservableIterator observables_begin, 
				 ObservableIterator observables_end, 
				 Observable (*function_of_observable)(const Observable&) = AnalysisHelper<Observable>::identity, 
				 Observable (*observables_reduce)(ObservableIterator, ObservableIterator) = AnalysisHelper<Observable>::observable_range_mean)
      {
	return result_type(function_of_observable(observables_reduce(observables_begin, observables_end)), 0);
      }
    };
  }
}

#endif
