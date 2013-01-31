/**
 * \file configuration_test.hpp
 * \brief Class for testing a user defined pair of configuration and step class
 * 
 * The following tests are provided:
 * - Does the Step::delta_E()-function match the Configuration::energy()-function
 * - Does the copying of the Configuration lead to an equal configuration
 * - Does the serialisation of the Configuration change the configuration or the ability to do steps
 * 
 * \author Benedikt Krüger
 */

#ifndef CONFIGURATION_TEST_HPP
#define CONFIGURATION_TEST_HPP

#include "simulation.hpp"

namespace Mocasinns
{

  //! Class for testing the user provided pair of Configuration and Step class
  template <class ConfigurationType, class StepType, class EnergyType, class RandomNumberGenerator>
  class ConfigurationTest : public Simulation<ConfigurationType, RandomNumberGenerator>
  {
  public:
    //! Constructor providing a initial configuration upon which the tests are executed
    ConfigurationTest(ConfigurationType* initial_configuration) : Simulation<ConfigurationType, RandomNumberGenerator>(initial_configuration) {}

    //! Test that the functions ConfigurationType::energy() and StepType::delta_E() agree
    bool test_energy_delta_E();

    //! Test that a copied ConfigurationType is equal to the original one (compared with operator==)
    bool test_copy();

    //! Test that the serialization of the ConfigurationType
    bool test_serialization();
  };

} // of namespace Mocasinns

#include "configuration_test.cpp"

#endif
