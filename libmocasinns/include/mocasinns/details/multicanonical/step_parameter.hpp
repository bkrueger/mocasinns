/*!
  \file step_parameter.hpp

  \brief File containing a struct for using the total energy and the energy difference of a step as parameters passed between the functions acceptance_probability and handle_executed_step

  \author Benedikt Krüger
*/

#ifndef MOCASINNS_DETAILS_MULTICANONICAL_STEP_PARAMETER
#define MOCASINNS_DETAILS_MULTICANONICAL_STEP_PARAMETER

namespace Mocasinns
{
  namespace Details
  {
    namespace Multicanonical
    {
      //! Struct storing the total energy before a step and the energy difference of a step
      template<class EnergyType> struct StepParameter
      {
      public:
	//! Total energy of the system before the step
	EnergyType total_energy;
	//! Energy difference induced by the step
	EnergyType delta_E;
	
	//! Standard constructor
	StepParameter() {}
	//! Constructor taking the two members
	StepParameter(const EnergyType& new_total_energy, const EnergyType& new_delta_E) 
	  : total_energy(new_total_energy), delta_E(new_delta_E) {}
      };
    }
  }
}

#endif
