#ifndef MOCASINNS_ANALYSIS_JACKKNIFE_ANALYSIS_HPP
#define MOCASINNS_ANALYSIS_JACKKNIFE_ANALYSIS_HPP

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
    //! Class for doing a jackknife analysis of a number of data points.
    /*!
      \details 
      
      \tparam Observable Type of the observables that should be jackknifed, must be able to calculate a pow with integers and a sqrt
      \tparam FunctionOfObservables Functor that has a 1) static operator() that takes two Iterators and calculating a function of observables (e.g. the variance or the specific heat) and has 2) a type return_type typedefed that indicates the return type of the functor.
    */
    template <class Observable>
    class JackknifeAnalysis
    {
    public:
      //! Type of the analysis result
      typedef std::pair<Observable, Observable> result_type;

      /*! 
	\brief Do the analyis

	\tparam InputIterator Type of the iterator that is used to iterate through the measurments.
	\param measurement_begin Iterator to the begin of the measurements
	\param measurement_end Iterator to the end of the measurments
	\param bin_size Number of successive measurements to gather in one jackknife bin (default value 1)
	\param observable_functor Functor of the observable

	\returns std::pair of Observables, the first entry is the mean of the observables and the second entry is the error of the mean of the observables.
      */
      template <class InputIterator>
      static result_type analyse(InputIterator measurement_begin, InputIterator measurement_end, Observable (*function_of_observable)(Observable) = identity, unsigned int bin_size = 1)
      {
	// Calculate the size of the samples and the number of bins
	int measurement_size = std::distance(measurement_begin, measurement_end);
	unsigned int bin_number = measurement_size / bin_size;

	// Define the accumulator accumulating the jackknife bins
	ba::accumulator_set<Observable, ba::stats<ba::tag::mean, ba::tag::variance> > jackknife_accumulator;

	// Accumulate the jackknife values
	for (unsigned int jackknife_bin = 0; jackknife_bin < bin_number; ++jackknife_bin)
	{
	  unsigned int jackknife_bin_begin = jackknife_bin * bin_size; // First value not to take into account
	  unsigned int jackknife_bin_end = jackknife_bin_begin + bin_size; // Last value not to take into account

	  // Define a accumulator for accumulating all observables except in the special bin
	  ba::accumulator_set<Observable, ba::stats<ba::tag::mean> > temp_accumulator;
	  // Accumulate
	  unsigned int counter = 0;
	  for (InputIterator it = measurement_begin; it != measurement_end; ++it)	  
	  {
	    if (counter < jackknife_bin_begin || counter >= jackknife_bin_end) temp_accumulator(*it);
	    counter++;
	  }

	  // Calculate the observable and push back to the jackknife accumulator
	  jackknife_accumulator(function_of_observable(ba::mean(temp_accumulator)));
	}

	// Make a pair and return it
	return result_type(ba::mean(jackknife_accumulator), sqrt((bin_number - 1)*ba::variance(jackknife_accumulator)));
      }

    private:
      static Observable identity(Observable x) { return x; }
    };
  }
}
  

#endif
