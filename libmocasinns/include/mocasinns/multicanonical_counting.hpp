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

#include "details/multicanonical/energy_type_extended.hpp"
#include "details/multicanonical/configuration_type_extended.hpp"
#include "details/multicanonical/step_type_extended.hpp"

namespace Mocasinns
{
  
  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator,
	    template<class,class,class,template<class,class> class,class> class AlgorithmType>
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
    // Typedef for the extended parameters
    typedef typename Base::Parameters ParametersExtendedType;
    // Typedef for the original algorithm
    typedef AlgorithmType<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator> OriginalAlgorithmType;
    // Typedef for the original parameters
    typedef typename OriginalAlgorithmType::Parameters ParametersOriginalType;

    //! Initialise a multicanonical counting simulation with default initial configuration and default reference configuration
    MulticanonicalCounting(const ParametersOriginalType& params) 
      : Base(typename Base::Parameters(params)) {}
    //! Initialise a multicanonical counting simulation with given initial configuration space that will be also used as a reference configuration
    MulticanonicalCounting(const ParametersOriginalType& params, ConfigurationType* initial_configuration)
      : Base(typename Base::Parameters(params), new ExtendedConfigurationType(initial_configuration)) {}
    //! Initialise a multicanonical counting simulation with given initial configuration and given reference configuration
    MulticanonicalCounting(const ParametersOriginalType& params, ConfigurationType* initial_configuration, ConfigurationType* reference_configuration)
      : Base(typename Base::Parameters(params), new ExtendedConfigurationType(initial_configuration, reference_configuration)) {}

    //! Calculate the normalised density of states, override the base class function
    HistoType<EnergyType, double> get_log_density_of_states() const;
    //! Set an estimation of the normalised density of states, override the base class function
    void set_log_density_of_states(const HistoType<EnergyType, double>& value);
    //! Return the extended density of states with ground state flag
    HistoType<ExtendedEnergyType, double> get_log_density_of_states_extended() const
    {
      // Get the original density of states
      HistoType<ExtendedEnergyType, double> result(static_cast<const Base*>(this)->get_log_density_of_states());
      
      // Shift the reference bin to zero
      ExtendedEnergyType reference_energy(this->get_config_space()->get_reference_configuration_energy(), 1);
      typename HistoType<ExtendedEnergyType, double>::const_iterator reference_bin = result.find(reference_energy);
      result.shift_bin_zero(reference_bin);

      return result;
    }
    //! Set the extended density of states with ground state flag
    void set_log_density_of_states_extended(const HistoType<ExtendedEnergyType, double>& value)
    {
      // Shift the reference bin to 0
      ExtendedEnergyType reference_energy(this->get_config_space()->get_reference_configuration_energy(), 1);
      HistoType<ExtendedEnergyType, double> shifted_value(value);
      shifted_value.shift_bin_zero(shifted_value.find(reference_energy));
      static_cast<Base*>(this)->set_log_density_of_states(shifted_value);
    }
    //! Get the number of states based on the density of states that was precalculated using the member functions of the original algorithm
    double number_of_states() const;

    //! DEBUG
    void print_configuration_pointers() const
    {
      std::cout << "Work: " << this->get_config_space()->get_original_configuration() << ", Reference: " << this->get_config_space()->get_reference_configuration() << ", Extended: " << this->get_config_space() << std::endl;
    }

  private:
    // Extended parameters that will be used for the simulation
    ParametersExtendedType extended_simulation_parameters;
  };
  
}

#include "src/multicanonical_counting.cpp"

#endif
