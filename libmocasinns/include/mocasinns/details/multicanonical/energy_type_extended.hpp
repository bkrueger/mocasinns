#ifndef MOCASINNS_DETAILS_MULTICANONICAL_ENERGY_TYPE_EXTENDED_HPP
#define MOCASINNS_DETAILS_MULTICANONICAL_ENERGY_TYPE_EXTENDED_HPP

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

      public:
	//! Constructor specifying the original energy and whether we are in ground state
	EnergyTypeExtended(EnergyType orig_energy, int in_gs);
	//! Copy-Constructor
	EnergyTypeExtended(const EnergyTypeExtended& other);
	
	//! Get-Accessor for the original energy
	const EnergyType& get_original_energy() { return original_energy; }
	//! Get-Accessor for the int specifying we are in ground state
	int get_in_ground_state() { return in_ground_state; }
	
	//! Assignment operator
	EnergyTypeExtended<EnergyType>& operator=(const EnergyTypeExtended& rhs);
	//! Operator for testing equality
	bool operator==(const EnergyTypeExtended& rhs) const;
	//! Operator for testing inequality
	bool operator!=(const EnergyTypeExtended& rhs) const;
	//! Operator for comparing two extended energy types
	bool operator<(const EnergyTypeExtended& rhs) const;
	
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
      
    }
  }
}

#include "../../src/details/multicanonical/energy_type_extended.cpp"

#endif
