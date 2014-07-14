#ifndef TEST_PARALLEL_TEMPERING_INVERSE_TEMPERATURE_OPTIMIZATION_HPP
#define TEST_PARALLEL_TEMPERING_INVERSE_TEMPERATURE_OPTIMIZATION_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <gespinst/spin_lattice.hpp>
#include <gespinst/spins/ising_spin.hpp>

#include <mocasinns/parallel_tempering.hpp>
#include <mocasinns/random/boost_random.hpp>
#include <mocasinns/details/parallel_tempering/inverse_temperature_optimization_equal_acceptance_probabilities.hpp>

using namespace Mocasinns;
using namespace Mocasinns::Details::ParallelTempering;

class TestInverseTemperatureOptimization : CppUnit::TestFixture
{
  typedef Gespinst::SpinLattice<2, Gespinst::IsingSpin> ConfigurationType;
  typedef Gespinst::SpinLatticeStep<2, Gespinst::IsingSpin> StepType;
  typedef ParallelTempering<ConfigurationType, StepType, Random::Boost_MT19937> SimulationType;
  typedef InverseTemperatureOptimizationEqualAcceptanceProbabilities<SimulationType, double> InverseTemperatureOptimizationType;
  typedef InverseTemperatureOptimizationEqualAcceptanceProbabilities<SimulationType, double, WeightWorstAcceptance> InverseTemperatureOptimizationType_WorstAcceptance;
  typedef InverseTemperatureOptimizationEqualAcceptanceProbabilities<SimulationType, double, WeightIndependentAcceptance> InverseTemperatureOptimizationType_IndependentAcceptance;

private:
  std::vector<ConfigurationType*> test_config_space_vector;
  SimulationType* test_simulation;
  SimulationType* measurement_simulation;
  SimulationType::Parameters test_parameters;
  SimulationType::Parameters measurement_parameters;
  std::vector<double> inverse_temperatures;
  std::vector<double> inverse_temperatures_negative;
  
public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_optimize_equal_acceptance_probabilities();
  void test_optimize_equal_acceptance_probabilities_worst_acceptance();
  void test_optimize_equal_acceptance_probabilities_independent_acceptance();
};

#endif
