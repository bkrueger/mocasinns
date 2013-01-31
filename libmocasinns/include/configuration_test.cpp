/*!
 * \file configuration_test.cpp
 * \brief Implementation of the class template ConfigurationTest
 * 
 * \author Benedikt Krüger
 */

#ifdef CONFIGURATION_TEST_HPP

// Boost classes for the serialization test
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>

namespace Mocasinns
{
  template <class ConfigurationType, class StepType, class EnergyType, class RandomNumberGenerator>
  bool ConfigurationTest<ConfigurationType, StepType, EnergyType, RandomNumberGenerator>::test_energy_delta_E()
  {
    // Do 1000 steps and test that the delta_E of executable steps and the energy before and after the step match
    for (unsigned int i = 0; i < 1000; ++i)
    {
      StepType step_to_do = this->configuration_space->propose_step(this->rng);
      if (step_to_do.is_executable())
      {
	// Calculate the energy difference of the step and the energy before the step
	EnergyType step_delta_E = step_to_do.delta_E();
	EnergyType energy_before_step = this->configuration_space->energy();
	
	// Execute the step and calculate the energy after the step
	step_to_do.execute();
	EnergyType energy_after_step = this->configuration_space->energy();

	// Compare the energies
	if (energy_before_step + step_delta_E != energy_after_step) return false;
      }
    }

    // If all energies matched, return true
    return true;
  }

  template <class ConfigurationType, class StepType, class EnergyType, class RandomNumberGenerator>
  bool ConfigurationTest<ConfigurationType, StepType, EnergyType, RandomNumberGenerator>::test_copy()
  {
    // Create a copy of the actual configuration space
    ConfigurationType configuration_space_copied(*this->configuration_space);

    // Test that these two configuration spaces match
    if (configuration_space_copied == (*this->configuration_space))
      return true;
    else return false;
  }

  template <class ConfigurationType, class StepType, class EnergyType, class RandomNumberGenerator>
  bool ConfigurationTest<ConfigurationType, StepType, EnergyType, RandomNumberGenerator>::test_serialization()
  {
    // Create a stringstream to serialize in
    std::stringstream serialize_stream;
    // Serialize the actual configuration space
    {
      boost::archive::text_oarchive output_archive(serialize_stream);
      output_archive << *this->configuration_space;
    }

    // Create an empty object to serialize in
    ConfigurationType configuration_space_loaded;
    // Deserialize the actual configuration space
    {
      boost::archive::text_iarchive input_archive(serialize_stream);
      input_archive >> configuration_space_loaded;
    }

    // Test that the original configuration space and the loaded configuration space match
    if (configuration_space_loaded == *(this->configuration_space))
      return true;
    else
      return false;
  }

} // of namespace Mocasinns

#endif
