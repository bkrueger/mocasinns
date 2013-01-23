#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>

#include <random_boost_mt19937.hpp>

#ifndef TEST_BOOST_MT19937_HPP
#define TEST_BOOST_MT19937_HPP

class TestBoostMT19937 : CppUnit::TestFixture
{
private:
  Boost_MT19937* test_rng;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void TestUniformity();
  void TestIntRange();
  void TestDoubleRange();
};

#endif
