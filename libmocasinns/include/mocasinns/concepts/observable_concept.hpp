#ifndef MOCASINNS_CONCEPTS_OBSERVABLE_CONCEPT
#define MOCASINNS_CONCEPTS_OBSERVABLE_CONCEPT

/*!
  \file energy_concept.hpp

  \author Benedikt Krüger
*/

#include "boost/concept_check.hpp"

namespace Mocasinns
{
  namespace Concepts
  {

    /*!
      \brief Structure for checking a class for compatibility with the concept of an observable used in the mocasinns library.
      \tparam ObservableType Class or type that should be checked for compatibility with the observable concept.
    */
    template <class ObservableType>
    struct ObservableConcept
    {
    public:
      BOOST_CONCEPT_USAGE(ObservableConcept)
      {
	// Two observables must be addible and the result must be an observable type (or convertible to an observable type)
	observable_type_result = observable_1 + observable_2;
	
	// Two observables can be compared using the < operator
	compare_result = (observable_1 < observable_2);
	
	// An observable can be multiplied with and divided by a number
	observable_type_result = 2.0*observable_1;
	observable_type_result = observable_1/2.0;

	// There must be the pow and the sqrt functions defined for observables
	observable_type_result = pow(observable_1, 2.0);
	observable_type_result = sqrt(observable_1);
      }
      
    private:
      ObservableType observable_1;
      ObservableType observable_2;
      
      ObservableType observable_type_result;
      bool compare_result;
    };
  }
}

#endif
