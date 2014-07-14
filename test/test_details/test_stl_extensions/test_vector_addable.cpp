#include "test_vector_addable.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

CppUnit::Test* TestVectorAddable::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestVectorAddable");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestVectorAddable>("TestObservables/TestVectorAddable: test_constructor", &TestVectorAddable::test_constructor) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestVectorAddable>("TestObservables/TestVectorAddable: test_operator_add", &TestVectorAddable::test_operator_add) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestVectorAddable>("TestObservables/TestVectorAddable: test_operator_substract", &TestVectorAddable::test_operator_substract) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestVectorAddable>("TestObservables/TestVectorAddable: test_operator_multiply", &TestVectorAddable::test_operator_multiply) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestVectorAddable>("TestObservables/TestVectorAddable: test_operator_divide", &TestVectorAddable::test_operator_divide) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestVectorAddable>("TestObservables/TestVectorAddable: test_serialization", &TestVectorAddable::test_serialization) );

  return suite_of_tests;
}

void TestVectorAddable::setUp()
{
  std::vector<double> init_double;
  init_double.push_back(1.0);
  init_double.push_back(2.5);
  init_double.push_back(-2.0);
  vector_addable_double = new VectorAddablePlain<double>(init_double);

  std::vector<int> init_int;
  init_int.push_back(2);
  init_int.push_back(6);
  init_int.push_back(-1);
  vector_addable_int = new VectorAddablePlain<int>(init_int);
}

void TestVectorAddable::tearDown()
{
  delete vector_addable_double;
  delete vector_addable_int;
}

void TestVectorAddable::test_constructor()
{
  // Test the allready constructed VectorAddablePlains
  CPPUNIT_ASSERT_EQUAL(1.0, (*vector_addable_double)[0]);
  CPPUNIT_ASSERT_EQUAL(2.5, (*vector_addable_double)[1]);
  CPPUNIT_ASSERT_EQUAL(-2.0, (*vector_addable_double)[2]);
  CPPUNIT_ASSERT_EQUAL(2, (*vector_addable_int)[0]);
  CPPUNIT_ASSERT_EQUAL(6, (*vector_addable_int)[1]);
  CPPUNIT_ASSERT_EQUAL(-1, (*vector_addable_int)[2]);
}

void TestVectorAddable::test_operator_add()
{
  // Test adding for VectorAddable<double>
  VectorAddablePlain<double> new_addable_vector_double(3,0.0);
  new_addable_vector_double[0] = 2.3;
  new_addable_vector_double[1] = -0.5;
  new_addable_vector_double[2] = 4.1;
  VectorAddablePlain<double> added_addable_vector_double = (*vector_addable_double) + new_addable_vector_double;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.3, added_addable_vector_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, added_addable_vector_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.1, added_addable_vector_double[2], 1e-6);
  // Test adding a scalar
  VectorAddablePlain<double> scalar_added_addable_vector_double = added_addable_vector_double + 2.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.3, scalar_added_addable_vector_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, scalar_added_addable_vector_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(4.1, scalar_added_addable_vector_double[2], 1e-6);

  // Test adding of an empty VectorAddable for VectorAddable<double>
  VectorAddablePlain<double> empty_addable_vector_double;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, ((*vector_addable_double) + empty_addable_vector_double)[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, ((*vector_addable_double) + empty_addable_vector_double)[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0, ((*vector_addable_double) + empty_addable_vector_double)[2], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, (empty_addable_vector_double + (*vector_addable_double))[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, (empty_addable_vector_double + (*vector_addable_double))[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0, (empty_addable_vector_double + (*vector_addable_double))[2], 1e-6);

  // Test adding for VectorAddable<int>
  VectorAddablePlain<int> new_addable_vector_int(3,0);
  new_addable_vector_int[0] = 2;
  new_addable_vector_int[1] = -5;
  new_addable_vector_int[2] = 3;
  VectorAddablePlain<int> added_addable_vector_int = (*vector_addable_int) + new_addable_vector_int;
  CPPUNIT_ASSERT_EQUAL(4, added_addable_vector_int[0]);
  CPPUNIT_ASSERT_EQUAL(1, added_addable_vector_int[1]);
  CPPUNIT_ASSERT_EQUAL(2, added_addable_vector_int[2]);
  // Test adding a scalar
  VectorAddablePlain<int> scalar_added_addable_vector_int = added_addable_vector_int + 2;
  CPPUNIT_ASSERT_EQUAL(6, scalar_added_addable_vector_int[0]);
  CPPUNIT_ASSERT_EQUAL(3, scalar_added_addable_vector_int[1]);
  CPPUNIT_ASSERT_EQUAL(4, scalar_added_addable_vector_int[2]);
}
void TestVectorAddable::test_operator_substract()
{
  // Test substracting for VectorAddablePlain<double>
  VectorAddablePlain<double> new_addable_vector_double(3,0.0);
  new_addable_vector_double[0] = 2.3;
  new_addable_vector_double[1] = -0.5;
  new_addable_vector_double[2] = 4.1;
  VectorAddablePlain<double> substracted_addable_vector_double = (*vector_addable_double) - new_addable_vector_double;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.3, substracted_addable_vector_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, substracted_addable_vector_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-6.1, substracted_addable_vector_double[2], 1e-6);
  // Test substracting a scalar
  VectorAddablePlain<double> scalar_substracted_addable_vector_double = substracted_addable_vector_double - 2.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-3.3, scalar_substracted_addable_vector_double[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, scalar_substracted_addable_vector_double[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-8.1, scalar_substracted_addable_vector_double[2], 1e-6);

  // Test substracting of an empty VectorAddable for VectorAddable<double>
  VectorAddablePlain<double> empty_addable_vector_double;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, ((*vector_addable_double) - empty_addable_vector_double)[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.5, ((*vector_addable_double) - empty_addable_vector_double)[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.0, ((*vector_addable_double) - empty_addable_vector_double)[2], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, (empty_addable_vector_double - (*vector_addable_double))[0], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-2.5, (empty_addable_vector_double - (*vector_addable_double))[1], 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, (empty_addable_vector_double - (*vector_addable_double))[2], 1e-6);

  // Test substracting for VectorAddable<int>
  VectorAddablePlain<int> new_addable_vector_int(3,0);
  new_addable_vector_int[0] = 2;
  new_addable_vector_int[1] = -5;
  new_addable_vector_int[2] = 3;
  VectorAddablePlain<int> substracted_addable_vector_int = (*vector_addable_int) - new_addable_vector_int;
  CPPUNIT_ASSERT_EQUAL(0, substracted_addable_vector_int[0]);
  CPPUNIT_ASSERT_EQUAL(11, substracted_addable_vector_int[1]);
  CPPUNIT_ASSERT_EQUAL(-4, substracted_addable_vector_int[2]);
  // Test substracting a scalar
  VectorAddablePlain<int> scalar_substracted_addable_vector_int = substracted_addable_vector_int - 2;
  CPPUNIT_ASSERT_EQUAL(-2, scalar_substracted_addable_vector_int[0]);
  CPPUNIT_ASSERT_EQUAL(9, scalar_substracted_addable_vector_int[1]);
  CPPUNIT_ASSERT_EQUAL(-6, scalar_substracted_addable_vector_int[2]);
}
void TestVectorAddable::test_operator_multiply()
{
  // Test scalar multiplication for VectorAddable<double>
  VectorAddablePlain<double> multiplied_addable_vector_double_1 = (*vector_addable_double) * 2.0;
  VectorAddablePlain<double> multiplied_addable_vector_double_2 = 2.0 * (*vector_addable_double);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, multiplied_addable_vector_double_1[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, multiplied_addable_vector_double_1[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0, multiplied_addable_vector_double_1[2], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, multiplied_addable_vector_double_2[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(5.0, multiplied_addable_vector_double_2[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-4.0, multiplied_addable_vector_double_2[2], 1e-4);
}
void TestVectorAddable::test_operator_divide()
{
  // Test scalar division for VectorAddable<double>
  VectorAddablePlain<double> divided_addable_vector_double = (*vector_addable_double) / 2.0;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, divided_addable_vector_double[0], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.25, divided_addable_vector_double[1], 1e-4);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, divided_addable_vector_double[2], 1e-4);
}

void TestVectorAddable::test_serialization()
{
  // Create an output file
  std::ofstream ofs("serialization.dat");
  {
    boost::archive::text_oarchive oa(ofs);
    oa << *vector_addable_int;
  }
  // Read from an input file
  VectorAddablePlain<int> vector_addable_int_loaded;
  {
    std::ifstream ifs("serialization.dat");
    boost::archive::text_iarchive ia(ifs);
    ia >> vector_addable_int_loaded;
  }
  // Test that both addable arrays are identical
  CPPUNIT_ASSERT(*vector_addable_int == vector_addable_int_loaded);
}
