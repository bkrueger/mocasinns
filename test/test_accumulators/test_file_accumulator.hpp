#ifndef TEST_FILE_ACCUMULATOR_HPP
#define TEST_FILE_ACCUMULATOR_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <mocasinns/accumulators/file_accumulator.hpp>

using namespace Mocasinns::Accumulators;

class TestFileAccumulator : CppUnit::TestFixture
{
private:
  FileAccumulator<double>* file_accumulator_1;
  FileAccumulator<double>* file_accumulator_2;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_operator_accumulate();
  void test_data();
};

#endif
