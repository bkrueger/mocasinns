#include "test_binning_analysis.hpp"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>

#include <mocasinns/observables/vector_observable.hpp>

using namespace Mocasinns::Observables;

namespace ba = boost::accumulators;

CppUnit::Test* TestBinningAnalysis::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestBinningAnalysis");

  suite_of_tests->addTest( new CppUnit::TestCaller<TestBinningAnalysis>("TestObservables/TestBinningAnalysis: test_analyse (doubles)", &TestBinningAnalysis::test_analyse_doubles) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestBinningAnalysis>("TestObservables/TestBinningAnalysis: test_analyse (VectorObservables)", &TestBinningAnalysis::test_analyse_vector_observables) );
  
  return suite_of_tests;
}

void TestBinningAnalysis::test_analyse_doubles()
{
  // Create a vector of 6 doubles
  std::vector<double> values(6, 0.0);
  values[0] = 1.0;
  values[1] = 2.0;
  values[2] = 1.5;
  values[3] = 0.0;
  values[4] = 2.0;
  values[5] = 1.0;

  // Calculate mean and error of mean with jackknife
  std::pair<double, double> result_mean = BinningAnalysis<double>::analyse(values.begin(), values.end(), 2);
  std::vector<double> compare_mean_vector(3, 0.0);
  compare_mean_vector[0] = 0.5*(values[0] + values[1]);
  compare_mean_vector[1] = 0.5*(values[2] + values[3]);
  compare_mean_vector[2] = 0.5*(values[4] + values[5]);
  double compare_mean = (1.0/3.0)*(compare_mean_vector[0] + compare_mean_vector[1] + compare_mean_vector[2]);
  double compare_mean_error = 0.0;
  compare_mean_error += (1.0/3.0)*pow((compare_mean_vector[0] - compare_mean), 2);
  compare_mean_error += (1.0/3.0)*pow((compare_mean_vector[1] - compare_mean), 2);
  compare_mean_error += (1.0/3.0)*pow((compare_mean_vector[2] - compare_mean), 2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_mean.first, compare_mean, 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_mean.second, sqrt(0.5*compare_mean_error), 1e-4);

  ///////////////////////////////////
  // Test using a big distribution //
    // Create a random number generator
  boost::random::mt19937 rng;
  // Create a normal distribution
  boost::random::normal_distribution<> dist(0.0, 1.0);

  // Create a vector and calculate a lot of normal random variables and calculate the exponential
  unsigned int samples = 10000;
  std::vector<double> exponential_results(samples, 0.0);
  for (unsigned int i = 0; i < samples; ++i)
    exponential_results[i] = exp(dist(rng));

  // Do a bootstrap analysis
  std::pair<double, double> binning_result = BinningAnalysis<double>::analyse(exponential_results.begin(), exponential_results.end(), 100);
  
  // For the results see 03.04.13 - 01
  CPPUNIT_ASSERT_DOUBLES_EQUAL(exp(0.5), binning_result.first, 0.05);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(exp(1.0)*(exp(1.0)-1)) / sqrt(samples), binning_result.second, 0.01);
}

void TestBinningAnalysis::test_analyse_vector_observables()
{
  // Create a vector of 5 doubles
  std::vector<VectorObservable<double> > values(6);
  values[0] = VectorObservable<double>(2);
  values[0][0] = 1.0;
  values[0][1] = 2.5;
  values[1] = VectorObservable<double>(2);
  values[1][0] = 2.0;
  values[1][1] = -1.0;
  values[2] = VectorObservable<double>(2);
  values[2][0] = 1.5;
  values[2][1] = 3.0;
  values[3] = VectorObservable<double>(2);
  values[3][0] = 0.0;
  values[3][1] = 1.5;
  values[4] = VectorObservable<double>(2);
  values[4][0] = 2.0;
  values[4][1] = 4.5;
  values[4] = VectorObservable<double>(2);
  values[4][0] = 1.0;
  values[4][1] = 2.5;

  // Calculate mean and error of mean with binning
  std::pair<VectorObservable<double>, VectorObservable<double> > result_mean = BinningAnalysis<VectorObservable<double> >::analyse(values.begin(), values.end(), 2);
  std::vector<VectorObservable<double> > compare_mean_vector(3, VectorObservable<double>(2, 0.0));
  compare_mean_vector[0] = 0.5*(values[0] + values[1]);
  compare_mean_vector[1] = 0.5*(values[2] + values[3]);
  compare_mean_vector[2] = 0.5*(values[4] + values[5]);
  VectorObservable<double> compare_mean = (1.0/3.0)*(compare_mean_vector[0] + compare_mean_vector[1] + compare_mean_vector[2]);
  VectorObservable<double> compare_mean_error(2, 0.0);
  compare_mean_error += (1.0/3.0)*pow((compare_mean_vector[0] - compare_mean), 2);
  compare_mean_error += (1.0/3.0)*pow((compare_mean_vector[1] - compare_mean), 2);
  compare_mean_error += (1.0/3.0)*pow((compare_mean_vector[2] - compare_mean), 2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_mean.first[0], compare_mean[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_mean.first[1], compare_mean[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_mean.second[0], sqrt(0.5*compare_mean_error[0]), 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_mean.second[1], sqrt(0.5*compare_mean_error[1]), 1e-4);
}
