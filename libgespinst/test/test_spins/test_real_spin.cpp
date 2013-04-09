#include "test_real_spin.hpp"

CppUnit::Test* TestRealSpin::suite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestRealSpin");
    
    suiteOfTests->addTest( new CppUnit::TestCaller<TestRealSpin>("TestRealSpin: test_constructor", &TestRealSpin::test_constructor ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestRealSpin>("TestRealSpin: test_get_set_value", &TestRealSpin::test_get_set_value ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestRealSpin>("TestRealSpin: test_operator_equal", &TestRealSpin::test_operator_equal ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestRealSpin>("TestRealSpin: test_operator_multiply", &TestRealSpin::test_operator_multiply ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestRealSpin>("TestRealSpin: test_random", &TestRealSpin::test_random ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestRealSpin>("TestRealSpin: test_random_differ", &TestRealSpin::test_random_differ ) );

    return suiteOfTests;
}

void TestRealSpin::setUp()
{
  spin_1.set_value(1.0);
  spin_2.set_value(0.5);
  spin_3.set_value(-0.5);
}

void TestRealSpin::tearDown()
{
}

void TestRealSpin::test_constructor()
{
  RealSpin spin_test_1(2.0);
  CPPUNIT_ASSERT_EQUAL(2.0, spin_test_1.get_value());
  
  RealSpin spin_test_2(0.0);
  CPPUNIT_ASSERT_EQUAL(0.0, spin_test_2.get_value());

  RealSpin spin_test_3(-5.0);
  CPPUNIT_ASSERT_EQUAL(-5.0, spin_test_3.get_value());
}

void TestRealSpin::test_get_set_value()
{
  CPPUNIT_ASSERT_EQUAL(1.0, spin_1.get_value());
  CPPUNIT_ASSERT_EQUAL(0.5, spin_2.get_value());
  CPPUNIT_ASSERT_EQUAL(-0.5, spin_3.get_value());

  RealSpin spin_test;
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

void TestRealSpin::test_operator_equal()
{
  CPPUNIT_ASSERT(spin_1 == spin_1);
  CPPUNIT_ASSERT(spin_2 == spin_2);
  CPPUNIT_ASSERT(spin_3 == spin_3);

  CPPUNIT_ASSERT(spin_1 != spin_2);
  CPPUNIT_ASSERT(spin_1 != spin_3);
  CPPUNIT_ASSERT(spin_2 != spin_3);
}
void TestRealSpin::test_operator_multiply()
{
  CPPUNIT_ASSERT_EQUAL(1.0, spin_1 * spin_1);
  CPPUNIT_ASSERT_EQUAL(0.25, spin_2 * spin_2);
  CPPUNIT_ASSERT_EQUAL(0.25, spin_3 * spin_3);
  CPPUNIT_ASSERT_EQUAL(0.5, spin_1 * spin_2);
  CPPUNIT_ASSERT_EQUAL(-0.5, spin_1 * spin_3);
  CPPUNIT_ASSERT_EQUAL(-0.25, spin_2 * spin_3);
}

void TestRealSpin::test_random()
{
}

void TestRealSpin::test_random_differ()
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
