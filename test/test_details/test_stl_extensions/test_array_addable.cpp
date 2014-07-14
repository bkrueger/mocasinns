#include "test_array_addable.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

CppUnit::Test* TestArrayAddable::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestArrayAddable");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayAddable>("TestObservables/TestArrayAddable: test_constructor", &TestArrayAddable::test_constructor) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayAddable>("TestObservables/TestArrayAddable: test_operator_add", &TestArrayAddable::test_operator_add) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayAddable>("TestObservables/TestArrayAddable: test_operator_substract", &TestArrayAddable::test_operator_substract) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayAddable>("TestObservables/TestArrayAddable: test_operator_multiply", &TestArrayAddable::test_operator_multiply) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayAddable>("TestObservables/TestArrayAddable: test_operator_divide", &TestArrayAddable::test_operator_divide) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayAddable>("TestObservables/TestArrayAddable: test_operator_outstream", &TestArrayAddable::test_operator_outstream) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayAddable>("TestObservables/TestArrayAddable: test_operator_instream", &TestArrayAddable::test_operator_instream) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestArrayAddable>("TestObservables/TestArrayAddable: test_serialization", &TestArrayAddable::test_serialization) );

  return suite_of_tests;
}

void TestArrayAddable::setUp()
{
  boost::array<double,3> init_double;
  init_double[0] = 1.0;
  init_double[1] = 2.5;
  init_double[2] = -2.0;
  array_addable_double = new ArrayAddablePlain<double,3>(init_double);

  boost::array<int,3> init_int;
  init_int[0] = 2;
  init_int[1] = 6;
  init_int[2] = -1;
  array_addable_int = new ArrayAddablePlain<int,3>(init_int);
}

void TestArrayAddable::tearDown()
{
  delete array_addable_double;
  delete array_addable_int;
}

void TestArrayAddable::test_constructor()
{
  // Test the allready constructed ArrayAddables
  CPPUNIT_ASSERT_EQUAL(1.0, (*array_addable_double)[0]);
  CPPUNIT_ASSERT_EQUAL(2.5, (*array_addable_double)[1]);
  CPPUNIT_ASSERT_EQUAL(-2.0, (*array_addable_double)[2]);
  CPPUNIT_ASSERT_EQUAL(2, (*array_addable_int)[0]);
  CPPUNIT_ASSERT_EQUAL(6, (*array_addable_int)[1]);
  CPPUNIT_ASSERT_EQUAL(-1, (*array_addable_int)[2]);
}

void TestArrayAddable::test_operator_add()
{
  // Test adding for ArrayAddable<double>
  ArrayAddablePlain<double,3> new_array_addable_double;
  new_array_addable_double[0] = 2.3;
  new_array_addable_double[1] = -0.5;
  new_array_addable_double[2] = 4.1;
  ArrayAddablePlain<double,3> added_array_addable_double = (*array_addable_double) + new_array_addable_double;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.3, added_array_addable_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, added_array_addable_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.1, added_array_addable_double[2], 1e-6);
  // Test adding a scalar
  ArrayAddablePlain<double,3> scalar_added_array_addable_double = added_array_addable_double + 2.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.3, scalar_added_array_addable_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, scalar_added_array_addable_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.1, scalar_added_array_addable_double[2], 1e-6);

  // Test adding for ArrayAddable<int>
  ArrayAddablePlain<int,3> new_array_addable_int;
  new_array_addable_int[0] = 2;
  new_array_addable_int[1] = -5;
  new_array_addable_int[2] = 3;
  ArrayAddablePlain<int,3> added_array_addable_int = (*array_addable_int) + new_array_addable_int;
  CPPUNIT_ASSERT_EQUAL(4, added_array_addable_int[0]);
  CPPUNIT_ASSERT_EQUAL(1, added_array_addable_int[1]);
  CPPUNIT_ASSERT_EQUAL(2, added_array_addable_int[2]);
  // Test adding a scalar
  ArrayAddablePlain<int,3> scalar_added_array_addable_int = added_array_addable_int + 2;
  CPPUNIT_ASSERT_EQUAL(6, scalar_added_array_addable_int[0]);
  CPPUNIT_ASSERT_EQUAL(3, scalar_added_array_addable_int[1]);
  CPPUNIT_ASSERT_EQUAL(4, scalar_added_array_addable_int[2]);
}
void TestArrayAddable::test_operator_substract()
{
  // Test substracting for ArrayAddable<double>
  ArrayAddablePlain<double,3> new_array_addable_double;
  new_array_addable_double[0] = 2.3;
  new_array_addable_double[1] = -0.5;
  new_array_addable_double[2] = 4.1;
  ArrayAddablePlain<double,3> substracted_array_addable_double = (*array_addable_double) - new_array_addable_double;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.3, substracted_array_addable_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, substracted_array_addable_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-6.1, substracted_array_addable_double[2], 1e-6);
  // Test substracting a scalar
  ArrayAddablePlain<double,3> scalar_substracted_array_addable_double = substracted_array_addable_double - 2.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.3, scalar_substracted_array_addable_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, scalar_substracted_array_addable_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-8.1, scalar_substracted_array_addable_double[2], 1e-6);

  // Test substracting for ArrayAddable<int>
  ArrayAddablePlain<int,3> new_array_addable_int;
  new_array_addable_int[0] = 2;
  new_array_addable_int[1] = -5;
  new_array_addable_int[2] = 3;
  ArrayAddablePlain<int,3> substracted_array_addable_int = (*array_addable_int) - new_array_addable_int;
  CPPUNIT_ASSERT_EQUAL(0, substracted_array_addable_int[0]);
  CPPUNIT_ASSERT_EQUAL(11, substracted_array_addable_int[1]);
  CPPUNIT_ASSERT_EQUAL(-4, substracted_array_addable_int[2]);
  // Test substracting a scalar
  ArrayAddablePlain<int,3> scalar_substracted_array_addable_int = substracted_array_addable_int - 2;
  CPPUNIT_ASSERT_EQUAL(-2, scalar_substracted_array_addable_int[0]);
  CPPUNIT_ASSERT_EQUAL(9, scalar_substracted_array_addable_int[1]);
  CPPUNIT_ASSERT_EQUAL(-6, scalar_substracted_array_addable_int[2]);
}
void TestArrayAddable::test_operator_multiply()
{
  // Test scalar multiplication for ArrayAddable<double>
  ArrayAddablePlain<double,3> multiplied_array_addable_double_1 = (*array_addable_double) * 2.0;
  ArrayAddablePlain<double,3> multiplied_array_addable_double_2 = 2.0 * (*array_addable_double);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, multiplied_array_addable_double_1[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, multiplied_array_addable_double_1[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0, multiplied_array_addable_double_1[2], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, multiplied_array_addable_double_2[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, multiplied_array_addable_double_2[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0, multiplied_array_addable_double_2[2], 1e-4);
}
void TestArrayAddable::test_operator_divide()
{
  // Test scalar division for ArrayAddable<double>
  ArrayAddablePlain<double,3> divided_array_addable_double = (*array_addable_double) / 2.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, divided_array_addable_double[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.25, divided_array_addable_double[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, divided_array_addable_double[2], 1e-4);
}

void TestArrayAddable::test_operator_outstream()
{
  std::stringstream ss;
  ss << *array_addable_int;
  CPPUNIT_ASSERT_EQUAL(static_cast<std::string>("2 6 -1"), ss.str());
}
void TestArrayAddable::test_operator_instream()
{
  
}

void TestArrayAddable::test_serialization()
{
  // Create an output file
  std::ofstream ofs("serialization.dat");
  {
    boost::archive::text_oarchive oa(ofs);
    oa << *array_addable_int;
  }
  // Read from an input file
  ArrayAddablePlain<int,3> array_addable_int_loaded;
  {
    std::ifstream ifs("serialization.dat");
    boost::archive::text_iarchive ia(ifs);
    ia >> array_addable_int_loaded;
  }
  // Test that both addable arrays are identical
  CPPUNIT_ASSERT(*array_addable_int == array_addable_int_loaded);
}
