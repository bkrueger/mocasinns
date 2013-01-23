#include "test_binnings.hpp"
#include <vector>
#include <cstdint>

CppUnit::Test* TestBinningNumber::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestBinningNumber");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestBinningNumber>("TestBinningNumber: test_functor", &TestBinningNumber::test_functor) );
  
  return suite_of_tests;
}

void TestBinningNumber::setUp()
{
  test_binning_int = new BinningNumber<int>(3);
  test_binning_int_base = new BinningNumber<int>(3,1);
  test_binning_double = new BinningNumber<double>(2.5);
  test_binning_double_base = new BinningNumber<double>(2.5, 1.0);
}

void TestBinningNumber::tearDown()
{
  delete test_binning_int;
  delete test_binning_int_base;
  delete test_binning_double;
  delete test_binning_double_base;
}

void TestBinningNumber::test_functor()
{
  // Test with integer binning without base value
  CPPUNIT_ASSERT_EQUAL(-9, (*test_binning_int)(-9));
  CPPUNIT_ASSERT_EQUAL(-9, (*test_binning_int)(-8));
  CPPUNIT_ASSERT_EQUAL(-9, (*test_binning_int)(-7));  
  CPPUNIT_ASSERT_EQUAL(-6, (*test_binning_int)(-6));
  CPPUNIT_ASSERT_EQUAL(-6, (*test_binning_int)(-5));
  CPPUNIT_ASSERT_EQUAL(-6, (*test_binning_int)(-4));
  CPPUNIT_ASSERT_EQUAL(-3, (*test_binning_int)(-3));
  CPPUNIT_ASSERT_EQUAL(-3, (*test_binning_int)(-2));
  CPPUNIT_ASSERT_EQUAL(-3, (*test_binning_int)(-1));
  CPPUNIT_ASSERT_EQUAL(0, (*test_binning_int)(0));
  CPPUNIT_ASSERT_EQUAL(0, (*test_binning_int)(1));
  CPPUNIT_ASSERT_EQUAL(0, (*test_binning_int)(2));
  CPPUNIT_ASSERT_EQUAL(3, (*test_binning_int)(3));
  CPPUNIT_ASSERT_EQUAL(3, (*test_binning_int)(4));
  CPPUNIT_ASSERT_EQUAL(3, (*test_binning_int)(5));
  CPPUNIT_ASSERT_EQUAL(6, (*test_binning_int)(6));
  CPPUNIT_ASSERT_EQUAL(6, (*test_binning_int)(7));
  CPPUNIT_ASSERT_EQUAL(6, (*test_binning_int)(8));

  // Test with integer binning with base value
  CPPUNIT_ASSERT_EQUAL(-2, (*test_binning_int_base)(-2));
  CPPUNIT_ASSERT_EQUAL(-2, (*test_binning_int_base)(-1));
  CPPUNIT_ASSERT_EQUAL(-2, (*test_binning_int_base)(0));
  CPPUNIT_ASSERT_EQUAL(1, (*test_binning_int_base)(1));
  CPPUNIT_ASSERT_EQUAL(1, (*test_binning_int_base)(2));
  CPPUNIT_ASSERT_EQUAL(1, (*test_binning_int_base)(3));
  CPPUNIT_ASSERT_EQUAL(4, (*test_binning_int_base)(4));
  CPPUNIT_ASSERT_EQUAL(4, (*test_binning_int_base)(5));

  // Test with double binning without base value
  CPPUNIT_ASSERT_EQUAL(-10.0, (*test_binning_double)(-9.1));
  CPPUNIT_ASSERT_EQUAL(-10.0, (*test_binning_double)(-8.1));
  CPPUNIT_ASSERT_EQUAL(-7.5, (*test_binning_double)(-7.1));  
  CPPUNIT_ASSERT_EQUAL(-7.5, (*test_binning_double)(-6.1));
  CPPUNIT_ASSERT_EQUAL(-7.5, (*test_binning_double)(-5.1));
  CPPUNIT_ASSERT_EQUAL(-5.0, (*test_binning_double)(-4.1));
  CPPUNIT_ASSERT_EQUAL(-5.0, (*test_binning_double)(-3.1));
  CPPUNIT_ASSERT_EQUAL(-2.5, (*test_binning_double)(-2.1));
  CPPUNIT_ASSERT_EQUAL(-2.5, (*test_binning_double)(-1.1));
  CPPUNIT_ASSERT_EQUAL(-2.5, (*test_binning_double)(-0.1));
  CPPUNIT_ASSERT_EQUAL(0.0, (*test_binning_double)(0.1));
  CPPUNIT_ASSERT_EQUAL(0.0, (*test_binning_double)(1.1));
  CPPUNIT_ASSERT_EQUAL(0.0, (*test_binning_double)(2.1));
  CPPUNIT_ASSERT_EQUAL(0.0, (*test_binning_double)(2.4));
  CPPUNIT_ASSERT_EQUAL(2.5, (*test_binning_double)(2.5));
  CPPUNIT_ASSERT_EQUAL(2.5, (*test_binning_double)(3.1));
  CPPUNIT_ASSERT_EQUAL(2.5, (*test_binning_double)(4.1));
  CPPUNIT_ASSERT_EQUAL(5.0, (*test_binning_double)(5.1));
  CPPUNIT_ASSERT_EQUAL(5.0, (*test_binning_double)(6.1));
  CPPUNIT_ASSERT_EQUAL(5.0, (*test_binning_double)(7.1));
  CPPUNIT_ASSERT_EQUAL(7.5, (*test_binning_double)(8.1));

  // Test with double binning with base value
  CPPUNIT_ASSERT_EQUAL(-11.5, (*test_binning_double_base)(-9.1));
  CPPUNIT_ASSERT_EQUAL(-9.0, (*test_binning_double_base)(-8.1));
  CPPUNIT_ASSERT_EQUAL(-9.0, (*test_binning_double_base)(-7.1));  
  CPPUNIT_ASSERT_EQUAL(-6.5, (*test_binning_double_base)(-6.1));
  CPPUNIT_ASSERT_EQUAL(-6.5, (*test_binning_double_base)(-5.1));
  CPPUNIT_ASSERT_EQUAL(-6.5, (*test_binning_double_base)(-4.1));
  CPPUNIT_ASSERT_EQUAL(-4.0, (*test_binning_double_base)(-3.1));
  CPPUNIT_ASSERT_EQUAL(-4.0, (*test_binning_double_base)(-2.1));
  CPPUNIT_ASSERT_EQUAL(-1.5, (*test_binning_double_base)(-1.1));
  CPPUNIT_ASSERT_EQUAL(-1.5, (*test_binning_double_base)(-0.1));
  CPPUNIT_ASSERT_EQUAL(-1.5, (*test_binning_double_base)(0.1));
  CPPUNIT_ASSERT_EQUAL(1.0, (*test_binning_double_base)(1.1));
  CPPUNIT_ASSERT_EQUAL(1.0, (*test_binning_double_base)(2.1));
  CPPUNIT_ASSERT_EQUAL(1.0, (*test_binning_double_base)(2.4));
  CPPUNIT_ASSERT_EQUAL(1.0, (*test_binning_double_base)(2.5));
  CPPUNIT_ASSERT_EQUAL(1.0, (*test_binning_double_base)(3.1));
  CPPUNIT_ASSERT_EQUAL(3.5, (*test_binning_double_base)(4.1));
  CPPUNIT_ASSERT_EQUAL(3.5, (*test_binning_double_base)(5.1));
  CPPUNIT_ASSERT_EQUAL(6.0, (*test_binning_double_base)(6.1));
  CPPUNIT_ASSERT_EQUAL(6.0, (*test_binning_double_base)(7.1));
  CPPUNIT_ASSERT_EQUAL(6.0, (*test_binning_double_base)(8.1));

}



CppUnit::Test* TestBinningNumberVector::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestBinningNumberVector");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestBinningNumberVector>("TestBinningNumberVector: test_functor", &TestBinningNumberVector::test_functor) );
  
  return suite_of_tests;
}

void TestBinningNumberVector::setUp()
{
  test_binning_double_same = new BinningNumberVector<double>(3, 2.5);
  std::vector<double> diff_bin_widths;
  diff_bin_widths.push_back(2.0);
  diff_bin_widths.push_back(2.5);
  test_binning_double_diff = new BinningNumberVector<double>(diff_bin_widths);
}

void TestBinningNumberVector::tearDown()
{
  delete test_binning_double_same;
  delete test_binning_double_diff;
}

void TestBinningNumberVector::test_functor()
{
  // Test with double and same bin widths
  std::vector<double> test_1;
  test_1.push_back(0.5), test_1.push_back(5.3), test_1.push_back(-0.8);
  CPPUNIT_ASSERT_EQUAL(0.0, (*test_binning_double_same)(test_1)[0]);
  CPPUNIT_ASSERT_EQUAL(5.0, (*test_binning_double_same)(test_1)[1]);
  CPPUNIT_ASSERT_EQUAL(-2.5, (*test_binning_double_same)(test_1)[2]);

  std::vector<double> test_2;
  test_2.push_back(-0.1), test_2.push_back(5.0), test_2.push_back(-4.8);
  CPPUNIT_ASSERT_EQUAL(-2.5, (*test_binning_double_same)(test_2)[0]);
  CPPUNIT_ASSERT_EQUAL(5.0, (*test_binning_double_same)(test_2)[1]);
  CPPUNIT_ASSERT_EQUAL(-5.0, (*test_binning_double_same)(test_2)[2]);

  // Test with double and different bin widths
  std::vector<double> test_3;
  test_3.push_back(5.3), test_3.push_back(0.5);
  CPPUNIT_ASSERT_EQUAL(4.0, (*test_binning_double_diff)(test_3)[0]);
  CPPUNIT_ASSERT_EQUAL(0.0, (*test_binning_double_diff)(test_3)[1]);

  std::vector<double> test_4;
  test_4.push_back(-0.1), test_4.push_back(5.0);
  CPPUNIT_ASSERT_EQUAL(-2.0, (*test_binning_double_diff)(test_4)[0]);
  CPPUNIT_ASSERT_EQUAL(5.0, (*test_binning_double_diff)(test_4)[1]);
}
