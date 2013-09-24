/**
 * \file metropolis.hpp
 * \brief Contains the declaration of the normal (non rejection-free) Metropolis algorithm
 * 
 * Do not include the file, but include <mocasinns/metropolis.hpp>
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_METROPOLIS_METROPOLIS_HPP
#define MOCASINNS_METROPOLIS_METROPOLIS_HPP

#include "metropolis_base.hpp"

namespace Mocasinns
{
  //! Class for standard Metropolis-Monte-Carlo simulations
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  class Metropolis : public MetropolisBase<ConfigurationType, StepType, RandomNumberGenerator, false>
  {
  public:
    //! Base class with spezialization "false" for template parameter rejection free
    typedef MetropolisBase<ConfigurationType, StepType, RandomNumberGenerator, false> Base;
    //! Parameters of the base class, equal the parameters of this class
    typedef typename Base::Parameters Parameters;
    
    //! Initialise a Metropolis-MC simulation with default configuration space and default Parameters
    Metropolis() : Base() {}
    //! Initialise a Metropolis-MC simulation with default configuration space and given Parameters
    Metropolis(const Parameters& params) : Base(params) {}
    //! Initialise a Metropolis-MC simulation with given parameters and given configuration space
    Metropolis(const Parameters& params, ConfigurationType* initial_configuration) : Base(params, initial_configuration) {}
  };
}

#endif
