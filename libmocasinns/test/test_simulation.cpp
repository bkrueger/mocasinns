#include "test_simulation.hpp"

#include <vector>
#include <cstdint>

CppUnit::Test* TestSimulation::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestSimulation");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestSimulation>("TestSimulation: test_serialize", &TestSimulation::test_serialize) );
    
  return suite_of_tests;
}

void TestSimulation::setUp()
{
  // 2d-lattice of Ising spins
  std::vector<unsigned int> size_2d;
  size_2d.push_back(4); size_2d.push_back(4);

  test_config_space = new ConfigurationType(size_2d);
  test_simulation = new SimulationType(test_config_space);
}

void TestSimulation::tearDown()
{
  delete test_config_space;
  delete test_simulation;
}

void TestSimulation::test_serialize()
{
  // Save the simulation in a file, create a new one and load
  test_simulation->save_serialize("serialize_test.dat");
  SimulationType* test_simulation_loaded = new SimulationType();
  test_simulation_loaded->load_serialize("serialize_test.dat");

  // Compare that the test simulation and the loaded simulation have the same properties
  CPPUNIT_ASSERT(*(test_simulation_loaded->get_config_space()) == *(test_simulation->get_config_space()));

  // Delete the loaded simulation
  delete test_simulation_loaded;
}
