#ifndef MOCASINNS_DETAILS_ITERATION_STEPS_POWER_LAW_STEPS_HPP
#define MOCASINNS_DETAILS_ITERATION_STEPS_POWER_LAW_STEPS_HPP

namespace Mocasinns
{
  namespace Details
  {
    namespace IterationSteps
    {
      //! Class for doing a number of steps that is a power law in the iteration number
      template <class int_t = long unsigned int>
      class PowerLawSteps
      {
      public:
	//! Constant for the base of the power law (default is 2, which corresponds to doubling the steps in each iteration)
	int_t power_law_base;

	//! Default constructor for setting the base of the power law
	PowerLawSteps() : power_law_base(2) { };

	//! Operator that is used to calculate the iteration steps
	int_t operator()(int_t initial_iteration_steps, int_t iteration) const { return initial_iteration_steps * pow(power_law_base, iteration); }
      };
    }
  }
}

#endif
