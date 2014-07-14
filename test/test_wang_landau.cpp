#include "test_wang_landau.hpp"

CppUnit::Test* TestWangLandau::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestWangLandau");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestWangLandau>("TestWangLandau: test_do_wang_landau_steps", &TestWangLandau::test_do_wang_landau_steps) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestWangLandau>("TestWangLandau: test_do_wang_landau_simulation", &TestWangLandau::test_do_wang_landau_simulation) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestWangLandau>("TestWangLandau: test_serialize", &TestWangLandau::test_serialize) );
    
  return suite_of_tests;
}

void TestWangLandau::setUp()
{
  // 1d-lattice of Ising spins
  std::vector<unsigned int> size_1d;
  size_1d.push_back(16);
  parameters_1d.modification_factor_initial = 1.0;
  parameters_1d.modification_factor_final = 1e-6;
  parameters_1d.modification_factor_multiplier = 0.9;
  parameters_1d.flatness = 0.8;
  parameters_1d.sweep_steps = 1000;

  test_ising_config_1d = new IsingConfiguration1d(size_1d);
  test_ising_simulation_1d = new IsingSimulation1d(parameters_1d, test_ising_config_1d);

  // 2d-lattice of Ising spins
  std::vector<unsigned int> size_2d;
  size_2d.push_back(4); size_2d.push_back(4);
  parameters_2d.modification_factor_initial = 1.0;
  parameters_2d.modification_factor_final = 1e-6;
  parameters_2d.modification_factor_multiplier = 0.9;
  parameters_2d.flatness = 0.8;
  parameters_2d.sweep_steps = 1000;

  test_ising_config_2d = new IsingConfiguration2d(size_2d);
  test_ising_simulation_2d = new IsingSimulation2d(parameters_2d, test_ising_config_2d);
}

void TestWangLandau::tearDown()
{
  delete test_ising_simulation_1d;
  delete test_ising_simulation_2d;
  delete test_ising_config_1d;
  delete test_ising_config_2d;
}

void TestWangLandau::test_do_wang_landau_steps()
{
  // Do steps until the flatness is reached and check
  test_ising_simulation_1d->do_wang_landau_steps();
  CPPUNIT_ASSERT(test_ising_simulation_1d->get_incidence_counter().flatness() >= parameters_1d.flatness);
  test_ising_simulation_2d->do_wang_landau_steps();
  CPPUNIT_ASSERT(test_ising_simulation_2d->get_incidence_counter().flatness() >= parameters_2d.flatness);
}

void TestWangLandau::test_do_wang_landau_simulation()
{
  // Register the information handler
  test_ising_simulation_1d->signal_handler_modfac_change.connect(IsingSimulation1d::SimulationStatus());

  // Do a complete Wang-Landau simulation in the 1d case
  test_ising_simulation_1d->do_wang_landau_simulation();

  // Test the density of states histocrete
  Histograms::Histocrete<int, double> entropy_estimation_1d = test_ising_simulation_1d->get_log_density_of_states();
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, entropy_estimation_1d[-16], 0.01);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.7875, entropy_estimation_1d[-12], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.5066, entropy_estimation_1d[-8], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(8.9882, entropy_estimation_1d[-4], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(9.4627, entropy_estimation_1d[0], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(8.9882, entropy_estimation_1d[4], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(7.5066, entropy_estimation_1d[8], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.7875, entropy_estimation_1d[12], 0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, entropy_estimation_1d[16], 0.1);

  // Do a complete Wang-Landau simulation in the 2d case
  test_ising_simulation_2d->do_wang_landau_simulation();

  // Test the density of states histocrete
  Histograms::Histocrete<int, double> entropy_estimation_2d = test_ising_simulation_2d->get_log_density_of_states();
  Histograms::Histocrete<int, double>::const_iterator ground_state = entropy_estimation_2d.begin();
  Histograms::Histocrete<int, double>::const_iterator first_excited = entropy_estimation_2d.begin();
  first_excited++;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(16.0, exp(first_excited->second) / exp(ground_state->second), 0.9);
}

void TestWangLandau::test_serialize()
{
  // Test the serialization of parameters
  IsingSimulation1d::Parameters parameters_test;
  parameters_test.modification_factor_initial = 2.0;
  parameters_test.modification_factor_final = 1e-6;
  std::ofstream ofs("serialize_parameters.dat");
  {
    boost::archive::text_oarchive oa(ofs);
    oa << parameters_test;
  }
  IsingSimulation1d::Parameters parameters_loaded;
  std::ifstream ifs("serialize_parameters.dat");
  {
    boost::archive::text_iarchive ia(ifs);
    ia >> parameters_loaded;
  }
  CPPUNIT_ASSERT(parameters_test == parameters_loaded);

  // Test before doing any steps
  // Save the 1d ising simulation in a file, create a new one and load
  test_ising_simulation_1d->save_serialize("serialize_test.dat");
  IsingSimulation1d* test_ising_simulation_1d_loaded = new IsingSimulation1d();
  test_ising_simulation_1d_loaded->load_serialize("serialize_test.dat");
  // Test Equality
  CPPUNIT_ASSERT(*test_ising_simulation_1d_loaded->get_config_space() ==
		 *test_ising_simulation_1d->get_config_space());
  CPPUNIT_ASSERT(test_ising_simulation_1d_loaded->get_log_density_of_states() ==
		 test_ising_simulation_1d->get_log_density_of_states());
  CPPUNIT_ASSERT(test_ising_simulation_1d_loaded->get_incidence_counter() ==
		 test_ising_simulation_1d->get_incidence_counter());
  CPPUNIT_ASSERT(test_ising_simulation_1d_loaded->get_simulation_parameters() ==
		 test_ising_simulation_1d->get_simulation_parameters());
  CPPUNIT_ASSERT(test_ising_simulation_1d_loaded->get_modification_factor_current() ==
		 test_ising_simulation_1d->get_modification_factor_current());
  // Delete the loaded simulation
  delete test_ising_simulation_1d_loaded;

  // Do some steps and do again
  test_ising_simulation_1d->do_wang_landau_steps(1000);
  test_ising_simulation_1d->save_serialize("serialize_test.dat");
  test_ising_simulation_1d_loaded = new IsingSimulation1d();
  test_ising_simulation_1d_loaded->load_serialize("serialize_test.dat");
  // Test Equality
  CPPUNIT_ASSERT(*test_ising_simulation_1d_loaded->get_config_space() ==
		 *test_ising_simulation_1d->get_config_space());
  CPPUNIT_ASSERT(test_ising_simulation_1d_loaded->get_log_density_of_states() ==
		 test_ising_simulation_1d->get_log_density_of_states());
  CPPUNIT_ASSERT(test_ising_simulation_1d_loaded->get_incidence_counter() ==
		 test_ising_simulation_1d->get_incidence_counter());
  CPPUNIT_ASSERT(test_ising_simulation_1d_loaded->get_simulation_parameters() ==
		 test_ising_simulation_1d->get_simulation_parameters());
  CPPUNIT_ASSERT(test_ising_simulation_1d_loaded->get_modification_factor_current() ==
		 test_ising_simulation_1d->get_modification_factor_current());
  // Delete the loaded simulation
  delete test_ising_simulation_1d_loaded;
}
