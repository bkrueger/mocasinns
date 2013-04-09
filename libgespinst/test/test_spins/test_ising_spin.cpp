#include "test_ising_spin.hpp"

CppUnit::Test* TestSpinIsing::suite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestSpinIsing");
    
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinIsing>("TestSpinIsing: test_constructor", &TestSpinIsing::test_constructor ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinIsing>("TestSpinIsing: test_get_set_value", &TestSpinIsing::test_get_set_value ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinIsing>("TestSpinIsing: test_operator_equal", &TestSpinIsing::test_operator_equal ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinIsing>("TestSpinIsing: test_operator_multiply", &TestSpinIsing::test_operator_multiply ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinIsing>("TestSpinIsing: test_all_possible_values", &TestSpinIsing::test_all_possible_values ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinIsing>("TestSpinIsing: test_random", &TestSpinIsing::test_random ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinIsing>("TestSpinIsing: test_random_differ", &TestSpinIsing::test_random_differ ) );

    return suiteOfTests;
}

void TestSpinIsing::setUp()
{
  spin_up.set_value(1);
  spin_down.set_value(-1);
}

void TestSpinIsing::tearDown()
{
}

void TestSpinIsing::test_constructor()
{
  SpinIsing spin_test_1(2);
  CPPUNIT_ASSERT_EQUAL(1, spin_test_1.get_value());
  
  SpinIsing spin_test_2(0);
  CPPUNIT_ASSERT_EQUAL(1, spin_test_2.get_value());

  SpinIsing spin_test_3(-5);
  CPPUNIT_ASSERT_EQUAL(-1, spin_test_3.get_value());
}

void TestSpinIsing::test_get_set_value()
{
  CPPUNIT_ASSERT_EQUAL(1, spin_up.get_value());
  CPPUNIT_ASSERT_EQUAL(-1, spin_down.get_value());

  SpinIsing spin_test;
  spin_test.set_value(13);
  CPPUNIT_ASSERT_EQUAL(1, spin_test.get_value());
  spin_test.set_value(9);
  CPPUNIT_ASSERT_EQUAL(1, spin_test.get_value());
  spin_test.set_value(0);
  CPPUNIT_ASSERT_EQUAL(1, spin_test.get_value());
  spin_test.set_value(-2);
  CPPUNIT_ASSERT_EQUAL(-1, spin_test.get_value());
  spin_test.set_value(-18);
  CPPUNIT_ASSERT_EQUAL(-1, spin_test.get_value());
}

void TestSpinIsing::test_operator_equal()
{
  CPPUNIT_ASSERT(spin_up == spin_up);
  CPPUNIT_ASSERT(!(spin_up != spin_up));

  CPPUNIT_ASSERT(spin_up != spin_down);
  CPPUNIT_ASSERT(!(spin_up == spin_down));

  CPPUNIT_ASSERT(spin_down != spin_up);
  CPPUNIT_ASSERT(!(spin_down == spin_up));

  CPPUNIT_ASSERT(spin_down == spin_down);
  CPPUNIT_ASSERT(!(spin_down != spin_down));
}
void TestSpinIsing::test_operator_multiply()
{
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(1), spin_up * spin_up);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(1), spin_down * spin_down);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(-1), spin_up * spin_down);
  CPPUNIT_ASSERT_EQUAL(static_cast<double>(-1), spin_down * spin_up);
}

void TestSpinIsing::test_all_possible_values()
{
  CPPUNIT_ASSERT(spin_up.all_possible_values()[0] == spin_up);
  CPPUNIT_ASSERT(spin_up.all_possible_values()[1] == spin_down);
  CPPUNIT_ASSERT(spin_down.all_possible_values()[0] == spin_up);
  CPPUNIT_ASSERT(spin_down.all_possible_values()[1] == spin_down);
}
void TestSpinIsing::test_random()
{
}

void TestSpinIsing::test_random_differ()
{
  CPPUNIT_ASSERT(spin_up.random_differ(0.3) == spin_down);
  CPPUNIT_ASSERT(spin_up.random_differ(0.15) == spin_down);
  CPPUNIT_ASSERT(spin_down.random_differ(0.3) == spin_up);
  CPPUNIT_ASSERT(spin_down.random_differ(0.15) == spin_up);
}
