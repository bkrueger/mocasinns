#ifndef MOCASINNS_CONCEPTS_ENERGY_CONCEPT
#define MOCASINNS_CONCEPTS_ENERGY_CONCEPT

/*!
  \file energy_concept.hpp

  \author Benedikt Krüger
*/

#include "boost/concept_check.hpp"

namespace Mocasinns
{
  namespace Concepts
  {

    /*!
      \brief Structure for checking a class for compatibility with the concept of energy used in the mocasinns library.
      \tparam EnergyType Class or type that should be checked for compatibility with the energy concept.
    */
    template <class EnergyType>
    struct EnergyConcept
    {
    public:
      BOOST_CONCEPT_USAGE(EnergyConcept)
      {
	// Two energies must be addible and the result must be an energy type (or convertible to an energy type)
	sum_result = energy_1 + energy_2;
	
	// Two energies can be compared using the < operator
	compare_result = (energy_1 < energy_2);
	
	// Two energies m
      }
      
    private:
      EnergyType energy_1;
      EnergyType energy_2;
      
      EnergyType sum_result;
      bool compare_result;
    };
  }
}

#endif
