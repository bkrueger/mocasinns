#ifndef MOCASINNS_CONCEPTS_CONFIGURATION_CONCEPT
#define MOCASINNS_CONCEPTS_CONFIGURATION_CONCEPT

/*!
  \file configuration_concept.hpp

  \author Benedikt Krüger
*/

#include <boost/concept_check.hpp>

#include "../random/boost_random.hpp"

namespace Mocasinns
{
  namespace Concepts
  {

    /*!
      \brief Structure for checking a class for compatibility with the concept of a configuration used in the mocasinns library.
      \tparam ConfigurationType Class or type that should be checked for compatibility with the configuration concept.
      \tparam StepType Class or type that should be the result of the propose_step function
    */
    template <class ConfigurationType, class StepType>
    struct ConfigurationConcept
    {
    public:
      BOOST_CONCEPT_USAGE(ConfigurationConcept)
      {
	// There must be a function for proposing a step and returning a StepType
	// same_type(proposed_step, configuration.propose_step(&rng));
	
	// There must be a function energy
	configuration.energy();
	
	// There must be a function to commit a given step
	configuration.commit(proposed_step);
      }
      
    private:
      ConfigurationType configuration;
      StepType proposed_step;
      Random::Boost_MT19937 rng;

      // Type deduction will fail unless the arguments have the same type.
      template <typename T>
      void same_type(T const&, T const&);
    };
  }
}

#endif
