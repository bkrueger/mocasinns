#ifndef SPINPOTTSTEST_HPP
#define SPINPOTTSTEST_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>
#include <gespinst/spins/potts_spin.hpp>

using namespace Gespinst;

class TestPottsSpin : public CppUnit::TestFixture
{
private:
  PottsSpin spin_0;
  PottsSpin spin_1;
  PottsSpin spin_2;
  PottsSpin spin_3;

public:
  static CppUnit::Test* suite();

  void setUp();
  void tearDown();

  void test_constructor();

  void test_get_set_value();
  void test_get_set_max_value();

  void test_operator_equal();
  void test_operator_multiply();

  void test_all_possible_values();
  void test_random();
  void test_random_differ();
};

#endif
