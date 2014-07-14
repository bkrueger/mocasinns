#ifndef TEST_HISTOGRAM_VECTOR_HPP
#define TEST_HISTOGRAM_VECTOR_HPP

#include <vector>

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/histograms/histogram.hpp>

using namespace Mocasinns::Histograms;

class TestHistogramVector : public CppUnit::TestFixture
{
private:
  HistogramVector<std::vector<int>, int>* testhisto_int;
  HistogramVector<std::vector<double>, double>* testhisto_double;

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
