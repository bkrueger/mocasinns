#include "test_optimal_ensemble_sampling.hpp"

CppUnit::Test* TestOptimalEnsembleSampling::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestOptimalEnsembleSampling");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestOptimalEnsembleSampling>("TestOptimalEnsembleSampling: test_do_optimal_ensemble_sampling_simulation", &TestOptimalEnsembleSampling::test_do_optimal_ensemble_sampling_simulation) );

  return suite_of_tests;
}

void TestOptimalEnsembleSampling::setUp()
{
  // 1d-lattice of Ising spins
  std::vector<unsigned int> size_1d;
  size_1d.push_back(16);
  parameters_1d.initial_steps_per_iteration = 160;
  parameters_1d.iterations = 15;
  parameters_1d.minimal_energy = -16.0;
  parameters_1d.maximal_energy = -16.0;

  test_ising_config_1d = new IsingConfiguration1d(size_1d);
  test_ising_simulation_1d = new IsingSimulation1d(parameters_1d, test_ising_config_1d);

  // 2d-lattice of Ising spins
  std::vector<unsigned int> size_2d;
  size_2d.push_back(4); size_2d.push_back(4);
  parameters_2d.initial_steps_per_iteration = 160;
  parameters_2d.iterations = 10;
  parameters_2d.minimal_energy = -32.0;
  parameters_2d.maximal_energy = -32.0;

  test_ising_config_2d = new IsingConfiguration2d(size_2d);
  test_ising_simulation_2d = new IsingSimulation2d(parameters_2d, test_ising_config_2d);
}

void TestOptimalEnsembleSampling::tearDown()
{
  delete test_ising_simulation_1d;
  delete test_ising_simulation_2d;
  delete test_ising_config_1d;
  delete test_ising_config_2d;
}

void TestOptimalEnsembleSampling::test_do_optimal_ensemble_sampling_simulation()
{
  // Register the information handler
  //  test_ising_simulation_1d->signal_handler_modfac_change.connect(IsingSimulation1d::SimulationStatus());

  // Do a complete Wang-Landau simulation in the 1d case
  //  test_ising_simulation_1d->estimate_weights();
  Histograms::Histocrete<int, double> entropy_estimation_1d = test_ising_simulation_1d->do_optimal_ensemble_sampling_simulation();

  std::cout << "Resulting dos: " << std::endl;
  entropy_estimation_1d.print();

  // Test the density of states histocrete
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, entropy_estimation_1d[-16], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.7875, entropy_estimation_1d[-12], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.5066, entropy_estimation_1d[-8], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(8.9882, entropy_estimation_1d[-4], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(9.4627, entropy_estimation_1d[0], 0.2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(8.9882, entropy_estimation_1d[4], 0.2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.5066, entropy_estimation_1d[8], 0.2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.7875, entropy_estimation_1d[12], 0.2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, entropy_estimation_1d[16], 0.2);

  // Do a complete Wang-Landau simulation in the 2d case
  test_ising_simulation_2d->estimate_weights();
  Histograms::Histocrete<int, double> entropy_estimation_2d = test_ising_simulation_2d->do_optimal_ensemble_sampling_simulation();

  // Test the density of states histocrete
  Histograms::Histocrete<int, double>::const_iterator ground_state = entropy_estimation_2d.begin();
  Histograms::Histocrete<int, double>::const_iterator first_excited = entropy_estimation_2d.begin();
  first_excited++;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(16.0, exp(first_excited->second) / exp(ground_state->second), 0.9);
}
