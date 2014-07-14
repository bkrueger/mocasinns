#ifndef TEST_ENTROPIC_SAMPLING_HPP
#define TEST_ENTROPIC_SAMPLING_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <gespinst/spin_lattice.hpp>
#include <gespinst/spins/ising_spin.hpp>

#include <mocasinns/entropic_sampling.hpp>
#include <mocasinns/histograms/histocrete.hpp>
#include <mocasinns/random/boost_random.hpp>

using namespace Mocasinns;

class TestEntropicSampling : CppUnit::TestFixture
{
public:
  typedef Gespinst::SpinLattice<1, Gespinst::IsingSpin> IsingConfiguration1d;
  typedef Gespinst::SpinLatticeStep<1, Gespinst::IsingSpin> IsingStep1d;
  typedef EntropicSampling<IsingConfiguration1d, IsingStep1d, int, Histograms::Histocrete, Random::Boost_MT19937> IsingSimulation1d;
  
  typedef Gespinst::SpinLattice<2, Gespinst::IsingSpin> IsingConfiguration2d;
  typedef Gespinst::SpinLatticeStep<2, Gespinst::IsingSpin> IsingStep2d;
  typedef EntropicSampling<IsingConfiguration2d, IsingStep2d, int, Histograms::Histocrete, Random::Boost_MT19937> IsingSimulation2d;

private:
  IsingConfiguration1d* test_ising_config_1d;
  IsingSimulation1d* test_ising_simulation_1d;
  IsingConfiguration2d* test_ising_config_2d;
  IsingSimulation2d* test_ising_simulation_2d;

  IsingSimulation1d::Parameters parameters_1d;
  IsingSimulation2d::Parameters parameters_2d;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_do_entropic_sampling_steps();
  void test_do_entropic_sampling_simulation();

  void test_serialize();
};

#endif
