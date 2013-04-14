#include "test_potts_spin.hpp"

CppUnit::Test* TestPottsSpin::suite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestPottsSpin");
    
    suiteOfTests->addTest( new CppUnit::TestCaller<TestPottsSpin>("TestPottsSpin: test_constructor", &TestPottsSpin::test_constructor ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestPottsSpin>("TestPottsSpin: test_get_set_value", &TestPottsSpin::test_get_set_value ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestPottsSpin>("TestPottsSpin: test_get_set_max_value", &TestPottsSpin::test_get_set_max_value ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestPottsSpin>("TestPottsSpin: test_operator_equal", &TestPottsSpin::test_operator_equal ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestPottsSpin>("TestPottsSpin: test_operator_multiply", &TestPottsSpin::test_operator_multiply ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestPottsSpin>("TestPottsSpin: test_all_possible_values", &TestPottsSpin::test_all_possible_values ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestPottsSpin>("TestPottsSpin: test_random", &TestPottsSpin::test_random ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestPottsSpin>("TestPottsSpin: test_random_differ", &TestPottsSpin::test_random_differ ) );

    return suiteOfTests;
}

void TestPottsSpin::setUp()
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

void TestPottsSpin::tearDown()
{
}

void TestPottsSpin::test_constructor()
{
  PottsSpin spin_test_1(5, 2);
  CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(spin_test_1.get_value()));
  CPPUNIT_ASSERT_EQUAL(5, static_cast<int>(spin_test_1.get_max_value()));
  
  PottsSpin spin_test_3(3, 6);
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(spin_test_3.get_value()));
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(spin_test_3.get_max_value()));
}

void TestPottsSpin::test_get_set_value()
{
  CPPUNIT_ASSERT_EQUAL(0, static_cast<int>(spin_0.get_value()));
  CPPUNIT_ASSERT_EQUAL(1, static_cast<int>(spin_1.get_value()));
  CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(spin_2.get_value()));
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(spin_3.get_value()));

  PottsSpin spin_test(5, 3);
  spin_test.set_value(13);
  CPPUNIT_ASSERT_EQUAL(5, static_cast<int>(spin_test.get_value()));
  spin_test.set_value(3);
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(spin_test.get_value()));
  spin_test.set_value(0);
  CPPUNIT_ASSERT_EQUAL(0, static_cast<int>(spin_test.get_value()));
}

void TestPottsSpin::test_get_set_max_value()
{

}

void TestPottsSpin::test_operator_equal()
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
void TestPottsSpin::test_operator_multiply()
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

void TestPottsSpin::test_all_possible_values()
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
void TestPottsSpin::test_random()
{
}

void TestPottsSpin::test_random_differ()
{
  PottsSpin spin_ising_up(1,1);
  PottsSpin spin_ising_down(1,0);
  CPPUNIT_ASSERT(spin_ising_up.random_differ(0.3) == spin_ising_down);
  CPPUNIT_ASSERT(spin_ising_up.random_differ(0.15) == spin_ising_down);
  CPPUNIT_ASSERT(spin_ising_down.random_differ(0.3) == spin_ising_up);
  CPPUNIT_ASSERT(spin_ising_down.random_differ(0.15) == spin_ising_up);

  PottsSpin spin_potts_4_2(4,2);
  CPPUNIT_ASSERT(spin_potts_4_2.random_differ(0.0) == PottsSpin(4,0));
  CPPUNIT_ASSERT(spin_potts_4_2.random_differ(0.3) == PottsSpin(4,1));
  CPPUNIT_ASSERT(spin_potts_4_2.random_differ(0.7) == PottsSpin(4,3));
  CPPUNIT_ASSERT(spin_potts_4_2.random_differ(1.0) == PottsSpin(4,4));
}
