#include "test_simulation.hpp"

#include <mocasinns/details/optional_member_functions.hpp>

#include <vector>
#include <cstdint>

#include <boost/static_assert.hpp>

CppUnit::Test* TestSimulation::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestSimulation");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestSimulation>("TestSimulation: test_serialize", &TestSimulation::test_serialize) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestSimulation>("TestSimulation: test_optional_member_functions", &TestSimulation::test_optional_member_functions) );
    
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

// Create different classes
class WithIsExecutable { public: bool is_executable() { return false; } };
class WithSelectionProbabilityFactor { public: double selection_probability_factor() { return 2.0; } };

void TestSimulation::test_optional_member_functions()
{
  // Test the has_function functions
  BOOST_STATIC_ASSERT_MSG(Mocasinns::Details::has_function_is_executable<WithIsExecutable, bool>::value == true,
			  "has_function_is_executable did not detect the (static) member function is_executable()");
  BOOST_STATIC_ASSERT_MSG(Mocasinns::Details::has_function_is_executable<WithSelectionProbabilityFactor, bool>::value == false,
			  "has_function_is_executable did detect a non-existant (static) member function is_executable()");
  BOOST_STATIC_ASSERT_MSG(Mocasinns::Details::has_function_selection_probability_factor<WithSelectionProbabilityFactor, double>::value == true,
			  "has_function_selection_probability_factor did not detect the (static) member selection_probability_factor()");
  BOOST_STATIC_ASSERT_MSG(Mocasinns::Details::has_function_selection_probability_factor<WithIsExecutable, double>::value == false,
			  "has_function_selection_probability_factor did detect a non-existant (static) member selection_probability_factor()");

  // Test the default function values
  WithIsExecutable with_is_executable;
  WithSelectionProbabilityFactor with_selection_probability_factor;
  CPPUNIT_ASSERT(!Mocasinns::Details::OptionalMemberFunctions::optional_is_executable<WithIsExecutable>(with_is_executable));
  CPPUNIT_ASSERT(Mocasinns::Details::OptionalMemberFunctions::optional_is_executable<WithSelectionProbabilityFactor>(with_selection_probability_factor));
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, Mocasinns::Details::OptionalMemberFunctions::optional_selection_probability_factor<WithIsExecutable>(with_is_executable), 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, Mocasinns::Details::OptionalMemberFunctions::optional_selection_probability_factor<WithSelectionProbabilityFactor>(with_selection_probability_factor), 1e-4);
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
