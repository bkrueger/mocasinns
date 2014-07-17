#include "test_histogram_constant_width.hpp"
#include <mocasinns/histograms/histocrete.hpp>

CppUnit::Test* TestHistogramConstantWidth::suite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestHistograms/TestHistogram");
    
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistogramConstantWidth>("TestHistograms/TestHistogramConstantWidth: test_operator_fill", &TestHistogramConstantWidth::test_operator_fill ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistogramConstantWidth>("TestHistograms/TestHistogramConstantWidth: test_operator_access", &TestHistogramConstantWidth::test_operator_access ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistogramConstantWidth>("TestHistograms/TestHistogramConstantWidth: test_operator_increment", &TestHistogramConstantWidth::test_operator_increment ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistogramConstantWidth>("TestHistograms/TestHistogramConstantWidth: test_operator_divide", &TestHistogramConstantWidth::test_operator_divide ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistogramConstantWidth>("TestHistograms/TestHistogramConstantWidth: test_insert", &TestHistogramConstantWidth::test_insert ) );

    return suiteOfTests;
}

void TestHistogramConstantWidth::setUp()
{
  testhisto_int = new HistogramConstantWidth<int, int>(typename HistogramConstantWidth<int, int>::BinningFunctorType(3,0));
  testhisto_double = new HistogramConstantWidth<double, double>(typename HistogramConstantWidth<double, double>::BinningFunctorType(2.5,0.0));

  (*testhisto_int) << std::pair<int,int>(0,4);
  (*testhisto_int) << std::pair<int,int>(3,5);
  (*testhisto_int) << std::pair<int,int>(6,1);
  (*testhisto_int) << std::pair<int,int>(9,5);

  (*testhisto_double) << std::pair<double,double>(0.0,0.8);
  (*testhisto_double) << std::pair<double,double>(2.5,1.0);
  (*testhisto_double) << std::pair<double,double>(5.0,4.8);
  (*testhisto_double) << std::pair<double,double>(7.5,2.1);
}

void TestHistogramConstantWidth::tearDown() 
{ 
  delete testhisto_int;
  delete testhisto_double;
}

void TestHistogramConstantWidth::test_operator_fill()
{ 
  // Test the increment by one at a given bin
  (*testhisto_int) << 1;
  (*testhisto_int) << 1;
  (*testhisto_int) << 2;
  (*testhisto_int) << 5;
  (*testhisto_int) << 6;
  CPPUNIT_ASSERT_EQUAL(7, (*testhisto_int)[0]);
  CPPUNIT_ASSERT_EQUAL(6, (*testhisto_int)[3]);
  CPPUNIT_ASSERT_EQUAL(2, (*testhisto_int)[6]);
  CPPUNIT_ASSERT_EQUAL(5, (*testhisto_int)[9]);

  (*testhisto_double) << 1.0;
  (*testhisto_double) << 1.0;
  (*testhisto_double) << 2.0;
  (*testhisto_double) << 5.0;
  (*testhisto_double) << 6.0;
  CPPUNIT_ASSERT_EQUAL(3.8, (*testhisto_double)[0.0]);
  CPPUNIT_ASSERT_EQUAL(1.0, (*testhisto_double)[2.5]);
  CPPUNIT_ASSERT_EQUAL(6.8, (*testhisto_double)[5.0]);
  CPPUNIT_ASSERT_EQUAL(2.1, (*testhisto_double)[7.5]);

  // Test the increment by a pair
  (*testhisto_int) << std::pair<int, int>(3,5);
  (*testhisto_int) << std::pair<int, int>(5,2);
  (*testhisto_int) << std::pair<int, int>(7,5);
  CPPUNIT_ASSERT_EQUAL(7, (*testhisto_int)[0]);
  CPPUNIT_ASSERT_EQUAL(13, (*testhisto_int)[3]);
  CPPUNIT_ASSERT_EQUAL(7, (*testhisto_int)[6]);
  CPPUNIT_ASSERT_EQUAL(5, (*testhisto_int)[9]);

  (*testhisto_double) << std::pair<double, double>(3.0,5.0);
  (*testhisto_double) << std::pair<double, double>(5.0,2.0);
  (*testhisto_double) << std::pair<double, double>(7.0,5.0);
  CPPUNIT_ASSERT_EQUAL(3.8, (*testhisto_double)[0.0]);
  CPPUNIT_ASSERT_EQUAL(6.0, (*testhisto_double)[2.5]);
  CPPUNIT_ASSERT_EQUAL(13.8, (*testhisto_double)[5.0]);
  CPPUNIT_ASSERT_EQUAL(2.1, (*testhisto_double)[7.5]);
}
void TestHistogramConstantWidth::test_operator_access()
{
  // Test the get-operation
  CPPUNIT_ASSERT_EQUAL(4, (*testhisto_int)[0]);
  CPPUNIT_ASSERT_EQUAL(4, (*testhisto_int)[1]);
  CPPUNIT_ASSERT_EQUAL(4, (*testhisto_int)[2]);
  CPPUNIT_ASSERT_EQUAL(5, (*testhisto_int)[3]);
  CPPUNIT_ASSERT_EQUAL(1, (*testhisto_int)[7]);
  CPPUNIT_ASSERT_EQUAL(1, (*testhisto_int)[8]);
  CPPUNIT_ASSERT_EQUAL(5, (*testhisto_int)[9]);

  CPPUNIT_ASSERT_EQUAL(0.8, (*testhisto_double)[0.0]);
  CPPUNIT_ASSERT_EQUAL(0.8, (*testhisto_double)[1.0]);
  CPPUNIT_ASSERT_EQUAL(0.8, (*testhisto_double)[1.4]);  
  CPPUNIT_ASSERT_EQUAL(0.8, (*testhisto_double)[2.1]);
  CPPUNIT_ASSERT_EQUAL(1.0, (*testhisto_double)[2.5]);
  CPPUNIT_ASSERT_EQUAL(4.8, (*testhisto_double)[5.4]);
  CPPUNIT_ASSERT_EQUAL(2.1, (*testhisto_double)[7.9]);

  // Test the set-operation
  (*testhisto_int)[1] = 4;
  (*testhisto_int)[2] = 1;
  (*testhisto_int)[10] = 5;
  (*testhisto_int)[13] = 3;
  CPPUNIT_ASSERT_EQUAL(1, (*testhisto_int)[0]);
  CPPUNIT_ASSERT_EQUAL(5, (*testhisto_int)[9]);
  CPPUNIT_ASSERT_EQUAL(3, (*testhisto_int)[12]);

  (*testhisto_double)[1.0] = 4.0;
  (*testhisto_double)[2.0] = 1.0;
  (*testhisto_double)[10.0] = 5.0;
  CPPUNIT_ASSERT_EQUAL(1.0, (*testhisto_double)[1.5]);
  CPPUNIT_ASSERT_EQUAL(5.0, (*testhisto_double)[10.0]);
}
void TestHistogramConstantWidth::test_operator_increment()
{ 
  // Test the += with a histocrete
  HistogramConstantWidth<int,int> histo2(typename HistogramConstantWidth<int, int>::BinningFunctorType(3,0));
  histo2[0] = 2;
  histo2[3] = -4;
  histo2[6] = 1;

  (*testhisto_int) += histo2;
  CPPUNIT_ASSERT_EQUAL(6, (*testhisto_int)[0]);
  CPPUNIT_ASSERT_EQUAL(1, (*testhisto_int)[3]);
  CPPUNIT_ASSERT_EQUAL(2, (*testhisto_int)[6]);
  CPPUNIT_ASSERT_EQUAL(5, (*testhisto_int)[9]);

  // Test the += with a constant value
  (*testhisto_int) += 3;
  CPPUNIT_ASSERT_EQUAL(9, (*testhisto_int)[0]);
  CPPUNIT_ASSERT_EQUAL(4, (*testhisto_int)[3]);
  CPPUNIT_ASSERT_EQUAL(5, (*testhisto_int)[6]);
  CPPUNIT_ASSERT_EQUAL(8, (*testhisto_int)[9]);
}
void TestHistogramConstantWidth::test_operator_divide()
{ 

}

void TestHistogramConstantWidth::test_insert()
{
  // Test the insertion of a pair
  testhisto_int->insert(std::pair<int, int>(14,3));
  CPPUNIT_ASSERT_EQUAL(3, (*testhisto_int)[12]);
  testhisto_double->insert(std::pair<double, double>(-0.5, 2.0));
  CPPUNIT_ASSERT_EQUAL(2.0, (*testhisto_double)[-2.5]);

  // Construct a new histocrete and test the insertion via the iterators
  Histocrete<int, int> testhisto_int_2;
  testhisto_int_2 << std::pair<int,int>(16,3);
  testhisto_int_2 << std::pair<int,int>(19,1);
  testhisto_int_2 << std::pair<int,int>(21,2);
  testhisto_int->insert(testhisto_int_2.begin(), testhisto_int_2.end());
  CPPUNIT_ASSERT_EQUAL(3, (*testhisto_int)[15]);
  CPPUNIT_ASSERT_EQUAL(1, (*testhisto_int)[18]);
  CPPUNIT_ASSERT_EQUAL(2, (*testhisto_int)[21]);
}
