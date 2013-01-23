#include "test_boost_mt19937.hpp"
#include <cstdint>
#include <iostream>

CppUnit::Test* TestBoostMT19937::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestBoostMT19937");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestBoostMT19937>("TestUniformity", &TestBoostMT19937::TestUniformity) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestBoostMT19937>("TestIntRange", &TestBoostMT19937::TestIntRange) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestBoostMT19937>("TestDoubleRange", &TestBoostMT19937::TestDoubleRange) );
  
  return suite_of_tests;
}

void TestBoostMT19937::setUp()
{
  test_rng = new Boost_MT19937();
}

void TestBoostMT19937::tearDown()
{
  delete test_rng;
}

void TestBoostMT19937::TestUniformity()
{
  double sum = 0;
  int n = 0;
  for (uint32_t i = 0; i < 1000000; i++)
    {
      n++;
      sum += test_rng->random_double();
    }
  double peak = sum / static_cast<double> (n);
  CPPUNIT_ASSERT((0.49 <= peak) && (peak <= 0.51));
}

void TestBoostMT19937::TestIntRange()
{
  uint32_t int_max = test_rng->get_int_max();
  for (uint32_t i = 0; i < 1000000; i++)
    {
      uint32_t rng = test_rng->random_uint32();
      CPPUNIT_ASSERT(rng <= int_max);
    }
  int_max = 10000;
  test_rng->set_int_max(int_max);
  for (uint32_t i = 0; i < 1000000; i++)
    {
      uint32_t rng = test_rng->random_uint32();
      CPPUNIT_ASSERT(rng <= int_max);
    }
  
  // Test that two equal values return allways the identical value
  for (uint32_t i = 0; i < 1000000; i++)
    {
      uint32_t rng = test_rng->random_uint32(0,0);
      uint32_t rng2 = test_rng->random_uint32(2,2);
      CPPUNIT_ASSERT_EQUAL((int)rng, 0);
      CPPUNIT_ASSERT_EQUAL((int)rng2, 2);
    }
}

void TestBoostMT19937::TestDoubleRange()
{
  for (uint32_t i = 0; i < 1000000; i++)
    {
      double rng = test_rng->random_double();
      CPPUNIT_ASSERT((0.0 <= rng) && (rng < 1.0));
    }
}
