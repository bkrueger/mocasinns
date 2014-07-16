#ifndef TEST_HISTOGRAM_CONSTANT_WIDTH_HPP
#define TEST_HISTOGRAM_CONSTANT_WIDTH_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/histograms/histogram.hpp>

using namespace Mocasinns::Histograms;

class TestHistogramConstantWidth : public CppUnit::TestFixture
{
private:
  HistogramConstantWidth<int, int>* testhisto_int;
  HistogramConstantWidth<double, double>* testhisto_double;

public:
  static CppUnit::Test* suite();

  void setUp();
  void tearDown();

  void test_operator_fill();
  void test_operator_access();
  void test_operator_increment();
  void test_operator_divide();

  void test_insert();
};

#endif
