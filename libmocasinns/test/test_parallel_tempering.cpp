#include "test_parallel_tempering.hpp"

#include <vector>
#include <cstdint>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/error_of_mean.hpp>

#include <mocasinns/observables/vector_observable.hpp>

namespace ba = boost::accumulators;

//! Helper class to measure the energy of a configuration
class TestParallelTempering::ObserveIsingEnergy
{
public:
  typedef double observable_type;
  static observable_type observe(ConfigurationType* config) { return config->energy(); }
};
//! Helper class to measure the energy and the magnetization of a configuration
class TestParallelTempering::ObserveIsingEnergyMagnetization
{
public:
  typedef Observables::VectorObservable<double> observable_type;
  static observable_type observe(ConfigurationType* config)
  {
    observable_type result(2, 0.0);
    result[0] = config->energy();
    result[1] = config->magnetization();
    return result;
  }
};

CppUnit::Test* TestParallelTempering::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestParallelTempering");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestParallelTempering>("TestParallelTempering: test_do_parallel_tempering_steps", &TestParallelTempering::test_do_parallel_tempering_steps) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestParallelTempering>("TestParallelTempering: test_do_parallel_tempering_simulation", &TestParallelTempering::test_do_parallel_tempering_simulation) );
    
  return suite_of_tests;
}

void TestParallelTempering::setUp()
{
  // Assign the simulation parameters
  SimulationType::Parameters test_parameters;
  test_parameters.relaxation_steps = 100;
  test_parameters.measurement_number = 1000;
  test_parameters.steps_between_measurement = 1000;
  test_parameters.steps_between_replica_exchange = 100;
  test_parameters.process_number = 4;

  // 2d-lattice of Ising spins
  std::vector<unsigned int> size_2d;
  size_2d.push_back(2); size_2d.push_back(2);
  // Create copies of the lattice
  for (unsigned int i = 0; i < 5; ++i)
    test_config_space_vector.push_back(new ConfigurationType(size_2d));

  // Create the simulation
  test_simulation = new SimulationType(test_parameters, test_config_space_vector.begin(), test_config_space_vector.end());

  // Create the inverse temperatures
  inverse_temperatures.push_back(0.0);
  inverse_temperatures.push_back(0.2);
  inverse_temperatures.push_back(0.4);
  inverse_temperatures.push_back(0.6);
  inverse_temperatures.push_back(0.8);
}

void TestParallelTempering::tearDown()
{
  for (unsigned int i = 0; i < 5; ++i)
    delete test_config_space_vector[i];

  delete test_simulation;
}

void TestParallelTempering::test_do_parallel_tempering_steps()
{
  std::vector<double> sum(5, 0.0);
  int n = 0;

  for (uint32_t i = 0; i < 10000; i++)
  {
    n++;
    test_simulation->do_parallel_tempering_steps(100, inverse_temperatures.begin(), inverse_temperatures.end());
    for (unsigned int r = 0; r < 5; ++r)
      sum[r] += test_simulation->get_config_space(r)->energy();
  }
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL(+0.00, sum[0]/static_cast<double>(n), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.41, sum[1]/static_cast<double>(n), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-6.41, sum[2]/static_cast<double>(n), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-7.62, sum[3]/static_cast<double>(n), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-7.92, sum[4]/static_cast<double>(n), 0.5);
}

void TestParallelTempering::test_do_parallel_tempering_simulation()
{
  // Perform the simulation without accumulator
  std::vector<std::vector<double> > result_vector = test_simulation->do_parallel_tempering_simulation<ObserveIsingEnergy>(inverse_temperatures.begin(), inverse_temperatures.end());
  // Check that the measurement number is correct
  //  CPPUNIT_ASSERT_EQUAL(10000, (int)result_vector[0].size());
  // Accumulate the vector
  std::vector<double> sum(5, 0.0);
  for (unsigned int b = 0; b < 5; ++b)
  {
    for (unsigned int i = 0; i < result_vector[b].size(); ++i) sum[b] += result_vector[b].at(i);
  }
  // Test the results
  CPPUNIT_ASSERT_DOUBLES_EQUAL(+0.00, sum[0]/static_cast<double>(result_vector[0].size()), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.41, sum[1]/static_cast<double>(result_vector[1].size()), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-6.41, sum[2]/static_cast<double>(result_vector[2].size()), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-7.62, sum[3]/static_cast<double>(result_vector[3].size()), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-7.92, sum[4]/static_cast<double>(result_vector[4].size()), 0.5);

  // Create a boost accumulator and test the default observable(energy)
  typedef ba::accumulator_set<double, ba::stats<ba::tag::mean, ba::tag::error_of<ba::tag::mean> > > AccumulatorType;
  AccumulatorType acc_default;
  // Perform the simulation with the defualt observable
  std::vector<AccumulatorType> acc_default_vector = test_simulation->do_parallel_tempering_simulation(acc_default, inverse_temperatures.begin(), inverse_temperatures.end());
  std::cout << std::endl;
  std::cout << "beta = 0.0, <E> = " << ba::mean(acc_default_vector[0]) << " +- " << ba::error_of<ba::tag::mean>(acc_default_vector[0]) << std::endl;
  std::cout << "beta = 0.2, <E> = " << ba::mean(acc_default_vector[1]) << " +- " << ba::error_of<ba::tag::mean>(acc_default_vector[1]) << std::endl;
  std::cout << "beta = 0.4, <E> = " << ba::mean(acc_default_vector[2]) << " +- " << ba::error_of<ba::tag::mean>(acc_default_vector[2]) << std::endl;
  std::cout << "beta = 0.6, <E> = " << ba::mean(acc_default_vector[3]) << " +- " << ba::error_of<ba::tag::mean>(acc_default_vector[3]) << std::endl;
  std::cout << "beta = 0.8, <E> = " << ba::mean(acc_default_vector[4]) << " +- " << ba::error_of<ba::tag::mean>(acc_default_vector[4]) << std::endl;
  std::cout << "Replica exchanges rejected:    " << test_simulation->get_replica_exchange_log()[0] << std::endl;
  std::cout << "Replica exchanges 0.0 <-> 0.2: " << test_simulation->get_replica_exchange_log()[1] << std::endl;
  std::cout << "Replica exchanges 0.2 <-> 0.4: " << test_simulation->get_replica_exchange_log()[2] << std::endl;
  std::cout << "Replica exchanges 0.4 <-> 0.6: " << test_simulation->get_replica_exchange_log()[3] << std::endl;
  std::cout << "Replica exchanges 0.6 <-> 0.8: " << test_simulation->get_replica_exchange_log()[4] << std::endl;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(+0.00, ba::mean(acc_default_vector[0]), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.41, ba::mean(acc_default_vector[1]), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-6.41, ba::mean(acc_default_vector[2]), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-7.62, ba::mean(acc_default_vector[3]), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-7.92, ba::mean(acc_default_vector[4]), 0.5);

  // Create a boost accumulator
  AccumulatorType acc;
  // Perform the simulation with accumulators
  std::vector<AccumulatorType> acc_vector = test_simulation->do_parallel_tempering_simulation<ObserveIsingEnergy>(acc, inverse_temperatures.begin(), inverse_temperatures.end());
  // Test the result
  CPPUNIT_ASSERT_DOUBLES_EQUAL(+0.00, ba::mean(acc_vector[0]), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.41, ba::mean(acc_vector[1]), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-6.41, ba::mean(acc_vector[2]), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-7.62, ba::mean(acc_vector[3]), 0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-7.92, ba::mean(acc_vector[4]), 0.5);
}
