#ifndef TEST_AUTOCORRELATION_HPP
#define TEST_AUTOCORRELATION_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/analysis/autocorrelation.hpp>

using namespace Mocasinns::Analysis;

class TestAutocorrelation : CppUnit::TestFixture
{
public:
  static CppUnit::Test* suite();

  void setUp() {}
  void tearDown() {}
  
  void test_autocorrelation_function();
};

#endif
