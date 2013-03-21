#ifndef TEST_METROPOLIS_PARALLEL_HPP
#define TEST_METROPOLIS_PARALLEL_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <spinlattice.hpp>
#include <spin_ising.hpp>

#include <mocasinns/metropolis.hpp>
#include <mocasinns/metropolis_parallel.hpp>
#include <mocasinns/random/boost_random.hpp>

using namespace Mocasinns;

class TestMetropolisParallel : CppUnit::TestFixture
{
  typedef Ising::SpinLattice<2, Ising::SpinIsing> ConfigurationType;
  typedef Ising::Step<2, Ising::SpinIsing> StepType;
  typedef MetropolisParallel<ConfigurationType, StepType, Random::Boost_MT19937> SimulationType;
  typedef Metropolis<ConfigurationType, StepType, Random::Boost_MT19937> SimulationTypeSerial;

private:
  ConfigurationType* test_config_space;
  SimulationType* test_simulation;
  SimulationType::Parameters test_parameters;

  class ObserveIsingEnergy;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_do_parallel_metropolis_simulation();
};

#endif
