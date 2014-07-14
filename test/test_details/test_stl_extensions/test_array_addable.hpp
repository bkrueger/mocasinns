#ifndef TEST_ARRAY_ADDABLE_HPP
#define TEST_ARRAY_ADDABLE_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/details/stl_extensions/array_addable.hpp>

using namespace Mocasinns::Details::STL_Extensions;

class TestArrayAddable : CppUnit::TestFixture
{
private:
  ArrayAddablePlain<double,3>* array_addable_double;
  ArrayAddablePlain<int,3>* array_addable_int;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_constructor();

  void test_operator_add();
  void test_operator_substract();
  void test_operator_multiply();
  void test_operator_divide();
  void test_operator_outstream();
  void test_operator_instream();

  void test_serialization();
};

#endif
