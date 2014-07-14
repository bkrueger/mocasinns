#ifndef MOCASINNS_CONCEPTS_ACCUMULATOR_CONCEPT
#define MOCASINNS_CONCEPTS_ACCUMULATOR_CONCEPT

/*!
  \file accumulator_concept.hpp

  \author Benedikt Krüger
*/

#include "boost/concept_check.hpp"

namespace Mocasinns
{
  namespace Concepts
  {

    /*!
      \brief Structure for checking a class for compatibility with the concept of an accumulator used in the mocasinns library.
      \tparam AccumulatorType Class or type that should be checked for compatibility with the accumulator concept.
      \tparam ObservableType Type of observable that will be accumulated
    */
    template <class AccumulatorType, class ObservableType>
    struct AccumulatorConcept
    {
    public:
      BOOST_CONCEPT_USAGE(AccumulatorConcept)
      {
	// The operator() must be defined by taking an ObservableType
	accumulator(observable);
      }
      
    private:
      AccumulatorType accumulator;
      ObservableType observable;
    };
  }
}

#endif
