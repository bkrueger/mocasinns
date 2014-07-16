#ifndef TEST_FIXED_BOUNDARY_BINNING_HPP
#define TEST_FIXED_BOUNDARY_BINNING_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/histograms/fixed_boundary_binning.hpp>
#include <mocasinns/energy_types/vector_energy.hpp>

using namespace Mocasinns::Histograms;

class TestFixedBoundaryBinning : CppUnit::TestFixture
{
public:
  typedef FixedBoundaryBinning<int> binning_t_int;
  typedef FixedBoundaryBinning<double> binning_t_double;

  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_functor();
private:
  binning_t_int* test_binning_int;
  binning_t_double* test_binning_double;
};

#endif
