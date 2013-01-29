#ifdef STEP_HPP

#include "spinlattice.hpp"

namespace Ising
{

/*!
 * \details Contructs a new Step object that belongs to the given lattice at the given index. The flip will be performed to the new spin.
 * \param lattice SpinLattice on which the step will be executed.
 * \param flip_index Index of the lattice where the flip will be done.
 * \param new_spin New spin value at the given lattice index.
 */
template<unsigned int dimension, class SpinType>
Step<dimension, SpinType>::Step(SpinLattice<dimension, SpinType>* lattice, index_type flip_index, SpinType new_spin)
  : _lattice(lattice),
    _flip_index(flip_index),
    _new_spin(new_spin)
{
  // Extract the old spin
  _old_spin = _lattice->get_spin(flip_index);
  
  // Extract the creation simulation time
  _creation_simulation_time = _lattice->get_simulation_time();
  
}

template<unsigned int dimension, class SpinType>
double Step<dimension, SpinType>::delta_E()
{
  std::vector<SpinType> neighbours = _lattice->next_neighbours(_flip_index);

  double old_energy = 0;
  double new_energy = 0;

  for (typename std::vector<SpinType>::iterator neighbour = neighbours.begin();
       neighbour != neighbours.end(); ++neighbour)
  {
    old_energy += - (_old_spin * (*neighbour));
    new_energy += - (_new_spin * (*neighbour));
  }

  return new_energy - old_energy;
}

/*!
 * \details Executes a step by commiting it to the corresponding SpinLattice.
 */
template<unsigned int dimension, class SpinType>
void Step<dimension, SpinType>::execute()
{
  _lattice->commit(*this);
}

template<unsigned int dimension, class SpinType>
void Step<dimension, SpinType>::undo()
{
  // Create a new step that reverts this one
  Step<dimension, SpinType> inverse_step(_lattice, _flip_index, _old_spin);

  // Assign the inverse spin the creation_simulation time of this step and add one (The reverse step may only be done one after the original step)
  inverse_step._creation_simulation_time = _creation_simulation_time + 1;

  // Commit the step
  _lattice->commit(inverse_step);
}

} // of namespace Ising

#endif
