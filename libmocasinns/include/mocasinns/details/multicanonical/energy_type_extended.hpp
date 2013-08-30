#ifndef MOCASINNS_DETAILS_MULTICANONICAL_ENERGY_TYPE_EXTENDED_HPP
#define MOCASINNS_DETAILS_MULTICANONICAL_ENERGY_TYPE_EXTENDED_HPP

// Header for stream operations
#include <ostream>

// Header for the serialization of the class
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace Mocasinns
{
  namespace Details
  {
    namespace Multicanonical
    {

      //! Class extending the energy type
      /*!
       * \bug For the class extended configuration type one has to do the following design decision: 1. Make ConfigurationTypeExtended a derived class from the ConfigurationType (pro: The functions of ConfigurationType that need not to be modified must not be repeated, contra: One has to do a copy of the given configuration space in the constructor, so the algorithm does not work directly on the configuration space as expected) 2. Make ConfigurationTypeExtended contain a pointer to a object of class ConfigurationType (pro and contra are interchanged) 3. Force the user to use an extended ConfigurationSpace (contra: the user should not know about this extensions.
       */
      template <class EnergyType>
      class EnergyTypeExtended
      {
      private:
	//! Member specifying the original energy
	EnergyType original_energy;
	//! Member specifying whether we are in ground state (1) or not (0)
	int in_ground_state;

	//! Member variable for boost serialization
	friend class boost::serialization::access;
	//! Method to serialize this class (omitted version name to avoid unused parameter warnings)
	template<class Archive> void serialize(Archive &ar, const unsigned int)
	{
	  ar & original_energy;
	  ar & in_ground_state;
	}

      public:
	//! Constructor specifying the original energy and whether we are in ground state
	EnergyTypeExtended(EnergyType orig_energy = EnergyType(), int in_gs = 0);
	//! Copy-Constructor
	EnergyTypeExtended(const EnergyTypeExtended& other);
	
	//! Get-Accessor for the original energy
	const EnergyType& get_original_energy() const { return original_energy; }
	//! Get-Accessor for the int specifying we are in ground state
	int get_in_ground_state() const { return in_ground_state; }
	
	//! Assignment operator
	EnergyTypeExtended<EnergyType>& operator=(const EnergyTypeExtended& rhs);
	//! Operator for testing equality
	bool operator==(const EnergyTypeExtended& rhs) const;
	//! Operator for testing inequality
	bool operator!=(const EnergyTypeExtended& rhs) const;
	//! Operator for comparing two extended energy types
	bool operator<(const EnergyTypeExtended& rhs) const;
	//! Operator for comparing two extended energy types
	bool operator>(const EnergyTypeExtended& rhs) const;
	//! Operator for comparing two extended energy types
	bool operator<=(const EnergyTypeExtended& rhs) const;
	//! Operator for comparing two extended energy types
	bool operator>=(const EnergyTypeExtended& rhs) const;
	
	//! Operator for adding another EnergyTypeExtended to this one
	void operator+=(const EnergyTypeExtended& rhs);
	//! Operator for substracting another EnergyTypeExtended from this one
	void operator-=(const EnergyTypeExtended& rhs);
      };
 
      //! Add two extended energy types
      template <class EnergyType>
      EnergyTypeExtended<EnergyType> operator+(const EnergyTypeExtended<EnergyType>& lhs, const EnergyTypeExtended<EnergyType>& rhs);
      //! Substract two simplex numbers
      template <class EnergyType>
      EnergyTypeExtended<EnergyType> operator-(const EnergyTypeExtended<EnergyType>& lhs, const EnergyTypeExtended<EnergyType>& rhs);
      
      //! Output of the extended energy
      template <class EnergyType>
      std::ostream& operator<<(std::ostream& lhs, const EnergyTypeExtended<EnergyType>& rhs)
      {
	lhs << rhs.get_original_energy() << " " << rhs.get_in_ground_state();
	return lhs;
      }
    }
  }
}

#include "../../src/details/multicanonical/energy_type_extended.cpp"

#endif
