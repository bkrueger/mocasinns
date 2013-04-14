#ifndef MOCASINNS_CONCEPTS_OBSERVATOR_CONCEPT
#define MOCASINNS_CONCEPTS_OBSERVATOR_CONCEPT

/*!
  \file energy_concept.hpp

  \author Benedikt Krüger
*/

#include "boost/concept_check.hpp"
#include "observable_concept.hpp"

namespace Mocasinns
{
  namespace Concepts
  {

    /*!
      \brief Structure for checking a class for compatibility with the concept of an observator used in the mocasinns library.
      \tparam ObservatorType Class or type that should be checked for compatibility with the observator concept.
    */
    template <class ObservatorType, class ConfigurationType>
    struct ObservatorConcept : ObservableConcept<typename ObservatorType::observable_type>
    {
    public:
      // There must be a typedef for observable_type
      typedef typename ObservatorType::observable_type observable_type;

      BOOST_CONCEPT_USAGE(ObservatorConcept)
      {
	// The observable type must be an observable
	BOOST_CONCEPT_ASSERT((ObservableConcept<observable_type>));

	// There must be a static observe function
	observe_result = ObservatorType::observe(configuration_pointer);
      }
      
    private:
      ConfigurationType* configuration_pointer;
      observable_type observe_result;
    };
  }
}

#endif
