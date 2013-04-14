#ifndef TEST_SPIN_NETWORK_STEP_HPP
#define TEST_SPIN_NETWORK_STEP_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>
#include <gespinst/spin_network_step.hpp>
#include <gespinst/spin_network.hpp>
#include <gespinst/spins/ising_spin.hpp>

using namespace Gespinst;
class TestSpinNetworkStep : public CppUnit::TestFixture
{
private:
  typedef SpinNetworkStep<IsingSpin, std::array<IsingSpin*, 2> > StaticStepType;
  typedef SpinNetworkStep<IsingSpin, std::vector<IsingSpin*> > DynamicStepType;

  SpinNetworkStatic<IsingSpin, 2>* testnetwork_static;
  SpinNetworkDynamic<IsingSpin>* testnetwork_dynamic;

  StaticStepType* test_static_step_0;
  StaticStepType* test_static_step_1;
  StaticStepType* test_static_step_2;

  DynamicStepType* test_dynamic_step_0;
  DynamicStepType* test_dynamic_step_1;
  DynamicStepType* test_dynamic_step_2;

public:
  static CppUnit::Test* suite();

  void setUp();
  void tearDown();

  void test_get_flip_index();
  void test_get_old_spin();
  void test_get_new_spin();
  void test_get_creation_simulation_time();

  void test_delta_E();
  void test_execute();
  void test_undo();
};

#endif
