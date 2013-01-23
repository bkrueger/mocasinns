#ifndef _SPINLATTICE_HPP
#define _SPINLATTICE_HPP

#include "step.hpp"

#include <vector>
#include "boost/multi_array.hpp"

// Header for the serialization of the class
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
// Serialization of boost multi array
#include "multi_array_serialize.hpp"

/*! 
 * \brief Class template for a arbitrary dimensional lattice of spins with next-neighbour interaction.
 * \author Benedikt Krüger
 * \details This class takes two template arguments: The first (unsigned int) specifies the number of dimensions, the second the class of spins that should be used. A class used as template argument must have the following functions: 
 * - operator== and operator!= for determining whether two spins are equal
 * - operator= for assigning two spins
 * - std::vector<SpinType> all_possible_values() const: Function that returns a vector with all possible values of a spin.
 * - SpinType random_differ(double random_number) const: Function that returns a new spin out of all possible values that is unequal to the actual spin.
 */
template<unsigned int dimension, class SpinType>
class SpinLattice
{
  typedef boost::multi_array<SpinType, dimension> lattice_array_type;
  typedef boost::array<typename lattice_array_type::index, dimension> index_type;

private:
  //! Integer for the simulation time
  int _simulation_time;

  //! Function to convert a coordinate vector into the right boost::array
  index_type to_boost_array(std::vector<unsigned int> coordinates);
  //! Function to convert a integer (from 0 to num_elements() - 1) to coordinates
  index_type int_to_index(unsigned int i) const;

  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    ar & spin_lattice;
    ar & _simulation_time;
  }

protected:
  //! Array with the Spins
  lattice_array_type spin_lattice;

public:
  //! Default constructor
  SpinLattice();
  //! Constructor
  SpinLattice(std::vector<unsigned int> lattice_extension);
  //! Constructor with default value
  SpinLattice(std::vector<unsigned int> lattice_extension, SpinType default_spin);
  //! Copy constructor
  SpinLattice(const SpinLattice<dimension, SpinType>& other);

  //! Function to get a spin at the given coordinates
  SpinType get_spin(index_type coordinates) const;
  //! Function to set a spin at the given coordinates
  void set_spin(index_type coordinates, SpinType value);
  //! Get-accessor for the simulation time
  int get_simulation_time() const { return _simulation_time; }

  //! Assignment Operator
  SpinLattice<dimension, SpinType>& operator=(const SpinLattice<dimension, SpinType>& other);
  //! Operator for testing equality
  bool operator==(const SpinLattice<dimension, SpinType>& other) const;
  //! Operator for testing inequality
  bool operator!=(const SpinLattice<dimension, SpinType>& other) const;

  //! Create a vector with all present possible steps
  std::vector<Step<dimension, SpinType> > all_steps();

  //! Commit and execute a given step
  void commit(Step<dimension, SpinType>& step_to_commit);

  //! Calculate the energy of this lattice
  double energy() const;

  //! Magnetization of the lattice (sum over all spin values)
  double magnetization() const;

  //! Get the neighbours of a coordinate position
  std::vector<SpinType> next_neighbours(index_type coordinates) const; 

  //! Propose a step given a random double number
  Step<dimension, SpinType> propose_step(double random_double);
  //! Propose a step given a random number generator
  template<class RandomNumberGenerator> Step<dimension, SpinType> propose_step(RandomNumberGenerator* rng);
};

// Include the implementation
#include "spinlattice.cpp"

#endif
