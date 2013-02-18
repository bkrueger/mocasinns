#include "test_histobase.hpp"

CppUnit::Test* TestHistoBase::suite()
{
    CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestHistograms/TestHistoBase");
    
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistoBase>("TestHistograms/TestHistoBase: test_operator_equal", &TestHistoBase::test_operator_equal ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistoBase>("TestHistograms/TestHistoBase: test_derivative", &TestHistoBase::test_derivative ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistoBase>("TestHistograms/TestHistoBase: test_flatness", &TestHistoBase::test_flatness ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistoBase>("TestHistograms/TestHistoBase: test_initialise_empty", &TestHistoBase::test_initialise_empty ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistoBase>("TestHistograms/TestHistoBase: test_max_x_value", &TestHistoBase::test_max_x_value ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistoBase>("TestHistograms/TestHistoBase: test_max_y_value", &TestHistoBase::test_max_y_value ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistoBase>("TestHistograms/TestHistoBase: test_min_x_value", &TestHistoBase::test_min_x_value ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistoBase>("TestHistograms/TestHistoBase: test_min_y_value", &TestHistoBase::test_min_y_value ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistoBase>("TestHistograms/TestHistoBase: test_set_all_y_values", &TestHistoBase::test_set_all_y_values ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistoBase>("TestHistograms/TestHistoBase: test_shift_bin_zero", &TestHistoBase::test_shift_bin_zero ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistoBase>("TestHistograms/TestHistoBase: test_sum", &TestHistoBase::test_sum ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistoBase>("TestHistograms/TestHistoBase: test_serialize", &TestHistoBase::test_serialize ) );
    suiteOfTests->addTest( new CppUnit::TestCaller<TestHistoBase>("TestHistograms/TestHistoBase: test_csv", &TestHistoBase::test_csv ) );

    return suiteOfTests;
}

void TestHistoBase::setUp()
{
  testhisto_int << std::pair<int,int>(1,0);
  testhisto_int << std::pair<int,int>(2,0);
  testhisto_int << std::pair<int,int>(3,0);
  testhisto_int << std::pair<int,int>(4,0);
  testhisto_int << std::pair<int,int>(5,1);

  testhisto_double << std::pair<double,double>(1.0,0.0);
  testhisto_double << std::pair<double,double>(2.0,0.0);
  testhisto_double << std::pair<double,double>(3.0,0.0);
  testhisto_double << std::pair<double,double>(4.0,0.0);
  testhisto_double << std::pair<double,double>(5.0,1.0);
}
void TestHistoBase::tearDown() { }

void TestHistoBase::test_operator_equal()
{
  // Set up the same histobases again
  Histocrete<int,int> testhisto_int_second;
  testhisto_int_second[1] = 0;
  testhisto_int_second[2] = 0;
  testhisto_int_second[3] = 0;
  testhisto_int_second[4] = 0;
  testhisto_int_second[5] = 1;
  Histocrete<double,double> testhisto_double_second;
  testhisto_double_second[1.0] = 0.0;
  testhisto_double_second[2.0] = 0.0;
  testhisto_double_second[3.0] = 0.0;
  testhisto_double_second[4.0] = 0.0;
  testhisto_double_second[5.0] = 1.0;
  // Test for equality
  CPPUNIT_ASSERT(testhisto_int == testhisto_int_second);
  CPPUNIT_ASSERT(testhisto_double == testhisto_double_second);

  // Change two entries and test again
  testhisto_int[1] = 2;
  testhisto_double[1.0] = 2.0;
  // Test for inequality
  CPPUNIT_ASSERT(testhisto_int != testhisto_int_second);
  CPPUNIT_ASSERT(testhisto_double != testhisto_double_second);

  // Add a new entry and tes again
  testhisto_int_second[6] = 2;
  testhisto_double_second[6.0] = 2.0;
  // Test for inequality
  CPPUNIT_ASSERT(testhisto_int != testhisto_int_second);
  CPPUNIT_ASSERT(testhisto_double != testhisto_double_second);
}

void TestHistoBase::test_derivative()
{
  // Test for integers
  // Fill with additional numbers
  testhisto_int << std::pair<int,int>(1,2);
  testhisto_int << std::pair<int,int>(2,1);
  // Test the derivatives
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, testhisto_int.derivative(testhisto_int.find(1)), 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, testhisto_int.derivative(testhisto_int.find(2)), 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.5, testhisto_int.derivative(testhisto_int.find(3)), 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, testhisto_int.derivative(testhisto_int.find(4)), 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, testhisto_int.derivative(testhisto_int.find(5)), 1e-6);

  // Test for doubles
  // Test for integers
  // Fill with additional numbers
  testhisto_double << std::pair<double,double>(1.0,2.0);
  testhisto_double << std::pair<double,double>(2.0,1.0);
  // Test the derivatives
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, testhisto_double.derivative(testhisto_double.find(1)), 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, testhisto_double.derivative(testhisto_double.find(2)), 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.5, testhisto_double.derivative(testhisto_double.find(3)), 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, testhisto_double.derivative(testhisto_double.find(4)), 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, testhisto_double.derivative(testhisto_double.find(5)), 1e-6);
}
void TestHistoBase::test_flatness()
{
  testhisto_int[1] = 5;
  testhisto_int[2] = 2;
  testhisto_int[3] = 7;
  testhisto_int[4] = 2;
  testhisto_int[5] = 3;

  double mean = 19.0/5.0;
  CPPUNIT_ASSERT(testhisto_int.flatness() == 2.0/mean);  

  testhisto_double[1.0] = 5.0;
  testhisto_double[2.0] = 2.0;
  testhisto_double[3.0] = 7.0;
  testhisto_double[4.0] = 2.0;
  testhisto_double[5.0] = 3.0;

  CPPUNIT_ASSERT(testhisto_double.flatness() == 2.0/mean);  
}
void TestHistoBase::test_initialise_empty()
{
  HistoBase<double, double> initialised_testhisto_double;
  initialised_testhisto_double.initialise_empty(testhisto_double);
  CPPUNIT_ASSERT_EQUAL(5, static_cast<int>(initialised_testhisto_double.size()));
  CPPUNIT_ASSERT(initialised_testhisto_double.find(1.0) != initialised_testhisto_double.end());
  CPPUNIT_ASSERT(initialised_testhisto_double.find(2.0) != initialised_testhisto_double.end());
  CPPUNIT_ASSERT(initialised_testhisto_double.find(3.0) != initialised_testhisto_double.end());
  CPPUNIT_ASSERT(initialised_testhisto_double.find(4.0) != initialised_testhisto_double.end());
  CPPUNIT_ASSERT(initialised_testhisto_double.find(5.0) != initialised_testhisto_double.end());
  CPPUNIT_ASSERT_EQUAL(0.0, initialised_testhisto_double.find(1.0)->second);
  CPPUNIT_ASSERT_EQUAL(0.0, initialised_testhisto_double.find(2.0)->second);
  CPPUNIT_ASSERT_EQUAL(0.0, initialised_testhisto_double.find(3.0)->second);
  CPPUNIT_ASSERT_EQUAL(0.0, initialised_testhisto_double.find(4.0)->second);
  CPPUNIT_ASSERT_EQUAL(0.0, initialised_testhisto_double.find(5.0)->second);

  HistoBase<double, int> initialised_testhisto_double_int;
  initialised_testhisto_double_int.initialise_empty(testhisto_double);
  CPPUNIT_ASSERT_EQUAL(5, static_cast<int>(initialised_testhisto_double_int.size()));
  CPPUNIT_ASSERT(initialised_testhisto_double_int.find(1.0) != initialised_testhisto_double_int.end());
  CPPUNIT_ASSERT(initialised_testhisto_double_int.find(2.0) != initialised_testhisto_double_int.end());
  CPPUNIT_ASSERT(initialised_testhisto_double_int.find(3.0) != initialised_testhisto_double_int.end());
  CPPUNIT_ASSERT(initialised_testhisto_double_int.find(4.0) != initialised_testhisto_double_int.end());
  CPPUNIT_ASSERT(initialised_testhisto_double_int.find(5.0) != initialised_testhisto_double_int.end());
  CPPUNIT_ASSERT_EQUAL(0, initialised_testhisto_double_int.find(1.0)->second);
  CPPUNIT_ASSERT_EQUAL(0, initialised_testhisto_double_int.find(2.0)->second);
  CPPUNIT_ASSERT_EQUAL(0, initialised_testhisto_double_int.find(3.0)->second);
  CPPUNIT_ASSERT_EQUAL(0, initialised_testhisto_double_int.find(4.0)->second);
  CPPUNIT_ASSERT_EQUAL(0, initialised_testhisto_double_int.find(5.0)->second);
}
void TestHistoBase::test_max_x_value()
{
  CPPUNIT_ASSERT_EQUAL(5, testhisto_int.max_x_value()->first);
  CPPUNIT_ASSERT_EQUAL(5.0, testhisto_double.max_x_value()->first);
}
void TestHistoBase::test_max_y_value()
{
  CPPUNIT_ASSERT_EQUAL(5, testhisto_int.max_y_value()->first);
  CPPUNIT_ASSERT_EQUAL(5.0, testhisto_double.max_y_value()->first);
}
void TestHistoBase::test_min_x_value()
{
  CPPUNIT_ASSERT_EQUAL(1, testhisto_int.min_x_value()->first);
  CPPUNIT_ASSERT_EQUAL(1.0, testhisto_double.min_x_value()->first);
}
void TestHistoBase::test_min_y_value()
{
  CPPUNIT_ASSERT_EQUAL(1, testhisto_int.min_y_value()->first);
  CPPUNIT_ASSERT_EQUAL(1.0, testhisto_double.min_y_value()->first);
}
void TestHistoBase::test_set_all_y_values()
{ 
  testhisto_int.set_all_y_values(3);
  CPPUNIT_ASSERT(testhisto_int[1] == 3);
  CPPUNIT_ASSERT(testhisto_int[2] == 3);
  CPPUNIT_ASSERT(testhisto_int[3] == 3);
  CPPUNIT_ASSERT(testhisto_int[4] == 3);
  CPPUNIT_ASSERT(testhisto_int[5] == 3);
  testhisto_double.set_all_y_values(3.0);
  CPPUNIT_ASSERT(testhisto_double[1] == 3.0);
  CPPUNIT_ASSERT(testhisto_double[2] == 3.0);
  CPPUNIT_ASSERT(testhisto_double[3] == 3.0);
  CPPUNIT_ASSERT(testhisto_double[4] == 3.0);
  CPPUNIT_ASSERT(testhisto_double[5] == 3.0);
}
void TestHistoBase::test_shift_bin_zero()
{
  testhisto_int[1] = 2;
  testhisto_int[2] = 3;
  testhisto_int[3] = 1;
  testhisto_int[4] = 4;
  testhisto_int[5] = 5;

  testhisto_int.shift_bin_zero(testhisto_int.min_y_value());
  CPPUNIT_ASSERT(testhisto_int[1] == 1);
  CPPUNIT_ASSERT(testhisto_int[2] == 2);
  CPPUNIT_ASSERT(testhisto_int[3] == 0);
  CPPUNIT_ASSERT(testhisto_int[4] == 3);
  CPPUNIT_ASSERT(testhisto_int[5] == 4);

  testhisto_double[1.0] = 2.0;
  testhisto_double[2.0] = 3.0;
  testhisto_double[3.0] = 1.0;
  testhisto_double[4.0] = 4.0;
  testhisto_double[5.0] = 5.0;

  testhisto_double.shift_bin_zero(testhisto_double.min_y_value());
  CPPUNIT_ASSERT(testhisto_double[1.0] == 1.0);
  CPPUNIT_ASSERT(testhisto_double[2.0] == 2.0);
  CPPUNIT_ASSERT(testhisto_double[3.0] == 0.0);
  CPPUNIT_ASSERT(testhisto_double[4.0] == 3.0);
  CPPUNIT_ASSERT(testhisto_double[5.0] == 4.0);
}
void TestHistoBase::test_sum()
{
  int sum = testhisto_int[1] + testhisto_int[2] + testhisto_int[3] + testhisto_int[4] + testhisto_int[5];
  CPPUNIT_ASSERT(testhisto_int.sum() == sum);

  double sum_double = testhisto_double[1] + testhisto_double[2] + testhisto_double[3] + testhisto_double[4] + testhisto_double[5];
  CPPUNIT_ASSERT(testhisto_double.sum() == sum_double);
}
void TestHistoBase::test_serialize()
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
void TestHistoBase::test_csv()
{ 
  testhisto_int.save_csv("csv_test.dat");
  
  Histocrete<int,int> testhisto_load;
  testhisto_load.load_csv("csv_test.dat");

  CPPUNIT_ASSERT(testhisto_int[1] == testhisto_load[1]);
  CPPUNIT_ASSERT(testhisto_int[2] == testhisto_load[2]);
  CPPUNIT_ASSERT(testhisto_int[3] == testhisto_load[3]);
  CPPUNIT_ASSERT(testhisto_int[4] == testhisto_load[4]);
  CPPUNIT_ASSERT(testhisto_int[5] == testhisto_load[5]);

  testhisto_double.save_csv("csv_test.dat");
  
  Histocrete<double,double> testhisto_double_load;
  testhisto_double_load.load_csv("csv_test.dat");

  CPPUNIT_ASSERT(testhisto_double[1.0] == testhisto_double_load[1.0]);
  CPPUNIT_ASSERT(testhisto_double[2.0] == testhisto_double_load[2.0]);
  CPPUNIT_ASSERT(testhisto_double[3.0] == testhisto_double_load[3.0]);
  CPPUNIT_ASSERT(testhisto_double[4.0] == testhisto_double_load[4.0]);
  CPPUNIT_ASSERT(testhisto_double[5.0] == testhisto_double_load[5.0]);
}
