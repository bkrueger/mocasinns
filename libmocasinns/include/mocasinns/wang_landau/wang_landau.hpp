/**
 * \file wang_landau/wang_landau.hpp
 * \brief Contains the declaration of the normal (non rejection-free) Wang-Landau algorithm
 * 
 * Do not include the file, but include <mocasinns/wang_landau.hpp>
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_WANG_LANDAU_WANG_LANDAU_HPP
#define MOCASINNS_WANG_LANDAU_WANG_LANDAU_HPP

#include "wang_landau_base.hpp"

namespace Mocasinns
{

  //! Class for standard Wang-Landau simulations
  template<class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
  class WangLandau : public WangLandauBase<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, false>
  {
  public:
    //! Base class with spezialization "false" for template parameter rejection free
    typedef WangLandauBase<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, false> Base;
    //! Parameters of the base class, equal the parameters of this class
    typedef typename Base::Parameters Parameters;
  
    // Initialise a WangLanday-MC simulation with default parameters, default configuration space and default RandomNumberGenerator
    WangLandau() : Base() { }
    // Initialise a WangLandau-MC simulation with default configuration space and default RandomNumberGenerator
    WangLandau(const Parameters& params) : Base(params) { }
    //! Initialise a WangLandau-MC simulation with given configuration space and default RandomNumberGenerator
    WangLandau(const Parameters& params, ConfigurationType* initial_configuration) : Base(params, initial_configuration) { }
  };
}

#endif
