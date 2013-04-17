#ifndef MOCASINNS_DETAILS_OPTIONAL_CONCEPT_CHECKS_STEP_TYPE_HAS_IS_EXECUTABLE
#define MOCASINNS_DETAILS_OPTIONAL_CONCEPT_CHECKS_STEP_TYPE_HAS_IS_EXECUTABLE

namespace Mocasinns
{
  namespace Details
  {
    namespace OptionalConceptChecks
    {
      //! Class for checking whether the given type provides a function is_executable
      /*!
	\tparam StepType Type that should be checked for the existance of the function
	\details See http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence for the concept of this class. Usage: StepTypeHasIsExecutable<StepType>::value is true if StepType::is_executable exists, otherwise it is false.
       */
      template <class StepType>
      class StepTypeHasIsExecutable
      {
      private:
	// Define two types that have guaranteed different sizes
	typedef char Small;
	struct Big { char dummy[2]; };
	
	template <typename C>
	static Small test(char[sizeof(&C::is_executable)]);
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
