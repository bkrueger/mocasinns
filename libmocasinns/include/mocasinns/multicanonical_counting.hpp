/*!
 * \file multicanonical_counting.hpp
 * \brief Class for counting the number of states using any algorithm calculating the density of states with additional ground state bin
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_MULTICANONICAL_COUNTING_HPP
#define MOCASINNS_MULTICANONICAL_COUNTING_HPP

#include "wang_landau.hpp"
#include "entropic_sampling.hpp"
#include "optimal_ensemble_sampling.hpp"

#include "details/multicanonical/extended_energy_type.hpp"
#include "details/multicanonical/extended_configuration_type.hpp"
#include "details/multicanonical/extended_step_type.hpp"

namespace Mocasinns
{
  
  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator,
	    template<class,class,class,template<class,class>,class> AlgorithmType>
  class MulticanonicalCounting
    : public AlgorithmType<Details::Multicanonical::ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>,
			   Details::Multicanonical::StepTypeExtended<ConfigurationType, StepType, EnergyType>,
			   Details::Multicanonical::EnergyTypeExtended<EnergyType>,
			   HistoType, RandomNumberGenerator>
  {
  public:
    // Typedef for the extensions
    typedef Details::Multicanonical::EnergyTypeExtended<EnergyType> ExtendedEnergyType;
    typedef Details::Multicanonical::ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType> ExtendedConfigurationType;
    typedef Details::Multicanonical::StepTypeExtended<ConfigurationType, StepType, EnergyType> ExtendedStepType;
    // Typedef for the base class
    typedef AlgorithmType<ExtendedConfigurationType, ExtendedStepType, ExtendedEnergyType, HistoType, RandomNumberGenerator> Base;
    // Typedef for the original algorithm
    typedef AlgorithmType<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator> OriginalAlgorithmType;

    //! Initialise a multicanonical counting simulation with default initial configuration and default reference configuration
    MulticanonicalCounting(const ParametersOriginalType& params) 
      : Base(params) {}
    //! Initialise a multicanonical counting simulation with given initial configuration space that will be also used as a reference configuration
    MulticanonicalCounting(const ParametersOriginalType& params, ConfigurationType* initial_configuration)
      : Base(params, new ExtendedConfigurationType(initial_configuration)) {}
    //! Initialise a multicanonical counting simulation with given initial configuration and given reference configuration
    MulticanonicalCounting(const ParametersOriginalType& params, ConfigurationType* initial_configuration, ConfigurationType* reference_configuration)
      : Base(params, new ExtendedConfigurationType(initial_configuration, reference_configuration)) {}

  private:
    // Configuration that will be used as a comparison for the reference bin
    ConfigurationType* reference_configuration;

    // Extended configuration that will be used for the simulations
    ExtendedConfigurationType* extended_configuration;

    // Extended parameters that will be used for the simulation
    ParametersExtendedType extended_simulation_parameters;
  };
  
}

#include "src/multicanonical_counting.cpp"

#endif
