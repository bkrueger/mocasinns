#ifndef TEST_HISTOGRAM_ACCUMULATOR_HPP
#define TEST_HISTOGRAM_ACCUMULATOR_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/histograms/histogram.hpp>
#include <mocasinns/accumulators/histogram_accumulator.hpp>

using namespace Mocasinns::Accumulators;
using namespace Mocasinns::Histograms;

class TestHistogramAccumulator : CppUnit::TestFixture
{
private:
  HistogramAccumulator<HistogramConstantWidth, double> histogram_accumulator;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_operator_accumulate();
  void test_normalized_histogram();
};

#endif
