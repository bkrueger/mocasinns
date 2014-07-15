#include "test_pair_energy.hpp"

CppUnit::Test* TestPairEnergy::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestPairEnergy");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestPairEnergy>("TestObservables/TestPairEnergy: test_operator_multiply", &TestPairEnergy::test_operator_multiply) );
  
  return suite_of_tests;
}

void TestPairEnergy::setUp()
{
  pair_energy_1.first = 2;
  pair_energy_1.second = -1.5;
  pair_energy_2.first = 3;
  pair_energy_2.second = 0.5;
}

void TestPairEnergy::tearDown() { }

void TestPairEnergy::test_operator_multiply()
{
  CPPUNIT_ASSERT_DOUBLES_EQUAL(6.0 - 0.75, pair_energy_1 * pair_energy_2, 1e-8);
}
