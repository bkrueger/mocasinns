#include "test_step_type_extended.hpp"

#include <mocasinns/random/boost_random.hpp>

CppUnit::Test* TestStepTypeExtended::suite()
{
  CppUnit::TestSuite *suite_of_tests = new CppUnit::TestSuite("TestDetails/TestMulticanonical/TestStepTypeExtended");

  suite_of_tests->addTest( new CppUnit::TestCaller<TestStepTypeExtended>("TestDetails/TestMulticanonical/TestStepTypeExtended: test_constructor", &TestStepTypeExtended::test_constructor) );

  suite_of_tests->addTest( new CppUnit::TestCaller<TestStepTypeExtended>("TestDetails/TestMulticanonical/TestStepTypeExtended: test_delta_E", &TestStepTypeExtended::test_delta_E) );  
  suite_of_tests->addTest( new CppUnit::TestCaller<TestStepTypeExtended>("TestDetails/TestMulticanonical/TestStepTypeExtended: test_delta_E_automatically", &TestStepTypeExtended::test_delta_E_automatically) );
  suite_of_tests->addTest( new CppUnit::TestCaller<TestStepTypeExtended>("TestDetails/TestMulticanonical/TestStepTypeExtended: test_execute", &TestStepTypeExtended::test_execute) ); 

  return suite_of_tests;
}

void TestStepTypeExtended::setUp()
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
  test_config_3 = new ConfigurationType(size_1d);
  test_config_4 = new ConfigurationType(size_1d);
  test_reference_1 = new ConfigurationType(size_1d);
  test_reference_2 = new ConfigurationType(size_1d);
  test_reference_3 = new ConfigurationType(size_1d);
  test_reference_4 = new ConfigurationType(size_1d);
  // Assign values to the configurations
  test_config_1->set_spin(index_0, Gespinst::IsingSpin(1));
  test_config_1->set_spin(index_1, Gespinst::IsingSpin(-1));
  test_config_1->set_spin(index_2, Gespinst::IsingSpin(1));
  test_config_1->set_spin(index_3, Gespinst::IsingSpin(-1));
  test_reference_1->set_spin(index_0, Gespinst::IsingSpin(1));
  test_reference_1->set_spin(index_1, Gespinst::IsingSpin(-1));
  test_reference_1->set_spin(index_2, Gespinst::IsingSpin(1));
  test_reference_1->set_spin(index_3, Gespinst::IsingSpin(-1));
  test_config_2->set_spin(index_0, Gespinst::IsingSpin(1));
  test_config_2->set_spin(index_1, Gespinst::IsingSpin(-1));
  test_config_2->set_spin(index_2, Gespinst::IsingSpin(1));
  test_config_2->set_spin(index_3, Gespinst::IsingSpin(-1));
  test_reference_2->set_spin(index_0, Gespinst::IsingSpin(1));
  test_reference_2->set_spin(index_1, Gespinst::IsingSpin(-1));
  test_reference_2->set_spin(index_2, Gespinst::IsingSpin(1));
  test_reference_2->set_spin(index_3, Gespinst::IsingSpin(-1));
  test_config_3->set_spin(index_0, Gespinst::IsingSpin(1));
  test_config_3->set_spin(index_1, Gespinst::IsingSpin(1));
  test_config_3->set_spin(index_2, Gespinst::IsingSpin(-1));
  test_config_3->set_spin(index_3, Gespinst::IsingSpin(-1));
  test_reference_3->set_spin(index_0, Gespinst::IsingSpin(1));
  test_reference_3->set_spin(index_1, Gespinst::IsingSpin(1));
  test_reference_3->set_spin(index_2, Gespinst::IsingSpin(-1));
  test_reference_3->set_spin(index_3, Gespinst::IsingSpin(1));
  test_config_4->set_spin(index_0, Gespinst::IsingSpin(1));
  test_config_4->set_spin(index_1, Gespinst::IsingSpin(1));
  test_config_4->set_spin(index_2, Gespinst::IsingSpin(-1));
  test_config_4->set_spin(index_3, Gespinst::IsingSpin(-1));
  test_reference_4->set_spin(index_0, Gespinst::IsingSpin(1));
  test_reference_4->set_spin(index_1, Gespinst::IsingSpin(1));
  test_reference_4->set_spin(index_2, Gespinst::IsingSpin(-1));
  test_reference_4->set_spin(index_3, Gespinst::IsingSpin(1));

  // Create the extended configs
  test_extended_config_1 = new ExtendedConfigType(test_config_1);
  test_extended_config_2 = new ExtendedConfigType(test_config_2);
  test_extended_config_3 = new ExtendedConfigType(test_config_3, test_reference_3);
  test_extended_config_4 = new ExtendedConfigType(test_config_4, test_reference_4);

  // Create some normal steps
  test_step_1 = new StepType(test_config_1, index_1, Gespinst::IsingSpin(1)); // Leaving ground state
  test_step_2 = new StepType(test_config_2, index_1, Gespinst::IsingSpin(-1)); // Not leaving ground state
  test_step_3 = new StepType(test_config_3, index_1, Gespinst::IsingSpin(-1)); // Beeing not at ground state and not going to ground state
  test_step_4 = new StepType(test_config_4, index_3, Gespinst::IsingSpin(1)); // Going to the ground state

  // Create the extended steps
  test_extended_step_1 = new ExtendedStepType(*test_step_1, test_extended_config_1);
  test_extended_step_2 = new ExtendedStepType(*test_step_2, test_extended_config_2);
  test_extended_step_3 = new ExtendedStepType(*test_step_3, test_extended_config_3);
  test_extended_step_4 = new ExtendedStepType(*test_step_4, test_extended_config_4);
}
void TestStepTypeExtended::tearDown()
{
  // Delete the steps first otherwise they cannot be undone
  delete test_extended_step_1;
  delete test_extended_step_2;
  delete test_extended_step_3;
  delete test_extended_step_4;

  delete test_step_1;
  delete test_step_2;
  delete test_step_3;
  delete test_step_4;

  delete test_extended_config_1;
  delete test_extended_config_2;
  delete test_extended_config_3;
  delete test_extended_config_4;

  delete test_config_1;
  delete test_config_2;
  delete test_config_3;
  delete test_config_4;
  delete test_reference_1;
  delete test_reference_2;
  delete test_reference_3;
  delete test_reference_4;
}

void TestStepTypeExtended::test_constructor()
{
  
}

void TestStepTypeExtended::test_delta_E()
{
  // Step leaving groundstate
  EnergyTypeExtended<int> delta_E_1(-4, -1);
  CPPUNIT_ASSERT_EQUAL(delta_E_1.get_original_energy(),
		       test_extended_step_1->delta_E().get_original_energy());
  CPPUNIT_ASSERT_EQUAL(delta_E_1.get_in_ground_state(),
		       test_extended_step_1->delta_E().get_in_ground_state());
  CPPUNIT_ASSERT(test_extended_step_1->delta_E() == delta_E_1);
  // Step not leaving groundstate
  EnergyTypeExtended<int> delta_E_2(0, 0);
  CPPUNIT_ASSERT_EQUAL(delta_E_2.get_original_energy(),
		       test_extended_step_2->delta_E().get_original_energy());
  CPPUNIT_ASSERT_EQUAL(delta_E_2.get_in_ground_state(),
		       test_extended_step_2->delta_E().get_in_ground_state());
  CPPUNIT_ASSERT(test_extended_step_2->delta_E() == delta_E_2);
  // Step doing nothing with ground state
  EnergyTypeExtended<int> delta_E_3(0, 0);
  CPPUNIT_ASSERT_EQUAL(delta_E_3.get_original_energy(),
		       test_extended_step_3->delta_E().get_original_energy());
  CPPUNIT_ASSERT_EQUAL(delta_E_3.get_in_ground_state(),
		       test_extended_step_3->delta_E().get_in_ground_state());
  CPPUNIT_ASSERT(test_extended_step_3->delta_E() == delta_E_3);
  // Step entering the ground state
  EnergyTypeExtended<int> delta_E_4(0, 1);
  CPPUNIT_ASSERT_EQUAL(delta_E_4.get_original_energy(),
		       test_extended_step_4->delta_E().get_original_energy());
  CPPUNIT_ASSERT_EQUAL(delta_E_4.get_in_ground_state(),
		       test_extended_step_4->delta_E().get_in_ground_state());
  CPPUNIT_ASSERT(test_extended_step_4->delta_E() == delta_E_4);
}
void TestStepTypeExtended::test_delta_E_automatically()
{
  Mocasinns::Random::Boost_MT19937 rng;

  // Do automatic tests with test_extended_config_1
  EnergyTypeExtended<int> energy_1 = test_extended_config_1->energy();
  for (unsigned int i = 0; i < 1000; i++)
  {
    // Propose a random step
    ExtendedStepType step = test_extended_config_1->propose_step(&rng);
    EnergyTypeExtended<int> energy_difference = step.delta_E();
    step.execute();

    // Test that the energies match
    CPPUNIT_ASSERT(energy_1 + energy_difference == test_extended_config_1->energy());
    energy_1 += energy_difference;
  }

  // Do automatic tests with test_extended_config_2
  EnergyTypeExtended<int> energy_2 = test_extended_config_2->energy();
  for (unsigned int i = 0; i < 1000; i++)
  {
    // Propose a random step
    ExtendedStepType step = test_extended_config_2->propose_step(&rng);
    EnergyTypeExtended<int> energy_difference = step.delta_E();
    step.execute();

    // Test that the energies match
    CPPUNIT_ASSERT(energy_2 + energy_difference == test_extended_config_2->energy());
    energy_2 += energy_difference;
  }

  // Do automatic tests with test_extended_config_3
  EnergyTypeExtended<int> energy_3 = test_extended_config_3->energy();
  for (unsigned int i = 0; i < 1000; i++)
  {
    // Propose a random step
    ExtendedStepType step = test_extended_config_3->propose_step(&rng);
    EnergyTypeExtended<int> energy_difference = step.delta_E();
    step.execute();

    // Test that the energies match
    CPPUNIT_ASSERT(energy_3 + energy_difference == test_extended_config_3->energy());
    energy_3 += energy_difference;
  }

  // Do automatic tests with test_extended_config_4
  EnergyTypeExtended<int> energy_4 = test_extended_config_4->energy();
  for (unsigned int i = 0; i < 1000; i++)
  {
    // Propose a random step
    ExtendedStepType step = test_extended_config_4->propose_step(&rng);
    EnergyTypeExtended<int> energy_difference = step.delta_E();
    step.execute();

    // Test that the energies match
    CPPUNIT_ASSERT(energy_4 + energy_difference == test_extended_config_4->energy());
    energy_4 += energy_difference;
  }
}
void TestStepTypeExtended::test_execute()
{
}


