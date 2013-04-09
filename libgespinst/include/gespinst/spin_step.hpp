#ifndef GESPINST_SPIN_STEP_HPP
#define GESPINST_SPIN_STEP_HPP

#include <vector>
#include "boost/multi_array.hpp"

namespace Ising
{

template<unsigned int T, class V> class SpinLattice;

template<unsigned int dimension, class SpinType>
class Step
{
  typedef boost::multi_array<SpinType, dimension> lattice_array_type;
  typedef boost::array<typename lattice_array_type::index, dimension> index_type;
  typedef std::vector<int> coordinate_type;

private:
  //! Pointer to the lattice
  SpinLattice<dimension, SpinType>* _lattice;
  //! Index of the spin to flip in the ising lattice
  index_type _flip_index;
  //! Creation simulation time of the flip
  int _creation_simulation_time;

  //! Old Spin value
  SpinType _old_spin;
  //! New Spin value
  SpinType _new_spin;

public:
  //! Creates a new step
  Step(SpinLattice<dimension, SpinType>* lattice, index_type flip_index, SpinType new_spin);

  //! Get-Accessor for the lattice
  SpinLattice<dimension, SpinType>* get_lattice() const { return _lattice; }
  //! Get-accessor for the index
  index_type get_flip_index() const { return _flip_index; }
  //! Get-accessor for the old spin value
  SpinType get_old_spin() const { return _old_spin; }
  //! Get-accessor for the new spin value
  SpinType get_new_spin() const { return _new_spin; }
  //! Get-accessor for the creation simulation time of the step
  int get_creation_simulation_time() const { return _creation_simulation_time;}

  //! Function to calculate the energy difference of the step
  double delta_E();

  //! Specify whether the step is executable, needed for the use of MoCaSins, is allways 1
  bool is_executable() { return true; }

  //! Function to execute the step
  void execute();

  //! Selection probability factor, needed for the use of MoCaSins, allwas 1
  double selection_probability_factor() { return 1.0; }

  //! Revert the step after it has been executed
  void undo();
};

} // of namespace Ising

#include "src/spin_step.cpp"

#endif
