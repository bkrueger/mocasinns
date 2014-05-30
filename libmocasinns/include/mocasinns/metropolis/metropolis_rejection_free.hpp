/**
 * \file metropolis_rejection_free.hpp
 * \brief Contains the declaration of the rejection-free Metropolis algorithm
 * 
 * Do not include the file, but include <mocasinns/metropolis.hpp>
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_METROPOLIS_METROPOLIS_REJECTION_FREE_HPP
#define MOCASINNS_METROPOLIS_METROPOLIS_REJECTION_FREE_HPP

#include "metropolis_base.hpp"

namespace Mocasinns
{
  /*!
   * \brief Class for the rejection-free Metropolis algorithm.
   * \copydetails Mocasinns::MetropolisBase
   */
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  class MetropolisRejectionFree : public MetropolisBase<ConfigurationType, StepType, RandomNumberGenerator, true>
  {
  public:
    //! Base class with spezialization "true" for template parameter rejection free
    typedef MetropolisBase<ConfigurationType, StepType, RandomNumberGenerator, true> Base;
    //! Parameters of the base class, equal the parameters of this class
    typedef typename Base::Parameters Parameters;
    
    //! Initialise a Metropolis-MC simulation with default configuration space and default Parameters
    MetropolisRejectionFree() : Base() {}
    //! Initialise a Metropolis-MC simulation with default configuration space and given Parameters
    MetropolisRejectionFree(const Parameters& params) : Base(params) {}
    //! Initialise a Metropolis-MC simulation with given parameters and given configuration space
    MetropolisRejectionFree(const Parameters& params, ConfigurationType* initial_configuration) : Base(params, initial_configuration) {}
    //! Initialise a Metropolis-MC simulation by copying from another one
    MetropolisRejectionFree(const MetropolisRejectionFree& other) : Base(other) {}

    //! Assignment operator
    MetropolisRejectionFree& operator=(const MetropolisRejectionFree& other) { Base::operator=(other); return *this;}
  };
}

#endif
