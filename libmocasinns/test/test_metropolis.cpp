#include "test_metropolis.hpp"

#include <vector>
#include <cstdint>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/error_of_mean.hpp>

namespace ba = boost::accumulators;

//! Helper class to measure the energy of a configuration
class TestMetropolis::ObserveIsingEnergy
{
public:
  typedef double observable_type;
  static observable_type observe(ConfigurationType* config) { return config->energy(); }
};

CppUnit::Test* TestMetropolis::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestMetropolis");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestMetropolis>("TestMetropolis: test_do_metropolis_steps", &TestMetropolis::test_do_metropolis_steps) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestMetropolis>("TestMetropolis: test_do_metropolis_simulation", &TestMetropolis::test_do_metropolis_simulation) );
    
  return suite_of_tests;
}

void TestMetropolis::setUp()
{
  // 2d-lattice of Ising spins
  std::vector<unsigned int> size_2d;
  size_2d.push_back(4); size_2d.push_back(4);

  test_config_space = new ConfigurationType(size_2d);
  test_simulation = new SimulationType(test_config_space);
}

void TestMetropolis::tearDown()
{
  delete test_config_space;
  delete test_simulation;
}

void TestMetropolis::test_do_metropolis_steps()
{
  double sum = 0;
  int n = 0;

  for (uint32_t i = 0; i < 10000; i++)
  {
    n++;
    test_simulation->do_metropolis_steps(100);
    sum += test_simulation->get_config_space()->energy();
  }
  
  double peak = sum / static_cast<double> (n);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, peak, 2.0);
}

void TestMetropolis::test_do_metropolis_simulation()
{
  // Assign the simulation parameters
  SimulationType::Parameters parameters;
  parameters.relaxation_steps = 10000;
  parameters.measurement_number = 10000;
  parameters.steps_between_measurement = 100;

  // Create a boost accumulator
  ba::accumulator_set<double, ba::stats<ba::tag::mean, ba::tag::error_of<ba::tag::mean> > > acc;
  // Perform the simulation with accumulators
  test_simulation->do_metropolis_simulation<ObserveIsingEnergy>(parameters, 0.0, acc);
  // Test the result
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, ba::mean(acc), 0.1);

  // Perform the simulation without accumulator
  std::vector<double> result_vector = test_simulation->do_metropolis_simulation<ObserveIsingEnergy>(parameters, 0.0);
  // Check that the measurement number is correct
  CPPUNIT_ASSERT_EQUAL(10000, (int)result_vector.size());
  // Accumulate the vector
  double sum = 0.0;
  for (unsigned int i = 0; i < result_vector.size(); ++i) sum += result_vector.at(i);
  // Test the results
  CPPUNIT_ASSERT_DOUBLES_EQUAL(ba::mean(acc), sum/result_vector.size(), 0.15);
}
