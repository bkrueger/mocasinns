#include "test_array_energy.hpp"

CppUnit::Test* TestArrayEnergy::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestArrayEnergy");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayEnergy>("TestObservables/TestArrayEnergy: test_operator_multiply", &TestArrayEnergy::test_operator_multiply) );
  
  return suite_of_tests;
}

void TestArrayEnergy::setUp()
{
  boost::array<double,3> init_double;
  init_double[0] = 1.0;
  init_double[1] = 2.5;
  init_double[2] = -2.0;
  array_energy_double = new ArrayEnergy<double,3>(init_double);

  boost::array<int,3> init_int;
  init_int[0] = 2;
  init_int[1] = 6;
  init_int[2] = -1;
  array_energy_int = new ArrayEnergy<int,3>(init_int);
}

void TestArrayEnergy::tearDown()
{
  delete array_energy_double;
  delete array_energy_int;
}

void TestArrayEnergy::test_operator_multiply()
{
  // Test the scalar multiplication
  double scalar_result_double = (*array_energy_double) * (*array_energy_double);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(11.25, scalar_result_double, 1e-4);
  int scalar_result_int = (*array_energy_int) * (*array_energy_int);
  CPPUNIT_ASSERT_EQUAL(41, scalar_result_int);
}
