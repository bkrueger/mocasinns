#include "test_spin_lattice.hpp"

#include "boost/multi_array.hpp"

#include <cstdlib>
#include <sstream>

CppUnit::Test* TestSpinLattice::suite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestSpinLattice");
    
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinLattice>("TestSpinLattice: test_constructor", &TestSpinLattice::test_constructor ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinLattice>("TestSpinLattice: test_get_set_spin", &TestSpinLattice::test_get_set_spin ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinLattice>("TestSpinLattice: test_get_simulation_time", &TestSpinLattice::test_get_simulation_time ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinLattice>("TestSpinLattice: test_operator_equal", &TestSpinLattice::test_operator_equal ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinLattice>("TestSpinLattice: test_operator_equal_automatically", &TestSpinLattice::test_operator_equal ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinLattice>("TestSpinLattice: test_operator_access", &TestSpinLattice::test_operator_access ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinLattice>("TestSpinLattice: test_all_steps", &TestSpinLattice::test_all_steps ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinLattice>("TestSpinLattice: test_commit", &TestSpinLattice::test_commit ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinLattice>("TestSpinLattice: test_energy", &TestSpinLattice::test_energy ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinLattice>("TestSpinLattice: test_magnetization", &TestSpinLattice::test_magnetization ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinLattice>("TestSpinLattice: test_next_neighbours", &TestSpinLattice::test_next_neighbours ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinLattice>("TestSpinLattice: test_propose_step", &TestSpinLattice::test_propose_step ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinLattice>("TestSpinLattice: test_system_size", &TestSpinLattice::test_system_size ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinLattice>("TestSpinLattice: test_serialize", &TestSpinLattice::test_serialize ) );

    return suiteOfTests;
}

void TestSpinLattice::setUp()
{
  // Create the size vectors
  std::vector<unsigned int> size_1d;
  size_1d.push_back(5);
  std::vector<unsigned int> size_2d;
  size_2d.push_back(3); size_2d.push_back(3);

  // Set up the lattices
  testlattice_1d = new SpinLattice<1, IsingSpin>(size_1d);
  testlattice_2d = new SpinLattice<2, IsingSpin>(size_2d);
  testlattice_2d_potts = new SpinLattice<2, PottsSpin>(size_2d);
  testlattice_1d_real = new SpinLattice<1, RealSpin>(size_1d);

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
  IsingSpin spin_up(1);
  IsingSpin spin_down(-1);
  PottsSpin spin_0(8,0);
  PottsSpin spin_1(8,1);
  PottsSpin spin_2(8,2);
  PottsSpin spin_3(8,3);
  PottsSpin spin_4(8,4);
  PottsSpin spin_5(8,5);
  PottsSpin spin_6(8,6);
  PottsSpin spin_7(8,7);
  PottsSpin spin_8(8,8);
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
  testlattice_2d_potts->set_spin(index_00, spin_0);
  testlattice_2d_potts->set_spin(index_01, spin_1);
  testlattice_2d_potts->set_spin(index_02, spin_2);
  testlattice_2d_potts->set_spin(index_10, spin_3);
  testlattice_2d_potts->set_spin(index_11, spin_4);
  testlattice_2d_potts->set_spin(index_12, spin_5);
  testlattice_2d_potts->set_spin(index_20, spin_6);
  testlattice_2d_potts->set_spin(index_21, spin_7);
  testlattice_2d_potts->set_spin(index_22, spin_8);
  testlattice_1d_real->set_spin(index_0, RealSpin(0.5));
  testlattice_1d_real->set_spin(index_1, RealSpin(0.1));
  testlattice_1d_real->set_spin(index_2, RealSpin(-0.1));
  testlattice_1d_real->set_spin(index_3, RealSpin(-1.0));
  testlattice_1d_real->set_spin(index_4, RealSpin(1.0));
}

void TestSpinLattice::tearDown()
{
  // Delete the lattices
  delete testlattice_1d;
  delete testlattice_2d;
  delete testlattice_2d_potts;
}

void TestSpinLattice::test_constructor()
{
  std::vector<unsigned int> extent(1,5);
  SpinLattice<1, IsingSpin> testlattice_default(extent, IsingSpin(-1));
  CPPUNIT_ASSERT(testlattice_default.get_spin(index_0) == IsingSpin(-1));
  CPPUNIT_ASSERT(testlattice_default.get_spin(index_1) == IsingSpin(-1));
  CPPUNIT_ASSERT(testlattice_default.get_spin(index_2) == IsingSpin(-1));
  CPPUNIT_ASSERT(testlattice_default.get_spin(index_3) == IsingSpin(-1));
  CPPUNIT_ASSERT(testlattice_default.get_spin(index_4) == IsingSpin(-1));
}

void TestSpinLattice::test_get_set_spin()
{
  IsingSpin spin_up(1);
  IsingSpin spin_down(-1);

  CPPUNIT_ASSERT_EQUAL(spin_up.get_value(), testlattice_1d->get_spin(index_0).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_up.get_value(), testlattice_1d->get_spin(index_1).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_down.get_value(), testlattice_1d->get_spin(index_2).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_down.get_value(), testlattice_1d->get_spin(index_3).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_up.get_value(), testlattice_1d->get_spin(index_4).get_value());
  
  CPPUNIT_ASSERT_EQUAL(0, static_cast<int>(testlattice_2d_potts->get_spin(index_00).get_value()));
  CPPUNIT_ASSERT_EQUAL(1, static_cast<int>(testlattice_2d_potts->get_spin(index_01).get_value()));
  CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(testlattice_2d_potts->get_spin(index_02).get_value()));
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(testlattice_2d_potts->get_spin(index_10).get_value()));
  CPPUNIT_ASSERT_EQUAL(4, static_cast<int>(testlattice_2d_potts->get_spin(index_11).get_value()));
  CPPUNIT_ASSERT_EQUAL(5, static_cast<int>(testlattice_2d_potts->get_spin(index_12).get_value()));
  CPPUNIT_ASSERT_EQUAL(6, static_cast<int>(testlattice_2d_potts->get_spin(index_20).get_value()));
  CPPUNIT_ASSERT_EQUAL(7, static_cast<int>(testlattice_2d_potts->get_spin(index_21).get_value()));
  CPPUNIT_ASSERT_EQUAL(8, static_cast<int>(testlattice_2d_potts->get_spin(index_22).get_value()));

  CPPUNIT_ASSERT_EQUAL(spin_up.get_value(), testlattice_2d->get_spin(index_00).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_up.get_value(), testlattice_2d->get_spin(index_01).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_down.get_value(), testlattice_2d->get_spin(index_02).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_down.get_value(), testlattice_2d->get_spin(index_10).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_up.get_value(), testlattice_2d->get_spin(index_11).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_down.get_value(), testlattice_2d->get_spin(index_12).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_down.get_value(), testlattice_2d->get_spin(index_20).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_down.get_value(), testlattice_2d->get_spin(index_21).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_up.get_value(), testlattice_2d->get_spin(index_22).get_value());
}
void TestSpinLattice::test_get_simulation_time()
{
  CPPUNIT_ASSERT_EQUAL(0, testlattice_1d->get_simulation_time());
  CPPUNIT_ASSERT_EQUAL(0, testlattice_2d->get_simulation_time());
}

void TestSpinLattice::test_operator_equal()
{
  // Copy the spin lattices
  SpinLattice<1, IsingSpin>* testlattice_1d_copy = new SpinLattice<1, IsingSpin>(*testlattice_1d);
  SpinLattice<2, IsingSpin>* testlattice_2d_copy = new SpinLattice<2, IsingSpin>(*testlattice_2d);
  SpinLattice<1, RealSpin>* testlattice_1d_real_copy = new SpinLattice<1, RealSpin>(*testlattice_1d_real);

  // Test that these two lattices are equal with the original ones
  CPPUNIT_ASSERT(*testlattice_1d_copy == *testlattice_1d);
  CPPUNIT_ASSERT(*testlattice_2d_copy == *testlattice_2d);
  CPPUNIT_ASSERT(*testlattice_1d_real_copy == *testlattice_1d_real);

  // Propose steps and execute them and check that the lattices are not equal
  SpinLatticeStep<1, IsingSpin> step_1d = testlattice_1d->propose_step(0.5);
  SpinLatticeStep<2, IsingSpin> step_2d = testlattice_2d->propose_step(0.5);
  SpinLatticeStep<1, RealSpin> step_1d_real = testlattice_1d_real->propose_step(0.5);
  step_1d.execute();
  step_2d.execute();
  step_1d_real.execute();
  CPPUNIT_ASSERT(*testlattice_1d_copy != *testlattice_1d);
  CPPUNIT_ASSERT(*testlattice_2d_copy != *testlattice_2d);
  CPPUNIT_ASSERT(*testlattice_1d_real_copy != *testlattice_1d_real);

  // Undo the steps and check that the lattices match again
  step_1d.undo();
  step_2d.undo();
  step_1d_real.undo();
  CPPUNIT_ASSERT(*testlattice_1d_copy == *testlattice_1d);
  CPPUNIT_ASSERT(*testlattice_2d_copy == *testlattice_2d);
  CPPUNIT_ASSERT(*testlattice_1d_real_copy == *testlattice_1d_real);
}
void TestSpinLattice::test_operator_equal_automatically()
{
  // Copy the spin lattices
  SpinLattice<1, IsingSpin>* testlattice_1d_copy = new SpinLattice<1, IsingSpin>(*testlattice_1d);

  // Propose and execute steps often on one of the test lattices and check that the comparison operator agrees with the output stream
  for (unsigned int i = 0; i < 10000; ++i)
  {
    // Propose and execute
    double random_double = static_cast<double>(rand())/RAND_MAX;
    SpinLatticeStep<1, IsingSpin> step = testlattice_1d->propose_step(random_double);
    step.execute();

    // Create the strings of the outstreams of the lattices
    std::stringstream stream_lattice; std::stringstream stream_copy;
    stream_lattice << *testlattice_1d;
    stream_copy << *testlattice_1d_copy;
    bool streams_agree = (stream_lattice.str() == stream_copy.str());

    bool operators_agree = (*testlattice_1d == *testlattice_1d_copy);

    CPPUNIT_ASSERT(streams_agree = operators_agree);
  }
}
void TestSpinLattice::test_operator_access()
{
  IsingSpin spin_up(1);
  IsingSpin spin_down(-1);

  CPPUNIT_ASSERT_EQUAL(spin_up.get_value(), (*testlattice_1d)(0).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_up.get_value(), (*testlattice_1d)(1).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_down.get_value(), (*testlattice_1d)(2).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_down.get_value(), (*testlattice_1d)(3).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_up.get_value(), (*testlattice_1d)(4).get_value());
  
  (*testlattice_1d)(0) = spin_down;
  (*testlattice_1d)(2) = spin_up;
  CPPUNIT_ASSERT_EQUAL(spin_down.get_value(), (*testlattice_1d)(0).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_up.get_value(), (*testlattice_1d)(2).get_value());

  CPPUNIT_ASSERT_EQUAL(0, static_cast<int>((*testlattice_2d_potts)(0,0).get_value()));
  CPPUNIT_ASSERT_EQUAL(1, static_cast<int>((*testlattice_2d_potts)(0,1).get_value()));
  CPPUNIT_ASSERT_EQUAL(2, static_cast<int>((*testlattice_2d_potts)(0,2).get_value()));
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>((*testlattice_2d_potts)(1,0).get_value()));
  CPPUNIT_ASSERT_EQUAL(4, static_cast<int>((*testlattice_2d_potts)(1,1).get_value()));
  CPPUNIT_ASSERT_EQUAL(5, static_cast<int>((*testlattice_2d_potts)(1,2).get_value()));
  CPPUNIT_ASSERT_EQUAL(6, static_cast<int>((*testlattice_2d_potts)(2,0).get_value()));
  CPPUNIT_ASSERT_EQUAL(7, static_cast<int>((*testlattice_2d_potts)(2,1).get_value()));
  CPPUNIT_ASSERT_EQUAL(8, static_cast<int>((*testlattice_2d_potts)(2,2).get_value()));

  CPPUNIT_ASSERT_EQUAL(spin_up.get_value(), (*testlattice_2d)(0,0).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_up.get_value(), (*testlattice_2d)(0,1).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_down.get_value(), (*testlattice_2d)(0,2).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_down.get_value(), (*testlattice_2d)(1,0).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_up.get_value(), (*testlattice_2d)(1,1).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_down.get_value(), (*testlattice_2d)(1,2).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_down.get_value(), (*testlattice_2d)(2,0).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_down.get_value(), (*testlattice_2d)(2,1).get_value());
  CPPUNIT_ASSERT_EQUAL(spin_up.get_value(), (*testlattice_2d)(2,2).get_value());
}

void TestSpinLattice::test_all_steps()
{
  IsingSpin spin_up(1);
  IsingSpin spin_down(-1);

  std::vector<SpinLatticeStep<1, IsingSpin> > possible_steps = testlattice_1d->all_steps();
  CPPUNIT_ASSERT(possible_steps.size() == 5);
  CPPUNIT_ASSERT(possible_steps[0].get_lattice() == testlattice_1d);
  CPPUNIT_ASSERT(possible_steps[0].get_flip_index() == index_0);
  CPPUNIT_ASSERT(possible_steps[0].get_old_spin() == spin_up);
  CPPUNIT_ASSERT(possible_steps[0].get_new_spin() == spin_down);
  CPPUNIT_ASSERT(possible_steps[1].get_lattice() == testlattice_1d);
  CPPUNIT_ASSERT(possible_steps[1].get_flip_index() == index_1);
  CPPUNIT_ASSERT(possible_steps[1].get_old_spin() == spin_up);
  CPPUNIT_ASSERT(possible_steps[1].get_new_spin() == spin_down);
  CPPUNIT_ASSERT(possible_steps[2].get_lattice() == testlattice_1d);
  CPPUNIT_ASSERT(possible_steps[2].get_flip_index() == index_2);
  CPPUNIT_ASSERT(possible_steps[2].get_old_spin() == spin_down);
  CPPUNIT_ASSERT(possible_steps[2].get_new_spin() == spin_up);
  CPPUNIT_ASSERT(possible_steps[3].get_lattice() == testlattice_1d);
  CPPUNIT_ASSERT(possible_steps[3].get_flip_index() == index_3);
  CPPUNIT_ASSERT(possible_steps[3].get_old_spin() == spin_down);
  CPPUNIT_ASSERT(possible_steps[3].get_new_spin() == spin_up);
  CPPUNIT_ASSERT(possible_steps[4].get_lattice() == testlattice_1d);
  CPPUNIT_ASSERT(possible_steps[4].get_flip_index() == index_4);
  CPPUNIT_ASSERT(possible_steps[4].get_old_spin() == spin_up);
  CPPUNIT_ASSERT(possible_steps[4].get_new_spin() == spin_down);

}
void TestSpinLattice::test_commit()
{
}
void TestSpinLattice::test_energy()
{
  CPPUNIT_ASSERT_EQUAL(-1.0, testlattice_1d->energy());
  CPPUNIT_ASSERT_EQUAL(6.0, testlattice_2d->energy());
  CPPUNIT_ASSERT_EQUAL(0.36, testlattice_1d_real->energy());
}
void TestSpinLattice::test_magnetization()
{
  CPPUNIT_ASSERT_EQUAL(1.0, testlattice_1d->magnetization());
  CPPUNIT_ASSERT_EQUAL(-1.0, testlattice_2d->magnetization());
}
void TestSpinLattice::test_next_neighbours()
{
  IsingSpin spin_up(1);
  IsingSpin spin_down(-1);

  CPPUNIT_ASSERT(testlattice_1d->next_neighbours(index_0).at(0) == spin_up);
  CPPUNIT_ASSERT(testlattice_1d->next_neighbours(index_0).at(1) == spin_up);
  CPPUNIT_ASSERT(testlattice_1d->next_neighbours(index_1).at(0) == spin_down);
  CPPUNIT_ASSERT(testlattice_1d->next_neighbours(index_1).at(1) == spin_up);
  CPPUNIT_ASSERT(testlattice_1d->next_neighbours(index_2).at(0) == spin_down);
  CPPUNIT_ASSERT(testlattice_1d->next_neighbours(index_2).at(1) == spin_up);
  CPPUNIT_ASSERT(testlattice_1d->next_neighbours(index_3).at(0) == spin_up);
  CPPUNIT_ASSERT(testlattice_1d->next_neighbours(index_3).at(1) == spin_down);
  CPPUNIT_ASSERT(testlattice_1d->next_neighbours(index_4).at(0) == spin_up);
  CPPUNIT_ASSERT(testlattice_1d->next_neighbours(index_4).at(1) == spin_down);

  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_00).at(0) == spin_down);
  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_00).at(1) == spin_down);
  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_00).at(2) == spin_up);
  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_00).at(3) == spin_down);
  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_11).at(0) == spin_down);
  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_11).at(1) == spin_up);
  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_11).at(2) == spin_down);
  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_11).at(3) == spin_down);
  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_12).at(0) == spin_up);
  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_12).at(1) == spin_down);
  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_12).at(2) == spin_down);
  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_12).at(3) == spin_up);
  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_20).at(0) == spin_up);
  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_20).at(1) == spin_down);
  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_20).at(2) == spin_down);
  CPPUNIT_ASSERT(testlattice_2d->next_neighbours(index_20).at(3) == spin_up);
}
void TestSpinLattice::test_propose_step()
{
  IsingSpin spin_up(1);
  IsingSpin spin_down(-1);

  SpinLatticeStep<1, IsingSpin> step_1d_0_1 = testlattice_1d->propose_step(0.1);
  CPPUNIT_ASSERT(step_1d_0_1.get_lattice() == testlattice_1d);
  CPPUNIT_ASSERT(step_1d_0_1.get_flip_index() == index_0);
  CPPUNIT_ASSERT(step_1d_0_1.get_old_spin() == spin_up);
  CPPUNIT_ASSERT(step_1d_0_1.get_new_spin() == spin_down);
  SpinLatticeStep<1, IsingSpin> step_1d_0_3 = testlattice_1d->propose_step(0.3);
  CPPUNIT_ASSERT(step_1d_0_3.get_lattice() == testlattice_1d);
  CPPUNIT_ASSERT(step_1d_0_3.get_flip_index() == index_1);
  CPPUNIT_ASSERT(step_1d_0_3.get_old_spin() == spin_up);
  CPPUNIT_ASSERT(step_1d_0_3.get_new_spin() == spin_down);
  SpinLatticeStep<1, IsingSpin> step_1d_0_5 = testlattice_1d->propose_step(0.5);
  CPPUNIT_ASSERT(step_1d_0_5.get_lattice() == testlattice_1d);
  CPPUNIT_ASSERT(step_1d_0_5.get_flip_index() == index_2);
  CPPUNIT_ASSERT(step_1d_0_5.get_old_spin() == spin_down);
  CPPUNIT_ASSERT(step_1d_0_5.get_new_spin() == spin_up);
  SpinLatticeStep<1, IsingSpin> step_1d_0_7 = testlattice_1d->propose_step(0.7);
  CPPUNIT_ASSERT(step_1d_0_7.get_lattice() == testlattice_1d);
  CPPUNIT_ASSERT(step_1d_0_7.get_flip_index() == index_3);
  CPPUNIT_ASSERT(step_1d_0_7.get_old_spin() == spin_down);
  CPPUNIT_ASSERT(step_1d_0_7.get_new_spin() == spin_up);
  SpinLatticeStep<1, IsingSpin> step_1d_0_9 = testlattice_1d->propose_step(0.9);
  CPPUNIT_ASSERT(step_1d_0_9.get_lattice() == testlattice_1d);
  CPPUNIT_ASSERT(step_1d_0_9.get_flip_index() == index_4);
  CPPUNIT_ASSERT(step_1d_0_9.get_old_spin() == spin_up);
  CPPUNIT_ASSERT(step_1d_0_9.get_new_spin() == spin_down);

  SpinLatticeStep<1, RealSpin> step_1d_real_0_1 = testlattice_1d_real->propose_step(0.1);
  CPPUNIT_ASSERT(step_1d_real_0_1.get_lattice() == testlattice_1d_real);
  CPPUNIT_ASSERT(step_1d_real_0_1.get_flip_index() == index_0);
  CPPUNIT_ASSERT(step_1d_real_0_1.get_old_spin() == RealSpin(0.5));
  CPPUNIT_ASSERT(step_1d_real_0_1.get_new_spin() == RealSpin(-0.5));
  SpinLatticeStep<1, RealSpin> step_1d_real_0_3 = testlattice_1d_real->propose_step(0.3);
  CPPUNIT_ASSERT(step_1d_real_0_3.get_lattice() == testlattice_1d_real);
  CPPUNIT_ASSERT(step_1d_real_0_3.get_flip_index() == index_1);
  CPPUNIT_ASSERT(step_1d_real_0_3.get_old_spin() == RealSpin(0.1));
  CPPUNIT_ASSERT(step_1d_real_0_3.get_new_spin() == RealSpin(-0.1));
  SpinLatticeStep<1, RealSpin> step_1d_real_0_5 = testlattice_1d_real->propose_step(0.5);
  CPPUNIT_ASSERT(step_1d_real_0_5.get_lattice() == testlattice_1d_real);
  CPPUNIT_ASSERT(step_1d_real_0_5.get_flip_index() == index_2);
  CPPUNIT_ASSERT(step_1d_real_0_5.get_old_spin() == RealSpin(-0.1));
  CPPUNIT_ASSERT(step_1d_real_0_5.get_new_spin() == RealSpin(0.1));
  SpinLatticeStep<1, RealSpin> step_1d_real_0_7 = testlattice_1d_real->propose_step(0.7);
  CPPUNIT_ASSERT(step_1d_real_0_7.get_lattice() == testlattice_1d_real);
  CPPUNIT_ASSERT(step_1d_real_0_7.get_flip_index() == index_3);
  CPPUNIT_ASSERT(step_1d_real_0_7.get_old_spin() == RealSpin(-1.0));
  CPPUNIT_ASSERT(step_1d_real_0_7.get_new_spin() == RealSpin(1.0));
  SpinLatticeStep<1, RealSpin> step_1d_real_0_9 = testlattice_1d_real->propose_step(0.9);
  CPPUNIT_ASSERT(step_1d_real_0_9.get_lattice() == testlattice_1d_real);
  CPPUNIT_ASSERT(step_1d_real_0_9.get_flip_index() == index_4);
  CPPUNIT_ASSERT(step_1d_real_0_9.get_old_spin() == RealSpin(1.0));
  CPPUNIT_ASSERT(step_1d_real_0_9.get_new_spin() == RealSpin(-1.0));
}

void TestSpinLattice::test_system_size()
{
  CPPUNIT_ASSERT_EQUAL(5u, testlattice_1d->system_size());
  CPPUNIT_ASSERT_EQUAL(9u, testlattice_2d->system_size());
}

void TestSpinLattice::test_serialize()
{
  // Create an output file stream to save the 1d testlattice
  std::ofstream ofs("serialize.dat");
  // Save data to archive
  {
    boost::archive::text_oarchive oa(ofs);
    oa << *testlattice_1d;
  }

  // Create a new instance and load
  SpinLattice<1, IsingSpin> testlattice_1d_loaded;
  // Load data from archive
  {
    std::ifstream ifs("serialize.dat");
    boost::archive::text_iarchive ia(ifs);
    ia >> testlattice_1d_loaded;
  }

  // Test that the loaded and the original lattice match
  CPPUNIT_ASSERT(*testlattice_1d == testlattice_1d_loaded);
}
