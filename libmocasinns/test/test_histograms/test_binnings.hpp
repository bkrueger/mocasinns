#ifndef TEST_BINNINGS_HPP
#define TEST_BINNINGS_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/histograms/binnings.hpp>

using namespace Mocasinns::Histograms;

class TestBinningNumber : CppUnit::TestFixture
{
private:
  BinningNumber<int>* test_binning_int;
  BinningNumber<int>* test_binning_int_base;
  BinningNumber<double>* test_binning_double;
  BinningNumber<double>* test_binning_double_base;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_functor();
};

class TestBinningNumberVector : CppUnit::TestFixture
{
private:
  BinningNumberVector<double>* test_binning_double_same;
  BinningNumberVector<double>* test_binning_double_diff;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_functor();
};

#endif
