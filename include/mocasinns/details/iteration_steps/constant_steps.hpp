#ifndef MOCASINNS_DETAILS_ITERATION_STEPS_CONSTANT_STEPS_HPP
#define MOCASINNS_DETAILS_ITERATION_STEPS_CONSTANT_STEPS_HPP

namespace Mocasinns
{
  namespace Details
  {
    namespace IterationSteps
    {
      //! Class for doing a constant number of steps in each iteration
      template <class int_t = long unsigned int>
      class ConstantSteps
      {
      public:
	//! Operator that is used to calculate the iteration steps
	int_t operator()(int_t initial_iteration_steps, int_t) const { return initial_iteration_steps; }
      };
    }
  }
}

#endif
