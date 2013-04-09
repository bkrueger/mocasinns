#include "test_spin_step.hpp"

CppUnit::Test* TestStep::suite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestStep");
    
    suiteOfTests->addTest( new CppUnit::TestCaller<TestStep>("TestStep: test_constructor", &TestStep::test_constructor ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestStep>("TestStep: test_get_flip_index", &TestStep::test_get_flip_index ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestStep>("TestStep: test_get_old_spin", &TestStep::test_get_old_spin ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestStep>("TestStep: test_get_new_spin", &TestStep::test_get_new_spin ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestStep>("TestStep: test_get_creation_simulation_time", &TestStep::test_get_creation_simulation_time ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestStep>("TestStep: test_delta_E", &TestStep::test_delta_E ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestStep>("TestStep: test_execute", &TestStep::test_execute ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestStep>("TestStep: test_undo", &TestStep::test_undo ) );

    return suiteOfTests;
}

void TestStep::setUp()
{
  // Assign the IsingSpins
  spin_up = SpinIsing(1);
  spin_down = SpinIsing(-1);

  // Create the size vectors
  std::vector<unsigned int> size_1d;
  size_1d.push_back(5);
  std::vector<unsigned int> size_2d;
  size_2d.push_back(3); size_2d.push_back(3);

  // Set up the lattices
  testlattice_1d = new SpinLattice<1, SpinIsing>(size_1d);
  testlattice_2d = new SpinLattice<2, SpinIsing>(size_2d);

  // Set up accessing indices
  index_0[0] = 0;
  index_1[0] = 1;
  index_2[0] = 2;
  index_3[0] = 3;
  index_4[0] = 4;
  index_00[0] = 0; index_00[1] = 0;
  index_01[0] = 0; index_01[1] = 1;
  index_02[0] = 0; index_02[1] = 2;
  index_10[0] = 1; index_10[1] = 0;
  index_11[0] = 1; index_11[1] = 1;
  index_12[0] = 1; index_12[1] = 2;
  index_20[0] = 2; index_20[1] = 0;
  index_21[0] = 2; index_21[1] = 1;
  index_22[0] = 2; index_22[1] = 2;

  // Set up some spins
  testlattice_1d->set_spin(index_0, spin_up);
  testlattice_1d->set_spin(index_1, spin_up);
  testlattice_1d->set_spin(index_2, spin_down);
  testlattice_1d->set_spin(index_3, spin_down);  
  testlattice_1d->set_spin(index_4, spin_up);
  testlattice_2d->set_spin(index_00, spin_up);
  testlattice_2d->set_spin(index_01, spin_up);
  testlattice_2d->set_spin(index_02, spin_down);
  testlattice_2d->set_spin(index_10, spin_down);
  testlattice_2d->set_spin(index_11, spin_up);
  testlattice_2d->set_spin(index_12, spin_down);
  testlattice_2d->set_spin(index_20, spin_down);
  testlattice_2d->set_spin(index_21, spin_down);
  testlattice_2d->set_spin(index_22, spin_up);

  // Set up the teststeps
  teststep_1d_0 = new Step<1, SpinIsing>(testlattice_1d, index_0, spin_down);
  teststep_1d_1 = new Step<1, SpinIsing>(testlattice_1d, index_1, spin_down);
  teststep_1d_2 = new Step<1, SpinIsing>(testlattice_1d, index_2, spin_up);
  teststep_1d_3 = new Step<1, SpinIsing>(testlattice_1d, index_3, spin_up);
  teststep_1d_4 = new Step<1, SpinIsing>(testlattice_1d, index_4, spin_down);
  teststep_2d_00 = new Step<2, SpinIsing>(testlattice_2d, index_00, spin_down);
  teststep_2d_01 = new Step<2, SpinIsing>(testlattice_2d, index_01, spin_down);
  teststep_2d_02 = new Step<2, SpinIsing>(testlattice_2d, index_02, spin_up);
  teststep_2d_10 = new Step<2, SpinIsing>(testlattice_2d, index_10, spin_up);
  teststep_2d_11 = new Step<2, SpinIsing>(testlattice_2d, index_11, spin_down);
  teststep_2d_12 = new Step<2, SpinIsing>(testlattice_2d, index_12, spin_up);
  teststep_2d_20 = new Step<2, SpinIsing>(testlattice_2d, index_20, spin_up);
  teststep_2d_21 = new Step<2, SpinIsing>(testlattice_2d, index_21, spin_up);
  teststep_2d_22 = new Step<2, SpinIsing>(testlattice_2d, index_22, spin_down);
}
void TestStep::tearDown()
{
  delete testlattice_1d;
  delete testlattice_2d;
}

void TestStep::test_constructor()
{
  Step<1, SpinIsing>* teststep_1d = new Step<1, SpinIsing>(testlattice_1d, index_0, spin_down);
}

void TestStep::test_get_flip_index()
{
  CPPUNIT_ASSERT(teststep_1d_0->get_flip_index() == index_0);
  CPPUNIT_ASSERT(teststep_1d_1->get_flip_index() == index_1);
  CPPUNIT_ASSERT(teststep_1d_2->get_flip_index() == index_2);
  CPPUNIT_ASSERT(teststep_1d_3->get_flip_index() == index_3);
  CPPUNIT_ASSERT(teststep_1d_4->get_flip_index() == index_4);
  CPPUNIT_ASSERT(teststep_2d_00->get_flip_index() == index_00);
  CPPUNIT_ASSERT(teststep_2d_01->get_flip_index() == index_01);
  CPPUNIT_ASSERT(teststep_2d_02->get_flip_index() == index_02);
  CPPUNIT_ASSERT(teststep_2d_10->get_flip_index() == index_10);
  CPPUNIT_ASSERT(teststep_2d_11->get_flip_index() == index_11);
  CPPUNIT_ASSERT(teststep_2d_12->get_flip_index() == index_12);
  CPPUNIT_ASSERT(teststep_2d_20->get_flip_index() == index_20);
  CPPUNIT_ASSERT(teststep_2d_21->get_flip_index() == index_21);
  CPPUNIT_ASSERT(teststep_2d_22->get_flip_index() == index_22);
}
void TestStep::test_get_old_spin()
{
  CPPUNIT_ASSERT(teststep_1d_0->get_old_spin() == spin_up);
  CPPUNIT_ASSERT(teststep_1d_1->get_old_spin() == spin_up);
  CPPUNIT_ASSERT(teststep_1d_2->get_old_spin() == spin_down);
  CPPUNIT_ASSERT(teststep_1d_3->get_old_spin() == spin_down);
  CPPUNIT_ASSERT(teststep_1d_4->get_old_spin() == spin_up);
  CPPUNIT_ASSERT(teststep_2d_00->get_old_spin() == spin_up);
  CPPUNIT_ASSERT(teststep_2d_01->get_old_spin() == spin_up);
  CPPUNIT_ASSERT(teststep_2d_02->get_old_spin() == spin_down);
  CPPUNIT_ASSERT(teststep_2d_10->get_old_spin() == spin_down);
  CPPUNIT_ASSERT(teststep_2d_11->get_old_spin() == spin_up);
  CPPUNIT_ASSERT(teststep_2d_12->get_old_spin() == spin_down);
  CPPUNIT_ASSERT(teststep_2d_20->get_old_spin() == spin_down);
  CPPUNIT_ASSERT(teststep_2d_21->get_old_spin() == spin_down);
  CPPUNIT_ASSERT(teststep_2d_22->get_old_spin() == spin_up);
}
void TestStep::test_get_new_spin()
{
  CPPUNIT_ASSERT(teststep_1d_0->get_new_spin() == spin_down);
  CPPUNIT_ASSERT(teststep_1d_1->get_new_spin() == spin_down);
  CPPUNIT_ASSERT(teststep_1d_2->get_new_spin() == spin_up);
  CPPUNIT_ASSERT(teststep_1d_3->get_new_spin() == spin_up);
  CPPUNIT_ASSERT(teststep_1d_4->get_new_spin() == spin_down);
  CPPUNIT_ASSERT(teststep_2d_00->get_new_spin() == spin_down);
  CPPUNIT_ASSERT(teststep_2d_01->get_new_spin() == spin_down);
  CPPUNIT_ASSERT(teststep_2d_02->get_new_spin() == spin_up);
  CPPUNIT_ASSERT(teststep_2d_10->get_new_spin() == spin_up);
  CPPUNIT_ASSERT(teststep_2d_11->get_new_spin() == spin_down);
  CPPUNIT_ASSERT(teststep_2d_12->get_new_spin() == spin_up);
  CPPUNIT_ASSERT(teststep_2d_20->get_new_spin() == spin_up);
  CPPUNIT_ASSERT(teststep_2d_21->get_new_spin() == spin_up);
  CPPUNIT_ASSERT(teststep_2d_22->get_new_spin() == spin_down);
}
void TestStep::test_get_creation_simulation_time()
{
  CPPUNIT_ASSERT_EQUAL(0, teststep_1d_0->get_creation_simulation_time());
}

void TestStep::test_delta_E()
{
  CPPUNIT_ASSERT_EQUAL(4.0, teststep_1d_0->delta_E());
  CPPUNIT_ASSERT_EQUAL(0.0, teststep_1d_1->delta_E());
  CPPUNIT_ASSERT_EQUAL(0.0, teststep_1d_2->delta_E());
  CPPUNIT_ASSERT_EQUAL(0.0, teststep_1d_3->delta_E());
  CPPUNIT_ASSERT_EQUAL(0.0, teststep_1d_4->delta_E());
  CPPUNIT_ASSERT_EQUAL(-4.0, teststep_2d_00->delta_E());
  CPPUNIT_ASSERT_EQUAL(0.0, teststep_2d_01->delta_E());
  CPPUNIT_ASSERT_EQUAL(-4.0, teststep_2d_02->delta_E());
  CPPUNIT_ASSERT_EQUAL(0.0, teststep_2d_10->delta_E());
  CPPUNIT_ASSERT_EQUAL(-4.0, teststep_2d_11->delta_E());
  CPPUNIT_ASSERT_EQUAL(0.0, teststep_2d_12->delta_E());
  CPPUNIT_ASSERT_EQUAL(0.0, teststep_2d_20->delta_E());
  CPPUNIT_ASSERT_EQUAL(-4.0, teststep_2d_21->delta_E());
  CPPUNIT_ASSERT_EQUAL(-8.0, teststep_2d_22->delta_E());
}
void TestStep::test_execute()
{
  // Execute the 1d step
  teststep_1d_0->execute();

  // Test that the spin has the correct new value and that the energy us correct
  CPPUNIT_ASSERT(testlattice_1d->get_spin(index_0) == SpinIsing(-1));
  CPPUNIT_ASSERT_EQUAL(3.0, testlattice_1d->energy());
  // Test that the triangulation has the right simulation time
  CPPUNIT_ASSERT_EQUAL(1, testlattice_1d->get_simulation_time());

  // Execute the 2d step
  teststep_2d_10->execute();
  
  // Test that the spin has the correct new value and that the energy is correct
  CPPUNIT_ASSERT(testlattice_2d->get_spin(index_10) == SpinIsing(1));
  CPPUNIT_ASSERT_EQUAL(6.0, testlattice_2d->energy());
  // Test that the triangulation has the right simulation time
  CPPUNIT_ASSERT_EQUAL(1, testlattice_2d->get_simulation_time());
}

void TestStep::test_undo()
{
  // Execute the 1d step
  teststep_1d_0->execute();
  // Undo the 1d step
  teststep_1d_0->undo();

  // Test that the spin has the correct old value and that the energy us correct
  CPPUNIT_ASSERT(testlattice_1d->get_spin(index_0) == SpinIsing(1));
  CPPUNIT_ASSERT_EQUAL(-1.0, testlattice_1d->energy());
  // Test that the triangulation has the right simulation time
  CPPUNIT_ASSERT_EQUAL(2, testlattice_1d->get_simulation_time());

  // Execute the 2d step
  teststep_2d_10->execute();
  // Undo the 2d step
  teststep_2d_10->undo();
  
  // Test that the spin has the correct new value and that the energy is correct
  CPPUNIT_ASSERT(testlattice_2d->get_spin(index_10) == SpinIsing(-1));
  CPPUNIT_ASSERT_EQUAL(6.0, testlattice_2d->energy());
  // Test that the triangulation has the right simulation time
  CPPUNIT_ASSERT_EQUAL(2, testlattice_2d->get_simulation_time());
}
