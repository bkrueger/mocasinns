#include "test_vector_observable.hpp"

CppUnit::Test* TestVectorObservable::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestVectorObservable");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestVectorObservable>("TestObservables/TestVectorObservable: test_constructor", &TestVectorObservable::test_constructor) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestVectorObservable>("TestObservables/TestVectorObservable: test_operator_add", &TestVectorObservable::test_operator_add) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestVectorObservable>("TestObservables/TestVectorObservable: test_operator_substract", &TestVectorObservable::test_operator_substract) );
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

void TestVectorObservable::test_operator_add()
{
  // Test adding for VectorObservable<double>
  VectorObservable<double> new_observable_double(3,0.0);
  new_observable_double[0] = 2.3;
  new_observable_double[1] = -0.5;
  new_observable_double[2] = 4.1;
  VectorObservable<double> added_observable_double = (*vector_observable_double) + new_observable_double;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.3, added_observable_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, added_observable_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.1, added_observable_double[2], 1e-6);
  // Test adding a scalar
  VectorObservable<double> scalar_added_observable_double = added_observable_double + 2.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.3, scalar_added_observable_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, scalar_added_observable_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.1, scalar_added_observable_double[2], 1e-6);

  // Test adding for VectorObservable<int>
  VectorObservable<int> new_observable_int(3,0);
  new_observable_int[0] = 2;
  new_observable_int[1] = -5;
  new_observable_int[2] = 3;
  VectorObservable<int> added_observable_int = (*vector_observable_int) + new_observable_int;
  CPPUNIT_ASSERT_EQUAL(4, added_observable_int[0]);
  CPPUNIT_ASSERT_EQUAL(1, added_observable_int[1]);
  CPPUNIT_ASSERT_EQUAL(2, added_observable_int[2]);
  // Test adding a scalar
  VectorObservable<int> scalar_added_observable_int = added_observable_int + 2;
  CPPUNIT_ASSERT_EQUAL(6, scalar_added_observable_int[0]);
  CPPUNIT_ASSERT_EQUAL(3, scalar_added_observable_int[1]);
  CPPUNIT_ASSERT_EQUAL(4, scalar_added_observable_int[2]);
}
void TestVectorObservable::test_operator_substract()
{
  // Test substracting for VectorObservable<double>
  VectorObservable<double> new_observable_double(3,0.0);
  new_observable_double[0] = 2.3;
  new_observable_double[1] = -0.5;
  new_observable_double[2] = 4.1;
  VectorObservable<double> substracted_observable_double = (*vector_observable_double) - new_observable_double;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.3, substracted_observable_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, substracted_observable_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-6.1, substracted_observable_double[2], 1e-6);
  // Test substracting a scalar
  VectorObservable<double> scalar_substracted_observable_double = substracted_observable_double - 2.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.3, scalar_substracted_observable_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, scalar_substracted_observable_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-8.1, scalar_substracted_observable_double[2], 1e-6);

  // Test substracting for VectorObservable<int>
  VectorObservable<int> new_observable_int(3,0);
  new_observable_int[0] = 2;
  new_observable_int[1] = -5;
  new_observable_int[2] = 3;
  VectorObservable<int> substracted_observable_int = (*vector_observable_int) - new_observable_int;
  CPPUNIT_ASSERT_EQUAL(0, substracted_observable_int[0]);
  CPPUNIT_ASSERT_EQUAL(11, substracted_observable_int[1]);
  CPPUNIT_ASSERT_EQUAL(-4, substracted_observable_int[2]);
  // Test substracting a scalar
  VectorObservable<int> scalar_substracted_observable_int = substracted_observable_int - 2;
  CPPUNIT_ASSERT_EQUAL(-2, scalar_substracted_observable_int[0]);
  CPPUNIT_ASSERT_EQUAL(9, scalar_substracted_observable_int[1]);
  CPPUNIT_ASSERT_EQUAL(-6, scalar_substracted_observable_int[2]);
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
}
void TestVectorObservable::test_operator_divide()
{
  // Test scalar division for VectorObservable<double>
  VectorObservable<double> divided_observable_double = (*vector_observable_double) / 2.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, divided_observable_double[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.25, divided_observable_double[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, divided_observable_double[2], 1e-4);
}
void TestVectorObservable::test_pow()
{
  // Test powing for VectorObservable<double>
  VectorObservable<double> powed_observable_double = pow(*vector_observable_double, 2.0);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, powed_observable_double[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(6.25, powed_observable_double[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, powed_observable_double[2], 1e-4);
}
