#include "test_file_accumulator.hpp"

CppUnit::Test* TestFileAccumulator::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestFileAccumulator");

  suite_of_tests->addTest( new CppUnit::TestCaller<TestFileAccumulator>("TestObservables/TestFileAccumulator: test_operator_accumulate", &TestFileAccumulator::test_operator_accumulate) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestFileAccumulator>("TestObservables/TestFileAccumulator: test_data", &TestFileAccumulator::test_data) );

  return suite_of_tests;
}

void TestFileAccumulator::setUp()
{
  // Create the file accumulators
  file_accumulator_1 = new FileAccumulator<double>("file_accumulator_data_1.dat");
  file_accumulator_2 = new FileAccumulator<double>("file_accumulator_data_2.dat");

  // Set the cache size
  file_accumulator_1->set_cache_size(4);
  file_accumulator_2->set_cache_size(4);
  
  // Accumulate some values
  (*file_accumulator_1)(0.1); (*file_accumulator_2)(0.1);
  (*file_accumulator_1)(0.4); (*file_accumulator_2)(0.4);
  (*file_accumulator_1)(0.0); (*file_accumulator_2)(0.0);
  (*file_accumulator_1)(2.1); (*file_accumulator_2)(2.1);
  (*file_accumulator_1)(4.1); (*file_accumulator_2)(4.1);
  (*file_accumulator_1)(0.0); (*file_accumulator_2)(0.0);
}

void TestFileAccumulator::tearDown()
{
}

void TestFileAccumulator::test_operator_accumulate()
{
  CPPUNIT_ASSERT_EQUAL(6, static_cast<int>(file_accumulator_1->get_count()));
}
void TestFileAccumulator::test_data()
{
  CPPUNIT_ASSERT_EQUAL(6, static_cast<int>(file_accumulator_2->get_count()));

  std::vector<double> data = file_accumulator_2->data();
  
  CPPUNIT_ASSERT_EQUAL(6, static_cast<int>(data.size()));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.1, data[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.4, data[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, data[2], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.1, data[3], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.1, data[4], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, data[5], 1e-4);
}
