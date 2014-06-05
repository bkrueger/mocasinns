#ifndef TEST_METROPOLIS_HPP
#define TEST_METROPOLIS_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <gespinst/spin_lattice.hpp>
#include <gespinst/spins/ising_spin.hpp>

#include <mocasinns/metropolis.hpp>
#include <mocasinns/random/boost_random.hpp>

using namespace Mocasinns;

class TestMetropolis : CppUnit::TestFixture
{
  typedef Gespinst::SpinLattice<2, Gespinst::IsingSpin> ConfigurationType;
  typedef Gespinst::SpinLatticeStep<2, Gespinst::IsingSpin> StepType;
  typedef Metropolis<ConfigurationType, StepType, Random::Boost_MT19937> SimulationType;

private:
  ConfigurationType* test_config_space;
  SimulationType* test_simulation;
  SimulationType::Parameters test_parameters;

  class ObserveIsingEnergy;
  class ObserveIsingEnergyMagnetization;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_do_metropolis_steps();
  void test_do_metropolis_simulation();
};

#endif
