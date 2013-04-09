// This file is for testing the compilation of the template files

#include "step.hpp"
#include "spinlattice.hpp"
#include "spin_ising.hpp"

namespace Ising
{

void test_spinlattice()
{
  std::vector<unsigned int> dim(1,5);
  SpinLattice<1, IsingSpin> test_spinlattice(dim);

  std::vector<unsigned int> dim2(2,3);
  SpinLattice<2, IsingSpin> test_spinlattice_2d(dim2);
  SpinLattice<2, IsingSpin> test_spinlattice_2d_default(dim2, SpinIsing(1));

  double mag = test_spinlattice_2d_default.magnetization();

  boost::array<boost::multi_array<IsingSpin, 1>::index, 1> index;
  index[0] = 3;
  Step<1, IsingSpin> step(&test_spinlattice, index, SpinIsing(-1));
   
}

} // of namespace Ising
