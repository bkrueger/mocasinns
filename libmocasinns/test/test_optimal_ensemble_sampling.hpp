#ifndef TEST_OPTIMAL_ENSEMBLE_SAMPLING_HPP
#define TEST_OPTIMAL_ENSEMBLE_SAMPLING_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <spinlattice.hpp>
#include <spin_ising.hpp>

#include <optimal_ensemble_sampling.hpp>
#include <histograms/histocrete.hpp>
#include <random_boost_mt19937.hpp>

using namespace Mocasinns;

class TestOptimalEnsembleSampling : CppUnit::TestFixture
{
public:
  typedef Ising::SpinLattice<1, Ising::SpinIsing> IsingConfiguration1d;
  typedef Ising::Step<1, Ising::SpinIsing> IsingStep1d;
  typedef OptimalEnsembleSampling<IsingConfiguration1d, IsingStep1d, int, Histograms::Histocrete, Boost_MT19937> IsingSimulation1d;
  
  typedef Ising::SpinLattice<2, Ising::SpinIsing> IsingConfiguration2d;
  typedef Ising::Step<2, Ising::SpinIsing> IsingStep2d;
  typedef OptimalEnsembleSampling<IsingConfiguration2d, IsingStep2d, int, Histograms::Histocrete, Boost_MT19937> IsingSimulation2d;

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

  void test_do_optimal_ensemble_sampling_simulation();
};

#endif
