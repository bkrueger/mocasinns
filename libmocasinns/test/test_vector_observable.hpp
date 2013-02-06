#ifndef TEST_VECTOR_OBSERVABLE_HPP
#define TEST_VECTOR_OBSERVABLE_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <observables/vector_observable.hpp>

using namespace Mocasinns::Observables;

class TestVectorObservable : CppUnit::TestFixture
{
private:
  VectorObservable<double>* vector_observable_double;
  VectorObservable<int>* vector_observable_int;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_constructor();

  void test_operator_add();
  void test_operator_substract();
  void test_operator_multiply();
  void test_operator_divide();
};

#endif
