#ifndef SPINREALTEST_HPP
#define SPINREALTEST_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/spin_real.hpp"

class TestSpinReal : public CppUnit::TestFixture
{
private:
  SpinReal spin_1;
  SpinReal spin_2;
  SpinReal spin_3;

public:
  static CppUnit::Test* suite();

  void setUp();
  void tearDown();

  void test_constructor();

  void test_get_set_value();

  void test_operator_equal();
  void test_operator_multiply();

  void test_random();
  void test_random_differ();
};

#endif
