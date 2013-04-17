#ifndef MOCASINNS_DETAILS_OPTIONAL_CONCEPT_CHECKS_STEP_TYPE_HAS_SELECTION_PROBABILITY_FACTOR
#define MOCASINNS_DETAILS_OPTIONAL_CONCEPT_CHECKS_STEP_TYPE_HAS_SELECTION_PROBABILITY_FACTOR

namespace Mocasinns
{
  namespace Details
  {
    namespace OptionalConceptChecks
    {
      //! Class for checking whether the given type provides a function selection_probability_factor
      /*!
	\tparam StepType Type that should be checked for the existance of the function
	\details See http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence for the concept of this class. Usage: StepTypeHasSelectionProbabilityFactor<StepType>::value is true if StepType::selection_probability_factor exists, otherwise it is false.
       */
      template <class StepType>
      class StepTypeHasSelectionProbabilityFactor
      {
      private:
	// Define two types that have guaranteed different sizes
	typedef char Small;
	struct Big { char dummy[2]; };
	
	template <typename C>
	static Small test(char[sizeof(&C::selection_probability_factor)]);
	template <typename C>
	static Big test(...);
	
      public:
	enum {
	  value = (sizeof(test<StepType>(0) == sizeof(Small)))
	};
      };
    }
  }
}

#endif
