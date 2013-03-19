/*!
  \file vector_accumulator.hpp

  \brief File containing an accumulator interface for a std::vector

  \author Benedikt Krüger
*/

#ifndef MOCASINNS_DETAILS_METROPOLIS_VECTOR_ACCUMULATOR
#define MOCASINNS_DETAILS_METROPOLIS_VECTOR_ACCUMULATOR

#include <vector>

namespace Mocasinns
{
  namespace Details
  {
    namespace Metropolis
    {
      //! Class for using a vector as an accumulator. Is used for Metropolis algorithm if all values are returned as a vector
      template<class Observable> class VectorAccumulator
      {
      public:
	//! Vector for the data
	std::vector<Observable> internal_vector;
	
	//! Standard constructor
	VectorAccumulator() : internal_vector() {}
	//! Copy constructor
	VectorAccumulator(const VectorAccumulator<Observable>& copy) : internal_vector(copy.internal_vector) {}
	
	//! Accumulating operator, refers to push-back
	void operator()(const Observable& new_value) { internal_vector.push_back(new_value); }
      };
    }
  }
}

#endif
