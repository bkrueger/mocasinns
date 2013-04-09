#ifndef SPINISINGTEST_HPP
#define SPINISINGTEST_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>
#include <gespinst/spins/ising_spin.hpp>

using namespace Gespinst;

class TestSpinIsing : public CppUnit::TestFixture
{
private:
  SpinIsing spin_up;
  SpinIsing spin_down;

public:
  static CppUnit::Test* suite();

  void setUp();
  void tearDown();

  void test_constructor();

  void test_get_set_value();

  void test_operator_equal();
  void test_operator_multiply();

  void test_all_possible_values();
  void test_random();
  void test_random_differ();
};

#endif
