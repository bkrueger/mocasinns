#include "test_histogram_observable.hpp"

CppUnit::Test* TestHistogramObservable::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestHistogramObservable");

  suite_of_tests->addTest( new CppUnit::TestCaller<TestHistogramObservable>("TestObservables/TestHistogramObservable: test_operator_add", &TestHistogramObservable::test_operator_add) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestHistogramObservable>("TestObservables/TestHistogramObservable: test_operator_substract", &TestHistogramObservable::test_operator_substract) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestHistogramObservable>("TestObservables/TestHistogramObservable: test_operator_multiply", &TestHistogramObservable::test_operator_multiply) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestHistogramObservable>("TestObservables/TestHistogramObservable: test_operator_divide", &TestHistogramObservable::test_operator_divide) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestHistogramObservable>("TestObservables/TestHistogramObservable: test_pow", &TestHistogramObservable::test_pow) );
  
  return suite_of_tests;
}

void TestHistogramObservable::setUp()
{
  histogram_observable_double_1[0] = 1.0;
  histogram_observable_double_1[2] = 2.5;
  histogram_observable_double_1[4] = -2.0;

  histogram_observable_double_2[0] = 1.0;
  histogram_observable_double_2[4] = 2.5;
  histogram_observable_double_2[2] = -2.0;

  histogram_observable_double_3[0] = 1.0;
  histogram_observable_double_3[2] = 2.5;
  histogram_observable_double_3[3] = -2.0;
}

void TestHistogramObservable::tearDown()
{
}

void TestHistogramObservable::test_operator_add()
{
  // Test the right result
  HistocreteObservable result = histogram_observable_double_1 + histogram_observable_double_2;
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(result.size()));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, result[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, result[2], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, result[4], 1e-4);

  // Test that the operator throws if using not valid histograms
  CPPUNIT_ASSERT_THROW(result = histogram_observable_double_1 + histogram_observable_double_3, XValuesDoNotMatchException);
}

void TestHistogramObservable::test_operator_substract()
{
  // Test the right result
  HistocreteObservable result = histogram_observable_double_1 - histogram_observable_double_2;
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(result.size()));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, result[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.5, result[2], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.5, result[4], 1e-4);

  // Test that the operator throws if using not valid histograms
  CPPUNIT_ASSERT_THROW(result = histogram_observable_double_1 - histogram_observable_double_3, XValuesDoNotMatchException);
}

void TestHistogramObservable::test_operator_multiply()
{
  // Test the right result
  HistocreteObservable result = histogram_observable_double_1 * histogram_observable_double_2;
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(result.size()));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, result[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-5.0, result[2], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-5.0, result[4], 1e-4);

  // Test that the operator throws if using not valid histograms
  CPPUNIT_ASSERT_THROW(result = histogram_observable_double_1 * histogram_observable_double_3, XValuesDoNotMatchException);
}
void TestHistogramObservable::test_operator_divide()
{
  // Test the right result
  HistocreteObservable result = histogram_observable_double_1 / histogram_observable_double_2;
  CPPUNIT_ASSERT_EQUAL(3, static_cast<int>(result.size()));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, result[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.25, result[2], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.8, result[4], 1e-4);

  // Test that the operator throws if using not valid histograms
  CPPUNIT_ASSERT_THROW(result = histogram_observable_double_1 / histogram_observable_double_3, XValuesDoNotMatchException);
}
void TestHistogramObservable::test_pow()
{
  // Test powing for VectorObservable<double>
  HistocreteObservable powed_observable = pow(histogram_observable_double_1, 2.0);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, powed_observable[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(6.25, powed_observable[2], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, powed_observable[4], 1e-4);
}
