#ifdef GESPINST_SPIN_NETWORK_HPP

#include <cmath>

namespace Gespinst
{
  template<class SpinType, class ContainerType>
  SpinNetwork<SpinType, ContainerType>::SpinNetwork() : simulation_time(0) {}

  template<class SpinType, class ContainerType>
  SpinNetwork<SpinType, ContainerType>::SpinNetwork(unsigned int spin_number, SpinType default_spin) 
    : spins(spin_number, default_spin), next_neighbour_spins(spin_number), simulation_time(0) {}

  template<class SpinType, class ContainerType>
  bool SpinNetwork<SpinType, ContainerType>::operator==(const SpinNetwork<SpinType, ContainerType>& other) const
  {
    // Test the vectors for equal size
    if (spins.size() != other.spins.size()) return false;
    
    // Test that the spin values are equal
    for (typename spin_container::const_iterator spin = spins.begin(), other_spin = other.spins.begin(); spin != spins.end(); ++spin, ++other_spin)
      if ((*spin) != (*other_spin)) return false;

    // Test that the structure of the next neighbours does match
    for (typename neighbour_container_list::const_iterator spin_neighbours = next_neighbour_spins.begin(), other_spin_neighbour = other.next_neighbour_spins.begin();
	 spin_neighbours != next_neighbour_spins.end(); ++spin_neighbours, ++other_spin_neighbour)
    {
      // Test for the size
      if (spin_neighbours->size() != other_spin_neighbour->size()) return false;

      // Check that every spin has the same neighbours
      for (typename ContainerType::const_iterator neighbour = spin_neighbours->begin(), other_neighbour = other_spin_neighbour->begin();
	   neighbour != spin_neighbours->end(); ++neighbour, ++other_neighbour)
      {
	if ((**neighbour) != (**other_neighbour)) return false;
      }
    }

    // If all checks were positive, return true
    return true;
  }
  template<class SpinType, class ContainerType>
  bool SpinNetwork<SpinType, ContainerType>::operator!=(const SpinNetwork<SpinType, ContainerType>& other) const
  {
    return !(operator==(other));
  }

  /*! 
   * \details Creates a STL vector of Step objects representing all steps that can be done in the lattice. For each lattice site there are the flips from the present value of the spin to all other values of the spin.
   */
  template<class SpinType, class ContainerType>
  std::vector<SpinNetworkStep<SpinType, ContainerType> > SpinNetwork<SpinType, ContainerType>::all_steps()
  {
    // Define the results
    std::vector<SpinNetworkStep<SpinType, ContainerType> > result;

    // Go through every index and add all possible steps
    for (unsigned int s = 0; s < spins.size(); ++s)
    {
      SpinType spin = spins[s];

      // Get the vector of all differing spins
      std::vector<SpinType> possible_values = spin.all_possible_values();
      for (typename std::vector<SpinType>::const_iterator value = possible_values.begin(); value != possible_values.end(); ++value)
      {
	if (*value != spin)
	  result.push_back(SpinNetworkStep<SpinType, ContainerType>(this, s, *value));
      }
    }
    
    return result;
  }
  
  /*!
   * \details Updates the spin lattice with the given spin. After the update the simulation time is increase by one. If the creation simulation time of the step does not match the actual simulation time of the spin lattice, an exception will be thrown (to be implemented).
   * \param step_to_commit A valid step that should update the spin lattice.
   */
  template<class SpinType, class ContainerType>
  void SpinNetwork<SpinType, ContainerType>::commit(SpinNetworkStep<SpinType, ContainerType>& step_to_commit)
  {
    spins[step_to_commit.get_flip_index()] = step_to_commit.get_new_spin();
    simulation_time++;
  }

  /*! 
   * \details Calculates the energy of the lattice by summing over all next-neighbour pairs and returning the negative value of the product of the pairs.
   */
  template<class SpinType, class ContainerType>
  double SpinNetwork<SpinType, ContainerType>::energy() const
  {
    double result = 0;

    typename spin_container::const_iterator spin = spins.begin();
    typename neighbour_container_list::const_iterator neighbours = next_neighbour_spins.begin();
    for (; spin != spins.end(); ++spin, ++neighbours)
    {
      for (typename ContainerType::const_iterator neighbour = neighbours->begin();
	   neighbour != neighbours->end(); ++neighbour)
      {
	result += (*spin)*(**neighbour);
      }
    }

    // Factor of 0.5 because energy is defined over all pairs of next neighbours, we count every pair twice
    return 0.5*result;
  }

  /*! 
   * \details Calculates the magnetization of the lattice by summing over all spins and returning the sum of their values
   */
  template<class SpinType, class ContainerType>
  double SpinNetwork<SpinType, ContainerType>::magnetization() const
  {
    double result = 0;
    
    for (typename spin_container::const_iterator spin = spins.begin(); spin != spins.end(); ++spin)
      result += spin->get_value();
    
    return result;
  }

  /*!
   * \details Proposes a step based on a given random number between 0 and 1. The integer part of (random_number * number_of_sites) will be used for determining the index where the step or the spin flip should occur, the non-integer part will be used for determining to which new spin the old spin will be flipped.
   * \param rng Random number generator so that rng() gives a random number between 0 and 1
   */
  template<class SpinType, class ContainerType>
  template<class RandomNumberGenerator>
  SpinNetworkStep<SpinType, ContainerType> SpinNetwork<SpinType, ContainerType>::propose_step(RandomNumberGenerator* rng)
  {
    // Use a random index
    unsigned int index = rng->random_int32(0, spins.size() - 1);
    
    // Create a step and return the step
    return SpinNetworkStep<SpinType, ContainerType>(this, // Pointer to the lattice of the step
						    index, // Index of the place to flip
						    spins[index].random_differ(rng->random_double())); // new random spin value differing from the old value
  }

  template<class SpinType, class ContainerType>
  unsigned int SpinNetwork<SpinType, ContainerType>::system_size() const
  {
    return spins.size();
  }


} // of namespace Ising

#endif
