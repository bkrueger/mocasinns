#ifndef TEST_METROPOLIS_HPP
#define TEST_METROPOLIS_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <spinlattice.hpp>
#include <spin_ising.hpp>

#include <metropolis.hpp>
#include <random_boost_mt19937.hpp>

using namespace Mocasinns;

class TestMetropolis : CppUnit::TestFixture
{
  typedef Ising::SpinLattice<2, Ising::SpinIsing> ConfigurationType;
  typedef Ising::Step<2, Ising::SpinIsing> StepType;
  typedef Metropolis<ConfigurationType, StepType, Boost_MT19937> SimulationType;

private:
  ConfigurationType* test_config_space;
  SimulationType* test_simulation;
  SimulationType::Parameters test_parameters;

  class ObserveIsingEnergy;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_do_metropolis_steps();
  void test_do_metropolis_simulation();
  void test_integrated_autocorrelation_time();
};

#endif
