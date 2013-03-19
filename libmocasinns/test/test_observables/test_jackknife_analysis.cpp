#include "test_jackknife_analysis.hpp"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>

#include <observables/vector_observable.hpp>

namespace ba = boost::accumulators;

CppUnit::Test* TestJackknifeAnalysis::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestJackknifeAnalysis");

  suite_of_tests->addTest( new CppUnit::TestCaller<TestJackknifeAnalysis>("TestObservables/TestJackknifeAnalysis: test_analyse (doubles)", &TestJackknifeAnalysis::test_analyse_doubles) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestJackknifeAnalysis>("TestObservables/TestJackknifeAnalysis: test_analyse (VectorObservables)", &TestJackknifeAnalysis::test_analyse_vector_observables) );
  
  return suite_of_tests;
}

void TestJackknifeAnalysis::test_analyse_doubles()
{
  // Create a vector of 5 doubles
  std::vector<double> values(5, 0.0);
  values[0] = 1.0;
  values[1] = 2.0;
  values[2] = 1.5;
  values[3] = 0.0;
  values[4] = 2.0;

  // Calculate mean and error of mean with jackknife
  std::pair<double, double> result_mean = JackknifeAnalysis<double>::analyse(values.begin(), values.end());
  std::vector<double> compare_mean_vector(5, 0.0);
  compare_mean_vector[0] = 0.25*(values[1] + values[2] + values[3] + values[4]);
  compare_mean_vector[1] = 0.25*(values[0] + values[2] + values[3] + values[4]);
  compare_mean_vector[2] = 0.25*(values[0] + values[1] + values[3] + values[4]);
  compare_mean_vector[3] = 0.25*(values[0] + values[1] + values[2] + values[4]);
  compare_mean_vector[4] = 0.25*(values[0] + values[1] + values[2] + values[3]);
  double compare_mean = 0.2*(compare_mean_vector[0] + compare_mean_vector[1] + compare_mean_vector[2] + compare_mean_vector[3] + compare_mean_vector[4]);
  double compare_mean_error = 0.0;
  compare_mean_error += 0.2*pow((compare_mean_vector[0] - compare_mean), 2);
  compare_mean_error += 0.2*pow((compare_mean_vector[1] - compare_mean), 2);
  compare_mean_error += 0.2*pow((compare_mean_vector[2] - compare_mean), 2);
  compare_mean_error += 0.2*pow((compare_mean_vector[3] - compare_mean), 2);
  compare_mean_error += 0.2*pow((compare_mean_vector[4] - compare_mean), 2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_mean.first, compare_mean, 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_mean.second, sqrt(4*compare_mean_error), 1e-4);

  // Calculate variance and error of variance using jackknife
  std::pair<double, double> result_variance = JackknifeAnalysis<double, VarianceOfObservables<double> >::analyse(values.begin(), values.end());
  std::vector<double> compare_variance_vector(5, 0.0);
  compare_variance_vector[0] = 0.25*(pow(values[1] - compare_mean_vector[0], 2) + pow(values[2] - compare_mean_vector[0], 2) +
				     pow(values[3] - compare_mean_vector[0], 2) + pow(values[4] - compare_mean_vector[0], 2));
  compare_variance_vector[1] = 0.25*(pow(values[0] - compare_mean_vector[1], 2) + pow(values[2] - compare_mean_vector[1], 2) +
				     pow(values[3] - compare_mean_vector[1], 2) + pow(values[4] - compare_mean_vector[1], 2));
  compare_variance_vector[2] = 0.25*(pow(values[0] - compare_mean_vector[2], 2) + pow(values[1] - compare_mean_vector[2], 2) +
				     pow(values[3] - compare_mean_vector[2], 2) + pow(values[4] - compare_mean_vector[2], 2));
  compare_variance_vector[3] = 0.25*(pow(values[0] - compare_mean_vector[3], 2) + pow(values[1] - compare_mean_vector[3], 2) +
				     pow(values[2] - compare_mean_vector[3], 2) + pow(values[4] - compare_mean_vector[3], 2));
  compare_variance_vector[4] = 0.25*(pow(values[0] - compare_mean_vector[4], 2) + pow(values[1] - compare_mean_vector[4], 2) +
				     pow(values[2] - compare_mean_vector[4], 2) + pow(values[3] - compare_mean_vector[4], 2));
  double compare_variance = 0.2*(compare_variance_vector[0] + compare_variance_vector[1] + compare_variance_vector[2] + compare_variance_vector[3] + compare_variance_vector[4]);
  double compare_variance_error = 0.0;
  compare_variance_error += 0.2*pow((compare_variance_vector[0] - compare_variance), 2);
  compare_variance_error += 0.2*pow((compare_variance_vector[1] - compare_variance), 2);
  compare_variance_error += 0.2*pow((compare_variance_vector[2] - compare_variance), 2);
  compare_variance_error += 0.2*pow((compare_variance_vector[3] - compare_variance), 2);
  compare_variance_error += 0.2*pow((compare_variance_vector[4] - compare_variance), 2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_variance.first, compare_variance, 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_variance.second, sqrt(4*compare_variance_error), 1e-4);

  // Calculate standard deviation and error of standard deviation using jackknife
  std::pair<double, double> result_stddev = JackknifeAnalysis<double, StandardDeviationOfObservables<double> >::analyse(values.begin(), values.end());
  std::vector<double> compare_stddev_vector(5, 0.0);
  compare_stddev_vector[0] = sqrt(compare_variance_vector[0]);
  compare_stddev_vector[1] = sqrt(compare_variance_vector[1]);
  compare_stddev_vector[2] = sqrt(compare_variance_vector[2]);
  compare_stddev_vector[3] = sqrt(compare_variance_vector[3]);
  compare_stddev_vector[4] = sqrt(compare_variance_vector[4]);
  double compare_stddev = 0.2*(compare_stddev_vector[0] + compare_stddev_vector[1] + compare_stddev_vector[2] + compare_stddev_vector[3] + compare_stddev_vector[4]);
  double compare_stddev_error = 0.0;
  compare_stddev_error += 0.2*pow((compare_stddev_vector[0] - compare_stddev), 2);
  compare_stddev_error += 0.2*pow((compare_stddev_vector[1] - compare_stddev), 2);
  compare_stddev_error += 0.2*pow((compare_stddev_vector[2] - compare_stddev), 2);
  compare_stddev_error += 0.2*pow((compare_stddev_vector[3] - compare_stddev), 2);
  compare_stddev_error += 0.2*pow((compare_stddev_vector[4] - compare_stddev), 2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_stddev.first, compare_stddev, 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_stddev.second, sqrt(4*compare_stddev_error), 1e-4);
}

void TestJackknifeAnalysis::test_analyse_vector_observables()
{
  // Create a vector of 5 doubles
  std::vector<VectorObservable<double> > values(5);
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

  // Calculate mean and error of mean with jackknife
  std::pair<VectorObservable<double>, VectorObservable<double> > result_mean = JackknifeAnalysis<VectorObservable<double> >::analyse(values.begin(), values.end());
  std::vector<VectorObservable<double> > compare_mean_vector(5, VectorObservable<double>(2, 0.0));
  compare_mean_vector[0] = 0.25*(values[1] + values[2] + values[3] + values[4]);
  compare_mean_vector[1] = 0.25*(values[0] + values[2] + values[3] + values[4]);
  compare_mean_vector[2] = 0.25*(values[0] + values[1] + values[3] + values[4]);
  compare_mean_vector[3] = 0.25*(values[0] + values[1] + values[2] + values[4]);
  compare_mean_vector[4] = 0.25*(values[0] + values[1] + values[2] + values[3]);
  VectorObservable<double> compare_mean = 0.2*(compare_mean_vector[0] + compare_mean_vector[1] + compare_mean_vector[2] + compare_mean_vector[3] + compare_mean_vector[4]);
  VectorObservable<double> compare_mean_error(2, 0.0);
  compare_mean_error += 0.2*pow((compare_mean_vector[0] - compare_mean), 2);
  compare_mean_error += 0.2*pow((compare_mean_vector[1] - compare_mean), 2);
  compare_mean_error += 0.2*pow((compare_mean_vector[2] - compare_mean), 2);
  compare_mean_error += 0.2*pow((compare_mean_vector[3] - compare_mean), 2);
  compare_mean_error += 0.2*pow((compare_mean_vector[4] - compare_mean), 2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_mean.first[0], compare_mean[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_mean.first[1], compare_mean[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_mean.second[0], sqrt(4*compare_mean_error[0]), 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(result_mean.second[1], sqrt(4*compare_mean_error[1]), 1e-4);
}
