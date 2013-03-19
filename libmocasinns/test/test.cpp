// STL
#include <string>

// Headers for CPPUnit
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResult.h>

// Boost header for program options
#include <boost/program_options.hpp>

// Header for all the test classes
#include "test_simulation.hpp"
#include "test_configuration_test.hpp"
#include "test_metropolis.hpp"
#include "test_metropolis_parallel.hpp"
#include "test_wang_landau.hpp"
#include "test_optimal_ensemble_sampling.hpp"
#include "test_histograms/test_binnings.hpp"
#include "test_histograms/test_histobase.hpp"
#include "test_histograms/test_histocrete.hpp"
#include "test_histograms/test_histogram.hpp"
#include "test_histograms/test_histogram_number.hpp"
#include "test_observables/test_vector_observable.hpp"
#include "test_observables/test_array_observable.hpp"
#include "test_observables/test_histogram_accumulator.hpp"
#include "test_observables/test_jackknife_analysis.hpp"

bool read_test_name(int argc, char *argv[], std::string& test_name);

int main(int argc, char *argv[])
{
  std::string test_name;
  read_test_name(argc, argv, test_name);
  bool test_all = (test_name == "all");

  CppUnit::TextUi::TestRunner runner;
  if (test_all || test_name == "Simulation")
    runner.addTest(TestSimulation::suite());
  if (test_all || test_name == "ConfigurationTest")
    runner.addTest(TestConfigurationTest::suite());
  if (test_all || test_name == "Metropolis")
    runner.addTest(TestMetropolis::suite());
  if (test_all || test_name == "MetropolisParallel")
    runner.addTest(TestMetropolisParallel::suite());
  if (test_all || test_name == "WangLandau")
    runner.addTest(TestWangLandau::suite());
  if (test_all || test_name == "OptimalEnsembleSampling")
    runner.addTest(TestOptimalEnsembleSampling::suite());
  if (test_all || test_name == "Histograms")
  {
    runner.addTest(TestBinningNumber::suite());
    runner.addTest(TestBinningNumberVector::suite());
    runner.addTest(TestHistoBase::suite());
    runner.addTest(TestHistocrete::suite());
    runner.addTest(TestHistogram::suite());
    runner.addTest(TestHistogramNumber::suite());
  }
  if (test_all || test_name == "Observables")
  {
    runner.addTest(TestVectorObservable::suite());
    runner.addTest(TestArrayObservable::suite());
    runner.addTest(TestHistogramAccumulator::suite());
    runner.addTest(TestJackknifeAnalysis::suite());
  }

  CppUnit::BriefTestProgressListener listener;
  runner.eventManager().addListener(&listener);

  runner.run();

  return 0;
}

bool read_test_name(int argc, char *argv[], std::string& test_name)
{
  // Read the program options
  boost::program_options::variables_map program_options;
  std::string program_name;
  // Read the variables
  try
  {
    boost::program_options::options_description program_options_description("Allowed parameters and options");
    program_options_description.add_options()
      ("help,h", "Produces this manual.")
      ("test,t", boost::program_options::value<std::string>()->default_value("all"), "Specifies which test to perform. The default value is all which executes all present tests.");

    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, program_options_description), program_options);
    boost::program_options::notify(program_options);

    if (program_options.count("help"))
    {
      std::cout << "Usage: test [options]" << std::endl;
      std::cout << program_options_description;
      return false;
    }
  }
  catch (std::exception &except)
  {
    std::cerr << except.what() << std::endl;
    return false;
  }
  // Read option and fill
  test_name = program_options["test"].as<std::string>();

  // If nothing bad happened, return true
  return true;
}
