#include "test_vector_observable.hpp"

CppUnit::Test* TestVectorObservable::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestVectorObservable");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestVectorObservable>("TestObservables/TestVectorObservable: test_constructor", &TestVectorObservable::test_constructor) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestVectorObservable>("TestObservables/TestVectorObservable: test_operator_multiply", &TestVectorObservable::test_operator_multiply) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestVectorObservable>("TestObservables/TestVectorObservable: test_operator_divide", &TestVectorObservable::test_operator_divide) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestVectorObservable>("TestObservables/TestVectorObservable: test_pow", &TestVectorObservable::test_pow) );
  
  return suite_of_tests;
}

void TestVectorObservable::setUp()
{
  std::vector<double> init_double;
  init_double.push_back(1.0);
  init_double.push_back(2.5);
  init_double.push_back(-2.0);
  vector_observable_double = new VectorObservable<double>(init_double);

  std::vector<int> init_int;
  init_int.push_back(2);
  init_int.push_back(6);
  init_int.push_back(-1);
  vector_observable_int = new VectorObservable<int>(init_int);
}

void TestVectorObservable::tearDown()
{
  delete vector_observable_double;
  delete vector_observable_int;
}

void TestVectorObservable::test_constructor()
{
  // Test the allready constructed VectorObservables
  CPPUNIT_ASSERT_EQUAL(1.0, (*vector_observable_double)[0]);
  CPPUNIT_ASSERT_EQUAL(2.5, (*vector_observable_double)[1]);
  CPPUNIT_ASSERT_EQUAL(-2.0, (*vector_observable_double)[2]);
  CPPUNIT_ASSERT_EQUAL(2, (*vector_observable_int)[0]);
  CPPUNIT_ASSERT_EQUAL(6, (*vector_observable_int)[1]);
  CPPUNIT_ASSERT_EQUAL(-1, (*vector_observable_int)[2]);
}

void TestVectorObservable::test_operator_multiply()
{
  // Test scalar multiplication for VectorObservable<double>
  VectorObservable<double> multiplied_observable_double_1 = (*vector_observable_double) * 2.0;
  VectorObservable<double> multiplied_observable_double_2 = 2.0 * (*vector_observable_double);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, multiplied_observable_double_1[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, multiplied_observable_double_1[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0, multiplied_observable_double_1[2], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, multiplied_observable_double_2[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, multiplied_observable_double_2[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0, multiplied_observable_double_2[2], 1e-4);

  // Test the component-wise multiplication
  VectorObservable<double> multiplied_observable_double_3 = multiplied_observable_double_1 * (*vector_observable_double);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, multiplied_observable_double_3[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(12.5, multiplied_observable_double_3[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, multiplied_observable_double_3[2], 1e-4);
}
void TestVectorObservable::test_operator_divide()
{
  // Test scalar division for VectorObservable<double>
  VectorObservable<double> divided_observable_double = (*vector_observable_double) / 2.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, divided_observable_double[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.25, divided_observable_double[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, divided_observable_double[2], 1e-4);

  // Test the component-wise division
  VectorObservable<double> divided_observable_double_2 = (*vector_observable_double) / divided_observable_double;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, divided_observable_double_2[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, divided_observable_double_2[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, divided_observable_double_2[2], 1e-4);
}
void TestVectorObservable::test_pow()
{
  // Test powing for VectorObservable<double>
  VectorObservable<double> powed_observable_double = pow(*vector_observable_double, 2.0);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, powed_observable_double[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(6.25, powed_observable_double[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, powed_observable_double[2], 1e-4);
}
