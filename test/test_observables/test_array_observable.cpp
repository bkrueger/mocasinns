#include "test_array_observable.hpp"

CppUnit::Test* TestArrayObservable::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestArrayObservable");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayObservable>("TestObservables/TestArrayObservable: test_constructor", &TestArrayObservable::test_constructor) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayObservable>("TestObservables/TestArrayObservable: test_operator_multiply", &TestArrayObservable::test_operator_multiply) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayObservable>("TestObservables/TestArrayObservable: test_operator_divide", &TestArrayObservable::test_operator_divide) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayObservable>("TestObservables/TestArrayObservable: test_pow", &TestArrayObservable::test_pow) );
  
  return suite_of_tests;
}

void TestArrayObservable::setUp()
{
  boost::array<double,3> init_double;
  init_double[0] = 1.0;
  init_double[1] = 2.5;
  init_double[2] = -2.0;
  array_observable_double = new ArrayObservable<double,3>(init_double);

  boost::array<int,3> init_int;
  init_int[0] = 2;
  init_int[1] = 6;
  init_int[2] = -1;
  array_observable_int = new ArrayObservable<int,3>(init_int);
}

void TestArrayObservable::tearDown()
{
  delete array_observable_double;
  delete array_observable_int;
}

void TestArrayObservable::test_constructor()
{
  // Test the allready constructed VectorObservables
  CPPUNIT_ASSERT_EQUAL(1.0, (*array_observable_double)[0]);
  CPPUNIT_ASSERT_EQUAL(2.5, (*array_observable_double)[1]);
  CPPUNIT_ASSERT_EQUAL(-2.0, (*array_observable_double)[2]);
  CPPUNIT_ASSERT_EQUAL(2, (*array_observable_int)[0]);
  CPPUNIT_ASSERT_EQUAL(6, (*array_observable_int)[1]);
  CPPUNIT_ASSERT_EQUAL(-1, (*array_observable_int)[2]);
}

void TestArrayObservable::test_operator_multiply()
{
  // Test scalar multiplication for VectorObservable<double>
  ArrayObservable<double,3> multiplied_observable_double_1 = (*array_observable_double) * 2.0;
  ArrayObservable<double,3> multiplied_observable_double_2 = 2.0 * (*array_observable_double);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, multiplied_observable_double_1[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, multiplied_observable_double_1[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0, multiplied_observable_double_1[2], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, multiplied_observable_double_2[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, multiplied_observable_double_2[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0, multiplied_observable_double_2[2], 1e-4);

  // Test the component-wise multiplication
  ArrayObservable<double,3> multiplied_observable_double_3 = multiplied_observable_double_1 * (*array_observable_double);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, multiplied_observable_double_3[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(12.5, multiplied_observable_double_3[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, multiplied_observable_double_3[2], 1e-4);
}
void TestArrayObservable::test_operator_divide()
{
  // Test scalar division for VectorObservable<double>
  ArrayObservable<double,3> divided_observable_double = (*array_observable_double) / 2.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, divided_observable_double[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.25, divided_observable_double[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, divided_observable_double[2], 1e-4);

  // Test the component-wise division
  ArrayObservable<double,3> divided_observable_double_2 = (*array_observable_double) / divided_observable_double;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, divided_observable_double_2[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, divided_observable_double_2[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, divided_observable_double_2[2], 1e-4);
}
void TestArrayObservable::test_pow()
{
  // Test powing for VectorObservable<double>
  ArrayObservable<double,3> powed_observable_double = pow(*array_observable_double, 2.0);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, powed_observable_double[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(6.25, powed_observable_double[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, powed_observable_double[2], 1e-4);
}
