#ifdef MOCASINNS_DETAILS_MULTICANONICAL_STEP_TYPE_EXTENDED_HPP

namespace Mocasinns
{
  namespace Details
  {
    namespace Multicanonical
    {      
      template <class ConfigurationType, class StepType, class EnergyType>
      StepTypeExtended<ConfigurationType, StepType, EnergyType>::StepTypeExtended(const StepType& original_step, ConfigurationTypeExtended<ConfigurationType,StepType,EnergyType>* proposing_configuration_space) 
	: work_step(original_step), extended_configuration_space(proposing_configuration_space), was_executed(false), was_executed_testwise(false), delta_E_calculated(false)
      { }
      
      template <class ConfigurationType, class StepType, class EnergyType>
      StepTypeExtended<ConfigurationType, StepType, EnergyType>::~StepTypeExtended()
      {
	// If the step was executed to test whether the resulting configuration equals the reference configuration, but was not executed by the user, than undo the step.
	if (was_executed_testwise && !was_executed)
	  work_step.undo();
      }

      template <class ConfigurationType, class StepType, class EnergyType>
      EnergyTypeExtended<EnergyType> StepTypeExtended<ConfigurationType, StepType, EnergyType>::delta_E()
      {
	// If the energy difference was calculated allready, return it
	if (delta_E_calculated)
	  return EnergyTypeExtended<EnergyType>(original_delta_E, groundstate_delta_E);

	// Otherwise calculate the energy difference
	delta_E_calculated = true;

	// Calculate the original energy difference
	original_delta_E = work_step.delta_E();
		
	// Make a case by case study whether the system is in reference state or not before and after the flip
	if (!extended_configuration_space->get_is_reference_configuration())
	{
	  if (extended_configuration_space->energy().get_original_energy() + original_delta_E != extended_configuration_space->get_reference_configuration_energy())
	  {
	    groundstate_delta_E = 0;
	    return EnergyTypeExtended<EnergyType>(original_delta_E, 0);
	  }
	  else
	  {
	    execute_testwise();
	    
	    if (*(extended_configuration_space->get_reference_configuration()) ==
		*(extended_configuration_space->get_original_configuration()))	
	    {
	      groundstate_delta_E = 1;
	      return EnergyTypeExtended<EnergyType>(original_delta_E, 1);
	    }
	    else
	    {
	      groundstate_delta_E = 0;
	      return EnergyTypeExtended<EnergyType>(original_delta_E, 0);
	    }
	  }
	}
	else // extended_configuration_space->get_is_reference_configuration
	{
	  if (extended_configuration_space->energy().get_original_energy() + original_delta_E != extended_configuration_space->get_reference_configuration_energy())
	  {
	    groundstate_delta_E = -1;
	    return EnergyTypeExtended<EnergyType>(original_delta_E, -1);
	  }
	  else
	  {
	    execute_testwise();
	      
	    if (*(extended_configuration_space->get_reference_configuration()) ==
		*(extended_configuration_space->get_original_configuration()))	
	    {
	      groundstate_delta_E = 0;
	      return EnergyTypeExtended<EnergyType>(original_delta_E, 0);
	    }
	    else
	    {
	      groundstate_delta_E = -1;
	      return EnergyTypeExtended<EnergyType>(original_delta_E, -1);
	    }
	  }
	}
      }
      
      template <class ConfigurationType, class StepType, class EnergyType>
      void StepTypeExtended<ConfigurationType, StepType, EnergyType>::execute()
      {
	// Set the flag indicating that the step was executed
	was_executed = true;
	// If the flip was executed before, just update the energy. Otherwise commit the step regularily
	if (was_executed_testwise)
	  extended_configuration_space->update_energy(original_delta_E, groundstate_delta_E);
	else
	  extended_configuration_space->commit(*this);
      }

      template <class ConfigurationType, class StepType, class EnergyType>
      void StepTypeExtended<ConfigurationType, StepType, EnergyType>::execute_testwise()
      {
	was_executed_testwise = true;
	extended_configuration_space->commit_testwise(*this);
      }

      
    }
  }
}

#endif
