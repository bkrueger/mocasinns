#ifndef TEST_CONFIGURATION_TEST_HPP
#define TEST_CONFIGURATION_TEST_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <spinlattice.hpp>
#include <spin_ising.hpp>

#include <configuration_test.hpp>
#include <random_boost_mt19937.hpp>

using namespace Mocasinns;

class TestConfigurationTest : CppUnit::TestFixture
{
  typedef Ising::SpinLattice<2, Ising::SpinIsing> ConfigurationType;
  typedef Ising::Step<2, Ising::SpinIsing> StepType;
  typedef ConfigurationTest<ConfigurationType, StepType, double, Boost_MT19937> SimulationType;

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
