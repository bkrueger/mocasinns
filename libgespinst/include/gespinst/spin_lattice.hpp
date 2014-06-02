#ifndef GESPINST_SPIN_LATTICE_HPP
#define GESPINST_SPIN_LATTICE_HPP

#include "spin_lattice_step.hpp"

#include <vector>
#include "boost/multi_array.hpp"

// Header for the serialization of the class
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
// Serialization of boost multi array
#include "details/multi_array_serialize.hpp"

namespace Gespinst
{
/*! 
 * \brief Class template for a arbitrary dimensional lattice of spins with next-neighbour interaction.
 * \author Benedikt Krüger
 * \details This class takes two template arguments: The first (unsigned int) specifies the number of dimensions, the second the class of spins that should be used. A class used as spin-template argument must have the following functions: 
 * - operator== and operator!= for determining whether two spins are equal
 * - operator= for assigning two spins
 * - std::vector<SpinType> all_possible_values() const: Function that returns a vector with all possible values of a spin.
 * - SpinType random_differ(double random_number) const: Function that returns a new spin out of all possible values that is unequal to the actual spin.
 */
template<unsigned int dimension, class SpinType, class Derived>
class SpinLatticeBase
{
protected:
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
  SpinLatticeBase();
  //! Constructor with optional default value
  SpinLatticeBase(std::vector<unsigned int> lattice_extension, SpinType default_spin = SpinType());
  //! Copy constructor
  SpinLatticeBase(const SpinLatticeBase<dimension, SpinType, Derived>& other);

  //! Function to get a spin at the given coordinates
  SpinType get_spin(index_type coordinates) const;
  //! Function to set a spin at the given coordinates
  void set_spin(index_type coordinates, SpinType value);
  //! Get-accessor for the simulation time
  int get_simulation_time() const { return _simulation_time; }

  //! Assignment Operator
  SpinLatticeBase<dimension, SpinType, Derived>& operator=(const SpinLatticeBase<dimension, SpinType, Derived>& other);
  //! Operator for testing equality
  bool operator==(const SpinLatticeBase<dimension, SpinType, Derived>& other) const;
  //! Operator for testing inequality
  bool operator!=(const SpinLatticeBase<dimension, SpinType, Derived>& other) const;

  //! Create a vector with all present possible steps
  std::vector<SpinLatticeStep<dimension, SpinType> > all_steps();

  //! Commit and execute a given step
  void commit(SpinLatticeStep<dimension, SpinType>& step_to_commit);

  //! Calculate the energy of this lattice
  double energy() const;

  //! Get the extension of the lattice in the specified direction. Returns 0 if there is no such direction
  unsigned int extension(unsigned int dim) const;

  //! Static function to specify that the class is serializable
  static bool is_serializable() { return true; }

  //! Magnetization of the lattice (sum over all spin values)
  double magnetization() const;

  //! Get the neighbours of a coordinate position
  std::vector<SpinType> next_neighbours(index_type coordinates) const; 

  //! Propose a step given a random double number
  SpinLatticeStep<dimension, SpinType> propose_step(double random_double);
  //! Propose a step given a random number generator
  template<class RandomNumberGenerator> SpinLatticeStep<dimension, SpinType> propose_step(RandomNumberGenerator* rng);

  //! Calculate the system size
  unsigned int system_size() const;
};

//! Derived class for an arbitrary-dimensional spin lattice
template<unsigned int dimension, class SpinType>
class SpinLattice : public SpinLatticeBase<dimension, SpinType, SpinLattice<dimension, SpinType> >
{
private:
  typedef SpinLatticeBase<dimension, SpinType, SpinLattice<dimension, SpinType> > Base;

public:
  //! Default constructor
  SpinLattice() : Base() {}
  //! Constructor with optional default value
  SpinLattice(std::vector<unsigned int> lattice_extension, SpinType default_spin = SpinType()) : Base(lattice_extension, default_spin) {}
  //! Copy constructor
  SpinLattice(const SpinLattice<dimension,SpinType>& other) : Base(other) {}
};

//! Specialised class for a one dimensional spin lattice
template<class SpinType>
class SpinLattice<1, SpinType> : public SpinLatticeBase<1, SpinType, SpinLattice<1, SpinType> >
{
private:
  typedef SpinLatticeBase<1, SpinType, SpinLattice<1, SpinType> > Base;
  typedef typename Base::lattice_array_type::index subindex_type;

  typename Base::index_type create_array_index(subindex_type x1) const
  {
    typename Base::index_type array_index;
    array_index[0] = x1;
    return array_index;
  }

public:
  //! Default constructor
  SpinLattice() : Base() {}
  //! Constructor with optional default value
  SpinLattice(std::vector<unsigned int> lattice_extension, SpinType default_spin = SpinType()) : Base(lattice_extension, default_spin) {}
  //! Copy constructor
  SpinLattice(const SpinLattice<1, SpinType>& other) : Base(other) {}

  //! Const-Access-Operator for three-dimensional spin
  const SpinType& operator()(subindex_type x1) const
  {
    return this->spin_lattice(create_array_index(x1));
  }
  //! Modifying Access-Operator for three-dimensional spin
  SpinType& operator()(subindex_type x1)
  {
    return this->spin_lattice(create_array_index(x1));
  }
};

//! Specialised class for a two-dimensional spin lattice
template<class SpinType>
class SpinLattice<2, SpinType> : public SpinLatticeBase<2, SpinType, SpinLattice<2, SpinType> >
{
private:
  typedef SpinLatticeBase<2, SpinType, SpinLattice<2, SpinType> > Base;
  typedef typename Base::lattice_array_type::index subindex_type;

  typename Base::index_type create_array_index(subindex_type x1, subindex_type x2) const
  {
    typename Base::index_type array_index;
    array_index[0] = x1;
    array_index[1] = x2;
    return array_index;
  }

public:
  //! Default constructor
  SpinLattice() : Base() {}
  //! Constructor with optional default value
  SpinLattice(std::vector<unsigned int> lattice_extension, SpinType default_spin = SpinType()) : Base(lattice_extension, default_spin) {}
  //! Copy constructor
  SpinLattice(const SpinLattice<2, SpinType>& other) : Base(other) {}

  //! Const-Access-Operator for three-dimensional spin
  const SpinType& operator()(subindex_type x1, subindex_type x2) const
  {
    return this->spin_lattice(create_array_index(x1, x2));
  }
  //! Modifying Access-Operator for three-dimensional spin
  SpinType& operator()(subindex_type x1, subindex_type x2)
  {
    return this->spin_lattice(create_array_index(x1, x2));
  }
};

//! Specialied class for a three-dimensional spin lattice
template<class SpinType>
class SpinLattice<3, SpinType> : public SpinLatticeBase<3, SpinType, SpinLattice<3, SpinType> >
{
private:
  typedef SpinLatticeBase<3, SpinType, SpinLattice<3, SpinType> > Base;
  typedef typename Base::lattice_array_type::index subindex_type;

  typename Base::index_type create_array_index(subindex_type x1, subindex_type x2, subindex_type x3) const
  {
    typename Base::index_type array_index;
    array_index[0] = x1;
    array_index[1] = x2;
    array_index[2] = x3;
    return array_index;
  }

public:
  //! Default constructor
  SpinLattice() : Base() {}
  //! Constructor with default value
  SpinLattice(std::vector<unsigned int> lattice_extension, SpinType default_spin = SpinType()) : Base(lattice_extension, default_spin) {}
  //! Copy constructor
  SpinLattice(const SpinLattice<3, SpinType>& other) : Base(other) {}

  //! Const-Access-Operator for three-dimensional spin
  const SpinType& operator()(subindex_type x1, subindex_type x2, subindex_type x3) const
  {
    return this->spin_lattice(create_array_index(x1, x2, x3));
  }
  //! Modifying Access-Operator for three-dimensional spin
  SpinType& operator()(subindex_type x1, subindex_type x2, subindex_type x3)
  {
    return this->spin_lattice(create_array_index(x1, x2, x3));
  }
};

//! Outstream operator for a 1d spin lattice
template <class SpinType>
std::ostream& operator<<(std::ostream& lhs, SpinLattice<1,SpinType> const& rhs)
{
  for (unsigned int i = 0; i < rhs.extension(0); ++i)
    lhs << rhs(i) << " ";
  return lhs;
} 
//! Outstream operator for a 2d spin lattice
template <class SpinType>
std::ostream& operator<<(std::ostream& lhs, SpinLattice<2,SpinType> const& rhs)
{
  for (unsigned int i = 0; i < rhs.extension(0); ++i)
  {
    for (unsigned int j = 0; j < rhs.extension(1); ++j)
    {
      lhs << rhs(i,j) << " ";
    }
    lhs << std::endl;
  }
  return lhs;
}

} // of namespace Ising

// Include the implementation
#include "src/spin_lattice.cpp"

#endif
