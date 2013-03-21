#ifndef MOCASINNS_ANALYSIS_OBSERVABLE_FUNCTORS_HPP
#define MOCASINNS_ANALYSIS_OBSERVABLE_FUNCTORS_HPP

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
    template <class Observable, class BoostAccumulatorTag>
    struct ObservableFunctorBoostAccumulator
    {
      typedef Observable return_type;
      
      template<class InputIterator>
      Observable operator()(InputIterator begin, InputIterator end)
      {
	// Define an accumulator with the accumulator tag
	ba::accumulator_set<Observable, ba::stats<BoostAccumulatorTag> > acc;
	
	// Fill the accumulator
	for (InputIterator it = begin; it != end; ++it) acc(*it);
	
	// Return the extracted quantity
	ba::extractor<BoostAccumulatorTag> my_extractor;
	return my_extractor(acc); 
      }
    };
    
    template <class Observable>
    struct MeanOfObservables : ObservableFunctorBoostAccumulator<Observable, ba::tag::mean> {};
    
    template <class Observable>
    struct VarianceOfObservables : ObservableFunctorBoostAccumulator<Observable, ba::tag::variance> {};
    
    template <class Observable>
    struct StandardDeviationOfObservables
    {
      typedef Observable return_type;
      
      template <class InputIterator>
      Observable operator()(InputIterator begin, InputIterator end)
      {
	VarianceOfObservables<Observable> variance_functor;
	return sqrt(variance_functor(begin, end));
      }
    };
  }
}

#endif
