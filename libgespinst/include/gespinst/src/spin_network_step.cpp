#ifdef GESPINST_SPIN_NETWORK_STEP_HPP

#include "../spin_network.hpp"

namespace Gespinst
{

  /*!
   * \details Contructs a new SpinNetworkStep object that belongs to the given lattice at the given index. The flip will be performed to the new spin.
   * \param network SpinNetwork on which the step will be executed.
   * \param index Index of the lattice where the flip will be done.
   * \param spin_new New spin value at the given lattice index.
   */
  template<class SpinType, class ContainerType>
  SpinNetworkStep<SpinType, ContainerType>::SpinNetworkStep(SpinNetworkType* step_network, unsigned int index, SpinType spin_new)
    : network(step_network),
      flip_index(index),
      new_spin(spin_new)
  {
    // Extract the old spin
    old_spin = (*network)[flip_index];
  
    // Extract the creation simulation time
    creation_simulation_time = network->get_simulation_time();
  }

  template<class SpinType, class ContainerType>
  double SpinNetworkStep<SpinType, ContainerType>::delta_E()
  {
    const ContainerType& neighbours = network->get_neighbours(flip_index);

    double old_energy = 0;
    double new_energy = 0;

    for (typename ContainerType::const_iterator neighbour = neighbours.begin();
	 neighbour != neighbours.end(); ++neighbour)
      {
	old_energy += - (old_spin * (**neighbour));
	new_energy += - (new_spin * (**neighbour));
      }
    
    return new_energy - old_energy;
  }

  /*!
   * \details Executes a step by commiting it to the corresponding SpinLattice.
   */
  template<class SpinType, class ContainerType>
  void SpinNetworkStep<SpinType, ContainerType>::execute()
  {
    network->commit(*this);
  }

  template<class SpinType, class ContainerType>
  void SpinNetworkStep<SpinType, ContainerType>::undo()
  {
    // Create a new step that reverts this one
    SpinNetworkStep<SpinType, ContainerType> inverse_step(network, flip_index, old_spin);
    
    // Assign the inverse spin the creation_simulation time of this step and add one (The reverse step may only be done one after the original step)
    inverse_step.creation_simulation_time = creation_simulation_time + 1;
    
    // Commit the step
    network->commit(inverse_step);
  }
  
} // of namespace Ising

#endif
