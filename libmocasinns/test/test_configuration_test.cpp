#include "test_configuration_test.hpp"

CppUnit::Test* TestConfigurationTest::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestConfigurationTest");

  suite_of_tests->addTest( new CppUnit::TestCaller<TestConfigurationTest>("TestConfigurationTest: test_test_energy_delta_E", &TestConfigurationTest::test_test_energy_delta_E) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestConfigurationTest>("TestConfigurationTest: test_test_copy", &TestConfigurationTest::test_test_copy) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestConfigurationTest>("TestConfigurationTest: test_test_serialization", &TestConfigurationTest::test_test_serialization) );
    
  return suite_of_tests;
}

void TestConfigurationTest::setUp()
{
  // 2d-lattice of Ising spins
  std::vector<unsigned int> size_2d;
  size_2d.push_back(4); size_2d.push_back(4);

  test_config_space = new ConfigurationType(size_2d);
  test_simulation = new SimulationType(test_config_space);
}

void TestConfigurationTest::tearDown()
{
  delete test_config_space;
  delete test_simulation;
}

void TestConfigurationTest::test_test_energy_delta_E()
{
  CPPUNIT_ASSERT(test_simulation->test_energy_delta_E());
}
void TestConfigurationTest::test_test_copy()
{
  CPPUNIT_ASSERT(test_simulation->test_copy());
}
void TestConfigurationTest::test_test_serialization()
{
  CPPUNIT_ASSERT(test_simulation->test_serialization());
}
