#ifndef SPINPOTTSTEST_HPP
#define SPINPOTTSTEST_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>
#include <gespinst/spins/potts_spin.hpp>

using namespace Gespinst;

class TestSpinPotts : public CppUnit::TestFixture
{
private:
  SpinPotts spin_0;
  SpinPotts spin_1;
  SpinPotts spin_2;
  SpinPotts spin_3;

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
