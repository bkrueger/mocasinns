#ifndef MOCASINNS_DETAILS_ITERATION_STEPS_LINEAR_STEPS_HPP
#define MOCASINNS_DETAILS_ITERATION_STEPS_LINEAR_STEPS_HPP

namespace Mocasinns
{
  namespace Details
  {
    namespace IterationSteps
    {
      //! Class for doing a number of steps that is linear in the iteration number
      template <class int_t = long unsigned int>
      class LinearSteps
      {
      public:
	int_t steps_growth;

	//! Operator that is used to calculate the iteration steps
	int_t operator()(int_t initial_iteration_steps, int_t iteration) const { return initial_iteration_steps + iteration*step_growth; }
      };
    }
  }
}

#endif
