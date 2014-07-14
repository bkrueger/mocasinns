#ifndef HISTOBASETEST_HPP
#define HISTOBASETEST_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/histograms/histobase.hpp>
#include <mocasinns/histograms/histocrete.hpp>

using namespace Mocasinns::Histograms;

class TestHistoBase : public CppUnit::TestFixture
{
private:
  Histocrete<int,int> testhisto_int;
  Histocrete<double, double> testhisto_double;

public:
  static CppUnit::Test* suite();

  void setUp();
  void tearDown();

  void test_operator_equal();
  void test_operator_add();
  void test_operator_substract();
  void test_operator_multiply();
  void test_operator_output();
  void test_compatible();
  void test_derivative();
  void test_flatness();
  void test_initialise_empty();
  void test_max_x_value();
  void test_max_y_value();
  void test_min_x_value();
  void test_min_y_value();
  void test_set_all_y_values();
  void test_shift_bin_zero();
  void test_sum();
  void test_serialize();
  void test_csv();
};

#endif
