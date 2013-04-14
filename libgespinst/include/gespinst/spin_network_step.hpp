#ifndef GESPINST_SPIN_NETWORK_STEP_HPP
#define GESPINST_SPIN_NETWORK_STEP_HPP

#include <vector>
#include <array>

namespace Gespinst
{
  // Forward declaration of the spin network class
  template<class SpinType, class ContainerType>
  class SpinNetwork;

  template<class SpinType, class ContainerType>
  class SpinNetworkStep
  {
  public:
    typedef SpinNetwork<SpinType, ContainerType> SpinNetworkType;
    typedef typename SpinNetwork<SpinType, ContainerType>::index_type index_type;

    //! Creates a new step
    SpinNetworkStep(SpinNetworkType* step_network, unsigned int index, SpinType spin_new);

    //! Get-Accessor for the lattice
    SpinNetworkType* get_lattice() const { return network; }
    //! Get-accessor for the index
    index_type get_flip_index() const { return flip_index; }
    //! Get-accessor for the old spin value
    const SpinType& get_old_spin() const { return old_spin; }
    //! Get-accessor for the new spin value
    const SpinType& get_new_spin() const { return new_spin; }
    //! Get-accessor for the creation simulation time of the step
    int get_creation_simulation_time() const { return creation_simulation_time;}

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

  private:
    //! Pointer to the lattice of the step
    SpinNetworkType* network;
    //! Index of the spin to flip of the network
    unsigned int flip_index;
    //! Creation simulation time of the flip
    int creation_simulation_time;
    
    //! Old Spin value
    SpinType old_spin;
    //! New Spin value
    SpinType new_spin;
  };
  
} // of namespace Ising

#include "src/spin_network_step.cpp"

#endif
