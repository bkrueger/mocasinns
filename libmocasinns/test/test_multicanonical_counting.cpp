#include "test_multicanonical_counting.hpp"

CppUnit::Test* TestMulticanonicalCounting::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestMulticanonicalCounting");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestMulticanonicalCounting>("TestMulticanonicalCounting: test_do_multicanonical_counting_simulation", &TestMulticanonicalCounting::test_do_multicanonical_counting_simulation) );
    
  return suite_of_tests;
}

void TestMulticanonicalCounting::setUp()
{
  // 1d-lattice of Ising spins
  std::vector<unsigned int> size_1d;
  size_1d.push_back(16);
  parameters_1d.modification_factor_initial = 1.0;
  parameters_1d.modification_factor_final = 1e-8;
  parameters_1d.modification_factor_multiplier = 0.9;
  parameters_1d.flatness = 0.8;
  parameters_1d.sweep_steps = 1000;

  test_ising_config_1d = new IsingConfiguration1d(size_1d);
  test_ising_simulation_1d = new IsingSimulation1d(parameters_1d, test_ising_config_1d);

  // 2d-lattice of Ising spins
  std::vector<unsigned int> size_2d;
  size_2d.push_back(4); size_2d.push_back(4);
  parameters_2d.modification_factor_initial = 1.0;
  parameters_2d.modification_factor_final = 1e-8;
  parameters_2d.modification_factor_multiplier = 0.9;
  parameters_2d.flatness = 0.8;
  parameters_2d.sweep_steps = 1000;

  test_ising_config_2d = new IsingConfiguration2d(size_2d);
  test_ising_simulation_2d = new IsingSimulation2d(parameters_2d, test_ising_config_2d);
}

void TestMulticanonicalCounting::tearDown()
{
  delete test_ising_simulation_1d;
  delete test_ising_simulation_2d;
  delete test_ising_config_1d;
  delete test_ising_config_2d;
}

void TestMulticanonicalCounting::test_do_multicanonical_counting_simulation()
{
  // Do the 1d simulation
  test_ising_simulation_1d->do_wang_landau_simulation();
  double count_1d = test_ising_simulation_1d->number_of_states();
  CPPUNIT_ASSERT_DOUBLES_EQUAL(65536.0, count_1d, 5000.0);
  // Do the 2d simulation
  test_ising_simulation_2d->do_wang_landau_simulation();  
  double count_2d = test_ising_simulation_2d->number_of_states();
  CPPUNIT_ASSERT_DOUBLES_EQUAL(65536.0, count_2d, 5000.0);
}
