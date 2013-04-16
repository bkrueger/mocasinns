#ifndef MOCASINNS_DETAILS_MULTICANONICAL_STEP_TYPE_EXTENDED_HPP
#define MOCASINNS_DETAILS_MULTICANONICAL_STEP_TYPE_EXTENDED_HPP

namespace Mocasinns
{
  namespace Details
  {
    namespace Multicanonical
    {
      //! Class extending the StepType to a class with modified delta_E
      template <class ConfigurationType, class StepType, class EnergyType>
      class StepTypeExtended
      {
      private:
	//! Original step that will be considered
	StepType work_step;
	//! Extended configuration to which this step belongs to
	ConfigurationTypeExtended<ConfigurationType,StepType,EnergyType>* extended_configuration_space;
	//! Flag that indicates whether this step was executed from outside
	bool was_executed;
	//! Flag that indicates whether this step was executed for reference testing from the inside
	bool was_executed_testwise;
	//! Original part of the energy difference of the step
	EnergyType original_delta_E;
	//! Integer part of the extended energy of the step
	int groundstate_delta_E;
	//! Flag that indicates whether the groundstate_delta_E was calculated;
	bool delta_E_calculated;
	
      public:
	//! Constructor
	StepTypeExtended(const StepType& original_step, ConfigurationTypeExtended<ConfigurationType,StepType,EnergyType>* proposing_configuration_space);
	
	//! Destructor, undoing a possibly done step if the step is destructed before beeing executed
	~StepTypeExtended();
	
	//! Get-Accessor for the original step
	StepType& get_original_step() { return work_step; }

	//! Function overriding the delta_E function of the original step
	EnergyTypeExtended<EnergyType> delta_E();
	
	//! Function overriding the execute function of the original step
	void execute();
	//! Function overriding the execute function of the original step, calls the commit_testwise functions of the configuration space
	void execute_testwise();

	//! Function forwarding the is_executable function of the original step
	inline bool is_executable() { return work_step.is_executable(); }
	
	//! Function forwarding the selection probability factor function of the original step
	inline double selection_probability_factor() { return work_step.selection_probability_factor(); }
      };
    }
  }
}

#include "../../src/details/multicanonical/step_type_extended.cpp"

#endif
