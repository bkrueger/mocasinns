/**
 * \file wang_landau_rejection free.hpp
 * \brief Contains the declaration of the rejection-free Wang-Landau algorithm
 * 
 * Do not include the file, but include <mocasinns/wang_landau.hpp>
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_WANG_LANDAU_WANG_LANDAU_REJECTION_FREE_HPP
#define MOCASINNS_WANG_LANDAU_WANG_LANDAU_REJECTION_FREE_HPP

#include "wang_landau_base.hpp"

namespace Mocasinns
{

  //! Class for rejection-free Wang-Landau simulations
  template<class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
  class WangLandauRejectionFree : public WangLandauBase<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, true>
  {
  public:
    //! Base class with spezialization "false" for template parameter rejection free
    typedef WangLandauBase<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, true> Base;
    //! Parameters of the base class, equal the parameters of this class
    typedef typename Base::Parameters Parameters;
    
    //! Initialise a WangLanday-MC simulation with default parameters, default configuration space and default RandomNumberGenerator
    WangLandauRejectionFree() : Base() { }
    //! Initialise a WangLandau-MC simulation with default configuration space and default RandomNumberGenerator
    WangLandauRejectionFree(const Parameters& params) : Base(params) { }
    //! Initialise a WangLandau-MC simulation with given configuration space and default RandomNumberGenerator
    WangLandauRejectionFree(const Parameters& params, ConfigurationType* initial_configuration) : Base(params, initial_configuration) { }
    //! Copy from a non-rejection free simulation
    WangLandauRejectionFree(const WangLandauBase<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, false>& simulation_non_rejection_free) : Base(simulation_non_rejection_free) { }
  };
}

#endif
