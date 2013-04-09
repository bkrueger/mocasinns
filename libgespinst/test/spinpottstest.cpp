#include "spinpottstest.hpp"

CppUnit::Test* TestSpinPotts::suite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestSpinPotts");
    
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinPotts>("TestSpinPotts: test_constructor", &TestSpinPotts::test_constructor ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinPotts>("TestSpinPotts: test_get_set_value", &TestSpinPotts::test_get_set_value ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinPotts>("TestSpinPotts: test_get_set_max_value", &TestSpinPotts::test_get_set_max_value ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinPotts>("TestSpinPotts: test_operator_equal", &TestSpinPotts::test_operator_equal ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinPotts>("TestSpinPotts: test_operator_multiply", &TestSpinPotts::test_operator_multiply ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinPotts>("TestSpinPotts: test_all_possible_values", &TestSpinPotts::test_all_possible_values ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinPotts>("TestSpinPotts: test_random", &TestSpinPotts::test_random ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinPotts>("TestSpinPotts: test_random_differ", &TestSpinPotts::test_random_differ ) );

    return suiteOfTests;
}

void TestSpinPotts::setUp()
{
  spin_0.set_max_value(3);
  spin_0.set_value(0);
  spin_1.set_max_value(3);
  spin_1.set_value(1);
  spin_2.set_max_value(3);
  spin_2.set_value(2);
  spin_3.set_max_value(3);
  spin_3.set_value(3);
}

void TestSpinPotts::tearDown()
{
}

void TestSpinPotts::test_constructor()
{
  SpinPotts spin_test_1(5, 2);
  CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(spin_test_1.get_value()));
  CPPUNIT_ASSERT_EQUAL(5, static_cast<int>(spin_test_1.get_max_value()));
  
  SpinPotts spin_test_3(3, 6);
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(spin_test_3.get_value()));
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(spin_test_3.get_max_value()));
}

void TestSpinPotts::test_get_set_value()
{
  CPPUNIT_ASSERT_EQUAL(0, static_cast<int>(spin_0.get_value()));
  CPPUNIT_ASSERT_EQUAL(1, static_cast<int>(spin_1.get_value()));
  CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(spin_2.get_value()));
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(spin_3.get_value()));

  SpinPotts spin_test(5, 3);
  spin_test.set_value(13);
  CPPUNIT_ASSERT_EQUAL(5, static_cast<int>(spin_test.get_value()));
  spin_test.set_value(3);
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(spin_test.get_value()));
  spin_test.set_value(0);
  CPPUNIT_ASSERT_EQUAL(0, static_cast<int>(spin_test.get_value()));
}

void TestSpinPotts::test_get_set_max_value()
{

}

void TestSpinPotts::test_operator_equal()
{
  CPPUNIT_ASSERT(spin_0 == spin_0);
  CPPUNIT_ASSERT(spin_0 != spin_1);
  CPPUNIT_ASSERT(spin_0 != spin_2);
  CPPUNIT_ASSERT(spin_0 != spin_3);

  CPPUNIT_ASSERT(spin_1 == spin_1);
  CPPUNIT_ASSERT(spin_1 != spin_0);
  CPPUNIT_ASSERT(spin_1 != spin_2);
  CPPUNIT_ASSERT(spin_1 != spin_3);

  CPPUNIT_ASSERT(spin_2 == spin_2);
  CPPUNIT_ASSERT(spin_2 != spin_0);
  CPPUNIT_ASSERT(spin_2 != spin_1);
  CPPUNIT_ASSERT(spin_2 != spin_3);

  CPPUNIT_ASSERT(spin_3 == spin_3);
  CPPUNIT_ASSERT(spin_3 != spin_0);
  CPPUNIT_ASSERT(spin_3 != spin_1);
  CPPUNIT_ASSERT(spin_3 != spin_2);
}
void TestSpinPotts::test_operator_multiply()
{
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(1), spin_0 * spin_0);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(1), spin_1 * spin_1);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(1), spin_2 * spin_2);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(1), spin_3 * spin_3);

  CPPUNIT_ASSERT_EQUAL(static_cast<double>(0), spin_0 * spin_1);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(0), spin_0 * spin_2);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(0), spin_0 * spin_3);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(0), spin_1 * spin_0);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(0), spin_1 * spin_2);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(0), spin_1 * spin_3);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(0), spin_2 * spin_0);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(0), spin_2 * spin_1);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(0), spin_2 * spin_3);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(0), spin_3 * spin_0);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(0), spin_3 * spin_1);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(0), spin_3 * spin_2);
}

void TestSpinPotts::test_all_possible_values()
{
  CPPUNIT_ASSERT(spin_0.all_possible_values().size() == 4);
  CPPUNIT_ASSERT(spin_0.all_possible_values()[0] == spin_0);
  CPPUNIT_ASSERT(spin_0.all_possible_values()[1] == spin_1);
  CPPUNIT_ASSERT(spin_0.all_possible_values()[2] == spin_2);
  CPPUNIT_ASSERT(spin_0.all_possible_values()[3] == spin_3);
  CPPUNIT_ASSERT(spin_1.all_possible_values()[0] == spin_0);
  CPPUNIT_ASSERT(spin_1.all_possible_values()[1] == spin_1);
  CPPUNIT_ASSERT(spin_1.all_possible_values()[2] == spin_2);
  CPPUNIT_ASSERT(spin_1.all_possible_values()[3] == spin_3);
}
void TestSpinPotts::test_random()
{
}

void TestSpinPotts::test_random_differ()
{
  SpinPotts spin_ising_up(1,1);
  SpinPotts spin_ising_down(1,0);
  CPPUNIT_ASSERT(spin_ising_up.random_differ(0.3) == spin_ising_down);
  CPPUNIT_ASSERT(spin_ising_up.random_differ(0.15) == spin_ising_down);
  CPPUNIT_ASSERT(spin_ising_down.random_differ(0.3) == spin_ising_up);
  CPPUNIT_ASSERT(spin_ising_down.random_differ(0.15) == spin_ising_up);

  SpinPotts spin_potts_4_2(4,2);
  CPPUNIT_ASSERT(spin_potts_4_2.random_differ(0.0) == SpinPotts(4,0));
  CPPUNIT_ASSERT(spin_potts_4_2.random_differ(0.3) == SpinPotts(4,1));
  CPPUNIT_ASSERT(spin_potts_4_2.random_differ(0.7) == SpinPotts(4,3));
  CPPUNIT_ASSERT(spin_potts_4_2.random_differ(1.0) == SpinPotts(4,4));
}
