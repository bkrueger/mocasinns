#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>

#include "test_boost_mt19937.hpp"

int main()
{

  CppUnit::TextUi::TestRunner runner;
  runner.addTest(TestBoostMT19937::suite());

  CppUnit::BriefTestProgressListener listener;
  runner.eventManager().addListener(&listener);

  runner.run();

  return 0;
}
