#ifdef MOCASINNS_DETAILS_MULTICANONICAL_CONFIGURATION_TYPE_EXTENDED_HPP

namespace Mocasinns
{
  namespace Details
  {
    namespace Multicanonical
    {
      template <class ConfigurationType, class StepType, class EnergyType>
      ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::ConfigurationTypeExtended(ConfigurationType* start_and_reference_configuration)
	: work_configuration(start_and_reference_configuration)
      {
	reference_configurations.insert(typename reference_configurations_map_t::value_type(work_configuration->energy(), reference_configurations_container_t(1, new ConfigurationType(*start_and_reference_configuration))));
	current_energy = work_configuration->energy();
	is_reference_configuration = 1;
      }

      template <class ConfigurationType, class StepType, class EnergyType>
      ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::ConfigurationTypeExtended(ConfigurationType* start_configuration, ConfigurationType* new_reference_configuration)
	: work_configuration(start_configuration)
      {
	reference_configurations.insert(typename reference_configurations_map_t::value_type(new_reference_configuration->energy(), reference_configurations_container_t(1, new ConfigurationType(*new_reference_configuration))));

	current_energy = work_configuration->energy();
	if (configuration_is_in_references(work_configuration, current_energy))
	  is_reference_configuration = 1;
	else
	  is_reference_configuration = 0;
      }
	
      template <class ConfigurationType, class StepType, class EnergyType>
      template <class InputIterator>
      ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::ConfigurationTypeExtended(ConfigurationType* start_configuration, InputIterator reference_configurations_begin, InputIterator reference_configurations_end)
	: work_configuration(start_configuration)
      {
	// Copy and store the reference configurations
	for (InputIterator reference_configuration = reference_configurations_begin;
	     reference_configuration != reference_configurations_end; ++reference_configuration)
	{
	  // If the energy is not allready contained, insert a new container
	  if (reference_configurations.find((*reference_configuration)->energy()) == reference_configurations.end())
	    reference_configurations.insert(typename reference_configurations_map_t::value_type((*reference_configuration)->energy(), reference_configurations_container_t(1, new ConfigurationType(**reference_configuration))));
	  // If the energy is allready contained, insert into the container (if not allready in references to avoid double checks)
	  else
	    if (!configuration_is_in_references(*reference_configuration))
	      reference_configurations[(*reference_configuration)->energy()].push_back(new ConfigurationType(**reference_configuration));
	}

	current_energy = work_configuration->energy();
	if (configuration_is_in_references(work_configuration, current_energy))
	  is_reference_configuration = 1;
	else
	  is_reference_configuration = 0;
      }
      
      template <class ConfigurationType, class StepType, class EnergyType>
      ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::~ConfigurationTypeExtended()
      {
	for (typename reference_configurations_map_t::const_iterator map_it = reference_configurations.begin();
	     map_it != reference_configurations.end(); ++map_it)
	{
	  for (typename reference_configurations_container_t::const_iterator reference_configuration = map_it->second.begin();
	       reference_configuration != map_it->second.end(); ++reference_configuration)
	    delete *reference_configuration;
	}
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
      bool ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::configuration_is_in_references(ConfigurationType* config) const
      {
	return configuration_is_in_references(config, config->energy());
      }

      template <class ConfigurationType, class StepType, class EnergyType>
      bool ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::configuration_is_in_references(ConfigurationType* config, const EnergyType& config_energy) const
      {
	typename reference_configurations_map_t::const_iterator current_energy_it = reference_configurations.find(config_energy);
	// Test whether the energy is a reference energy
	if (current_energy_it != reference_configurations.end())
	{
	  const reference_configurations_container_t& energy_ref_configs = current_energy_it->second;
	  for (typename reference_configurations_container_t::const_iterator reference = energy_ref_configs.begin();
	       reference != energy_ref_configs.end(); ++reference)
	    if (**reference == *config) return true;
	  
	  return false;
	}
	else // current energy is no reference energy
	  return false;
      }

      template <class ConfigurationType, class StepType, class EnergyType>
      unsigned int ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::count_reference_configurations(const EnergyType& energy) const
      {
	typename reference_configurations_map_t::const_iterator it = reference_configurations.find(energy);
	if (it == reference_configurations.end())
	  return 0;
	else
	  return it->second.size();
      }
      template <class ConfigurationType, class StepType, class EnergyType>
      unsigned int ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>::count_reference_configurations() const
      {
	unsigned int count = 0;
	for (typename reference_configurations_map_t::const_iterator map_it = reference_configurations.begin();
	     map_it != reference_configurations.end(); ++map_it)
	  count += map_it->second.size();
	return count;
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
