#ifndef MOCASINNS_ANALYSIS_BOOTSTRAP_ANALYSIS_HPP
#define MOCASINNS_ANALYSIS_BOOTSTRAP_ANALYSIS_HPP

// Standard libraries
#include <algorithm>

// Standard observable functions
#include "observable_functors.hpp"

// Random number generator
#include "../random/boost_random.hpp"

// Boost accumulator
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>
// Boost iterator
#include <boost/iterator/permutation_iterator.hpp>

namespace Mocasinns
{
  namespace Analysis
  {
    //! Class for doing a bootstrap analysis of a number of data points.
    /*!
      \details 
      
      \tparam Observable Type of the observables that should be bootstraped, must be able to calculate a pow with integers and a sqrt
      \tparam FunctionOfObservables Functor that has a 1) static operator() that takes two Iterators and calculating a function of observables (e.g. the variance or the specific heat) and has 2) a type return_type typedefed that indicates the return type of the functor.
      \tparam RandomNumberGenerator Class of the random number generator
    */
    template <class Observable, class FunctionOfObservables = MeanOfObservables<Observable>, class RandomNumberGenerator = Random::Boost_MT19937>
    class BootstrapAnalysis
    {
    public:
      //! Type of the analysis result
      typedef std::pair<typename FunctionOfObservables::return_type, typename FunctionOfObservables::return_type> result_type;

      /*! 
	\brief Do the analyis
	\tparam InputIterator Type of the iterator that is used to iterate through the measurments.
	\param measurement_begin Iterator to the begin of the measurements
	\param measurement_end Iterator to the end of the measurments
	\param resampling_size Number of measurments to take into account for every bootstrap calculation
	\param resampling_number Number of resampling sets

	\returns Pair of Observables, the first entry is the mean of the observables and the second entry is the error of the mean of the observables.
      */
      template <class InputIterator>
      static result_type analyse(InputIterator measurement_begin, InputIterator measurement_end, unsigned int resampling_size, unsigned int resampling_number, FunctionOfObservables observable_functor = FunctionOfObservables())
      {
	int measurement_size = std::distance(measurement_begin, measurement_end);
	RandomNumberGenerator rnd;

	// Define the accumulator accumulating the single bootstrap resamplings
	ba::accumulator_set<typename FunctionOfObservables::return_type, ba::stats<ba::tag::mean, ba::tag::variance> > bootstrap_accumulator;

	for (unsigned int res = 0; res < resampling_number; ++res)
	{
	  // Vector for the indizes of the samples that should be used
	  std::vector<int> resampling_indizes(resampling_size);

	  // Fill the vector with the resampling indices
	  for (unsigned int i = 0; i < resampling_size; ++i)
	    resampling_indizes[i] = rnd.random_uint32(0, measurement_size - 1);    
	  std::sort(resampling_indizes.begin(), resampling_indizes.end());

	  // Use the boost::permutation_iterator to iterate over the selected subset
	  typedef boost::permutation_iterator<InputIterator, std::vector<int>::iterator> permutation_it;
	  permutation_it permutation_begin = boost::make_permutation_iterator(measurement_begin, resampling_indizes.begin());
	  permutation_it permutation_end = boost::make_permutation_iterator(measurement_begin, resampling_indizes.end());

	  bootstrap_accumulator(observable_functor(permutation_begin, permutation_end));
	}

	return result_type(ba::mean(bootstrap_accumulator), sqrt(resampling_number*ba::variance(bootstrap_accumulator)));
      }
    };
  }
}

#endif
