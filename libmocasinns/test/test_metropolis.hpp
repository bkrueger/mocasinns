#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <spinlattice.hpp>
#include <spin_ising.hpp>

#include <metropolis.hpp>
#include <random_boost_mt19937.hpp>

#ifndef TEST_METROPOLIS_HPP
#define TEST_METROPOLIS_HPP

class TestMetropolis : CppUnit::TestFixture
{
  typedef SpinLattice<2, SpinIsing> ConfigurationType;
  typedef Step<2, SpinIsing> StepType;
  typedef Metropolis<ConfigurationType, StepType, Boost_MT19937> SimulationType;

private:
  ConfigurationType* test_config_space;
  SimulationType* test_simulation;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_do_metropolis_steps();
};

#endif
