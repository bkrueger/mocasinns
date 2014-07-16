#ifndef TEST_CONSTANT_WIDTH_BINNING_HPP
#define TEST_CONSTANT_WIDTH_BINNING_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/histograms/constant_width_binning.hpp>
#include <mocasinns/energy_types/vector_energy.hpp>

using namespace Mocasinns::Histograms;

class TestConstantWidthBinningNumber : CppUnit::TestFixture
{
public:
  typedef ConstantWidthBinning<int> binning_t_int;
  typedef ConstantWidthBinning<double> binning_t_double;

  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_functor();
private:
  binning_t_int* test_binning_int;
  binning_t_int* test_binning_int_base;
  binning_t_double* test_binning_double;
  binning_t_double* test_binning_double_base;
};

class TestConstantWidthBinningVector : CppUnit::TestFixture
{
public:
  typedef Mocasinns::EnergyTypes::VectorEnergy<double> binning_value_t;
  typedef ConstantWidthBinning<binning_value_t> binning_t;

  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_functor();

private:
  binning_t* test_binning_double_same;
  binning_t* test_binning_double_diff;
};

#endif
