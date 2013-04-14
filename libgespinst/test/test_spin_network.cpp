#include "test_spin_network.hpp"

#include <array>

#include "../../libmocasinns/include/mocasinns/random/boost_random.hpp"

CppUnit::Test* TestSpinNetwork::suite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestSpinNetwork");
    
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetwork>("TestSpinNetwork: test_constructor", &TestSpinNetwork::test_constructor ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetwork>("TestSpinNetwork: test_get_set_neighbours", &TestSpinNetwork::test_get_set_neighbours ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetwork>("TestSpinNetwork: test_get_simulation_time", &TestSpinNetwork::test_get_simulation_time ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetwork>("TestSpinNetwork: test_operator_equal", &TestSpinNetwork::test_operator_equal ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetwork>("TestSpinNetwork: test_operator_access", &TestSpinNetwork::test_operator_access ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetwork>("TestSpinNetwork: test_all_steps", &TestSpinNetwork::test_all_steps ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetwork>("TestSpinNetwork: test_commit", &TestSpinNetwork::test_commit ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetwork>("TestSpinNetwork: test_energy", &TestSpinNetwork::test_energy ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetwork>("TestSpinNetwork: test_magnetization", &TestSpinNetwork::test_magnetization ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetwork>("TestSpinNetwork: test_propose_step", &TestSpinNetwork::test_propose_step ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetwork>("TestSpinNetwork: test_system_size", &TestSpinNetwork::test_system_size ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinNetwork>("TestSpinNetwork: test_serialize", &TestSpinNetwork::test_serialize ) );

    return suiteOfTests;
}

void TestSpinNetwork::setUp()
{
  // Set up the networks
  testnetwork_dynamic = new SpinNetworkDynamic<IsingSpin>(3, IsingSpin(1));
  testnetwork_static = new SpinNetworkStatic<IsingSpin,2>(3, IsingSpin(1));

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
}

void TestSpinNetwork::tearDown()
{
  // Delete the networks
  delete testnetwork_dynamic;
  delete testnetwork_static;
}

void TestSpinNetwork::test_constructor()
{
  // Test that the sizes match
  CPPUNIT_ASSERT(testnetwork_dynamic->system_size() == 3);
  CPPUNIT_ASSERT(testnetwork_static->system_size() == 3);

  // Test that the spins are correct
  CPPUNIT_ASSERT((*testnetwork_dynamic)[0] == IsingSpin(1));
  CPPUNIT_ASSERT((*testnetwork_dynamic)[1] == IsingSpin(1));
  CPPUNIT_ASSERT((*testnetwork_dynamic)[2] == IsingSpin(1));
  CPPUNIT_ASSERT((*testnetwork_static)[0] == IsingSpin(1));
  CPPUNIT_ASSERT((*testnetwork_static)[1] == IsingSpin(1));
  CPPUNIT_ASSERT((*testnetwork_static)[2] == IsingSpin(1));
}
void TestSpinNetwork::test_get_set_neighbours()
{
}
void TestSpinNetwork::test_get_simulation_time()
{
  CPPUNIT_ASSERT_EQUAL(0, testnetwork_dynamic->get_simulation_time());
  CPPUNIT_ASSERT_EQUAL(0, testnetwork_static->get_simulation_time());
}

void TestSpinNetwork::test_operator_equal()
{
  // Copy the configurations and test whether they are equal
  SpinNetwork<IsingSpin, std::vector<IsingSpin*> > testnetwork_dynamic_copied(*testnetwork_dynamic);
  SpinNetwork<IsingSpin, std::array<IsingSpin*, 2> > testnetwork_static_copied(*testnetwork_static);
  CPPUNIT_ASSERT(testnetwork_dynamic_copied == *testnetwork_dynamic);
  CPPUNIT_ASSERT(testnetwork_static_copied == *testnetwork_static);

  // Flip a spin and check that the networks are not equal
  testnetwork_dynamic_copied[0] = IsingSpin(-1);
  testnetwork_static_copied[0] = IsingSpin(-1);
  CPPUNIT_ASSERT(testnetwork_dynamic_copied != *testnetwork_dynamic);
  CPPUNIT_ASSERT(testnetwork_static_copied != *testnetwork_static);
}
void TestSpinNetwork::test_operator_access()
{
  (*testnetwork_dynamic)[0] = IsingSpin(-1);
  CPPUNIT_ASSERT((*testnetwork_dynamic)[0] == IsingSpin(-1));
}

void TestSpinNetwork::test_all_steps()
{
  // Calculate all steps
  std::vector<SpinNetworkStep<IsingSpin, std::vector<IsingSpin*> > > all_steps_dynamic = 
    testnetwork_dynamic->all_steps();
  std::vector<SpinNetworkStep<IsingSpin, std::array<IsingSpin*, 2> > > all_steps_static = 
    testnetwork_static->all_steps();
  
  // Check the size of all steps
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(all_steps_dynamic.size()));
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(all_steps_static.size()));

  // Check that the old and the new spin are correct
  CPPUNIT_ASSERT(all_steps_dynamic[0].get_old_spin() == IsingSpin(1));
  CPPUNIT_ASSERT(all_steps_dynamic[1].get_old_spin() == IsingSpin(1));
  CPPUNIT_ASSERT(all_steps_dynamic[2].get_old_spin() == IsingSpin(1));
  CPPUNIT_ASSERT(all_steps_dynamic[0].get_new_spin() == IsingSpin(-1));
  CPPUNIT_ASSERT(all_steps_dynamic[1].get_new_spin() == IsingSpin(-1));
  CPPUNIT_ASSERT(all_steps_dynamic[2].get_new_spin() == IsingSpin(-1));
  CPPUNIT_ASSERT(all_steps_static[0].get_old_spin() == IsingSpin(1));
  CPPUNIT_ASSERT(all_steps_static[1].get_old_spin() == IsingSpin(1));
  CPPUNIT_ASSERT(all_steps_static[2].get_old_spin() == IsingSpin(1));
  CPPUNIT_ASSERT(all_steps_static[0].get_new_spin() == IsingSpin(-1));
  CPPUNIT_ASSERT(all_steps_static[1].get_new_spin() == IsingSpin(-1));
  CPPUNIT_ASSERT(all_steps_static[2].get_new_spin() == IsingSpin(-1));

  // Define boolean variables indicating whether an index was tried to flip
  bool dynamic_index_0_found = false; bool static_index_0_found = false;
  bool dynamic_index_1_found = false; bool static_index_1_found = false;
  bool dynamic_index_2_found = false; bool static_index_2_found = false;
  if (all_steps_dynamic[0].get_flip_index() == 0) dynamic_index_0_found = true;
  if (all_steps_dynamic[0].get_flip_index() == 1) dynamic_index_1_found = true;
  if (all_steps_dynamic[0].get_flip_index() == 2) dynamic_index_2_found = true;
  if (all_steps_dynamic[1].get_flip_index() == 0) dynamic_index_0_found = true;
  if (all_steps_dynamic[1].get_flip_index() == 1) dynamic_index_1_found = true;
  if (all_steps_dynamic[1].get_flip_index() == 2) dynamic_index_2_found = true;
  if (all_steps_dynamic[2].get_flip_index() == 0) dynamic_index_0_found = true;
  if (all_steps_dynamic[2].get_flip_index() == 1) dynamic_index_1_found = true;
  if (all_steps_dynamic[2].get_flip_index() == 2) dynamic_index_2_found = true;
  if (all_steps_static[0].get_flip_index() == 0) static_index_0_found = true;
  if (all_steps_static[0].get_flip_index() == 1) static_index_1_found = true;
  if (all_steps_static[0].get_flip_index() == 2) static_index_2_found = true;
  if (all_steps_static[1].get_flip_index() == 0) static_index_0_found = true;
  if (all_steps_static[1].get_flip_index() == 1) static_index_1_found = true;
  if (all_steps_static[1].get_flip_index() == 2) static_index_2_found = true;
  if (all_steps_static[2].get_flip_index() == 0) static_index_0_found = true;
  if (all_steps_static[2].get_flip_index() == 1) static_index_1_found = true;
  if (all_steps_static[2].get_flip_index() == 2) static_index_2_found = true;
  // Check wether all indices were found
  CPPUNIT_ASSERT(dynamic_index_0_found);
  CPPUNIT_ASSERT(dynamic_index_1_found);
  CPPUNIT_ASSERT(dynamic_index_2_found);
  CPPUNIT_ASSERT(static_index_0_found);
  CPPUNIT_ASSERT(static_index_1_found);
  CPPUNIT_ASSERT(static_index_2_found);
}
void TestSpinNetwork::test_commit()
{
}
void TestSpinNetwork::test_energy()
{
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, testnetwork_dynamic->energy(), 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, testnetwork_static->energy(), 1e-6);
}
void TestSpinNetwork::test_magnetization()
{
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, testnetwork_dynamic->energy(), 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, testnetwork_static->energy(), 1e-6);
}
void TestSpinNetwork::test_propose_step()
{
  // Create a random number generator
  Mocasinns::Random::Boost_MT19937 rng;
  
  // Define boolean variables indicating whether an index was tried to flip
  bool dynamic_index_0_found = false; bool static_index_0_found = false;
  bool dynamic_index_1_found = false; bool static_index_1_found = false;
  bool dynamic_index_2_found = false; bool static_index_2_found = false;

  // Suggest a lot of steps and test them
  for (unsigned int i = 0; i < 1000; ++i)
  {
    // Propose the two steps
    SpinNetworkStep<IsingSpin, std::vector<IsingSpin*> > step_dynamic
      = testnetwork_dynamic->propose_step(&rng);
    SpinNetworkStep<IsingSpin, std::array<IsingSpin*, 2> > step_static
      = testnetwork_static->propose_step(&rng);

    // Check the old and the new spins
    CPPUNIT_ASSERT(step_dynamic.get_old_spin() == IsingSpin(1));
    CPPUNIT_ASSERT(step_dynamic.get_new_spin() == IsingSpin(-1));
    CPPUNIT_ASSERT(step_static.get_old_spin() == IsingSpin(1));
    CPPUNIT_ASSERT(step_static.get_new_spin() == IsingSpin(-1));

    // Check the range of the indizes
    CPPUNIT_ASSERT(step_dynamic.get_flip_index() <= 2);
    CPPUNIT_ASSERT(step_static.get_flip_index() <= 2);

    // Check the indizes
    if (step_dynamic.get_flip_index() == 0) dynamic_index_0_found = true;
    if (step_dynamic.get_flip_index() == 1) dynamic_index_1_found = true;
    if (step_dynamic.get_flip_index() == 2) dynamic_index_2_found = true;
    if (step_static.get_flip_index() == 0) static_index_0_found = true;
    if (step_static.get_flip_index() == 1) static_index_1_found = true;
    if (step_static.get_flip_index() == 2) static_index_2_found = true;
  }

  CPPUNIT_ASSERT(dynamic_index_0_found);
  CPPUNIT_ASSERT(dynamic_index_1_found);
  CPPUNIT_ASSERT(dynamic_index_2_found);
  CPPUNIT_ASSERT(static_index_0_found);
  CPPUNIT_ASSERT(static_index_1_found);
  CPPUNIT_ASSERT(static_index_2_found);
}

void TestSpinNetwork::test_system_size()
{
  CPPUNIT_ASSERT_EQUAL(3u, testnetwork_dynamic->system_size());
  CPPUNIT_ASSERT_EQUAL(3u, testnetwork_static->system_size());
}

void TestSpinNetwork::test_serialize()
{
}
