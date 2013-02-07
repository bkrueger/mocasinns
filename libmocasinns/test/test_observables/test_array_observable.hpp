#ifndef TEST_ARRAY_OBSERVABLE_HPP
#define TEST_ARRAY_OBSERVABLE_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <observables/array_observable.hpp>

using namespace Mocasinns::Observables;

class TestArrayObservable : CppUnit::TestFixture
{
private:
  ArrayObservable<double,3>* array_observable_double;
  ArrayObservable<int,3>* array_observable_int;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_constructor();

  void test_operator_add();
  void test_operator_substract();
  void test_operator_multiply();
  void test_operator_divide();
  void test_pow();
};

#endif
