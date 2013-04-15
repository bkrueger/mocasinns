#ifndef TEST_ENERGY_TYPE_EXTENDED_HPP
#define TEST_ENERGY_TYPE_EXTENDED_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/details/multicanonical/energy_type_extended.hpp>

using namespace Mocasinns::Details::Multicanonical;

typedef EnergyTypeExtended<int> ExtendedInt;

class TestEnergyTypeExtended : CppUnit::TestFixture
{
private:
  ExtendedInt* test_extended_int_1;
  ExtendedInt* test_extended_int_2;
  ExtendedInt* test_extended_int_3;
  ExtendedInt* test_extended_int_4;
  
public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_constructor();
  
  void test_operator_compare();
  void test_operator_add();
  void test_operator_substract();
};

#endif
