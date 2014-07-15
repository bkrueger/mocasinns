#ifndef TEST_PAIR_ENERGY_HPP
#define TEST_PAIR_ENERGY_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/energy_types/pair_energy.hpp>

using namespace Mocasinns::EnergyTypes;

class TestPairEnergy : CppUnit::TestFixture
{
private:
  PairEnergy<int,double> pair_energy_1;
  PairEnergy<int,double> pair_energy_2;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_operator_multiply();
};

#endif
