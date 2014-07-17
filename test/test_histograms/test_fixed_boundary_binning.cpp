#include "test_fixed_boundary_binning.hpp"
#include <vector>
#include <cstdint>

CppUnit::Test* TestFixedBoundaryBinning::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestHistograms/TestFixedBoundaryBinning");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestFixedBoundaryBinning>("TestHistograms/TestFixedBoundaryBinning: test_functor", &TestFixedBoundaryBinning::test_functor) );
  
  return suite_of_tests;
}

void TestFixedBoundaryBinning::setUp()
{
  int int_initializer[5] = {-3, 3, -1, 7, 10};
  test_binning_int = new binning_t_int(std::begin(int_initializer), std::end(int_initializer));
  double double_initializer[5] = {-3.0, 3.0, -1.0, 7.0, 10.0};
  test_binning_double = new binning_t_double(std::begin(double_initializer), std::end(double_initializer));
}

void TestFixedBoundaryBinning::tearDown()
{
  delete test_binning_int;
  delete test_binning_double;
}

void TestFixedBoundaryBinning::test_functor()
{
  CPPUNIT_ASSERT_EQUAL(-1, (*test_binning_int)(-2));
  CPPUNIT_ASSERT_EQUAL(-1, (*test_binning_int)(-3));
  CPPUNIT_ASSERT_EQUAL(-3, (*test_binning_int)(-4));
  CPPUNIT_ASSERT_EQUAL(3, (*test_binning_int)(-1));
  CPPUNIT_ASSERT_EQUAL(std::numeric_limits<int>::max(), (*test_binning_int)(11));
}
