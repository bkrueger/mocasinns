#ifndef MOCASINNS_CONCEPTS_STEP_CONCEPT
#define MOCASINNS_CONCEPTS_STEP_CONCEPT

/*!
  \file step_concept.hpp

  \author Benedikt Krüger
*/

#include "boost/concept_check.hpp"

namespace Mocasinns
{
  namespace Concepts
  {

    /*!
      \brief Structure for checking a class for compatibility with the concept of a step used in the mocasinns library.
      \tparam StepType Class or type that should be checked for compatibility with the step concept.
    */
    template <class StepType>
    struct StepConcept
    {
    public:
      BOOST_CONCEPT_USAGE(StepConcept)
      {
	// There must be a function for delta_E()
	step.delta_E();
	
	// There must be a function execute
	step.execute();

	// There must be a function is_executable with result bool
	//is_executable_result = step.is_executable();

	// There must be a function selection_probability_factor with result double
	//selection_probability_factor_result = step.selection_probability_factor();
      }
      
    private:
      StepType step;
      //      bool is_executable_result;
      //double selection_probability_factor_result;

    };
  }
}

#endif
