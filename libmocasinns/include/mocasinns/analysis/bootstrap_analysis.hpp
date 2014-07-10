#ifndef MOCASINNS_ANALYSIS_BOOTSTRAP_ANALYSIS_HPP
#define MOCASINNS_ANALYSIS_BOOTSTRAP_ANALYSIS_HPP

// Standard libraries
#include <algorithm>

// Random number generator
#include "../random/boost_random.hpp"

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
    //! Class for doing a bootstrap analysis of a number of data points.
    /*!
      \details The functionality of the bootstrap analysis is encapsulated in the static function analyse.
      The given range of observables with \f$ N \f$ is resampled \f$r\f$ times, i.e. one samples with replacement \f$N\f$ times from the range of all observables.
      Then the averages of the resamplings
      \f[ 
        \langle \mathcal O \rangle_n := \frac{1}{N} \sum{i = 1}^N \mathcal O_i
      \f] 
      (where \f$ \mathcal O_i \f$ is the \f$i\f$-th sampled observable) are calculated. The average of the given function \f$ f \f$ applied to the observables is then calculated as
      \f[ 
        \overline{f} = \frac{1}{r} \sum_{n=1}^{r} f(\langle \mathcal O \rangle_n)
      \f] 
      the standard error is calculated using
      \f[ 
        \Delta \overline{f} = \sqrt{ \sum_{n=1}^{r} \left[ f(\langle \mathcal O \rangle_n) - \overline{f} \right]^2}
      \f] 
      
      \tparam Observable \concept{ObservableType}
      \tparam RandomNumberGenerator \concept{RandomNumberGenerator}
    */
    template <class Observable, class RandomNumberGenerator = Random::Boost_MT19937>
    class BootstrapAnalysis
    {
    public:
      //! Type of the analysis result
      typedef std::pair<Observable, Observable> result_type;

      /*! 
	\brief Static function to execute the analysis
	\tparam ObservableIterator Iterator over the range of observables to take into account
	\param observables_begin Iterator to the begin of the observable range
	\param observables_end Iterator to the end of the observable range
	\param resampling_number Number of resamplings
	\param function_of_observable Function taking an Observable and returning an Observable to be applied to every bin. If no function is given, the mean and the error of the observables is calculated.
	\param observables_reduce Function taking a range of observables and returning one reduced observable (e.g. the mean or the variance of the range). The default is to calculate the mean of the range.
	\returns Pair of Observables, the first entry is the mean of the function and the second entry is the error of the mean of the function.
      */
#ifdef MOCASINNS_DOXYGEN_DOCUMENTATION
      template <class ObservableIterator>
      static result_type analyse(ObservableIterator observables_begin, 
				 ObservableIterator observables_end, 
				 unsigned int resampling_number, 
				 Observable (*function_of_observable)(const Observable&) = AnalysisHelper<Observable>::identity, 
				 Observable (*observables_reduce)(ObservableIterator, ObservableIterator) = AnalysisHelper<Observable>::observable_range_mean)
#else
      template <class ObservableIterator>
      static result_type analyse(ObservableIterator observables_begin, 
				 ObservableIterator observables_end, 
				 unsigned int resampling_number, 
				 Observable (*function_of_observable)(const Observable&) = AnalysisHelper<Observable>::identity, 
				 Observable (*observables_reduce)(boost::permutation_iterator<ObservableIterator, std::vector<int>::iterator>,
								  boost::permutation_iterator<ObservableIterator, std::vector<int>::iterator>) 
				 = AnalysisHelper<Observable>::observable_range_mean)
#endif
      {
	unsigned int observables_size = std::distance(observables_begin, observables_end);
	RandomNumberGenerator rnd;

	// Define the accumulator accumulating the single bootstrap resamplings
	ba::accumulator_set<Observable, ba::stats<ba::tag::mean, ba::tag::variance> > bootstrap_accumulator;

	for (unsigned int res = 0; res < resampling_number; ++res)
	{
	  // Vector for the indizes of the samples that should be used
	  std::vector<int> resampling_indizes(observables_size);

	  // Fill the vector with the resampling indices
	  for (unsigned int i = 0; i < observables_size; ++i)
	    resampling_indizes[i] = rnd.random_uint32(0, observables_size - 1);    
	  std::sort(resampling_indizes.begin(), resampling_indizes.end());

	  // Use the boost::permutation_iterator to iterate over the selected subset
	  typedef boost::permutation_iterator<ObservableIterator, std::vector<int>::iterator> permutation_it;
	  permutation_it permutation_begin = boost::make_permutation_iterator(observables_begin, resampling_indizes.begin());
	  permutation_it permutation_end = boost::make_permutation_iterator(observables_begin, resampling_indizes.end());

	  // Calculate the observable and push back to the boostrap accumulator
	  bootstrap_accumulator(function_of_observable(observables_reduce(permutation_begin, permutation_end)));
	}

	return result_type(ba::mean(bootstrap_accumulator), sqrt(resampling_number*ba::variance(bootstrap_accumulator)));
      }
    };
  }
}

#endif
