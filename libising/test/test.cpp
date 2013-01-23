// CppUnit-Tutorial

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>
#include "spinisingtest.hpp"
#include "spinpottstest.hpp"
#include "spinrealtest.hpp"
#include "spinlatticetest.hpp"
#include "steptest.hpp"

int main()
{
  CppUnit::TextUi::TestRunner runner;
  
  runner.addTest(TestSpinIsing::suite());
  runner.addTest(TestSpinPotts::suite());
  runner.addTest(TestSpinReal::suite());
  runner.addTest(TestSpinLattice::suite());
  runner.addTest(TestStep::suite());

  CppUnit::BriefTestProgressListener listener; 
  runner.eventManager().addListener(&listener);

  runner.run();
  
  return 0;
}
