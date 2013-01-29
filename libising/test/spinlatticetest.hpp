#ifndef SPINLATTICETEST_HPP
#define SPINLATTICETEST_HPP

#include <cppunit/TestCaller.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/Test.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../src/spinlattice.hpp"
#include "../src/spin_ising.hpp"
#include "../src/spin_potts.hpp"
#include "../src/spin_real.hpp"

using namespace Ising;

typedef boost::multi_array<SpinIsing, 1> lattice_array_type_1d;
typedef boost::array<lattice_array_type_1d::index, 1> index_type_1d;
typedef boost::multi_array<SpinIsing, 1> lattice_array_real_type_1d;
typedef boost::array<lattice_array_type_1d::index, 1> index__real_type_1d;
typedef boost::multi_array<SpinIsing, 2> lattice_array_type_2d;
typedef boost::array<lattice_array_type_2d::index, 2> index_type_2d;

class TestSpinLattice : public CppUnit::TestFixture
{
private:
  SpinLattice<1, SpinIsing>* testlattice_1d;
  SpinLattice<2, SpinIsing>* testlattice_2d;
  SpinLattice<2, SpinPotts>* testlattice_2d_potts;
  SpinLattice<1, SpinReal>* testlattice_1d_real;
  
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
  void test_operator_access();

  void test_all_steps();
  void test_commit();
  void test_energy();
  void test_magnetization();
  void test_next_neighbours();
  void test_propose_step();

  void test_serialize();
};

#endif
