#include "test_pair_addable.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

CppUnit::Test* TestPairAddable::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestObservables/TestPairAddable");
  suite_of_tests->addTest( new CppUnit::TestCaller<TestPairAddable>("TestObservables/TestPairAddable: test_constructor", &TestPairAddable::test_constructor) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestPairAddable>("TestObservables/TestPairAddable: test_operator_add", &TestPairAddable::test_operator_add) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestPairAddable>("TestObservables/TestPairAddable: test_operator_substract", &TestPairAddable::test_operator_substract) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestPairAddable>("TestObservables/TestPairAddable: test_operator_multiply", &TestPairAddable::test_operator_multiply) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestPairAddable>("TestObservables/TestPairAddable: test_operator_divide", &TestPairAddable::test_operator_divide) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestPairAddable>("TestObservables/TestPairAddable: test_operator_outstream", &TestPairAddable::test_operator_outstream) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestPairAddable>("TestObservables/TestPairAddable: test_operator_instream", &TestPairAddable::test_operator_instream) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestPairAddable>("TestObservables/TestPairAddable: test_serialization", &TestPairAddable::test_serialization) );

  return suite_of_tests;
}

void TestPairAddable::setUp()
{
  pair_addable.first = 2;
  pair_addable.second = -1.5;
}

void TestPairAddable::tearDown() { }

void TestPairAddable::test_constructor()
{ 
  // Test the allready constructed PairAddables
  CPPUNIT_ASSERT_EQUAL(2, pair_addable.first);
  CPPUNIT_ASSERT_EQUAL(-1.5, pair_addable.second);
}

void TestPairAddable::test_operator_add()
{
  PairAddablePlain<int, double> other_pair;
  other_pair.first = 3;
  other_pair.second = 0.5;
  
  PairAddablePlain<int, double> pair_added = pair_addable + other_pair;
  CPPUNIT_ASSERT_EQUAL(pair_addable.first + other_pair.first, pair_added.first);
  CPPUNIT_ASSERT_EQUAL(pair_addable.second + other_pair.second, pair_added.second);
}

void TestPairAddable::test_operator_substract()
{
  PairAddablePlain<int, double> other_pair;
  other_pair.first = 3;
  other_pair.second = 0.5;
  
  PairAddablePlain<int, double> pair_substracted = pair_addable - other_pair;
  CPPUNIT_ASSERT_EQUAL(pair_addable.first - other_pair.first, pair_substracted.first);
  CPPUNIT_ASSERT_EQUAL(pair_addable.second - other_pair.second, pair_substracted.second);
}

void TestPairAddable::test_operator_multiply()
{
  CPPUNIT_ASSERT_EQUAL(2*pair_addable.first, (2*pair_addable).first);
  CPPUNIT_ASSERT_EQUAL(2*pair_addable.second, (2*pair_addable).second);
}
void TestPairAddable::test_operator_divide()
{
  CPPUNIT_ASSERT_EQUAL(pair_addable.first / 3, (pair_addable / 3).first);
  CPPUNIT_ASSERT_EQUAL(pair_addable.second / 3, (pair_addable / 3).second);
}

void TestPairAddable::test_operator_outstream()
{
  std::stringstream ss;
  ss << pair_addable;
  CPPUNIT_ASSERT_EQUAL(static_cast<std::string>("2 -1.5"), ss.str());
}
void TestPairAddable::test_operator_instream()
{
  
}

void TestPairAddable::test_serialization()
{
  // Create an output file
  std::ofstream ofs("serialization.dat");
  {
    boost::archive::text_oarchive oa(ofs);
    oa << pair_addable;
  }
  // Read from an input file
  PairAddablePlain<int,double> pair_addable_loaded;
  {
    std::ifstream ifs("serialization.dat");
    boost::archive::text_iarchive ia(ifs);
    ia >> pair_addable_loaded;
  }
  // Test that both addable arrays are identical
  CPPUNIT_ASSERT(pair_addable == pair_addable_loaded);
}
