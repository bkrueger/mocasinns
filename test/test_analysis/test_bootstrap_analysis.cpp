#include "test_bootstrap_analysis.hpp"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>

#include <mocasinns/observables/vector_observable.hpp>

using namespace Mocasinns::Observables;

namespace ba = boost::accumulators;

CppUnit::Test* TestBootstrapAnalysis::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestBootstrapAnalysis");

  suite_of_tests->addTest( new CppUnit::TestCaller<TestBootstrapAnalysis>("TestObservables/TestBootstrapAnalysis: test_analyse (doubles)", &TestBootstrapAnalysis::test_analyse_doubles) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestBootstrapAnalysis>("TestObservables/TestBootstrapAnalysis: test_analyse (VectorObservables)", &TestBootstrapAnalysis::test_analyse_vector_observables) );
  
  return suite_of_tests;
}

void TestBootstrapAnalysis::test_analyse_doubles()
{
  // Create a random number generator
  boost::random::mt19937 rng;
  // Create a normal distribution
  boost::random::normal_distribution<> dist(0.0, 1.0);

  // Create a vector and calculate a lot of normal random variables and calculate the exponential
  unsigned int samples = 10000;
  std::vector<double> exponential_results(samples, 0.0);
  for (unsigned int i = 0; i < samples; ++i)
  {
    exponential_results[i] = exp(dist(rng));
  }

  // Do a bootstrap analysis
  std::pair<double, double> bootstrap_result = BootstrapAnalysis<double>::analyse(exponential_results.begin(), exponential_results.end(), 10000);
  
  // For the results see 03.04.13 - 01
  CPPUNIT_ASSERT_DOUBLES_EQUAL(exp(0.5), bootstrap_result.first, 0.05);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(exp(1.0)*(exp(1.0)-1)) / sqrt(samples), bootstrap_result.second, 0.01);
}

void TestBootstrapAnalysis::test_analyse_vector_observables()
{

}
