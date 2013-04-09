#include "spinrealtest.hpp"

CppUnit::Test* TestSpinReal::suite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestSpinReal");
    
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinReal>("TestSpinReal: test_constructor", &TestSpinReal::test_constructor ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinReal>("TestSpinReal: test_get_set_value", &TestSpinReal::test_get_set_value ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinReal>("TestSpinReal: test_operator_equal", &TestSpinReal::test_operator_equal ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinReal>("TestSpinReal: test_operator_multiply", &TestSpinReal::test_operator_multiply ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinReal>("TestSpinReal: test_random", &TestSpinReal::test_random ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestSpinReal>("TestSpinReal: test_random_differ", &TestSpinReal::test_random_differ ) );

    return suiteOfTests;
}

void TestSpinReal::setUp()
{
  spin_1.set_value(1.0);
  spin_2.set_value(0.5);
  spin_3.set_value(-0.5);
}

void TestSpinReal::tearDown()
{
}

void TestSpinReal::test_constructor()
{
  SpinReal spin_test_1(2.0);
  CPPUNIT_ASSERT_EQUAL(2.0, spin_test_1.get_value());
  
  SpinReal spin_test_2(0.0);
  CPPUNIT_ASSERT_EQUAL(0.0, spin_test_2.get_value());

  SpinReal spin_test_3(-5.0);
  CPPUNIT_ASSERT_EQUAL(-5.0, spin_test_3.get_value());
}

void TestSpinReal::test_get_set_value()
{
  CPPUNIT_ASSERT_EQUAL(1.0, spin_1.get_value());
  CPPUNIT_ASSERT_EQUAL(0.5, spin_2.get_value());
  CPPUNIT_ASSERT_EQUAL(-0.5, spin_3.get_value());

  SpinReal spin_test;
  spin_test.set_value(13.0);
  CPPUNIT_ASSERT_EQUAL(13.0, spin_test.get_value());
  spin_test.set_value(9.0);
  CPPUNIT_ASSERT_EQUAL(9.0, spin_test.get_value());
  spin_test.set_value(0.0);
  CPPUNIT_ASSERT_EQUAL(0.0, spin_test.get_value());
  spin_test.set_value(-2.0);
  CPPUNIT_ASSERT_EQUAL(-2.0, spin_test.get_value());
  spin_test.set_value(-18.0);
  CPPUNIT_ASSERT_EQUAL(-18.0, spin_test.get_value());
}

void TestSpinReal::test_operator_equal()
{
  CPPUNIT_ASSERT(spin_1 == spin_1);
  CPPUNIT_ASSERT(spin_2 == spin_2);
  CPPUNIT_ASSERT(spin_3 == spin_3);

  CPPUNIT_ASSERT(spin_1 != spin_2);
  CPPUNIT_ASSERT(spin_1 != spin_3);
  CPPUNIT_ASSERT(spin_2 != spin_3);
}
void TestSpinReal::test_operator_multiply()
{
  CPPUNIT_ASSERT_EQUAL(1.0, spin_1 * spin_1);
  CPPUNIT_ASSERT_EQUAL(0.25, spin_2 * spin_2);
  CPPUNIT_ASSERT_EQUAL(0.25, spin_3 * spin_3);
  CPPUNIT_ASSERT_EQUAL(0.5, spin_1 * spin_2);
  CPPUNIT_ASSERT_EQUAL(-0.5, spin_1 * spin_3);
  CPPUNIT_ASSERT_EQUAL(-0.25, spin_2 * spin_3);
}

void TestSpinReal::test_random()
{
}

void TestSpinReal::test_random_differ()
{
  CPPUNIT_ASSERT_EQUAL(-1.0, spin_1.random_differ(0.1).get_value());
  CPPUNIT_ASSERT_EQUAL(-1.0, spin_1.random_differ(0.5).get_value());
  CPPUNIT_ASSERT_EQUAL(-1.0, spin_1.random_differ(0.8).get_value());
  CPPUNIT_ASSERT_EQUAL(-0.5, spin_2.random_differ(0.1).get_value());
  CPPUNIT_ASSERT_EQUAL(-0.5, spin_2.random_differ(0.5).get_value());
  CPPUNIT_ASSERT_EQUAL(-0.5, spin_2.random_differ(0.8).get_value());
  CPPUNIT_ASSERT_EQUAL(0.5, spin_3.random_differ(0.1).get_value());
  CPPUNIT_ASSERT_EQUAL(0.5, spin_3.random_differ(0.5).get_value());
  CPPUNIT_ASSERT_EQUAL(0.5, spin_3.random_differ(0.8).get_value());
}
