/*!
 * \file wang_landau_counting.hpp
 * \brief Class for counting the number of states using the Wang-Landau algorithm with additional ground state bin
 * 
 * Extends the given configuration space to a new configuration space with modified energy and delta E that determines whether the configuration is in the ground state or not. Usage examples are found in the test cases. 
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_WANG_LANDAU_COUNTING_HPP
#define MOCASINNS_WANG_LANDAU_COUNTING_HPP

#include "wang_landau.hpp"



//! Class for Metropolis-Monte-Carlo simulations
template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
class WangLandauCounting 
  : public WangLandau<ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>, 
		      StepTypeExtended<ConfigurationType, StepType, EnergyType>, 
		      EnergyTypeExtended<EnergyType>, 
		      HistoType, RandomNumberGenerator>
{
  typedef ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType> ConfTypeExt;
  typedef StepTypeExtended<ConfigurationType, StepType, EnergyType> StpTypeExt;
  typedef WangLandau<ConfTypeExt, StpTypeExt, EnergyTypeExtended<EnergyType>, HistoType, RandomNumberGenerator> WangLandauType;

  typedef typename WangLandauType::template Parameters<EnergyType> ParametersOriginalType;
  typedef typename WangLandauType::template Parameters<EnergyTypeExtended<EnergyType> > ParametersExtendedType;

private:
  //! Convert parameters with different energy types
  static ParametersExtendedType convert_parameters(const ParametersOriginalType& user_parameters);

public:
  //! Initialise a WangLandauCounting-MC simulation with default initial configuration and default reference configuration
  WangLandauCounting(const ParametersOriginalType& params);
  //! Initialise a WangLandauCounting-MC simulation with given initial configuration space that will be also used as a reference configuration
  WangLandauCounting(const ParametersOriginalType& params, ConfigurationType* initial_configuration);
  //! Initialise a WangLandauCounting-MC simulation with given initial configuration and given reference configuration
  WangLandauCounting(const ParametersOriginalType& params, ConfigurationType* initial_configuration, ConfigurationType* reference_configuration);

  //! Do a complete wang-landau simulation to count the number of states
  double do_wang_landau_counting();
};

#include "src/wang_landau_counting.cpp"

#endif
