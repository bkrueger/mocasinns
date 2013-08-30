#include "test_configuration_type_extended.hpp"

#include <mocasinns/random/boost_random.hpp>

CppUnit::Test* TestConfigurationTypeExtended::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestDetails/TestMulticanonical/TestConfigurationTypeExtended");

  suite_of_tests->addTest( new CppUnit::TestCaller<TestConfigurationTypeExtended>("TestDetails/TestMulticanonical/TestConfigurationTypeExtended: test_constructor", &TestConfigurationTypeExtended::test_constructor) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestConfigurationTypeExtended>("TestDetails/TestMulticanonical/TestConfigurationTypeExtended: test_constructor", &TestConfigurationTypeExtended::test_constructor) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestConfigurationTypeExtended>("TestDetails/TestMulticanonical/TestConfigurationTypeExtended: test_get_reference_configuration", &TestConfigurationTypeExtended::test_get_reference_configuration) );    
  suite_of_tests->addTest( new CppUnit::TestCaller<TestConfigurationTypeExtended>("TestDetails/TestMulticanonical/TestConfigurationTypeExtended: test_get_reference_configuration_energy", &TestConfigurationTypeExtended::test_get_reference_configuration_energy) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestConfigurationTypeExtended>("TestDetails/TestMulticanonical/TestConfigurationTypeExtended: test_get_is_reference_configuration", &TestConfigurationTypeExtended::test_get_is_reference_configuration) );        

  suite_of_tests->addTest( new CppUnit::TestCaller<TestConfigurationTypeExtended>("TestDetails/TestMulticanonical/TestConfigurationTypeExtended: test_energy", &TestConfigurationTypeExtended::test_energy) );  
  suite_of_tests->addTest( new CppUnit::TestCaller<TestConfigurationTypeExtended>("TestDetails/TestMulticanonical/TestConfigurationTypeExtended: test_propose_step", &TestConfigurationTypeExtended::test_propose_step) ); 

  return suite_of_tests;
}

void TestConfigurationTypeExtended::setUp()
{
  // Set the size vector
  std::vector<unsigned int> size_1d;
  size_1d.push_back(4);
  // Create the indices
  index_type_1d index_0, index_1, index_2, index_3;
  index_0[0] = 0; index_1[0] = 1; index_2[0] = 2; index_3[0] = 3;

  // Create some normal configurations
  test_config_1 = new ConfigurationType(size_1d);
  test_config_2 = new ConfigurationType(size_1d);
  test_reference_1 = new ConfigurationType(size_1d);
  test_reference_2 = new ConfigurationType(size_1d);
  // Assign values to the configurations
  test_config_1->set_spin(index_0, Gespinst::IsingSpin(1));
  test_config_1->set_spin(index_1, Gespinst::IsingSpin(-1));
  test_config_1->set_spin(index_2, Gespinst::IsingSpin(1));
  test_config_1->set_spin(index_3, Gespinst::IsingSpin(-1));
  test_config_2->set_spin(index_0, Gespinst::IsingSpin(1));
  test_config_2->set_spin(index_1, Gespinst::IsingSpin(1));
  test_config_2->set_spin(index_2, Gespinst::IsingSpin(-1));
  test_config_2->set_spin(index_3, Gespinst::IsingSpin(-1));
  test_reference_1->set_spin(index_0, Gespinst::IsingSpin(1));
  test_reference_1->set_spin(index_1, Gespinst::IsingSpin(-1));
  test_reference_1->set_spin(index_2, Gespinst::IsingSpin(1));
  test_reference_1->set_spin(index_3, Gespinst::IsingSpin(-1));
  test_reference_2->set_spin(index_0, Gespinst::IsingSpin(1));
  test_reference_2->set_spin(index_1, Gespinst::IsingSpin(1));
  test_reference_2->set_spin(index_2, Gespinst::IsingSpin(-1));
  test_reference_2->set_spin(index_3, Gespinst::IsingSpin(1));

  // Create the extended configs
  test_extended_config_1 = new ExtendedConfigType(test_config_1);
  test_extended_config_2 = new ExtendedConfigType(test_config_2, test_reference_2);
}
void TestConfigurationTypeExtended::tearDown()
{
  delete test_config_1;
  delete test_config_2;
  delete test_reference_1;
  delete test_reference_2;

  delete test_extended_config_1;
  delete test_extended_config_2;
}

void TestConfigurationTypeExtended::test_constructor()
{
  
}

void TestConfigurationTypeExtended::test_get_reference_configuration()
{
  CPPUNIT_ASSERT(*(test_extended_config_1->get_reference_configuration()) ==
		 *test_reference_1);
  CPPUNIT_ASSERT(*(test_extended_config_2->get_reference_configuration()) ==
		 *test_reference_2);
}
void TestConfigurationTypeExtended::test_get_reference_configuration_energy()
{
  CPPUNIT_ASSERT_EQUAL(4, test_extended_config_1->get_reference_configuration_energy());
  CPPUNIT_ASSERT_EQUAL(0, test_extended_config_2->get_reference_configuration_energy());
}
void TestConfigurationTypeExtended::test_get_is_reference_configuration()
{
  CPPUNIT_ASSERT(test_extended_config_1->get_is_reference_configuration());
  CPPUNIT_ASSERT(!test_extended_config_2->get_is_reference_configuration());
}

void TestConfigurationTypeExtended::test_energy()
{
  EnergyTypeExtended<int> energy_1(4, 1);
  CPPUNIT_ASSERT(test_extended_config_1->energy() == energy_1);
  EnergyTypeExtended<int> energy_2(0, 0);
  CPPUNIT_ASSERT(test_extended_config_2->energy() == energy_2);
}
void TestConfigurationTypeExtended::test_propose_step()
{
  // Test one proposed step
  Mocasinns::Random::Boost_MT19937 rng;
  ExtendedStepType step = test_extended_config_1->propose_step(&rng);
}


