#ifndef TEST_BINNING_ANALYSIS_HPP
#define TEST_BINNING_ANALYSIS_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/analysis/binning_analysis.hpp>

using namespace Mocasinns::Analysis;

class TestBinningAnalysis : CppUnit::TestFixture
{
public:
  static CppUnit::Test* suite();

  void setUp() {}
  void tearDown() {}
  
  void test_analyse_doubles();
  void test_analyse_vector_observables();
};

#endif
