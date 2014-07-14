#include "test_histogram_accumulator.hpp"

CppUnit::Test* TestHistogramAccumulator::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestHistogramAccumulator");

  suite_of_tests->addTest( new CppUnit::TestCaller<TestHistogramAccumulator>("TestObservables/TestHistogramAccumulator: test_operator_accumulate", &TestHistogramAccumulator::test_operator_accumulate) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestHistogramAccumulator>("TestObservables/TestHistogramAccumulator: test_normalized_histogram", &TestHistogramAccumulator::test_normalized_histogram) );

  return suite_of_tests;
}

void TestHistogramAccumulator::setUp()
{
  // Set the binning
  histogram_accumulator.set_binning_width(1.0);
  histogram_accumulator.set_binning_reference(0.0);

  // Set some values
  histogram_accumulator[-0.5] = 2;
  histogram_accumulator[0.5] = 5;
  histogram_accumulator[1.5] = 2;
  histogram_accumulator[2.5] = 1;
}

void TestHistogramAccumulator::tearDown()
{
}

void TestHistogramAccumulator::test_operator_accumulate()
{
  histogram_accumulator(0.1);
  histogram_accumulator(0.3);
  histogram_accumulator(2.3);
  histogram_accumulator(4.1);
  CPPUNIT_ASSERT_EQUAL(5, static_cast<int>(histogram_accumulator.size()));
  CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(histogram_accumulator[-0.1]));
  CPPUNIT_ASSERT_EQUAL(7, static_cast<int>(histogram_accumulator[0.1]));
  CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(histogram_accumulator[1.1]));
  CPPUNIT_ASSERT_EQUAL(2, static_cast<int>(histogram_accumulator[2.1]));
  CPPUNIT_ASSERT_EQUAL(1, static_cast<int>(histogram_accumulator[4.1]));
}
void TestHistogramAccumulator::test_normalized_histogram()
{
  HistogramNumber<double, double> normalized_histogram = histogram_accumulator.normalized_histogram();
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2, normalized_histogram[-0.1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, normalized_histogram[0.1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.2, normalized_histogram[1.1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, normalized_histogram[2.1], 1e-4);
}
