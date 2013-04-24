#ifndef TEST_HISTOGRAM_OBSERVABLE_HPP
#define TEST_HISTOGRAM_OBSERVABLE_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/observables/histogram_observable.hpp>
#include <mocasinns/histograms/histocrete.hpp>

using namespace Mocasinns::Observables;
using namespace Mocasinns::Histograms;

class TestHistogramObservable : CppUnit::TestFixture
{
private:
  typedef HistogramObservable<Histocrete, int, double> HistocreteObservable;
  HistocreteObservable histogram_observable_double_1;
  HistocreteObservable histogram_observable_double_2;
  HistocreteObservable histogram_observable_double_3;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_operator_add();
  void test_operator_substract();
  void test_operator_multiply();
  void test_operator_divide();
  void test_pow();
};

#endif
