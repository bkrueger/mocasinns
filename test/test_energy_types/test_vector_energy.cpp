#include "test_vector_energy.hpp"

CppUnit::Test* TestVectorEnergy::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestVectorEnergy");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestVectorEnergy>("TestObservables/TestVectorEnergy: test_operator_multiply", &TestVectorEnergy::test_operator_multiply) );
  
  return suite_of_tests;
}

void TestVectorEnergy::setUp()
{
  std::vector<double> init_double;
  init_double.push_back(1.0);
  init_double.push_back(2.5);
  init_double.push_back(-2.0);
  vector_energy_double = new VectorEnergy<double>(init_double);

  std::vector<int> init_int;
  init_int.push_back(2);
  init_int.push_back(6);
  init_int.push_back(-1);
  vector_energy_int = new VectorEnergy<int>(init_int);
}

void TestVectorEnergy::tearDown()
{
  delete vector_energy_double;
  delete vector_energy_int;
}

void TestVectorEnergy::test_operator_multiply()
{
  // Test the scalar multiplication
  double scalar_result_double = (*vector_energy_double) * (*vector_energy_double);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(11.25, scalar_result_double, 1e-4);
  int scalar_result_int = (*vector_energy_int) * (*vector_energy_int);
  CPPUNIT_ASSERT_EQUAL(41, scalar_result_int);
}

