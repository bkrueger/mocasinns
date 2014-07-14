#ifndef TEST_SERIAL_TEMPERING_HPP
#define TEST_SERIAL_TEMPERING_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>

#include <gespinst/spin_lattice.hpp>
#include <gespinst/spins/ising_spin.hpp>

#include <mocasinns/serial_tempering.hpp>
#include <mocasinns/random/boost_random.hpp>

using namespace Mocasinns;

class TestSerialTempering : CppUnit::TestFixture
{
  typedef Gespinst::SpinLattice<2, Gespinst::IsingSpin> ConfigurationType;
  typedef Gespinst::SpinLatticeStep<2, Gespinst::IsingSpin> StepType;
  typedef SerialTempering<ConfigurationType, StepType, Random::Boost_MT19937> SimulationType;

private:
  std::vector<ConfigurationType*> test_config_space_vector;
  SimulationType* test_simulation;
  SimulationType::Parameters test_parameters;
  std::vector<double> inverse_temperatures;

  class ObserveIsingEnergy;
  class ObserveIsingEnergyMagnetization;

public:
  static CppUnit::Test* suite();
  
  void setUp();
  void tearDown();

  void test_do_serial_tempering_steps();
  void test_do_serial_tempering_simulation();
};

#endif
