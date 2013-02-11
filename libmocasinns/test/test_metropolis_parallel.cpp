#include "test_metropolis_parallel.hpp"

#include <vector>
#include <cstdint>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>

namespace ba = boost::accumulators;

//! Helper class to measure the energy of a configuration
class TestMetropolisParallel::ObserveIsingEnergy
{
public:
  typedef double observable_type;
  static observable_type observe(ConfigurationType* config) { return config->energy(); }
};

CppUnit::Test* TestMetropolisParallel::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestMetropolisParallel");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestMetropolisParallel>("TestMetropolisParallel: test_do_parallel_metropolis_simulation", &TestMetropolisParallel::test_do_parallel_metropolis_simulation) );
    
  return suite_of_tests;
}

void TestMetropolisParallel::setUp()
{
  // 2d-lattice of Ising spins
  std::vector<unsigned int> size_2d;
  size_2d.push_back(4); size_2d.push_back(4);

  test_config_space = new ConfigurationType(size_2d);
  test_simulation = new SimulationType(test_config_space);
}

void TestMetropolisParallel::tearDown()
{
  delete test_config_space;
  delete test_simulation;
}

void TestMetropolisParallel::test_do_parallel_metropolis_simulation()
{
  // Assign the simulation parameters for the parallel simulation
  SimulationType::Parameters parameters_parallel;
  parameters_parallel.relaxation_steps = 10000;
  parameters_parallel.measurement_number = 1000;
  parameters_parallel.steps_between_measurement = 1000;
  parameters_parallel.run_number = 4;
  // Assign the simulation parameters for the serial simulation
  SimulationTypeSerial::Parameters parameters_serial;
  parameters_serial.relaxation_steps = 10000;
  parameters_serial.measurement_number = 1000;
  parameters_serial.steps_between_measurement = 1000;

  // Perform four serial simulations and store the vector
  SimulationTypeSerial serial_simulation_0(new ConfigurationType(*test_config_space));
  SimulationTypeSerial serial_simulation_1(new ConfigurationType(*test_config_space));
  SimulationTypeSerial serial_simulation_2(new ConfigurationType(*test_config_space));
  SimulationTypeSerial serial_simulation_3(new ConfigurationType(*test_config_space));
  serial_simulation_0.set_random_seed(0);
  serial_simulation_1.set_random_seed(1);
  serial_simulation_2.set_random_seed(2);
  serial_simulation_3.set_random_seed(3);
  std::vector<double> serial_result_0 = serial_simulation_0.do_metropolis_simulation<ObserveIsingEnergy>(parameters_serial, 0.0);
  std::vector<double> serial_result_1 = serial_simulation_1.do_metropolis_simulation<ObserveIsingEnergy>(parameters_serial, 0.0);
  std::vector<double> serial_result_2 = serial_simulation_2.do_metropolis_simulation<ObserveIsingEnergy>(parameters_serial, 0.0);
  std::vector<double> serial_result_3 = serial_simulation_3.do_metropolis_simulation<ObserveIsingEnergy>(parameters_serial, 0.0);

  // Perform the parallel simulation
  test_simulation->set_random_seed(0);
  std::vector<double> parallel_result = test_simulation->do_parallel_metropolis_simulation<ObserveIsingEnergy>(parameters_parallel, 0.0);

  // Check that the size of the two results agree
  CPPUNIT_ASSERT_EQUAL(4*serial_result_0.size(), parallel_result.size());
  
  // Define two accumulators and check that the moments of the simulation agree
  ba::accumulator_set<double, ba::stats<ba::tag::mean, ba::tag::moment<2> > > accumulator_serial;
  ba::accumulator_set<double, ba::stats<ba::tag::mean, ba::tag::moment<2> > > accumulator_parallel;
  for (unsigned int i = 0; i < serial_result_0.size(); ++i)
  {
    accumulator_serial(serial_result_0[i]);
    accumulator_serial(serial_result_1[i]);
    accumulator_serial(serial_result_2[i]);
    accumulator_serial(serial_result_3[i]);
  }
  for (unsigned int j = 0; j < parallel_result.size(); ++j)
  {
    accumulator_parallel(parallel_result[j]);
  }

  // Check that the moments agree
  CPPUNIT_ASSERT_DOUBLES_EQUAL(ba::mean(accumulator_serial), ba::mean(accumulator_parallel), 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(ba::moment<2>(accumulator_serial), ba::moment<2>(accumulator_parallel), 1e-4);
}
