#ifndef HISTOGRAMTEST_HPP
#define HISTOGRAMTEST_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <histogram.hpp>
#include <binnings.hpp>

using namespace Mocasinns::Histograms;

class TestHistogram : public CppUnit::TestFixture
{
private:
  Histogram<int, int, BinningNumber<int> >* testhisto_int;
  Histogram<double, double, BinningNumber<double> >* testhisto_double;

public:
  static CppUnit::Test* suite();

  void setUp();
  void tearDown();

  void test_operator_fill();
  void test_operator_access();
  void test_operator_increment();
  void test_operator_divide();

  void test_insert();
  void test_serialize();
};

#endif
