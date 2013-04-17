#ifndef TEST_VECTOR_ENERGY_HPP
#define TEST_VECTOR_ENERGY_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/energy_types/vector_energy.hpp>

using namespace Mocasinns::EnergyTypes;

class TestVectorEnergy : CppUnit::TestFixture
{
private:
  VectorEnergy<double>* vector_energy_double;
  VectorEnergy<int>* vector_energy_int;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_operator_multiply();
};

#endif
