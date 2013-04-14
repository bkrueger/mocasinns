#ifndef TEST_SPIN_NETWORK_HPP
#define TEST_SPIN_NETWORK_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>
#include <gespinst/spin_network.hpp>
#include <gespinst/spins/ising_spin.hpp>

using namespace Gespinst;

class TestSpinNetwork : public CppUnit::TestFixture
{
private:
  SpinNetworkDynamic<IsingSpin>* testnetwork_dynamic;
  SpinNetworkStatic<IsingSpin,2>* testnetwork_static;

public:
  static CppUnit::Test* suite();

  void setUp();
  void tearDown();

  void test_constructor();

  void test_get_set_neighbours();
  void test_get_simulation_time();

  void test_operator_equal();
  void test_operator_access();

  void test_all_steps();
  void test_commit();
  void test_energy();
  void test_magnetization();
  void test_propose_step();
  void test_system_size();

  void test_serialize();
};

#endif
