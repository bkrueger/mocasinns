#include "test_spin_network_step.hpp"

CppUnit::Test* TestSpinNetworkStep::suite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestSpinNetworkStep");
    
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetworkStep>("TestSpinNetworkStep: test_get_flip_index", &TestSpinNetworkStep::test_get_flip_index ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetworkStep>("TestSpinNetworkStep: test_get_old_spin", &TestSpinNetworkStep::test_get_old_spin ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetworkStep>("TestSpinNetworkStep: test_get_new_spin", &TestSpinNetworkStep::test_get_new_spin ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetworkStep>("TestSpinNetworkStep: test_get_creation_simulation_time", &TestSpinNetworkStep::test_get_creation_simulation_time ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetworkStep>("TestSpinNetworkStep: test_delta_E", &TestSpinNetworkStep::test_delta_E ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetworkStep>("TestSpinNetworkStep: test_execute", &TestSpinNetworkStep::test_execute ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetworkStep>("TestSpinNetworkStep: test_undo", &TestSpinNetworkStep::test_undo ) );

    return suiteOfTests;
}

void TestSpinNetworkStep::setUp()
{
  // Set up the networks
  testnetwork_static = new SpinNetworkStatic<IsingSpin,2>(3, IsingSpin(1));
  testnetwork_dynamic = new SpinNetworkDynamic<IsingSpin>(3, IsingSpin(1));
  
  // Assign some differing spins
  (*testnetwork_static)[0] = IsingSpin(-1);
  (*testnetwork_dynamic)[0] = IsingSpin(-1);

  // Set up the neighbours for the dynamic network
  std::vector<IsingSpin*> neighbours_dynamic_0;
  neighbours_dynamic_0.push_back(&(*testnetwork_dynamic)[1]);
  neighbours_dynamic_0.push_back(&(*testnetwork_dynamic)[2]);
  std::vector<IsingSpin*> neighbours_dynamic_1;
  neighbours_dynamic_1.push_back(&(*testnetwork_dynamic)[0]);
  neighbours_dynamic_1.push_back(&(*testnetwork_dynamic)[2]);
  std::vector<IsingSpin*> neighbours_dynamic_2;
  neighbours_dynamic_2.push_back(&(*testnetwork_dynamic)[0]);
  neighbours_dynamic_2.push_back(&(*testnetwork_dynamic)[1]);
  // Set up the neighbpurs for the static network
  std::array<IsingSpin*, 2> neighbours_static_0;
  neighbours_static_0[0] = &(*testnetwork_dynamic)[1];
  neighbours_static_0[1] = &(*testnetwork_dynamic)[2];
  std::array<IsingSpin*, 2> neighbours_static_1;
  neighbours_static_1[0] = &(*testnetwork_dynamic)[0];
  neighbours_static_1[1] = &(*testnetwork_dynamic)[2];
  std::array<IsingSpin*, 2> neighbours_static_2;
  neighbours_static_2[0] = &(*testnetwork_dynamic)[0];
  neighbours_static_2[1] = &(*testnetwork_dynamic)[1];
  // Assign the neighbours
  testnetwork_dynamic->set_neighbours(0, neighbours_dynamic_0);
  testnetwork_dynamic->set_neighbours(1, neighbours_dynamic_1);
  testnetwork_dynamic->set_neighbours(2, neighbours_dynamic_2);
  testnetwork_static->set_neighbours(0, neighbours_static_0);
  testnetwork_static->set_neighbours(1, neighbours_static_1);
  testnetwork_static->set_neighbours(2, neighbours_static_2);

  // Create the steps
  test_static_step_0 = new StaticStepType(testnetwork_static, 0, IsingSpin(1));
  test_static_step_1 = new StaticStepType(testnetwork_static, 1, IsingSpin(-1));
  test_static_step_2 = new StaticStepType(testnetwork_static, 2, IsingSpin(-1));
  test_dynamic_step_0 = new DynamicStepType(testnetwork_dynamic, 0, IsingSpin(1));
  test_dynamic_step_1 = new DynamicStepType(testnetwork_dynamic, 1, IsingSpin(-1));
  test_dynamic_step_2 = new DynamicStepType(testnetwork_dynamic, 2, IsingSpin(-1));
}
void TestSpinNetworkStep::tearDown()
{
  // Delete the networks
  delete testnetwork_static;
  delete testnetwork_dynamic;

  // Delete the steps
  delete test_static_step_0;
  delete test_static_step_1;
  delete test_static_step_2;
  delete test_dynamic_step_0;
  delete test_dynamic_step_1;
  delete test_dynamic_step_2;
}

void TestSpinNetworkStep::test_get_flip_index()
{
  CPPUNIT_ASSERT_EQUAL(0, static_cast<int>(test_static_step_0->get_flip_index()));
  CPPUNIT_ASSERT_EQUAL(1, static_cast<int>(test_static_step_1->get_flip_index()));
  CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(test_static_step_2->get_flip_index()));
  CPPUNIT_ASSERT_EQUAL(0, static_cast<int>(test_dynamic_step_0->get_flip_index()));
  CPPUNIT_ASSERT_EQUAL(1, static_cast<int>(test_dynamic_step_1->get_flip_index()));
  CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(test_dynamic_step_2->get_flip_index()));
}
void TestSpinNetworkStep::test_get_old_spin()
{
  CPPUNIT_ASSERT(test_static_step_0->get_old_spin() == IsingSpin(-1));
  CPPUNIT_ASSERT(test_static_step_1->get_old_spin() == IsingSpin(1));
  CPPUNIT_ASSERT(test_static_step_2->get_old_spin() == IsingSpin(1));
  CPPUNIT_ASSERT(test_dynamic_step_0->get_old_spin() == IsingSpin(-1));
  CPPUNIT_ASSERT(test_dynamic_step_1->get_old_spin() == IsingSpin(1));
  CPPUNIT_ASSERT(test_dynamic_step_2->get_old_spin() == IsingSpin(1));
}
void TestSpinNetworkStep::test_get_new_spin()
{
  CPPUNIT_ASSERT(test_static_step_0->get_new_spin() == IsingSpin(1));
  CPPUNIT_ASSERT(test_static_step_1->get_new_spin() == IsingSpin(-1));
  CPPUNIT_ASSERT(test_static_step_2->get_new_spin() == IsingSpin(-1));
  CPPUNIT_ASSERT(test_dynamic_step_0->get_new_spin() == IsingSpin(1));
  CPPUNIT_ASSERT(test_dynamic_step_1->get_new_spin() == IsingSpin(-1));
  CPPUNIT_ASSERT(test_dynamic_step_2->get_new_spin() == IsingSpin(-1));
}
void TestSpinNetworkStep::test_get_creation_simulation_time()
{
  CPPUNIT_ASSERT_EQUAL(0, test_static_step_0->get_creation_simulation_time());
  CPPUNIT_ASSERT_EQUAL(0, test_static_step_1->get_creation_simulation_time());
  CPPUNIT_ASSERT_EQUAL(0, test_static_step_2->get_creation_simulation_time());
  CPPUNIT_ASSERT_EQUAL(0, test_dynamic_step_0->get_creation_simulation_time());
  CPPUNIT_ASSERT_EQUAL(0, test_dynamic_step_1->get_creation_simulation_time());
  CPPUNIT_ASSERT_EQUAL(0, test_dynamic_step_2->get_creation_simulation_time());
}

void TestSpinNetworkStep::test_delta_E()
{
  CPPUNIT_ASSERT_EQUAL(-4.0, test_static_step_0->delta_E());
  CPPUNIT_ASSERT_EQUAL(0.0, test_static_step_1->delta_E());
  CPPUNIT_ASSERT_EQUAL(0.0, test_static_step_2->delta_E());
  CPPUNIT_ASSERT_EQUAL(-4.0, test_dynamic_step_0->delta_E());
  CPPUNIT_ASSERT_EQUAL(0.0, test_dynamic_step_1->delta_E());
  CPPUNIT_ASSERT_EQUAL(0.0, test_dynamic_step_2->delta_E());
}
void TestSpinNetworkStep::test_execute()
{
  // Execute the first step and test that the steps have the same value
  test_static_step_1->execute();
  test_dynamic_step_1->execute();

  CPPUNIT_ASSERT((*testnetwork_static)[0] == IsingSpin(-1));
  CPPUNIT_ASSERT((*testnetwork_static)[1] == IsingSpin(-1));
  CPPUNIT_ASSERT((*testnetwork_static)[2] == IsingSpin(1));
  CPPUNIT_ASSERT((*testnetwork_dynamic)[0] == IsingSpin(-1));
  CPPUNIT_ASSERT((*testnetwork_dynamic)[1] == IsingSpin(-1));
  CPPUNIT_ASSERT((*testnetwork_dynamic)[2] == IsingSpin(1));
}

void TestSpinNetworkStep::test_undo()
{
  // Execute the step at index 0
  test_static_step_0->execute();
  test_static_step_0->undo();
  test_dynamic_step_0->execute();
  test_dynamic_step_0->undo();

  // Assert that everything is as before
  CPPUNIT_ASSERT((*testnetwork_static)[0] == IsingSpin(-1));
  CPPUNIT_ASSERT((*testnetwork_static)[1] == IsingSpin(1));
  CPPUNIT_ASSERT((*testnetwork_static)[2] == IsingSpin(1));
  CPPUNIT_ASSERT((*testnetwork_dynamic)[0] == IsingSpin(-1));
  CPPUNIT_ASSERT((*testnetwork_dynamic)[1] == IsingSpin(1));
  CPPUNIT_ASSERT((*testnetwork_dynamic)[2] == IsingSpin(1));
  // Test that the simulation time has changed
  CPPUNIT_ASSERT_EQUAL(2, testnetwork_static->get_simulation_time());
  CPPUNIT_ASSERT_EQUAL(2, testnetwork_dynamic->get_simulation_time());
}
