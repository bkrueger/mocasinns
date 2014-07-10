#include "test_autocorrelation.hpp"

using namespace Mocasinns::Analysis;

CppUnit::Test* TestAutocorrelation::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestAutocorrelation");

  suite_of_tests->addTest( new CppUnit::TestCaller<TestAutocorrelation>("TestObservables/TestAutocorrelation: test_autocorrelation_function", &TestAutocorrelation::test_autocorrelation_function) );
  
  return suite_of_tests;
}

void TestAutocorrelation::test_autocorrelation_function()
{
  // Create a vector of 8 doubles
  std::vector<double> values_1(8, 0.0);
  values_1[0] = 1.0;
  values_1[1] = 1.5;
  values_1[2] = 1.25;
  values_1[3] = 0.5;
  values_1[4] = 2.5;
  values_1[5] = 1.5;
  values_1[6] = 0.5;
  values_1[7] = 1.25;

  // The autocorrelation is given by
  // 1.0 -0,2236024845	-0,1739130435	0,9391304348	-0,1739130435	-0,8115942029	-1,0434782609	-0,8695652174
  // Calculate and compare the autocorrelation
  std::vector<double> autocorrelation_function_1 = Autocorrelation<double>::autocorrelation_function(values_1.begin(), values_1.end());
  CPPUNIT_ASSERT_DOUBLES_EQUAL(+1.0000, autocorrelation_function_1[0], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.2236, autocorrelation_function_1[1], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1739, autocorrelation_function_1[2], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(+0.9391, autocorrelation_function_1[3], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.1739, autocorrelation_function_1[4], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.8116, autocorrelation_function_1[5], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0435, autocorrelation_function_1[6], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.8696, autocorrelation_function_1[7], 0.0002);


  // Create a vector of 8 doubles
  std::vector<double> values_2(8, 0.0);
  values_2[0] = 5.0;
  values_2[1] = 3.0;
  values_2[2] = 4.5;
  values_2[3] = 1.0;
  values_2[4] = 0.0;
  values_2[5] = -1.0;
  values_2[6] = 0.5;
  values_2[7] = -0.5;

  // The autocorrelation is given by
  // 1.0 0,4655151733	0,3708368318	-0,4817800168	-0,5919395466	-0,936747831	-0,4172963896	-1,0621326616
  // Calculate and compare the autocorrelation
  std::vector<double> autocorrelation_function_2 = Autocorrelation<double>::autocorrelation_function(values_2.begin(), values_2.end());
  CPPUNIT_ASSERT_DOUBLES_EQUAL(+1.0000, autocorrelation_function_2[0], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(+0.4655, autocorrelation_function_2[1], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(+0.3708, autocorrelation_function_2[2], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.4818, autocorrelation_function_2[3], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.5919, autocorrelation_function_2[4], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.9367, autocorrelation_function_2[5], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.4173, autocorrelation_function_2[6], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0621, autocorrelation_function_2[7], 0.0002);


  // Create vectors consisting of the iterators
  std::vector<std::vector<double>::const_iterator> value_begins;
  value_begins.push_back(values_1.begin());
  value_begins.push_back(values_2.begin());
  std::vector<std::vector<double>::const_iterator> value_ends;
  value_ends.push_back(values_1.end());
  value_ends.push_back(values_2.end());

  // Calculate the combined autocorrelation
  std::pair<std::vector<double>, std::vector<double> > autocorrelation_function_combined = Autocorrelation<double>::autocorrelation_function(value_begins, value_ends);
  std::vector<double> autocorrelation_means = autocorrelation_function_combined.first;
  std::vector<double> autocorrelation_errors = autocorrelation_function_combined.second;
  CPPUNIT_ASSERT_DOUBLES_EQUAL((+1.0000 + 1.0000)/2, autocorrelation_means[0], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL((-0.2236 + 0.4655)/2, autocorrelation_means[1], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL((-0.1739 + 0.3708)/2, autocorrelation_means[2], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL((+0.9391 - 0.4818)/2, autocorrelation_means[3], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL((-0.1739 - 0.5919)/2, autocorrelation_means[4], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL((-0.8116 - 0.9367)/2, autocorrelation_means[5], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL((-1.0435 - 0.4173)/2, autocorrelation_means[6], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL((-0.8696 - 1.0621)/2, autocorrelation_means[7], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(0.5*pow(+1.0000 - autocorrelation_means[0], 2) + 0.5*pow(+1.0000 - autocorrelation_means[0], 2)), autocorrelation_errors[0], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(0.5*pow(-0.2236 - autocorrelation_means[1], 2) + 0.5*pow(+0.4655 - autocorrelation_means[1], 2)), autocorrelation_errors[1], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(0.5*pow(-0.1739 - autocorrelation_means[2], 2) + 0.5*pow(+0.3708 - autocorrelation_means[2], 2)), autocorrelation_errors[2], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(0.5*pow(+0.9391 - autocorrelation_means[3], 2) + 0.5*pow(-0.4818 - autocorrelation_means[3], 2)), autocorrelation_errors[3], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(0.5*pow(-0.1739 - autocorrelation_means[4], 2) + 0.5*pow(-0.5919 - autocorrelation_means[4], 2)), autocorrelation_errors[4], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(0.5*pow(-0.8116 - autocorrelation_means[5], 2) + 0.5*pow(-0.9367 - autocorrelation_means[5], 2)), autocorrelation_errors[5], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(0.5*pow(-1.0435 - autocorrelation_means[6], 2) + 0.5*pow(-0.4173 - autocorrelation_means[6], 2)), autocorrelation_errors[6], 0.0002);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(0.5*pow(-0.8696 - autocorrelation_means[7], 2) + 0.5*pow(-1.0621 - autocorrelation_means[7], 2)), autocorrelation_errors[7], 0.0002);
}
