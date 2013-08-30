#ifndef TEST_CONFIGURATION_TYPE_EXTENDED_HPP
#define TEST_CONFIGURATION_TYPE_EXTENDED_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <gespinst/spin_lattice.hpp>
#include <gespinst/spins/ising_spin.hpp>

#include <mocasinns/details/multicanonical/energy_type_extended.hpp>
#include <mocasinns/details/multicanonical/configuration_type_extended.hpp>
#include <mocasinns/details/multicanonical/step_type_extended.hpp>

using namespace Mocasinns::Details::Multicanonical;

typedef boost::multi_array<Gespinst::IsingSpin, 1> lattice_array_type_1d;
typedef boost::array<lattice_array_type_1d::index, 1> index_type_1d;

typedef EnergyTypeExtended<int> ExtendedInt;
typedef Gespinst::SpinLattice<1, Gespinst::IsingSpin> ConfigurationType;
typedef Gespinst::SpinLatticeStep<1, Gespinst::IsingSpin> StepType;
typedef ConfigurationTypeExtended<ConfigurationType, StepType, int> ExtendedConfigType;
typedef StepTypeExtended<ConfigurationType, StepType, int> ExtendedStepType;

class TestConfigurationTypeExtended : CppUnit::TestFixture
{
private:
  ConfigurationType* test_config_1;
  ConfigurationType* test_config_2;
  ConfigurationType* test_reference_1;
  ConfigurationType* test_reference_2;

  ExtendedConfigType* test_extended_config_1;
  ExtendedConfigType* test_extended_config_2;
  
public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_constructor();
  
  void test_get_reference_configuration();
  void test_get_reference_configuration_energy();
  void test_get_is_reference_configuration();
  
  void test_energy();
  void test_propose_step();
};

#endif
