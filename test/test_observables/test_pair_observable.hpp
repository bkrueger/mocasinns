#ifndef TEST_PAIR_OBSERVABLE_HPP
#define TEST_PAIR_OBSERVABLE_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/observables/pair_observable.hpp>

using namespace Mocasinns::Observables;

class TestPairObservable : CppUnit::TestFixture
{
private:
  PairObservable<int, double> pair_observable;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_constructor();

  void test_operator_multiply();
  void test_operator_divide();
  void test_pow();
};

#endif
