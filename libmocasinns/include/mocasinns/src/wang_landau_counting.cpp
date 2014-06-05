/*!
 * \file wang_landau_counting.cpp
 * \brief Implementation of the WangLandauCounting simulation class
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_WANG_LANDAU_COUNTING_HPP





template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
typename WangLandauCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::ParametersExtendedType WangLandauCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::convert_parameters(const ParametersOriginalType& user_parameters)
{
  ParametersExtendedType parameters_result;
  parameters_result.binning_reference = EnergyTypeExtended<EnergyType>(user_parameters.binning_reference, 0);
  parameters_result.binning_width = EnergyTypeExtended<EnergyType>(user_parameters.binning_width, 1);
  parameters_result.energy_cutoff_use = user_parameters.energy_cutoff_use;
  parameters_result.energy_cutoff = EnergyTypeExtended<EnergyType>(user_parameters.energy_cutoff, 0);
  parameters_result.flatness = user_parameters.flatness;
  parameters_result.modification_factor_initial = user_parameters.modification_factor_initial;
  parameters_result.modification_factor_final = user_parameters.modification_factor_final;
  parameters_result.modification_factor_modifyer = user_parameters.modification_factor_modifyer;
  parameters_result.sweep_steps = user_parameters.sweep_steps;
  return parameters_result;
}

template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
WangLandauCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::WangLandauCounting(const ParametersOriginalType& params)
  : WangLandauType(convert_parameters(params), ConfigTypeExt(new ConfigurationType()))
{ }

template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
WangLandauCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::WangLandauCounting(const ParametersOriginalType& params, ConfigurationType* initial_configuration) 
  : WangLandauType(convert_parameters(params), ConfigTypeExt(initial_configuration))
{ }
		   
template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
WangLandauCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::WangLandauCounting(const ParametersOriginalType& params, ConfigurationType* initial_configuration, ConfigurationType* reference_configuration) 
  : WangLandauType(convert_parameters(params), ConfigTypeExt(initial_configuration, reference_configuration))
{ }

template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
double WangLandauCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::do_wang_landau_counting()
{
  // Execute the simulation
  this->do_wang_landau_simulation();
  
  // Extract the total number of states
  double number_of_states = 0;

  return number_of_states;
}

#endif
