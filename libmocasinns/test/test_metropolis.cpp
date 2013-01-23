#include "test_metropolis.hpp"

#include <vector>
#include <cstdint>

CppUnit::Test* TestMetropolis::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestMetropolis");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestMetropolis>("TestMetropolis: test_do_metropolis_steps", &TestMetropolis::test_do_metropolis_steps) );
    
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
