#ifndef MOCASINNS_CONCEPTS_RANDOM_NUMBER_GENERATOR_CONCEPT_HPP
#define MOCASINNS_CONCEPTS_RANDOM_NUMBER_GENERATOR_CONCEPT_HPP

/*!
  \file random_number_generator_concept.hpp

  \author Benedikt Krüger
*/

#include "boost/concept_check.hpp"

namespace Mocasinns
{
  namespace Concepts
  {

    /*!
      \brief Structure for checking a class for compatibility with the concept of a random number generator used in the mocasinns library.
      \tparam RandomNumberGenerator Class or type that should be checked for compatibility with the random number generator concept.
    */
    template <class RandomNumberGenerator>
    struct RandomNumberGeneratorConcept
    {
    public:
      BOOST_CONCEPT_USAGE(RandomNumberGeneratorConcept)
      {
	// There must be a function set_seed accepting an int
	rng.set_seed(0);

	// There must be a function random_double() to calculate the uniformly distributed random number
	rng.random_double();
      }
      
    private:
      RandomNumberGenerator rng;
    };
  }
}

#endif
