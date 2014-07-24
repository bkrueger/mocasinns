#ifndef TEST_WANG_LANDAU_REPLICA_EXCHANGE_HPP
#define TEST_WANG_LANDAU_REPLICA_EXCHANGE_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <gespinst/spin_lattice.hpp>
#include <gespinst/spins/ising_spin.hpp>

#include <mocasinns/wang_landau_replica_exchange.hpp>
#include <mocasinns/histograms/histocrete.hpp>
#include <mocasinns/random/boost_random.hpp>

using namespace Mocasinns;

class TestWangLandauReplicaExchange : CppUnit::TestFixture
{
  typedef boost::multi_array<Gespinst::IsingSpin, 1> lattice_array_type_1d;
  typedef boost::array<lattice_array_type_1d::index, 1> index_type_1d;

  typedef Gespinst::SpinLattice<1, Gespinst::IsingSpin> IsingConfiguration1d;
  typedef Gespinst::SpinLatticeStep<1, Gespinst::IsingSpin> IsingStep1d;
  typedef WangLandauReplicaExchange<IsingConfiguration1d, IsingStep1d, int, Histograms::Histocrete, Random::Boost_MT19937> IsingSimulation1d;

private:
  std::vector<IsingConfiguration1d*> test_configuration_spaces_1d;
  IsingSimulation1d* test_ising_simulation_1d;

  IsingSimulation1d::Parameters parameters_1d;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_do_wang_landau_replica_exchange_simulation();
};

#endif
