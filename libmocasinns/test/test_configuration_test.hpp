#ifndef TEST_CONFIGURATION_TEST_HPP
#define TEST_CONFIGURATION_TEST_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <gespinst/spin_lattice.hpp>
#include <gespinst/spins/ising_spin.hpp>

#include <mocasinns/configuration_test.hpp>
#include <mocasinns/random/boost_random.hpp>

using namespace Mocasinns;

class TestConfigurationTest : CppUnit::TestFixture
{
  typedef Gespinst::SpinLattice<2, Gespinst::IsingSpin> ConfigurationType;
  typedef Gespinst::Step<2, Gespinst::IsingSpin> StepType;
  typedef ConfigurationTest<ConfigurationType, StepType, double, Random::Boost_MT19937> SimulationType;

private:
  ConfigurationType* test_config_space;
  SimulationType* test_simulation;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_test_energy_delta_E();
  void test_test_copy();
  void test_test_serialization();
};

#endif
