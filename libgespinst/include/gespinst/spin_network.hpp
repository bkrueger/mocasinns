#ifndef GESPINST_SPIN_NETWORK_HPP
#define GESPINST_SPIN_NETWORK_HPP

#include "spin_network_step.hpp"

// Header for the serialization of the class
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace Gespinst
{
  /*! 
   * \brief Class template for a generic network of spin with next-neighbour interactions.
   * \author Benedikt Krüger
   * \tparam SpinType Spin to use in the network
   * \tparam ContainerType Container that is used to store the next neighbours of a certain spin
   */
  template<class SpinType, class ContainerType>
  class SpinNetwork
  {
  private:
    //! Vector of spins representing the different spins
    std::vector<SpinType> spins;
    //! Vector of containers for the next neighbours of each spin
    std::vector<ContainerType> next_neighbour_spins;

    //! Integer for the simulation time
    int simulation_time;

    //! Member variable for boost serialization
    friend class boost::serialization::access;
    //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
    template<class Archive> void serialize(Archive & ar, const unsigned int)
    {
      ar & next_neighbour_spins;
      ar & simulation_time;
    }
    
  public:
    //! Typedef for the spin container
    typedef std::vector<SpinType> spin_container;
    //! Typedef for the next-neigbour container
    typedef std::vector<ContainerType> neighbour_container;
    //! Typedef for the index
    typedef typename spin_container::size_type index_type;
    
    //! Default constructor
    SpinNetwork();
    //! Constructor with the number of spins and the default spin
    SpinNetwork(unsigned int spin_number, SpinType default_spin = SpinType());
    
    //! Operator for write-access to the spin at given index
    SpinType& operator[](index_type index) { return spins[index]; }
    //! Operator for read-access to the spin at given index
    const SpinType& operator[](index_type index) const { return spins[index]; }
       
    //! Get-Accessor for the neighbours at a given index
    const ContainerType& get_neighbours(typename neighbour_container::size_type index) const { return next_neighbour_spins[index]; }
    //! Set-Accessor for the neighbours at a given index
    void set_neighbours(typename neighbour_container::size_type index, const ContainerType& new_neighbours) { next_neighbour_spins[index] = new_neighbours; }

    //! Get-accessor for the simulation time
    int get_simulation_time() const { return simulation_time; }

    //! Operator for testing equality
    bool operator==(const SpinNetwork<SpinType, ContainerType>& other) const;
    //! Operator for testing inequality
    bool operator!=(const SpinNetwork<SpinType, ContainerType>& other) const;

    //! Create a vector with all present possible steps
    std::vector<SpinNetworkStep<SpinType, ContainerType> > all_steps();
    
    //! Commit and execute a given step
    void commit(SpinNetworkStep<SpinType, ContainerType>& step_to_commit);

    //! Calculate the energy of this lattice
    double energy() const;

    //! Magnetization of the lattice (sum over all spin values)
    double magnetization() const;

    //! Propose a step given a random number generator
    template<class RandomNumberGenerator> SpinNetworkStep<SpinType, ContainerType> propose_step(RandomNumberGenerator* rng);

    //! Calculate the system size (equals size)
    unsigned int system_size() const;
  };

  /*! 
   * \brief Class template for a network of spins with next-neighbour interactions and fixes number of neighbours
   * \author Benedikt Krüger
   * \tparam SpinType Spin to use in the network
   * \tparam neighbour_number Fixed number of neighbours of every spin
   */
  template<class SpinType, unsigned int neighbour_number>
  class SpinNetworkStatic : public SpinNetwork<SpinType, std::array<SpinType*,neighbour_number> >
  {
  public:
    //! Typedef for the base class
    typedef SpinNetwork<SpinType, std::array<SpinType*, neighbour_number> > Base;

    //! Default constructor
    SpinNetworkStatic() : Base() {}
    //! Constructor with the number of spins and the default spin
    SpinNetworkStatic(unsigned int spin_number, SpinType default_spin = SpinType()) : Base(spin_number, default_spin) {}
  };

  /*! 
   * \brief Class template for a network of spins with next-neighbour interactions and variable number of neighbours
   * \author Benedikt Krüger
   * \tparam SpinType Spin to use in the network
   * \tparam neighbour_number Fixed number of neighbours of every spin
   */
  template<class SpinType>
  class SpinNetworkDynamic : public SpinNetwork<SpinType, std::vector<SpinType*> >
  {
  public:
    //! Typedef for the base class
    typedef SpinNetwork<SpinType, std::vector<SpinType*> > Base;

    //! Default constructor
    SpinNetworkDynamic() : Base() {}
    //! Constructor with the number of spins and the default spin
    SpinNetworkDynamic(unsigned int spin_number, SpinType default_spin = SpinType()) : Base(spin_number, default_spin) {}
  };

} // of namespace Ising

// Include the implementation
#include "src/spin_network.cpp"

#endif
