#include "test_histocrete.hpp"

CppUnit::Test* TestHistocrete::suite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestHistocrete");
    
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistocrete>("TestHistocrete: test_operator_fill", &TestHistocrete::test_operator_fill ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistocrete>("TestHistocrete: test_operator_access", &TestHistocrete::test_operator_access ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistocrete>("TestHistocrete: test_operator_increment", &TestHistocrete::test_operator_increment ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistocrete>("TestHistocrete: test_operator_divide", &TestHistocrete::test_operator_divide ) );

    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistocrete>("TestHistocrete: test_insert", &TestHistocrete::test_insert ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistocrete>("TestHistocrete: test_serialize", &TestHistocrete::test_serialize ) );

    return suiteOfTests;
}

void TestHistocrete::setUp()
{
  testhisto_int << pair<int,int>(1,0);
  testhisto_int << pair<int,int>(2,0);
  testhisto_int << pair<int,int>(3,0);
  testhisto_int << pair<int,int>(4,0);
  testhisto_int << pair<int,int>(5,1);

  testhisto_double << pair<double,double>(1.0,0.0);
  testhisto_double << pair<double,double>(2.0,0.0);
  testhisto_double << pair<double,double>(3.0,0.0);
  testhisto_double << pair<double,double>(4.0,0.0);
  testhisto_double << pair<double,double>(5.0,1.0);
}

void TestHistocrete::tearDown() { }

void TestHistocrete::test_operator_fill()
{ 
  // Test the increment by one at a given bin
  testhisto_int << 1;
  testhisto_int << 1;
  testhisto_int << 2;
  testhisto_int << 5;
  testhisto_int << 6;
  CPPUNIT_ASSERT(testhisto_int[1] == 2);
  CPPUNIT_ASSERT(testhisto_int[2] == 1);
  CPPUNIT_ASSERT(testhisto_int[5] == 2);
  CPPUNIT_ASSERT(testhisto_int[6] == 1);
  testhisto_double << 1.0;
  testhisto_double << 1.0;
  testhisto_double << 2.0;
  testhisto_double << 5.0;
  testhisto_double << 6.0;
  CPPUNIT_ASSERT(testhisto_double[1.0] == 2.0);
  CPPUNIT_ASSERT(testhisto_double[2.0] == 1.0);
  CPPUNIT_ASSERT(testhisto_double[5.0] == 2.0);
  CPPUNIT_ASSERT(testhisto_double[6.0] == 1.0);

  // Test the increment by a pair
  testhisto_int << pair<int, int>(3,5);
  testhisto_int << pair<int, int>(5,2);
  testhisto_int << pair<int, int>(7,5);
  CPPUNIT_ASSERT(testhisto_int[3] == 5);
  CPPUNIT_ASSERT(testhisto_int[5] == 4);
  CPPUNIT_ASSERT(testhisto_int[7] == 5);
  testhisto_double << pair<double, double>(3.0,5.0);
  testhisto_double << pair<double, double>(5.0,2.0);
  testhisto_double << pair<double, double>(7.0,5.0);
  CPPUNIT_ASSERT(testhisto_double[3.0] == 5.0);
  CPPUNIT_ASSERT(testhisto_double[5.0] == 4.0);
  CPPUNIT_ASSERT(testhisto_double[7.0] == 5.0);
}
void TestHistocrete::test_operator_access()
{
  // Test the get-operation
  CPPUNIT_ASSERT(testhisto_int[1] == 0);
  CPPUNIT_ASSERT(testhisto_int[5] == 1);
  CPPUNIT_ASSERT(testhisto_double[1.0] == 0.0);
  CPPUNIT_ASSERT(testhisto_double[5.0] == 1.0);

  // Test the set-operation
  testhisto_int[1] = 4;
  testhisto_int[2] = 1;
  testhisto_int[10] = 5;
  CPPUNIT_ASSERT(testhisto_int[1] == 4);
  CPPUNIT_ASSERT(testhisto_int[2] == 1);
  CPPUNIT_ASSERT(testhisto_int[10] == 5);
  testhisto_double[1.0] = 4.0;
  testhisto_double[2.0] = 1.0;
  testhisto_double[10.0] = 5.0;
  CPPUNIT_ASSERT(testhisto_double[1.0] == 4.0);
  CPPUNIT_ASSERT(testhisto_double[2.0] == 1.0);
  CPPUNIT_ASSERT(testhisto_double[10.0] == 5.0);
}
void TestHistocrete::test_operator_increment()
{ 
  // Test the += with a histocrete
  Histocrete<int,int> histo2;
  histo2[1] = 0;
  histo2[2] = 3;
  histo2[3] = -4;
  histo2[4] = 2;
  histo2[5] = 1;

  testhisto_int += histo2;
  CPPUNIT_ASSERT(testhisto_int[1] == 0);
  CPPUNIT_ASSERT(testhisto_int[2] == 3);
  CPPUNIT_ASSERT(testhisto_int[3] == -4);
  CPPUNIT_ASSERT(testhisto_int[4] == 2);
  CPPUNIT_ASSERT(testhisto_int[5] == 2);

  Histocrete<double,double> histo_double2;
  histo_double2[1.0] = 0.0;
  histo_double2[2.0] = 3.0;
  histo_double2[3.0] = -4.0;
  histo_double2[4.0] = 2.0;
  histo_double2[5.0] = 1.0;

  testhisto_double += histo_double2;
  CPPUNIT_ASSERT(testhisto_double[1.0] == 0.0);
  CPPUNIT_ASSERT(testhisto_double[2.0] == 3.0);
  CPPUNIT_ASSERT(testhisto_double[3.0] == -4.0);
  CPPUNIT_ASSERT(testhisto_double[4.0] == 2.0);
  CPPUNIT_ASSERT(testhisto_double[5.0] == 2.0);

  // Test the += with a constant value
  testhisto_int += 3;
  CPPUNIT_ASSERT(testhisto_int[1] == 3);
  CPPUNIT_ASSERT(testhisto_int[2] == 6);
  CPPUNIT_ASSERT(testhisto_int[3] == -1);
  CPPUNIT_ASSERT(testhisto_int[4] == 5);
  CPPUNIT_ASSERT(testhisto_int[5] == 5);
  testhisto_double += 3.0;
  CPPUNIT_ASSERT(testhisto_double[1.0] == 3.0);
  CPPUNIT_ASSERT(testhisto_double[2.0] == 6.0);
  CPPUNIT_ASSERT(testhisto_double[3.0] == -1.0);
  CPPUNIT_ASSERT(testhisto_double[4.0] == 5.0);
  CPPUNIT_ASSERT(testhisto_double[5.0] == 5.0);
}
void TestHistocrete::test_operator_divide()
{ 

}

void TestHistocrete::test_insert()
{
  // Test the insertion of a pair
  testhisto_int.insert(pair<int, int>(6,3));
  CPPUNIT_ASSERT_EQUAL(3, testhisto_int[6]);
  testhisto_double.insert(pair<double, double>(4.4, 2.0));
  CPPUNIT_ASSERT_EQUAL(2.0, testhisto_double[4.4]);

  // Construct a new histocrete and test the insertion via the iterators
  Histocrete<int, int> testhisto_int_2;
  testhisto_int_2 << pair<int,int>(7,3);
  testhisto_int_2 << pair<int,int>(9,1);
  testhisto_int_2 << pair<int,int>(11,2);
  testhisto_int_2 << pair<int,int>(12,5);
  testhisto_int.insert(testhisto_int_2.begin(), testhisto_int_2.end());
  CPPUNIT_ASSERT_EQUAL(3, testhisto_int[7]);
  CPPUNIT_ASSERT_EQUAL(1, testhisto_int[9]);
  CPPUNIT_ASSERT_EQUAL(2, testhisto_int[11]);
  CPPUNIT_ASSERT_EQUAL(5, testhisto_int[12]);
}

void TestHistocrete::test_serialize()
{
  testhisto_int.save_serialize("serialize_test.dat");
  
  Histocrete<int,int> testhisto_load;
  testhisto_load.load_serialize("serialize_test.dat");

  CPPUNIT_ASSERT(testhisto_int[1] == testhisto_load[1]);
  CPPUNIT_ASSERT(testhisto_int[2] == testhisto_load[2]);
  CPPUNIT_ASSERT(testhisto_int[3] == testhisto_load[3]);
  CPPUNIT_ASSERT(testhisto_int[4] == testhisto_load[4]);
  CPPUNIT_ASSERT(testhisto_int[5] == testhisto_load[5]);

  testhisto_double.save_serialize("serialize_test.dat");
  
  Histocrete<double,double> testhisto_load_double;
  testhisto_load_double.load_serialize("serialize_test.dat");

  CPPUNIT_ASSERT(testhisto_double[1.0] == testhisto_load_double[1.0]);
  CPPUNIT_ASSERT(testhisto_double[2.0] == testhisto_load_double[2.0]);
  CPPUNIT_ASSERT(testhisto_double[3.0] == testhisto_load_double[3.0]);
  CPPUNIT_ASSERT(testhisto_double[4.0] == testhisto_load_double[4.0]);
  CPPUNIT_ASSERT(testhisto_double[5.0] == testhisto_load_double[5.0]);
}
