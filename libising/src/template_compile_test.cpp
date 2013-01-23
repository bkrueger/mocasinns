// This file is for testing the compilation of the template files

#include "step.hpp"
#include "spinlattice.hpp"
#include "spin_ising.hpp"

void test_spinlattice()
{
  std::vector<unsigned int> dim(1,5);
  SpinLattice<1, SpinIsing> test_spinlattice(dim);

  std::vector<unsigned int> dim2(2,3);
  SpinLattice<2, SpinIsing> test_spinlattice_2d(dim2);
  SpinLattice<2, SpinIsing> test_spinlattice_2d_default(dim2, SpinIsing(1));

  double mag = test_spinlattice_2d_default.magnetization();

  boost::array<boost::multi_array<SpinIsing, 1>::index, 1> index;
  index[0] = 3;
  Step<1, SpinIsing> step(&test_spinlattice, index, SpinIsing(-1));
   
}
