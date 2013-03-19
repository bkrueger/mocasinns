#include "test_histogram.hpp"
#include <mocasinns/histograms/histocrete.hpp>

CppUnit::Test* TestHistogram::suite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestHistograms/TestHistogram");
    
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistogram>("TestHistograms/TestHistogram: test_operator_fill", &TestHistogram::test_operator_fill ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistogram>("TestHistograms/TestHistogram: test_operator_access", &TestHistogram::test_operator_access ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistogram>("TestHistograms/TestHistogram: test_operator_increment", &TestHistogram::test_operator_increment ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistogram>("TestHistograms/TestHistogram: test_operator_divide", &TestHistogram::test_operator_divide ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistogram>("TestHistograms/TestHistogram: test_initialise_empty", &TestHistogram::test_initialise_empty ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistogram>("TestHistograms/TestHistogram: test_insert", &TestHistogram::test_insert ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistogram>("TestHistograms/TestHistogram: test_serialize", &TestHistogram::test_serialize ) );

    return suiteOfTests;
}

void TestHistogram::setUp()
{
  testhisto_int = new Histogram<int, int, BinningNumber<int> >(BinningNumber<int>(3));
  testhisto_double = new Histogram<double, double, BinningNumber<double> >(BinningNumber<double>(2.5));

  (*testhisto_int) << std::pair<int,int>(0,4);
  (*testhisto_int) << std::pair<int,int>(3,5);
  (*testhisto_int) << std::pair<int,int>(6,1);
  (*testhisto_int) << std::pair<int,int>(9,5);

  (*testhisto_double) << std::pair<double,double>(0.0,0.8);
  (*testhisto_double) << std::pair<double,double>(2.5,1.0);
  (*testhisto_double) << std::pair<double,double>(5.0,4.8);
  (*testhisto_double) << std::pair<double,double>(7.5,2.1);
}

void TestHistogram::tearDown() 
{ 
  delete testhisto_int;
  delete testhisto_double;
}

void TestHistogram::test_operator_fill()
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

  // Test the increment by a std::pair
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
void TestHistogram::test_operator_access()
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
void TestHistogram::test_operator_increment()
{ 
  // Test the += with a histocrete
  Histogram<int,int, BinningNumber<int> > histo2(BinningNumber<int>(3));
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
void TestHistogram::test_operator_divide()
{ 

}

void TestHistogram::test_initialise_empty()
{
  Histogram<double, double, BinningNumber<double> > initialised_testhisto_double;
  initialised_testhisto_double.initialise_empty(*testhisto_double);
  CPPUNIT_ASSERT_EQUAL(4, static_cast<int>(initialised_testhisto_double.size()));
  CPPUNIT_ASSERT(initialised_testhisto_double.find(0.0) != initialised_testhisto_double.end());
  CPPUNIT_ASSERT(initialised_testhisto_double.find(2.5) != initialised_testhisto_double.end());
  CPPUNIT_ASSERT(initialised_testhisto_double.find(5.0) != initialised_testhisto_double.end());
  CPPUNIT_ASSERT(initialised_testhisto_double.find(7.5) != initialised_testhisto_double.end());
  CPPUNIT_ASSERT_EQUAL(0.0, initialised_testhisto_double.find(0.0)->second);
  CPPUNIT_ASSERT_EQUAL(0.0, initialised_testhisto_double.find(2.5)->second);
  CPPUNIT_ASSERT_EQUAL(0.0, initialised_testhisto_double.find(5.0)->second);
  CPPUNIT_ASSERT_EQUAL(0.0, initialised_testhisto_double.find(7.5)->second);
  CPPUNIT_ASSERT_EQUAL(2.5, initialised_testhisto_double.get_binning_width());
  CPPUNIT_ASSERT_EQUAL(0.0, initialised_testhisto_double.get_binning_reference());

  Histogram<double, int, BinningNumber<double> > initialised_testhisto_double_int;
  initialised_testhisto_double_int.initialise_empty(*testhisto_double);
  CPPUNIT_ASSERT_EQUAL(4, static_cast<int>(initialised_testhisto_double_int.size()));
  CPPUNIT_ASSERT(initialised_testhisto_double_int.find(0.0) != initialised_testhisto_double_int.end());
  CPPUNIT_ASSERT(initialised_testhisto_double_int.find(2.5) != initialised_testhisto_double_int.end());
  CPPUNIT_ASSERT(initialised_testhisto_double_int.find(5.0) != initialised_testhisto_double_int.end());
  CPPUNIT_ASSERT(initialised_testhisto_double_int.find(7.5) != initialised_testhisto_double_int.end());
  CPPUNIT_ASSERT_EQUAL(0, initialised_testhisto_double_int.find(0.0)->second);
  CPPUNIT_ASSERT_EQUAL(0, initialised_testhisto_double_int.find(2.5)->second);
  CPPUNIT_ASSERT_EQUAL(0, initialised_testhisto_double_int.find(5.0)->second);
  CPPUNIT_ASSERT_EQUAL(0, initialised_testhisto_double_int.find(7.5)->second);
  CPPUNIT_ASSERT_EQUAL(2.5, initialised_testhisto_double_int.get_binning_width());
  CPPUNIT_ASSERT_EQUAL(0.0, initialised_testhisto_double_int.get_binning_reference());
}
void TestHistogram::test_insert()
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

void TestHistogram::test_serialize()
{
  (*testhisto_int).save_serialize("serialize_test.dat");
  
  Histogram<int, int, BinningNumber<int> > testhisto_load;
  testhisto_load.load_serialize("serialize_test.dat");

  CPPUNIT_ASSERT_EQUAL((*testhisto_int)[1], testhisto_load[1]);
  CPPUNIT_ASSERT_EQUAL((*testhisto_int)[2], testhisto_load[2]);
  CPPUNIT_ASSERT_EQUAL((*testhisto_int)[3], testhisto_load[3]);
  CPPUNIT_ASSERT_EQUAL((*testhisto_int)[4], testhisto_load[4]);
  CPPUNIT_ASSERT_EQUAL((*testhisto_int)[5], testhisto_load[5]);
  CPPUNIT_ASSERT_EQUAL((*testhisto_int).get_binning().get_binning_width(), testhisto_load.get_binning().get_binning_width());
  CPPUNIT_ASSERT_EQUAL((*testhisto_int).get_binning().get_binning_reference(), testhisto_load.get_binning().get_binning_reference());

  (*testhisto_double).save_serialize("serialize_test.dat");
  
  Histogram<double, double, BinningNumber<double> > testhisto_load_double;
  testhisto_load_double.load_serialize("serialize_test.dat");

  CPPUNIT_ASSERT_EQUAL((*testhisto_double)[1.0], testhisto_load_double[1.0]);
  CPPUNIT_ASSERT_EQUAL((*testhisto_double)[2.0], testhisto_load_double[2.0]);
  CPPUNIT_ASSERT_EQUAL((*testhisto_double)[3.0], testhisto_load_double[3.0]);
  CPPUNIT_ASSERT_EQUAL((*testhisto_double)[4.0], testhisto_load_double[4.0]);
  CPPUNIT_ASSERT_EQUAL((*testhisto_double)[5.0], testhisto_load_double[5.0]);
  CPPUNIT_ASSERT_EQUAL((*testhisto_double).get_binning().get_binning_width(), testhisto_load_double.get_binning().get_binning_width());
  CPPUNIT_ASSERT_EQUAL((*testhisto_double).get_binning().get_binning_reference(), testhisto_load_double.get_binning().get_binning_reference());
}
