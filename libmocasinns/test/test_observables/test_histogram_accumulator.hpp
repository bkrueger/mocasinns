#ifndef TEST_HISTOGRAM_ACCUMULATOR_HPP
#define TEST_HISTOGRAM_ACCUMULATOR_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <histograms/histogram_number.hpp>
#include <observables/histogram_accumulator.hpp>

using namespace Mocasinns::Observables;
using namespace Mocasinns::Histograms;

class TestHistogramAccumulator : CppUnit::TestFixture
{
private:
  HistogramAccumulator<HistogramNumber, double> histogram_accumulator;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_operator_accumulate();
  void test_normalized_histogram();
};

#endif
