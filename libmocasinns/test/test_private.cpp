// STL
#include <string>

// Headers for CPPUnit
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>

// Header for all the test classes
#include "test_details/test_multicanonical/test_energy_type_extended.hpp"
#include "test_details/test_multicanonical/test_configuration_type_extended.hpp"
#include "test_details/test_multicanonical/test_step_type_extended.hpp"

int main()
{
  CppUnit::TextUi::TestRunner runner;
  runner.addTest(TestEnergyTypeExtended::suite());
  runner.addTest(TestConfigurationTypeExtended::suite());
  runner.addTest(TestStepTypeExtended::suite());
  
  CppUnit::BriefTestProgressListener listener;
  runner.eventManager().addListener(&listener);

  runner.run();

  return 0;
}
