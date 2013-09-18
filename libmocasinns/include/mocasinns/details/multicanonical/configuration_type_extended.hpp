#ifndef MOCASINNS_DETAILS_MULTICANONICAL_CONFIGURATION_TYPE_EXTENDED_HPP
#define MOCASINNS_DETAILS_MULTICANONICAL_CONFIGURATION_TYPE_EXTENDED_HPP

// Header for the serialization of the class
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
// Serialization for vector and map
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

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
      public:
	//! Type of the container of reference configurations of the same energy
	typedef std::vector<ConfigurationType*> reference_configurations_container_t;
	//! Type of the map from the reference energy to vector of reference configurations
	typedef std::map<EnergyType, reference_configurations_container_t> reference_configurations_map_t;

	//! Default constructor for serializing
	ConfigurationTypeExtended() {}
	//! Constructor for start configuration equal one reference configuration
	ConfigurationTypeExtended(ConfigurationType* start_and_reference_configuration);
	//! Constructor for start configuration different from one reference configuration
	ConfigurationTypeExtended(ConfigurationType* start_configuration, ConfigurationType* new_reference_configuration);
	//! Constructor for start configuration different from severel reference configurations
	template <class InputIterator>
	ConfigurationTypeExtended(ConfigurationType* start_configurations, InputIterator reference_configurations_begin, InputIterator reference_configurations_end);
	//! Destructor, deletes the copied reference_configuration
	~ConfigurationTypeExtended();
	
	//! Get-Accessor for the work configuration
	ConfigurationType* get_original_configuration() { return work_configuration; }
	//! Get-Accessor for the reference configurations
	const reference_configurations_map_t& get_reference_configurations() const { return reference_configurations; }
	//! Get-Accessor for the reference configurations of a certain energy
	const reference_configurations_container_t& get_reference_configurations(const EnergyType& energy) const { return reference_configurations->find(energy)->second(); }
	//! Get-Accessor for the flag is reference configuration
	bool get_is_reference_configuration() { return (is_reference_configuration == 1); }

	int get_simulation_time() const { return work_configuration->get_simulation_time(); }
	
	//! Function overriding the commit function of the original configuration, sets the flag whether the system is on reference configuration
	void commit(StepTypeExtended<ConfigurationType, StepType, EnergyType>& step_to_commit);
	//! Function overriding the commit function of the original configuration, does not update the internal tracked energy.
	void commit_testwise(StepTypeExtended<ConfigurationType, StepType, EnergyType>& step_to_commit);

	//! Tests whether a given configuration is in the reference configurations
	bool configuration_is_in_references(ConfigurationType* config) const;
	//! Tests whether a given configuration with given energy is in the reference configurations
	bool configuration_is_in_references(ConfigurationType* config, const EnergyType& config_energy) const;
	
	//! Number of reference configurations
	unsigned int count_reference_configurations() const;
	//! Number of reference configurations with given energy
	unsigned int count_reference_configurations(const EnergyType& energy) const;

	//! Function overriding the energy function of the original configuration
	EnergyTypeExtended<EnergyType> energy();
	
	//! Function overriding the propose step functionality
	template <class RandomNumberGenerator>
	StepTypeExtended<ConfigurationType, StepType, EnergyType> propose_step(RandomNumberGenerator* rng);

	//! Function to modify the internal energy of the configuration space by adding the given energy
	void update_energy(const EnergyType& delta_E_original, const int& delta_E_groundstate);

      private:
	//! Configuration that is used in the simulation
	ConfigurationType* work_configuration;
	//! Map with energies as keys and vectors of configurations as value that are used as reference
	reference_configurations_map_t reference_configurations;
	//! Saved energy of the configuration (to not calculate the energy again and again ...)
	EnergyType current_energy;
	//! Flag that indicates whether this configuration is in a reference configuration
	int is_reference_configuration;

	//! Member variable for boost serialization
	friend class boost::serialization::access;
	//! Method to serialize this class (omitted version name to avoid unused parameter warnings)
	template<class Archive> void serialize(Archive &ar, const unsigned int)
	{
	  ar & work_configuration;
	  ar & reference_configurations;
	  ar & current_energy;
	  ar & is_reference_configuration;
	}
      };
    }
  }
}

#include "../../src/details/multicanonical/configuration_type_extended.cpp"

#endif
