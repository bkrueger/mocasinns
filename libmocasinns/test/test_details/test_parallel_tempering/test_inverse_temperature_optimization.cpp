#include "test_inverse_temperature_optimization.hpp"

#include <vector>
#include <cstdint>

namespace ba = boost::accumulators;


CppUnit::Test* TestInverseTemperatureOptimization::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestInverseTemperatureOptimization");

  suite_of_tests->addTest( new CppUnit::TestCaller<TestInverseTemperatureOptimization>("TestInverseTemperatureOptimization: test_optimize_equal_acceptance_probabilities", &TestInverseTemperatureOptimization::test_optimize_equal_acceptance_probabilities) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestInverseTemperatureOptimization>("TestInverseTemperatureOptimization: test_optimize_equal_acceptance_probabilities_worst_acceptance", &TestInverseTemperatureOptimization::test_optimize_equal_acceptance_probabilities_worst_acceptance) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestInverseTemperatureOptimization>("TestInverseTemperatureOptimization: test_optimize_equal_acceptance_probabilities_independent_acceptance", &TestInverseTemperatureOptimization::test_optimize_equal_acceptance_probabilities_independent_acceptance) );
    
  return suite_of_tests;
}

void TestInverseTemperatureOptimization::setUp()
{
  // Assign the simulation parameters
  test_parameters.relaxation_steps = 100;
  test_parameters.measurement_number = 1000;
  test_parameters.steps_between_measurement = 10000;
  test_parameters.steps_between_replica_exchange = 100;
  test_parameters.process_number = 4;
  // Assign the measurement parameters
  measurement_parameters.relaxation_steps = 100;
  measurement_parameters.measurement_number = 1000;
  measurement_parameters.steps_between_measurement = 100000;
  measurement_parameters.steps_between_replica_exchange = 100;
  measurement_parameters.process_number = 4;

  // 2d-lattice of Ising spins
  std::vector<unsigned int> size_2d;
  size_2d.push_back(2); size_2d.push_back(2);
  // Create copies of the lattice
  for (unsigned int i = 0; i < 5; ++i)
    test_config_space_vector.push_back(new ConfigurationType(size_2d));

  // Create the simulation
  test_simulation = new SimulationType(test_parameters, test_config_space_vector.begin(), test_config_space_vector.end());
  measurement_simulation = new SimulationType(measurement_parameters, test_config_space_vector.begin(), test_config_space_vector.end());

  // Create the inverse temperatures
  inverse_temperatures.push_back(0.0);
  inverse_temperatures.push_back(0.2);
  inverse_temperatures.push_back(0.4);
  inverse_temperatures.push_back(0.6);
  inverse_temperatures.push_back(0.8);
  inverse_temperatures_negative.push_back(-0.2);
  inverse_temperatures_negative.push_back(-0.1);
  inverse_temperatures_negative.push_back(0.0);
  inverse_temperatures_negative.push_back(0.1);
  inverse_temperatures_negative.push_back(0.2);
}

void TestInverseTemperatureOptimization::tearDown()
{
  for (unsigned int i = 0; i < 5; ++i)
    delete test_config_space_vector[i];

  delete test_simulation;
  delete measurement_simulation;
}

void TestInverseTemperatureOptimization::test_optimize_equal_acceptance_probabilities()
{
  InverseTemperatureOptimizationType inverse_temperature_optimization(test_simulation, 100);
  InverseTemperatureOptimizationType measurement(measurement_simulation, 1);

  // Do the optimization for positive temperatures
  inverse_temperature_optimization.optimize(inverse_temperatures.begin(), inverse_temperatures.end());

  // Measure the acceptance probabilities
  measurement.optimize(inverse_temperatures.begin(), inverse_temperatures.end());
  // Get the last acceptance probabilities
  std::vector<double> acceptance_probabilities = measurement.get_acceptance_probabilities().back();
  
  // Test wether the acceptance probabilities are nearly the same
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, acceptance_probabilities[1]/acceptance_probabilities[0], 0.2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, acceptance_probabilities[2]/acceptance_probabilities[1], 0.2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, acceptance_probabilities[3]/acceptance_probabilities[2], 0.2);


  // Do the optimization for negative temperatures
  inverse_temperature_optimization.optimize(inverse_temperatures_negative.begin(), inverse_temperatures_negative.end());

  // Measure the acceptance probabilities
  measurement.optimize(inverse_temperatures_negative.begin(), inverse_temperatures_negative.end());
  // Get the last acceptance probabilities
  std::vector<double> acceptance_probabilities_negative = measurement.get_acceptance_probabilities().back();
  
  // Test wether the acceptance probabilities are nearly the same
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, acceptance_probabilities_negative[1]/acceptance_probabilities_negative[0], 0.2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, acceptance_probabilities_negative[2]/acceptance_probabilities_negative[1], 0.2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, acceptance_probabilities_negative[3]/acceptance_probabilities_negative[2], 0.2);
}

void TestInverseTemperatureOptimization::test_optimize_equal_acceptance_probabilities_worst_acceptance()
{
  InverseTemperatureOptimizationType_WorstAcceptance inverse_temperature_optimization(test_simulation, 100);
  InverseTemperatureOptimizationType_WorstAcceptance measurement(measurement_simulation, 1);

  // Do the optimization for positive temperatures
  inverse_temperature_optimization.optimize(inverse_temperatures.begin(), inverse_temperatures.end());

  // Measure the acceptance probabilities
  measurement.optimize(inverse_temperatures.begin(), inverse_temperatures.end());
  // Get the last acceptance probabilities
  std::vector<double> acceptance_probabilities = measurement.get_acceptance_probabilities().back();
  
  // Test wether the acceptance probabilities are nearly the same
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, acceptance_probabilities[1]/acceptance_probabilities[0], 0.2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, acceptance_probabilities[2]/acceptance_probabilities[1], 0.2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, acceptance_probabilities[3]/acceptance_probabilities[2], 0.2);


  // Do the optimization for negative temperatures
  inverse_temperature_optimization.optimize(inverse_temperatures_negative.begin(), inverse_temperatures_negative.end());

  // Measure the acceptance probabilities
  measurement.optimize(inverse_temperatures_negative.begin(), inverse_temperatures_negative.end());
  // Get the last acceptance probabilities
  std::vector<double> acceptance_probabilities_negative = measurement.get_acceptance_probabilities().back();
  
  // Test wether the acceptance probabilities are nearly the same
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, acceptance_probabilities_negative[1]/acceptance_probabilities_negative[0], 0.2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, acceptance_probabilities_negative[2]/acceptance_probabilities_negative[1], 0.2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, acceptance_probabilities_negative[3]/acceptance_probabilities_negative[2], 0.2);
}

void TestInverseTemperatureOptimization::test_optimize_equal_acceptance_probabilities_independent_acceptance()
{
  // Do the optimization
  InverseTemperatureOptimizationType_IndependentAcceptance inverse_temperature_optimization(test_simulation, 100);
  inverse_temperature_optimization.optimize(inverse_temperatures.begin(), inverse_temperatures.end());
  inverse_temperatures = inverse_temperature_optimization.get_inverse_temperatures().back();
  
  // Measure the acceptance probabilities
  InverseTemperatureOptimizationType_IndependentAcceptance measurement(measurement_simulation, 1);
  measurement.optimize(inverse_temperatures.begin(), inverse_temperatures.end());

  // Get the last acceptance probabilities
  std::vector<double> acceptance_probabilities = measurement.get_acceptance_probabilities().back();
  
  // Test wether the acceptance probabilities are nearly the same
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, acceptance_probabilities[1]/acceptance_probabilities[0], 0.2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, acceptance_probabilities[2]/acceptance_probabilities[1], 0.2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, acceptance_probabilities[3]/acceptance_probabilities[2], 0.2);
}
