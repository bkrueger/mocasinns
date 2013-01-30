#ifndef HISTOGRAMNUMBERTEST_HPP
#define HISTOGRAMNUMBERTEST_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <histogram_number.hpp>

using namespace Mocasinns::Histograms;

class TestHistogramNumber : public CppUnit::TestFixture
{
private:
  HistogramNumber<int, int>* testhisto_int;
  HistogramNumber<double, double>* testhisto_double;

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
