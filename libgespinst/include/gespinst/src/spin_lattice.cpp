#ifdef GESPINST_SPIN_LATTICE_HPP

#include "../spin_step.hpp"

#include <cmath>

namespace Ising
{

template<unsigned int dimension, class SpinType, class Derived>
typename SpinLatticeBase<dimension, SpinType, Derived>::index_type SpinLatticeBase<dimension, SpinType, Derived>::to_boost_array(std::vector<unsigned int> coordinates)
{
  SpinLatticeBase<dimension, SpinType, Derived>::index_type result;
  for (unsigned int d = 0; d < dimension; d++) result[d] = coordinates[d];
  return result;
}
/*!
 * \details Converts an integer (between 0 and the the number of lattice sites) to a correct index.
 * \param i Integer to convert
 */
template<unsigned int dimension, class SpinType, class Derived>
typename SpinLatticeBase<dimension, SpinType, Derived>::index_type SpinLatticeBase<dimension, SpinType, Derived>::int_to_index(unsigned int i) const
{
  // Calculate the coordinates of the point
  index_type coordinates;
  for (int d = dimension - 1; d >= 0; d--)
  {
    coordinates[d] = i % spin_lattice.shape()[d];
    i = i / spin_lattice.shape()[d];
  }
  return coordinates;
}

/*!
 * \details Construct an empty SpinLatticeBase (for reloading serialized stuff)
 */
template<unsigned int dimension, class SpinType, class Derived>
SpinLatticeBase<dimension, SpinType, Derived>::SpinLatticeBase()
  : _simulation_time(0),
    spin_lattice()
{
  // ToDo: Check lattice_extension size with the dimension
}

/*!
 * \details Constructs a spin lattice with the given extension. The dimension and the spin type used is given as a template parameter. A default spin value is given.
 * \param lattice_extension A STL vector of integers giving the extension of the lattice for each dimension. The size of the vector must match the dimension, the integers mustn't be zero or negative.
 * \param default_spin Default value for all spins
 */
template<unsigned int dimension, class SpinType, class Derived>
SpinLatticeBase<dimension, SpinType, Derived>::SpinLatticeBase(std::vector<unsigned int> lattice_extension, SpinType default_spin)
  : _simulation_time(0),
    spin_lattice(to_boost_array(lattice_extension))
{
  for (SpinType* spin = spin_lattice.data();
       spin != (spin_lattice.data() + spin_lattice.num_elements()); ++spin)
  {
    *spin = default_spin;
  }
}

/*!
 * \details Constructs by copying another SpinLatticeBase with the same dimension and the same SpinType. The dimension and the spin type used is given as a template parameter. 
 * \param other SpinLatticeBase to copy
 */
template<unsigned int dimension, class SpinType, class Derived>
SpinLatticeBase<dimension, SpinType, Derived>::SpinLatticeBase(const SpinLatticeBase<dimension, SpinType, Derived>& other)
  : _simulation_time(0),
    spin_lattice(other.spin_lattice)
{ }

template<unsigned int dimension, class SpinType, class Derived>
SpinType SpinLatticeBase<dimension, SpinType, Derived>::get_spin(index_type coordinates) const
{
  return spin_lattice(coordinates);
}
template<unsigned int dimension, class SpinType, class Derived>
void SpinLatticeBase<dimension, SpinType, Derived>::set_spin(index_type coordinates, SpinType value)
{
  spin_lattice(coordinates) = value;
}

template<unsigned int dimension, class SpinType, class Derived>
SpinLatticeBase<dimension, SpinType, Derived>& SpinLatticeBase<dimension, SpinType, Derived>::operator=(const SpinLatticeBase<dimension, SpinType, Derived>& other)
{
  // Check for self-assignment
  if (this == &other)
    return *this;

  // Do the copy
  spin_lattice = other.spin_lattice;
  _simulation_time = other._simulation_time;

  // Return the existing object
  return *this;
}
template<unsigned int dimension, class SpinType, class Derived>
bool SpinLatticeBase<dimension, SpinType, Derived>::operator==(const SpinLatticeBase<dimension, SpinType, Derived>& other) const
{
  return spin_lattice == other.spin_lattice;
}
template<unsigned int dimension, class SpinType, class Derived>
bool SpinLatticeBase<dimension, SpinType, Derived>::operator!=(const SpinLatticeBase<dimension, SpinType, Derived>& other) const
{
  return spin_lattice != other.spin_lattice;
}

/*! 
 * \details Creates a STL vector of Step objects representing all steps that can be done in the lattice. For each lattice site there are the flips from the present value of the spin to all other values of the spin.
 */
template<unsigned int dimension, class SpinType, class Derived>
std::vector<Step<dimension, SpinType> > SpinLatticeBase<dimension, SpinType, Derived>::all_steps()
{
  std::vector<Step<dimension, SpinType> > result;
  // Go through all lattice sites and through every differing spin
  // Add the resulting step to the result vector
  for (unsigned int i = 0; i < spin_lattice.num_elements(); i++)
  {
    index_type step_index = int_to_index(i);
    std::vector<SpinType> new_spins = spin_lattice(step_index).all_possible_values();
    for (typename std::vector<SpinType>::iterator new_spin = new_spins.begin();
	 new_spin != new_spins.end(); ++new_spin)
    {
      if (*new_spin != spin_lattice(step_index))
      {
	result.push_back(Step<dimension, SpinType>(static_cast<Derived*>(this), step_index, *new_spin));
      }
    }
  }

  return result;
}

/*!
 * \details Updates the spin lattice with the given spin. After the update the simulation time is increase by one. If the creation simulation time of the step does not match the actual simulation time of the spin lattice, an exception will be thrown (to be implemented).
 * \param step_to_commit A valid step that should update the spin lattice.
 */
template<unsigned int dimension, class SpinType, class Derived>
void SpinLatticeBase<dimension, SpinType, Derived>::commit(Step<dimension, SpinType>& step_to_commit)
{
  // ToDo: Check for the right simulation time
  
  spin_lattice(step_to_commit.get_flip_index()) = step_to_commit.get_new_spin();
  
  _simulation_time++;
}

/*! 
 * \details Calculates the energy of the lattice by summing over all next-neighbour pairs and returning the negative value of the product of the pairs.
 */
template<unsigned int dimension, class SpinType, class Derived>
double SpinLatticeBase<dimension, SpinType, Derived>::energy() const
{
  double result = 0;

  // Go through all points
  for (unsigned int i = 0; i < spin_lattice.num_elements(); i++)
  {
    // Calculate the energy using the next neighbours
    SpinType this_spin = get_spin(int_to_index(i));
    std::vector<SpinType> neighbours = next_neighbours(int_to_index(i));
    for (typename std::vector<SpinType>::iterator neighbour = neighbours.begin();
	 neighbour != neighbours.end(); ++neighbour)
    {
      result += - (this_spin * (*neighbour));
    }
  }

  // Factor of 0.5 because energy is defined over all pairs of next neighbours, we count every pair twice
  return 0.5*result;
}

/*! 
 * \details Calculates the magnetization of the lattice by summing over all spins and returning the sum of their values
 */
template<unsigned int dimension, class SpinType, class Derived>
double SpinLatticeBase<dimension, SpinType, Derived>::magnetization() const
{
  double result = 0;

  for (const SpinType* spin = spin_lattice.data();
       spin != spin_lattice.data() + spin_lattice.num_elements(); ++spin)
  {
    result += spin->get_value();
  }

  return result;
}

/*! 
 * \details Returns a STL vector of spins that are the next neighbours of the spin at the given coordinates.
 * \param coordinates Index of the spin of which the next neighbours will be calculated.
 */
template<unsigned int dimension, class SpinType, class Derived>
std::vector<SpinType> SpinLatticeBase<dimension, SpinType, Derived>::next_neighbours(index_type coordinates) const
{
  std::vector<SpinType> result;

  // Go through all dimensions
  for (unsigned int d = 0; d < dimension; d++)
  {
    // Construct two coordinates representing the two next neighbours in this dimension
    index_type index_upper_neighbour = coordinates;
    index_type index_lower_neighbour = coordinates;
    // Periodic boundary conditions
    if (coordinates[d] == 0)
    {
      index_upper_neighbour[d]++;
      index_lower_neighbour[d] = spin_lattice.shape()[d] - 1;
    }
    else if (static_cast<typename lattice_array_type::size_type>(coordinates[d]) == spin_lattice.shape()[d] - 1)
    {
      index_upper_neighbour[d] = 0;
      index_lower_neighbour[d]--;
    }
    else
    {
      index_upper_neighbour[d]++;
      index_lower_neighbour[d]--;
    }

    // Assign the spins to the result vector
    result.push_back(spin_lattice(index_upper_neighbour));
    result.push_back(spin_lattice(index_lower_neighbour));
  }

  return result;
}

/*!
 * \details Proposes a step based on a given random number between 0 and 1. The integer part of (random_number * number_of_sites) will be used for determining the index where the step or the spin flip should occur, the non-integer part will be used for determining to which new spin the old spin will be flipped.
 * \param rng Random number generator so that rng() gives a random number between 0 and 1
 */
template<unsigned int dimension, class SpinType, class Derived>
Step<dimension, SpinType> SpinLatticeBase<dimension, SpinType, Derived>::propose_step(double random_double)
{
  // Use a random number for calculating a random int between 0 and the number of spins in the system
  double random_number_index = random_double*spin_lattice.num_elements();
  int lattice_index = static_cast<unsigned int>(floor(random_number_index));

  // Calculate from the lattice index the multidimensional index
  index_type lattice_multiindex;
  for (int d = dimension - 1; d >= 0; d--)
  {
    lattice_multiindex[d] = lattice_index % spin_lattice.shape()[d];
    lattice_index /= spin_lattice.shape()[d];
  }

  // Create a step and return the step
  return Step<dimension, SpinType>(static_cast<Derived*>(this), // Pointer to the lattice of the step
				   lattice_multiindex, // Index of the place to flip
				   spin_lattice(lattice_multiindex).random_differ(random_number_index - lattice_index)); // new random spin value differing from the old value
}

/*!
 * \details Proposes a step based on a given random number between 0 and 1. The integer part of (random_number * number_of_sites) will be used for determining the index where the step or the spin flip should occur, the non-integer part will be used for determining to which new spin the old spin will be flipped.
 * \param rng Random number generator so that rng() gives a random number between 0 and 1
 */
template<unsigned int dimension, class SpinType, class Derived> template<class RandomNumberGenerator>
Step<dimension, SpinType> SpinLatticeBase<dimension, SpinType, Derived>::propose_step(RandomNumberGenerator* rng)
{
  // Use a random number for calculating a random int between 0 and the number of spins in the system
  double random_number_index = rng->random_double()*spin_lattice.num_elements();
  int lattice_index = static_cast<unsigned int>(floor(random_number_index));

  // Calculate from the lattice index the multidimensional index
  index_type lattice_multiindex;
  for (int d = dimension - 1; d >= 0; d--)
  {
    lattice_multiindex[d] = lattice_index % spin_lattice.shape()[d];
    lattice_index /= spin_lattice.shape()[d];
  }

  // Create a step and return the step
  return Step<dimension, SpinType>(static_cast<Derived*>(this), // Pointer to the lattice of the step
				   lattice_multiindex, // Index of the place to flip
				   spin_lattice(lattice_multiindex).random_differ(rng->random_double())); // new random spin value differing from the old value
}

template<unsigned int dimension, class SpinType, class Derived>
unsigned int SpinLatticeBase<dimension, SpinType, Derived>::system_size() const
{
  unsigned int result = 1;
  for (unsigned int d = 0; d < dimension; d++)
    result *= spin_lattice.shape()[d];

  return result;
}


} // of namespace Ising

#endif
