#include "test_array_observable.hpp"

CppUnit::Test* TestArrayObservable::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestArrayObservable");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayObservable>("TestObservables/TestArrayObservable: test_constructor", &TestArrayObservable::test_constructor) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayObservable>("TestObservables/TestArrayObservable: test_operator_add", &TestArrayObservable::test_operator_add) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayObservable>("TestObservables/TestArrayObservable: test_operator_substract", &TestArrayObservable::test_operator_substract) );
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

void TestArrayObservable::test_operator_add()
{
  // Test adding for VectorObservable<double>
  ArrayObservable<double,3> new_observable_double;
  new_observable_double[0] = 2.3;
  new_observable_double[1] = -0.5;
  new_observable_double[2] = 4.1;
  ArrayObservable<double,3> added_observable_double = (*array_observable_double) + new_observable_double;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.3, added_observable_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, added_observable_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.1, added_observable_double[2], 1e-6);
  // Test adding a scalar
  ArrayObservable<double,3> scalar_added_observable_double = added_observable_double + 2.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.3, scalar_added_observable_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, scalar_added_observable_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.1, scalar_added_observable_double[2], 1e-6);

  // Test adding for VectorObservable<int>
  ArrayObservable<int,3> new_observable_int;
  new_observable_int[0] = 2;
  new_observable_int[1] = -5;
  new_observable_int[2] = 3;
  ArrayObservable<int,3> added_observable_int = (*array_observable_int) + new_observable_int;
  CPPUNIT_ASSERT_EQUAL(4, added_observable_int[0]);
  CPPUNIT_ASSERT_EQUAL(1, added_observable_int[1]);
  CPPUNIT_ASSERT_EQUAL(2, added_observable_int[2]);
  // Test adding a scalar
  ArrayObservable<int,3> scalar_added_observable_int = added_observable_int + 2;
  CPPUNIT_ASSERT_EQUAL(6, scalar_added_observable_int[0]);
  CPPUNIT_ASSERT_EQUAL(3, scalar_added_observable_int[1]);
  CPPUNIT_ASSERT_EQUAL(4, scalar_added_observable_int[2]);
}
void TestArrayObservable::test_operator_substract()
{
  // Test substracting for VectorObservable<double>
  ArrayObservable<double,3> new_observable_double;
  new_observable_double[0] = 2.3;
  new_observable_double[1] = -0.5;
  new_observable_double[2] = 4.1;
  ArrayObservable<double,3> substracted_observable_double = (*array_observable_double) - new_observable_double;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.3, substracted_observable_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, substracted_observable_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-6.1, substracted_observable_double[2], 1e-6);
  // Test substracting a scalar
  ArrayObservable<double,3> scalar_substracted_observable_double = substracted_observable_double - 2.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.3, scalar_substracted_observable_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, scalar_substracted_observable_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-8.1, scalar_substracted_observable_double[2], 1e-6);

  // Test substracting for VectorObservable<int>
  ArrayObservable<int,3> new_observable_int;
  new_observable_int[0] = 2;
  new_observable_int[1] = -5;
  new_observable_int[2] = 3;
  ArrayObservable<int,3> substracted_observable_int = (*array_observable_int) - new_observable_int;
  CPPUNIT_ASSERT_EQUAL(0, substracted_observable_int[0]);
  CPPUNIT_ASSERT_EQUAL(11, substracted_observable_int[1]);
  CPPUNIT_ASSERT_EQUAL(-4, substracted_observable_int[2]);
  // Test substracting a scalar
  ArrayObservable<int,3> scalar_substracted_observable_int = substracted_observable_int - 2;
  CPPUNIT_ASSERT_EQUAL(-2, scalar_substracted_observable_int[0]);
  CPPUNIT_ASSERT_EQUAL(9, scalar_substracted_observable_int[1]);
  CPPUNIT_ASSERT_EQUAL(-6, scalar_substracted_observable_int[2]);
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
}
void TestArrayObservable::test_operator_divide()
{
  // Test scalar division for VectorObservable<double>
  ArrayObservable<double,3> divided_observable_double = (*array_observable_double) / 2.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, divided_observable_double[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.25, divided_observable_double[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, divided_observable_double[2], 1e-4);
}
void TestArrayObservable::test_pow()
{
  // Test powing for VectorObservable<double>
  ArrayObservable<double,3> powed_observable_double = pow(*array_observable_double, 2.0);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, powed_observable_double[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(6.25, powed_observable_double[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, powed_observable_double[2], 1e-4);
}
