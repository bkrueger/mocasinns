#ifndef HISTOCRETETEST_HPP
#define HISTOCRETETEST_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <histograms/histocrete.hpp>

using namespace Mocasinns::Histograms;

class TestHistocrete : public CppUnit::TestFixture
{
private:
  Histocrete<int,int> testhisto_int;
  Histocrete<double, double> testhisto_double;

public:
  static CppUnit::Test* suite();

  void setUp();
  void tearDown();

  void test_operator_fill();
  void test_operator_access();
  void test_operator_increment();
  void test_operator_divide();

  void test_insert();
  void test_serialize();
};

#endif
