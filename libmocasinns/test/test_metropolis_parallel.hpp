#ifndef TEST_METROPOLIS_PARALLEL_HPP
#define TEST_METROPOLIS_PARALLEL_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <gespinst/spin_lattice.hpp>
#include <gespinst/spins/ising_spin.hpp>

#include <mocasinns/metropolis.hpp>
#include <mocasinns/metropolis_parallel.hpp>
#include <mocasinns/random/boost_random.hpp>

using namespace Mocasinns;

class TestMetropolisParallel : CppUnit::TestFixture
{
  typedef Gespinst::SpinLattice<2, Gespinst::SpinIsing> ConfigurationType;
  typedef Gespinst::Step<2, Gespinst::SpinIsing> StepType;
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
