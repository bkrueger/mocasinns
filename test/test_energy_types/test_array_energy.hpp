#ifndef TEST_ARRAY_ENERGY_HPP
#define TEST_ARRAY_ENERGY_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/energy_types/array_energy.hpp>

using namespace Mocasinns::EnergyTypes;

class TestArrayEnergy : CppUnit::TestFixture
{
private:
  ArrayEnergy<double,3>* array_energy_double;
  ArrayEnergy<int,3>* array_energy_int;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_operator_multiply();
};

#endif
