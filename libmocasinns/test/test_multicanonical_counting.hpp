#ifndef TEST_MULTICANONICAL_COUNTING_HPP
#define TEST_MULTICANONICAL_COUNTING_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <gespinst/spin_lattice.hpp>
#include <gespinst/spins/ising_spin.hpp>

#include <mocasinns/multicanonical_counting.hpp>
#include <mocasinns/wang_landau.hpp>
#include <mocasinns/histograms/histocrete.hpp>
#include <mocasinns/random/boost_random.hpp>

using namespace Mocasinns;

class TestMulticanonicalCounting : CppUnit::TestFixture
{
public:
  typedef Gespinst::SpinLattice<1, Gespinst::IsingSpin> IsingConfiguration1d;
  typedef Gespinst::SpinLatticeStep<1, Gespinst::IsingSpin> IsingStep1d;
  typedef WangLandau<IsingConfiguration1d, IsingStep1d, int, Histograms::Histocrete, Random::Boost_MT19937> OriginalIsingSimulation1d;
  typedef MulticanonicalCounting<IsingConfiguration1d, IsingStep1d, int, Histograms::Histocrete, Random::Boost_MT19937, WangLandau> IsingSimulation1d;
  
  typedef Gespinst::SpinLattice<2, Gespinst::IsingSpin> IsingConfiguration2d;
  typedef Gespinst::SpinLatticeStep<2, Gespinst::IsingSpin> IsingStep2d;
  typedef WangLandau<IsingConfiguration2d, IsingStep2d, int, Histograms::Histocrete, Random::Boost_MT19937> OriginalIsingSimulation2d;
  typedef MulticanonicalCounting<IsingConfiguration2d, IsingStep2d, int, Histograms::Histocrete, Random::Boost_MT19937, WangLandau> IsingSimulation2d;

  typedef Details::Multicanonical::EnergyTypeExtended<int> EnergyTypeExtended;
  typedef Histograms::Histocrete<int, double> HistoType;
  typedef Histograms::Histocrete<EnergyTypeExtended, double> HistoTypeExtended; 
  
private:
  IsingConfiguration1d* test_ising_config_1d;
  IsingSimulation1d* test_ising_simulation_1d;
  IsingConfiguration2d* test_ising_config_2d;
  IsingSimulation2d* test_ising_simulation_2d;

  OriginalIsingSimulation1d::Parameters parameters_1d;
  OriginalIsingSimulation2d::Parameters parameters_2d;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_get_log_density_of_states();
  void test_set_log_density_of_states();
  void test_do_multicanonical_counting_simulation();
};

#endif
