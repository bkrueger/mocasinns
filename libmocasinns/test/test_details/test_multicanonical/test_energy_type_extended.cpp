#include "test_energy_type_extended.hpp"

CppUnit::Test* TestEnergyTypeExtended::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestDetails/TestMulticanonical/TestEnergyTypeExtended");

  suite_of_tests->addTest( new CppUnit::TestCaller<TestEnergyTypeExtended>("TestDetails/TestMulticanonical/TestEnergyTypeExtended: test_constructor", &TestEnergyTypeExtended::test_constructor) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestEnergyTypeExtended>("TestDetails/TestMulticanonical/TestEnergyTypeExtended: test_operator_compare", &TestEnergyTypeExtended::test_operator_compare) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestEnergyTypeExtended>("TestDetails/TestMulticanonical/TestEnergyTypeExtended: test_operator_add", &TestEnergyTypeExtended::test_operator_add) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestEnergyTypeExtended>("TestDetails/TestMulticanonical/TestEnergyTypeExtended: test_operator_substract", &TestEnergyTypeExtended::test_operator_substract) );
    
  return suite_of_tests;
}

void TestEnergyTypeExtended::setUp()
{
  test_extended_int_1 = new ExtendedInt(5, 0);
  test_extended_int_2 = new ExtendedInt(3, 0);
  test_extended_int_3 = new ExtendedInt(4, 1);
  test_extended_int_4 = new ExtendedInt(2, -1);
}
void TestEnergyTypeExtended::tearDown()
{
  delete test_extended_int_1;
  delete test_extended_int_2;
  delete test_extended_int_3;
  delete test_extended_int_4;
}

void TestEnergyTypeExtended::test_constructor()
{
  // Test the constructors from setUp
  CPPUNIT_ASSERT_EQUAL(5, test_extended_int_1->get_original_energy());
  CPPUNIT_ASSERT_EQUAL(3, test_extended_int_2->get_original_energy());
  CPPUNIT_ASSERT_EQUAL(4, test_extended_int_3->get_original_energy());
  CPPUNIT_ASSERT_EQUAL(2, test_extended_int_4->get_original_energy());
  CPPUNIT_ASSERT_EQUAL(0, test_extended_int_1->get_in_ground_state());
  CPPUNIT_ASSERT_EQUAL(0, test_extended_int_2->get_in_ground_state());
  CPPUNIT_ASSERT_EQUAL(1, test_extended_int_3->get_in_ground_state());
  CPPUNIT_ASSERT_EQUAL(-1, test_extended_int_4->get_in_ground_state());
  
  // Test the copy constructor
  ExtendedInt test_extended_int_1_copied(*test_extended_int_1);
  ExtendedInt test_extended_int_2_copied(*test_extended_int_2);
  ExtendedInt test_extended_int_3_copied(*test_extended_int_3);
  ExtendedInt test_extended_int_4_copied(*test_extended_int_4);
  CPPUNIT_ASSERT_EQUAL(test_extended_int_1->get_original_energy(),
		       test_extended_int_1_copied.get_original_energy());
  CPPUNIT_ASSERT_EQUAL(test_extended_int_2->get_original_energy(),
		       test_extended_int_2_copied.get_original_energy());
  CPPUNIT_ASSERT_EQUAL(test_extended_int_3->get_original_energy(),
		       test_extended_int_3_copied.get_original_energy());
  CPPUNIT_ASSERT_EQUAL(test_extended_int_4->get_original_energy(),
		       test_extended_int_4_copied.get_original_energy());
  CPPUNIT_ASSERT_EQUAL(test_extended_int_1->get_in_ground_state(),
		       test_extended_int_1_copied.get_in_ground_state());
  CPPUNIT_ASSERT_EQUAL(test_extended_int_2->get_in_ground_state(),
		       test_extended_int_2_copied.get_in_ground_state());
  CPPUNIT_ASSERT_EQUAL(test_extended_int_3->get_in_ground_state(),
		       test_extended_int_3_copied.get_in_ground_state());
  CPPUNIT_ASSERT_EQUAL(test_extended_int_4->get_in_ground_state(),
		       test_extended_int_4_copied.get_in_ground_state());
}

void TestEnergyTypeExtended::test_operator_compare()
{
  // Copy to compare
  ExtendedInt test_extended_int_1_copied(*test_extended_int_1);
  ExtendedInt test_extended_int_2_copied(*test_extended_int_2);
  ExtendedInt test_extended_int_3_copied(*test_extended_int_3);
  ExtendedInt test_extended_int_4_copied(*test_extended_int_4);
  
  // Test that originals and copies are equal
  CPPUNIT_ASSERT(test_extended_int_1_copied == *test_extended_int_1);
  CPPUNIT_ASSERT(test_extended_int_2_copied == *test_extended_int_2);
  CPPUNIT_ASSERT(test_extended_int_3_copied == *test_extended_int_3);
  CPPUNIT_ASSERT(test_extended_int_4_copied == *test_extended_int_4);

  // Test inequality
  CPPUNIT_ASSERT(*test_extended_int_1 != *test_extended_int_2);
  CPPUNIT_ASSERT(*test_extended_int_1 != *test_extended_int_3);
  CPPUNIT_ASSERT(*test_extended_int_1 != *test_extended_int_4);
  CPPUNIT_ASSERT(*test_extended_int_2 != *test_extended_int_3);
  CPPUNIT_ASSERT(*test_extended_int_2 != *test_extended_int_4);
  CPPUNIT_ASSERT(*test_extended_int_3 != *test_extended_int_4);

  // Test the comparison operator
  CPPUNIT_ASSERT(*test_extended_int_4 < *test_extended_int_2);
  CPPUNIT_ASSERT(*test_extended_int_2 < *test_extended_int_3);
  CPPUNIT_ASSERT(*test_extended_int_3 < *test_extended_int_1);

  // Create a new extended energy that differs only in the ground state properties
  ExtendedInt test_extended_int_5(5, 1);
  CPPUNIT_ASSERT(*test_extended_int_4 < test_extended_int_5);
}
void TestEnergyTypeExtended::test_operator_add()
{
  // Test the member addition
  (*test_extended_int_1) += (*test_extended_int_2);
  CPPUNIT_ASSERT(*test_extended_int_1 == ExtendedInt(8,0));

  (*test_extended_int_1) += (*test_extended_int_3);
  CPPUNIT_ASSERT(*test_extended_int_1 == ExtendedInt(12,1));

  (*test_extended_int_1) += (*test_extended_int_4);
  CPPUNIT_ASSERT(*test_extended_int_1 == ExtendedInt(14,0));

  // Test the binary addition
  CPPUNIT_ASSERT(*test_extended_int_2 + *test_extended_int_3 == ExtendedInt(7,1));
}
void TestEnergyTypeExtended::test_operator_substract()
{
  // Test the member substraction
  (*test_extended_int_1) -= (*test_extended_int_2);
  CPPUNIT_ASSERT(*test_extended_int_1 == ExtendedInt(2,0));

  (*test_extended_int_1) -= (*test_extended_int_3);
  CPPUNIT_ASSERT(*test_extended_int_1 == ExtendedInt(-2,-1));

  (*test_extended_int_1) -= (*test_extended_int_4);
  CPPUNIT_ASSERT(*test_extended_int_1 == ExtendedInt(-4,0));

  // Test the binary substraction
  CPPUNIT_ASSERT(*test_extended_int_2 - *test_extended_int_3 == ExtendedInt(-1,-1));
}
