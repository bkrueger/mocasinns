#ifndef SPINLATTICETEST_HPP
#define SPINLATTICETEST_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>
#include <gespinst/spin_lattice.hpp>
#include <gespinst/spins/ising_spin.hpp>
#include <gespinst/spins/potts_spin.hpp>
#include <gespinst/spins/real_spin.hpp>

using namespace Gespinst;

typedef boost::multi_array<IsingSpin, 1> lattice_array_type_1d;
typedef boost::array<lattice_array_type_1d::index, 1> index_type_1d;
typedef boost::multi_array<IsingSpin, 1> lattice_array_real_type_1d;
typedef boost::array<lattice_array_type_1d::index, 1> index__real_type_1d;
typedef boost::multi_array<IsingSpin, 2> lattice_array_type_2d;
typedef boost::array<lattice_array_type_2d::index, 2> index_type_2d;

class TestSpinLattice : public CppUnit::TestFixture
{
private:
  SpinLattice<1, IsingSpin>* testlattice_1d;
  SpinLattice<2, IsingSpin>* testlattice_2d;
  SpinLattice<2, PottsSpin>* testlattice_2d_potts;
  SpinLattice<1, RealSpin>* testlattice_1d_real;
  
  index_type_1d index_0, index_1, index_2, index_3, index_4;
  index_type_2d index_00, index_01, index_02, index_10, index_11, index_12, index_20, index_21, index_22;

public:
  static CppUnit::Test* suite();

  void setUp();
  void tearDown();

  void test_constructor();

  void test_get_set_spin();
  void test_get_simulation_time();

  void test_operator_equal();
  void test_operator_equal_automatically();
  void test_operator_access();

  void test_all_steps();
  void test_commit();
  void test_energy();
  void test_magnetization();
  void test_next_neighbours();
  void test_propose_step();
  void test_system_size();

  void test_serialize();
};

#endif
