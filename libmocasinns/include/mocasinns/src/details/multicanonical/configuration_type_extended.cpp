#ifdef MOCASINNS_DETAILS_MULTICANONICAL_CONFIGURATION_TYPE_EXTENDED_HPP

namespace Mocasinns
{
  namespace Details
  {
    namespace Multicanonical
    {
      template <class ConfigurationType, class StepType, class EnergyType>
      ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::ConfigurationTypeExtended(ConfigurationType* start_and_reference_configuration)
	: work_configuration(start_and_reference_configuration), reference_configuration(new ConfigurationType(*start_and_reference_configuration))
      {
	reference_configuration_energy = reference_configuration->energy();
	current_energy = work_configuration->energy();
	is_reference_configuration = 1;
      }

      template <class ConfigurationType, class StepType, class EnergyType>
      ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::ConfigurationTypeExtended(ConfigurationType* start_configuration, ConfigurationType* new_reference_configuration)
	: work_configuration(start_configuration), reference_configuration(new ConfigurationType(*new_reference_configuration))
      {
	reference_configuration_energy = reference_configuration->energy();
	current_energy = work_configuration->energy();
	if (*start_configuration == *reference_configuration)
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
      std::vector<StepTypeExtended<ConfigurationType, StepType, EnergyType> > ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::all_steps()
      {
	// Get the steps of the original configuration type
	std::vector<StepType> all_original_steps = work_configuration->all_steps();
	
	// Construct the extended steps
	std::vector<StepTypeExtended<ConfigurationType, StepType, EnergyType> > all_extended_steps;
	all_extended_steps.reserve(all_original_steps.size());
	for (unsigned int s = 0; s < all_original_steps.size(); ++s)
	{
	  all_extended_steps.push_back(StepTypeExtended<ConfigurationType, StepType, EnergyType>(all_original_steps[s], this));
	}

	// Return all extended steps
	return all_extended_steps;
      }
      
      template <class ConfigurationType, class StepType, class EnergyType>
      void ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::commit(StepTypeExtended<ConfigurationType, StepType, EnergyType>& step_to_commit)
      {
	// Set the new energy
	current_energy += step_to_commit.delta_E().get_original_energy();

	// Set the reference configuration indicator
	if (is_reference_configuration + step_to_commit.delta_E().get_in_ground_state() == 1)
	  is_reference_configuration = 1;
	else
	  is_reference_configuration = 0;

	// Commit the step to the original configuration
	work_configuration->commit(step_to_commit.get_original_step());
      }

      template <class ConfigurationType, class StepType, class EnergyType>
      void ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::commit_testwise(StepTypeExtended<ConfigurationType, StepType, EnergyType>& step_to_commit)
      {
	// Commit the step to the original configuration
	work_configuration->commit(step_to_commit.get_original_step());
      }
      
      template <class ConfigurationType, class StepType, class EnergyType>
      EnergyTypeExtended<EnergyType> ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::energy()
      {
	return EnergyTypeExtended<EnergyType>(current_energy, is_reference_configuration);
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
      void ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::update_energy(const EnergyType& delta_E_original, const int& delta_E_groundstate)
      {
	// Set the new energy
	current_energy += delta_E_original;

	// Set the reference configuration indicator
	is_reference_configuration += delta_E_groundstate;
      }
    }
  }
}

#endif
