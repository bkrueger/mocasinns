#ifndef TEST_TUPLE_ADDABLE_HPP
#define TEST_TUPLE_ADDABLE_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/details/stl_extensions/tuple_addable.hpp>

using namespace Mocasinns::Details::STL_Extensions;

class TestTupleAddable : CppUnit::TestFixture
{
private:
  TupleAddable<double,int,double>* tuple_addable;

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
