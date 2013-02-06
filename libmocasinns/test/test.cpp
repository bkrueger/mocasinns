#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>

#include "test_simulation.hpp"
#include "test_configuration_test.hpp"
#include "test_metropolis.hpp"
#include "test_wang_landau.hpp"
#include "test_binnings.hpp"
#include "test_histobase.hpp"
#include "test_histocrete.hpp"
#include "test_histogram.hpp"
#include "test_histogram_number.hpp"
#include "test_vector_observable.hpp"

int main()
{

  CppUnit::TextUi::TestRunner runner;
  runner.addTest(TestSimulation::suite());
  runner.addTest(TestConfigurationTest::suite());
  runner.addTest(TestMetropolis::suite());
  runner.addTest(TestWangLandau::suite());
  runner.addTest(TestBinningNumber::suite());
  runner.addTest(TestBinningNumberVector::suite());
  runner.addTest(TestHistoBase::suite());
  runner.addTest(TestHistocrete::suite());
  runner.addTest(TestHistogram::suite());
  runner.addTest(TestHistogramNumber::suite());
  runner.addTest(TestVectorObservable::suite());

  CppUnit::BriefTestProgressListener listener;
  runner.eventManager().addListener(&listener);

  runner.run();

  return 0;
}
