#ifndef MOCASINNS_ANALYSIS_JACKKNIFE_ANALYSIS_HPP
#define MOCASINNS_ANALYSIS_JACKKNIFE_ANALYSIS_HPP

// Boost accumulator
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>
// Boost iterator
#include <boost/iterator/permutation_iterator.hpp>

#include "analysis_helper.hpp"

namespace ba = boost::accumulators;

namespace Mocasinns
{
  namespace Analysis
  {
    //! Class for doing a jackknife analysis of a number of data points.
    /*!
      \details The functionality of the jackknife analysis is encapsulated in the static function analyse.
      The function is evaluated on the given range of \f$N\f$ observables \f$x_j\f$ leaving out once every observable
      \f[ 
        \overline{f}_i := f\left( \frac{1}{N-1} \sum{j = 0, j \neq i}^N x_j \right) 
      \f] 
      The average of the given function \f$ f \f$ applied to the observables is then calculated as
      \f[ 
        \overline{f} = \frac{1}{N} \sum_{j = 1}^{N} \overline{f}_j
      \f] 
      the standard error is calculated using
      \f[ 
        \Delta \overline{f} = \sqrt{ \frac{N - 1}{N} \sum_{j = 1}^{N} \left[ \overline{f}_j - \overline{f}\right]}
      \f] 
      
      \tparam Observable \concept{ObservableType}
    */
    template <class Observable>
    class JackknifeAnalysis
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
#ifdef MOCASINNS_DOXYGEN_DOCUMENTATION
      template <class ObservableIterator>
      static result_type analyse(ObservableIterator observables_begin, 
				 ObservableIterator observables_end, 
				 Observable (*function_of_observable)(const Observable&) = AnalysisHelper<Observable>::identity, 
				 Observable (*observables_reduce)(ObservableIterator, ObservableIterator) = AnalysisHelper<Observable>::observable_range_mean)
#else
      template <class ObservableIterator>
      static result_type analyse(ObservableIterator observables_begin, 
				 ObservableIterator observables_end, 
				 Observable (*function_of_observable)(const Observable&) = AnalysisHelper<Observable>::identity, 
				 Observable (*observables_reduce)(boost::permutation_iterator<ObservableIterator, std::vector<int>::iterator>,
								  boost::permutation_iterator<ObservableIterator, std::vector<int>::iterator>) 
				 = AnalysisHelper<Observable>::observable_range_mean)
#endif
      {
	// Calculate the size of the samples and the number of bins
	unsigned int observables_size = std::distance(observables_begin, observables_end);

	// Define the accumulator accumulating the jackknife bins
	ba::accumulator_set<Observable, ba::stats<ba::tag::mean, ba::tag::variance> > jackknife_accumulator;
	
	// Define the vector with the resampling indizes
	std::vector<int> resampling_indizes(observables_size - 1);
	for (unsigned int i = 0; i < observables_size; ++i) resampling_indizes[i] = i;

	// Do the analysis leaving out every index once
	for (unsigned int jackknife_bin = 0; jackknife_bin < observables_size; ++jackknife_bin)
	{
	  // Correct the resampling indizes
	  if (jackknife_bin != observables_size - 1) resampling_indizes[jackknife_bin] = observables_size - 1;

	  // Use the boost::permutation_iterator to iterate over the selected subset
	  typedef boost::permutation_iterator<ObservableIterator, std::vector<int>::iterator> permutation_it;
	  permutation_it permutation_begin = boost::make_permutation_iterator(observables_begin, resampling_indizes.begin());
	  permutation_it permutation_end = boost::make_permutation_iterator(observables_begin, resampling_indizes.end());

	  // Calculate the observable and push back to the boostrap accumulator
	  jackknife_accumulator(function_of_observable(observables_reduce(permutation_begin, permutation_end)));

	  // Correct the resampling indizes again
	  if (jackknife_bin != observables_size - 1) resampling_indizes[jackknife_bin] = jackknife_bin;
	}

	// Make a pair and return it
	return result_type(ba::mean(jackknife_accumulator), sqrt((observables_size - 1)*ba::variance(jackknife_accumulator)));
      }
    };

    //! Class for doing a jackknife analysis of a binned number of data points.
    /*!
      \details The functionality of the jackknife analysis is encapsulated in the static function analyse.
      The \f$N\f$ observables given by an iterator range are gathered in \f$ N_B\f$ bins of the given bin length \f$ b \f$.
      If as in the default case no bin length is given, it is set to 1.
      Then the averages of the single bins
      \f[ 
        \langle \mathcal O \rangle_n := \frac{1}{b} \sum{i = 0}^b \mathcal O_{n*b + i}
      \f] 
      are calculated. The average of the given function \f$ f \f$ applied to the observables is then calculated as
      \f[ 
        \overline{f} = \frac{1}{N_B} \sum_{n = 1}^{N_B} f\left( \frac{N \cdot \overline{\mathcal O} - b \cdot \langle \mathcal O \rangle_n}{N - b} \right)
      \f] 
      the standard error is calculated using
      \f[ 
        \Delta \overline{f} = \sqrt{ \frac{N_B - 1}{N_B} \sum_{n = 1}^{N_B} \left[ f\left( \frac{N \cdot \overline{\mathcal O} - b \cdot \langle \mathcal O \rangle_n}{N - b} \right) - \overline{f} \right]^2}
      \f] 
      
      \tparam Observable \concept{ObservableType}
    */
    template <class Observable>
    class JackknifeBinnedAnalysis
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
	\param bin_size Number of observables that are binned together (Default value is 1)

	\returns Pair of Observables, the first entry is the mean of the function and the second entry is the error of the mean of the function.
      */
      template <class ObservableIterator>
      static result_type analyse(ObservableIterator observables_begin, 
				 ObservableIterator observables_end, 
				 unsigned int bin_size = 1, 
				 Observable (*function_of_observable)(const Observable&) = AnalysisHelper<Observable>::identity, 
				 Observable (*observables_reduce)(ObservableIterator, ObservableIterator) = AnalysisHelper<Observable>::observable_range_mean)
      {
	// Calculate the size of the samples and the number of bins
	int observables_size = std::distance(observables_begin, observables_end);
	unsigned int bin_number = observables_size / bin_size;

	// Define the accumulator accumulating the jackknife bins
	ba::accumulator_set<Observable, ba::stats<ba::tag::mean, ba::tag::variance> > jackknife_accumulator;

	for (unsigned int jackknife_bin = 0; jackknife_bin < bin_number; ++jackknife_bin)
	{
	  ba::accumulator_set<Observable, ba::stats<ba::tag::mean> > temp_accumulator;
	  for (unsigned int bin = 0; bin < bin_number; ++bin)
	  {
	    if (bin != jackknife_bin)
	    {
	      // Determine the begin and the end of the bin
	      ObservableIterator bin_begin = observables_begin; std::advance(bin_begin, bin*bin_size);
	      ObservableIterator bin_end = observables_begin; if (bin == bin_number - 1) bin_end = observables_end; else std::advance(bin_end, (bin + 1)*bin_size);

	      temp_accumulator(observables_reduce(bin_begin, bin_end));
	    }    
	  }

	  jackknife_accumulator(function_of_observable(ba::mean(temp_accumulator)));
	}

	// Make a pair and return it
	return result_type(ba::mean(jackknife_accumulator), sqrt((bin_number - 1)*ba::variance(jackknife_accumulator)));
      }
    };
  }
}
  

#endif
