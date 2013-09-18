#ifdef MOCASINNS_DETAILS_MULTICANONICAL_ENERGY_TYPE_EXTENDED_HPP

namespace Mocasinns
{ 
  namespace Details
  {
    namespace Multicanonical
    {
      template <class EnergyType>
      EnergyTypeExtended<EnergyType>::EnergyTypeExtended(EnergyType orig_energy, int in_gs) : original_energy(orig_energy), in_ground_state(in_gs)
      { }
      
      template <class EnergyType>
      EnergyTypeExtended<EnergyType>::EnergyTypeExtended(const EnergyTypeExtended& other) : original_energy(other.original_energy), in_ground_state(other.in_ground_state)
      { }

      template <class EnergyType>
      EnergyTypeExtended<EnergyType>& EnergyTypeExtended<EnergyType>::operator=(const EnergyTypeExtended& rhs)
      {
	if (this != &rhs)
	{
	  original_energy = rhs.original_energy;
	  in_ground_state = rhs.in_ground_state;
	}

	return *this;
      }
      
      template <class EnergyType>
      bool EnergyTypeExtended<EnergyType>::operator==(const EnergyTypeExtended& rhs) const
      {
	// If both energies indicate that one is in the reference state, return true
	if (in_ground_state)
	{
	  if (rhs.in_ground_state) 
	    return true;
	  else 
	    return false;
	}
	else // !in_ground_state
	{
	  if (original_energy == rhs.original_energy)
	    return true;
	  else
	    return false;
	}
      }
      template <class EnergyType>
      bool EnergyTypeExtended<EnergyType>::operator!=(const EnergyTypeExtended& rhs) const
      {
	return !operator==(rhs);
      }

      template <class EnergyType>
      bool EnergyTypeExtended<EnergyType>::operator<(const EnergyTypeExtended& rhs) const
      {
	// Test first whether the two states are in ground state
	if (in_ground_state < rhs.in_ground_state) return true;
	if (in_ground_state > rhs.in_ground_state) return false;

	// If the reference bons match, check the original energy
	if (original_energy < rhs.original_energy) return true;
	else return false;
      }
      template <class EnergyType>
      bool EnergyTypeExtended<EnergyType>::operator>(const EnergyTypeExtended& rhs) const
      {
	// Test first whether the two states are in ground state
	if (in_ground_state > rhs.in_ground_state) return true;
	if (in_ground_state < rhs.in_ground_state) return false;

	// If the reference bons match, check the original energy
	if (original_energy > rhs.original_energy) return true;
	else return false;
      }
      template <class EnergyType>
      bool EnergyTypeExtended<EnergyType>::operator<=(const EnergyTypeExtended& rhs) const
      {
	return !(operator>(rhs));
      }
      template <class EnergyType>
      bool EnergyTypeExtended<EnergyType>::operator>=(const EnergyTypeExtended& rhs) const
      {
	return !(operator<(rhs));
      }
      
      template <class EnergyType>
      void EnergyTypeExtended<EnergyType>::operator+=(const EnergyTypeExtended& rhs)
      {
	// Add the usual energies
	original_energy += rhs.original_energy;
	// Add the in_ground_state numbers
	in_ground_state += rhs.in_ground_state;
      }
      template <class EnergyType>
      void EnergyTypeExtended<EnergyType>::operator-=(const EnergyTypeExtended& rhs)
      {
	// Subtract the usual energies
	original_energy -= rhs.original_energy;
	// Substract the in_ground_state numbers
	in_ground_state -= rhs.in_ground_state;
      }

      template <class EnergyType>
      EnergyTypeExtended<EnergyType> operator+(const EnergyTypeExtended<EnergyType>& lhs, const EnergyTypeExtended<EnergyType>& rhs)
      {
	EnergyTypeExtended<EnergyType> result(lhs);
	result += rhs;
	return result;
      }
      template <class EnergyType>
      EnergyTypeExtended<EnergyType> operator-(const EnergyTypeExtended<EnergyType>& lhs, const EnergyTypeExtended<EnergyType>& rhs)
      {
	EnergyTypeExtended<EnergyType> result(lhs);
	result -= rhs;
	return result;
      }
      
    }
  }
}
#endif
      
