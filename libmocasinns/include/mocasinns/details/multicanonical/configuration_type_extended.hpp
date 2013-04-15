#ifndef MOCASINNS_DETAILS_MULTICANONICAL_CONFIGURATION_TYPE_EXTENDED_HPP
#define MOCASINNS_DETAILS_MULTICANONICAL_CONFIGURATION_TYPE_EXTENDED_HPP

namespace Mocasinns
{
  namespace Details
  {
    namespace Multicanonical
    {
      // Forward declaration of the extended step type
      template <class ConfigurationType, class StepType, class EnergyType>
      class StepTypeExtended;
      
      //! Class extending the ConfigurationType to a class with modified energy
      template <class ConfigurationType, class StepType, class EnergyType>
      class ConfigurationTypeExtended
      {
      private:
	//! Configuration that is used in the simulation
	ConfigurationType* work_configuration;
	//! Configuration that is used as reference
	ConfigurationType* reference_configuration;
	//! Energy of the reference configuration
	EnergyType reference_configuration_energy;
	//! Saved energy of the configuration (to not calculate the energy again and again ...)
	EnergyType current_energy;
	//! Flag that indicates whether this configuration is the reference configuration
	int is_reference_configuration;
	
      public:
	//! Constructor for start configuration equal the reference configuration
	ConfigurationTypeExtended(ConfigurationType* start_and_reference_configuration);
	//! Constructor for start configuration different from reference configuration
	ConfigurationTypeExtended(ConfigurationType* start_configuration, ConfigurationType* new_reference_configuration);
	//! Destructor, deletes the copied reference_configuration
	~ConfigurationTypeExtended();
	
	//! Get-Accessor for the work configuration
	ConfigurationType* get_original_configuration() { return work_configuration; }
	//! Get-Accessor for the reference configuration
	ConfigurationType* get_reference_configuration() { return reference_configuration; }
	//! Get-Accessor for the energy of the reference configuration
	const EnergyType& get_reference_configuration_energy() { return reference_configuration_energy; }
	//! Get-Accessor for the flag is reference configuration
	bool get_is_reference_configuration() { return (is_reference_configuration == 1); }
	
	//! Function overriding the commit function of the original configuration, sets the flag whether the system is on reference configuration
	void commit(StepTypeExtended<ConfigurationType, StepType, EnergyType>& step_to_commit);
	
	//! Function overriding the energy function of the original configuration
	EnergyTypeExtended<EnergyType> energy();
	
	//! Function overriding the propose step functionality
	template <class RandomNumberGenerator>
	StepTypeExtended<ConfigurationType, StepType, EnergyType> propose_step(RandomNumberGenerator* rng);
      };
    }
  }
}

#include "../../src/details/multicanonical/configuration_type_extended.cpp"

#endif
