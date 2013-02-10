/*!
 * \file wang_landau_counting.cpp
 * \brief Implementation of the WangLandauCounting simulation class
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_WANG_LANDAU_COUNTING_HPP

template <class EnergyType>
EnergyTypeExtended<EnergyType>::EnergyTypeExtended(EnergyType orig_energy, int in_gs) : original_energy(orig_energy), in_ground_state(in_gs)
{ }

template <class EnergyType>
EnergyTypeExtended<EnergyType>::EnergyTypeExtended(const EnergyTypeExtended& other) : original_energy(other.original_energy), in_ground_state(other.in_ground_state)
{ }

template <class EnergyType>
EnergyTypeExtended<EnergyType>& EnergyTypeExtended<EnergyType>::operator=(const EnergyTypeExtended& rhs)
{
  if (this != &rhs)
  {
    original_energy = rhs.original_energy;
    in_ground_state = rhs.in_ground_state;
  }
}

template <class EnergyType>
bool EnergyTypeExtended<EnergyType>::operator==(const EnergyTypeExtended& rhs) const
{
  return (original_energy == rhs.original_energy) && (in_ground_state == rhs.in_ground_state);
}
template <class EnergyType>
bool EnergyTypeExtended<EnergyType>::operator!=(const EnergyTypeExtended& rhs) const
{
  return !operator==(rhs);
}
template <class EnergyType>
bool EnergyTypeExtended<EnergyType>::operator<(const EnergyTypeExtended& rhs) const
{
  // Test first the original energys
  if (original_energy < rhs.original_energy) return true;
  if (original_energy > rhs.original_energy) return false;
  
  // If the original energies match, check the in_ground_state integer
  if (in_ground_state < rhs.in_ground_state) return true;
  else return false;
}

template <class EnergyType>
void EnergyTypeExtended<EnergyType>::operator+=(const EnergyTypeExtended& rhs)
{
  // Add the usual energies
  original_energy += rhs.original_energy;
  // Add the in_ground_state numbers
  in_ground_state += rhs.in_ground_state;
}
template <class EnergyType>
void EnergyTypeExtended<EnergyType>::operator-=(const EnergyTypeExtended& rhs)
{
  // Subtract the usual energies
  original_energy -= rhs.original_energy;
  // Substract the in_ground_state numbers
  in_ground_state -= rhs.in_ground_state;
}

template <class EnergyType>
EnergyTypeExtended<EnergyType> operator+(const EnergyTypeExtended<EnergyType>& lhs, const EnergyTypeExtended<EnergyType>& rhs)
{
  EnergyTypeExtended<EnergyType> result(lhs);
  result += rhs;
  return result;
}
template <class EnergyType>
EnergyTypeExtended<EnergyType> operator-(const EnergyTypeExtended<EnergyType>& lhs, const EnergyTypeExtended<EnergyType>& rhs)
{
  EnergyTypeExtended<EnergyType> result(lhs);
  result -= rhs;
  return result;
}

template <class ConfigurationType, class StepType, class EnergyType>
ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::ConfigurationTypeExtended(ConfigurationType* start_and_reference_configuration)
  : work_configuration(start_and_reference_configuration), reference_configuration(new ConfigurationType(*start_and_reference_configuration))
{
  reference_configuration_energy = reference_configuration->energy();
  actual_energy = work_configuration->energy();
  is_reference_configuration = 1;
}

template <class ConfigurationType, class StepType, class EnergyType>
ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::ConfigurationTypeExtended(ConfigurationType* start_configuration, ConfigurationType* new_reference_configuration)
  : work_configuration(start_configuration), reference_configuration(new ConfigurationType(*new_reference_configuration))
{
  reference_configuration_energy = reference_configuration->energy();
  actual_energy = work_configuration->energy();
  if (start_configuration == reference_configuration)
    is_reference_configuration = 1;
  else
    is_reference_configuration = 0;
}

template <class ConfigurationType, class StepType, class EnergyType>
ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::~ConfigurationTypeExtended()
{
  delete reference_configuration;
}

template <class ConfigurationType, class StepType, class EnergyType>
void ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::commit(StepTypeExtended<ConfigurationType, StepType, EnergyType>& step_to_commit)
{
  // Set the new energy
  actual_energy += step_to_commit.delta_E().get_original_energy();
  
  // Set the reference configuration indicator
  if (is_reference_configuration + step_to_commit.delta_E().get_in_groundstate() == 1)
    is_reference_configuration = 1;
  else
    is_reference_configuration = 0;
  
  // Commit the step to the original configuration
  work_configuration->commit(static_cast<StepType>(step_to_commit));
}

template <class ConfigurationType, class StepType, class EnergyType>
EnergyTypeExtended<EnergyType> ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::energy()
{
  return EnergyTypeExtended<EnergyType>(actual_energy, is_reference_configuration);
}

template <class ConfigurationType, class StepType, class EnergyType>
template <class RandomNumberGenerator>
StepTypeExtended<ConfigurationType, StepType, EnergyType> ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::propose_step(RandomNumberGenerator* rng)
{
  // Construct an original step
  StepType original_step = work_configuration->propose_step(rng);
  // Return the extended step
  return StepTypeExtended<ConfigurationType, StepType, EnergyType>(original_step, this);
}

template <class ConfigurationType, class StepType, class EnergyType>
StepTypeExtended<ConfigurationType, StepType, EnergyType>::StepTypeExtended(StepType* original_step, ConfigurationTypeExtended<ConfigurationType,StepType,EnergyType>* proposing_configuration_space) 
  : work_step(original_step), extended_configuration_space(proposing_configuration_space), was_executed(false), was_executed_testwise(false), groundstate_delta_E_calculated(false)
{ }

template <class ConfigurationType, class StepType, class EnergyType>
StepTypeExtended<ConfigurationType, StepType, EnergyType>::~StepTypeExtended()
{
  // If the step was executed to test whether the resulting configuration equals the reference configuration, but was not executed by the user, than undo the step.
  if (was_executed_testwise && !was_executed)
    work_step->undo();
}

template <class ConfigurationType, class StepType, class EnergyType>
EnergyTypeExtended<EnergyType> StepTypeExtended<ConfigurationType, StepType, EnergyType>::delta_E()
{
  // Calculate the original energy difference
  EnergyType original_delta_E = work_step->delta_E();
  
  // If the energy difference was calculated, just return the value
  // Do not do test step anymore
  if (groundstate_delta_E_calculated)
  {
    return EnergyTypeExtended<EnergyType>(original_delta_E, groundstate_delta_E);
  }
  else
  {
    // Make a case by case study whether the system is in reference state or not
    if (!extended_configuration_space->get_is_reference_configuration())
    {
      if (extended_configuration_space->energy().get_original_energy() + original_delta_E != extended_configuration_space->get_reference_configuration_energy())
      {
	groundstate_delta_E = 0;
	return EnergyTypeExtended<EnergyType>(original_delta_E, 0);
      }
      else
      {
	work_step->execute();
	was_executed_testwise = true;
	
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
	work_step->execute();
	was_executed_testwise = true;
	
	if (extended_configuration_space->get_is_reference_configuration())
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
    groundstate_delta_E_calculated = true;
  } // of (!groundstate_delta_E_calculated)
}

template <class ConfigurationType, class StepType, class EnergyType>
void StepTypeExtended<ConfigurationType, StepType, EnergyType>::execute()
{
  was_executed = true;
  if (!was_executed_testwise)
    extended_configuration_space->commit(*this);
}

template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
typename WangLandauCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::ParametersExtendedType WangLandauCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::convert_parameters(const ParametersOriginalType& user_parameters)
{
  ParametersExtendedType parameters_result;
  parameters_result.binning_reference = EnergyTypeExtended<EnergyType>(user_parameters.binning_reference, 0);
  parameters_result.binning_width = EnergyTypeExtended<EnergyType>(user_parameters.binning_width, 1);
  parameters_result.energy_cutoff_use = user_parameters.energy_cutoff_use;
  parameters_result.energy_cutoff = EnergyTypeExtended<EnergyType>(user_parameters.energy_cutoff, 0);
  parameters_result.flatness = user_parameters.flatness;
  parameters_result.modification_factor_initial = user_parameters.modification_factor_initial;
  parameters_result.modification_factor_final = user_parameters.modification_factor_final;
  parameters_result.modification_factor_modifyer = user_parameters.modification_factor_modifyer;
  parameters_result.sweep_steps = user_parameters.sweep_steps;
  return parameters_result;
}

template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
WangLandauCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::WangLandauCounting(const ParametersOriginalType& params)
  : WangLandauType(convert_parameters(params), ConfigTypeExt(new ConfigurationType()))
{ }

template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
WangLandauCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::WangLandauCounting(const ParametersOriginalType& params, ConfigurationType* initial_configuration) 
  : WangLandauType(convert_parameters(params), ConfigTypeExt(initial_configuration))
{ }
		   
template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
WangLandauCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::WangLandauCounting(const ParametersOriginalType& params, ConfigurationType* initial_configuration, ConfigurationType* reference_configuration) 
  : WangLandauType(convert_parameters(params), ConfigTypeExt(initial_configuration, reference_configuration))
{ }

template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
double WangLandauCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::do_wang_landau_counting()
{
  // Execute the simulation
  this->do_wang_landau_simulation();
  
  // Extract the total number of states
  double number_of_states = 0;

  return number_of_states;
}

#endif
