#include "test_pair_observable.hpp"

CppUnit::Test* TestPairObservable::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestPairObservable");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestPairObservable>("TestObservables/TestPairObservable: test_constructor", &TestPairObservable::test_constructor) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestPairObservable>("TestObservables/TestPairObservable: test_operator_multiply", &TestPairObservable::test_operator_multiply) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestPairObservable>("TestObservables/TestPairObservable: test_operator_divide", &TestPairObservable::test_operator_divide) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestPairObservable>("TestObservables/TestPairObservable: test_pow", &TestPairObservable::test_pow) );
  
  return suite_of_tests;
}

void TestPairObservable::setUp()
{
  pair_observable.first = 2;
  pair_observable.second = -1.5;
}

void TestPairObservable::tearDown() { }

void TestPairObservable::test_constructor()
{
  CPPUNIT_ASSERT_EQUAL(2, pair_observable.first);
  CPPUNIT_ASSERT_EQUAL(-1.5, pair_observable.second);
}

void TestPairObservable::test_operator_multiply()
{
  // Test the component-wise multiplication
  PairObservable<int, double> pair_observable_temp;
  pair_observable_temp.first = 4;
  pair_observable_temp.second = 2.0;
  PairObservable<int, double> result = pair_observable * pair_observable_temp;
  CPPUNIT_ASSERT_EQUAL(pair_observable.first * pair_observable_temp.first, result.first);
  CPPUNIT_ASSERT_EQUAL(pair_observable.second * pair_observable_temp.second, result.second);
}
void TestPairObservable::test_operator_divide()
{
  // Test the component-wise division
  PairObservable<int, double> pair_observable_temp;
  pair_observable_temp.first = 4;
  pair_observable_temp.second = 2.0;
  PairObservable<int, double> result = pair_observable / pair_observable_temp;
  CPPUNIT_ASSERT_EQUAL(pair_observable.first / pair_observable_temp.first, result.first);
  CPPUNIT_ASSERT_EQUAL(pair_observable.second / pair_observable_temp.second, result.second);
}
void TestPairObservable::test_pow()
{
  // Test powing for VectorObservable<double>
  PairObservable<int, double> powed_pair_observable = pow(pair_observable, 2);
  CPPUNIT_ASSERT_EQUAL((int)(pow(pair_observable.first, 2)), powed_pair_observable.first);
  CPPUNIT_ASSERT_EQUAL(pow(pair_observable.second, 2.0), powed_pair_observable.second);
}
