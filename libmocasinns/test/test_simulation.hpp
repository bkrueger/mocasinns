#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <spinlattice.hpp>
#include <spin_ising.hpp>

#include <simulation.hpp>
#include <random_boost_mt19937.hpp>

#ifndef TEST_SIMULATION_HPP
#define TEST_SIMULATION_HPP

class TestSimulation : CppUnit::TestFixture
{
  typedef SpinLattice<2, SpinIsing> ConfigurationType;
  typedef Step<2, SpinIsing> StepType;
  typedef Simulation<ConfigurationType, Boost_MT19937> SimulationType;

private:
  ConfigurationType* test_config_space;
  SimulationType* test_simulation;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_serialize();
};

#endif
