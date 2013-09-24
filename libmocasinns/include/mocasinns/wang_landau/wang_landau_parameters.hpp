/**
 * \file wang_landau_parameters.hpp
 * \brief Contains the declaration and the definition of the parameter class of the Wang-Landau algorithms
 * 
 * Do not include the file, but include <mocasinns/wang_landau.hpp>
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_WANG_LANDAU_WANG_LANDAU_PARAMETERS_HPP
#define MOCASINNS_WANG_LANDAU_WANG_LANDAU_PARAMETERS_HPP

#include "wang_landau_base.hpp"

namespace Mocasinns
{

  //! Struct for dealing with the parameters of a Wang-Landau-simulation
  template<class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  struct WangLandauBase<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, rejection_free>::Parameters
  {
  public:
    //! Energy value that is used as a reference point for the binning
    EnergyType binning_reference;
    //! Energy value range that is comprehended in one bin
    EnergyType binning_width;
    
    //! Value of the minimal energy cutoff
    EnergyType energy_cutoff_lower;
    //! Value of the maximal energy cutoff
    EnergyType energy_cutoff_upper;
    //! Flag indicating whether to use the minimal energy cutoff, default value is false
    bool use_energy_cutoff_lower;
    //! Flag indicating whether to use the maximal energy cutoff, default value is false
    bool use_energy_cutoff_upper;
    
    //! Flatness below that the modification factor is changed.
    double flatness;
    
    //! Modification factor for the entropy histogram at the beginning of the simulation
    double modification_factor_initial;
    //! Modification factor for the entropy histogram at the end of the simulation, triggers the exit
    double modification_factor_final;
    //! Factor to modify the multiplication factor after the desired flatness has been reached
    double modification_factor_multiplier;
    
    //! Number of steps to take before checking again the flatness
    StepNumberType sweep_steps;
    
    //! Prototype histogram for all settings that the histograms of the simulation can have (e.g. binning width ...)
    HistoType<EnergyType, IncidenceCounterYValueType> prototype_histo;
    
    //! Number of sweeps that should be executed before a reset of the incidence counter and recalculation. Choose 0 (default) for no resets.
    unsigned int reset_sweep_number;
    
    //! Constructor to set default values
    Parameters() :
      binning_reference(0), 
      binning_width(1), 
      energy_cutoff_lower(0),
      energy_cutoff_upper(0),
      use_energy_cutoff_lower(false),
      use_energy_cutoff_upper(false),
      flatness(0.8),
      modification_factor_initial(1.0),
      modification_factor_final(1e-7),
      modification_factor_multiplier(0.9),
      sweep_steps(1000),
      prototype_histo(),
      reset_sweep_number(0) { }
    
    //! Constructor to copy parameters from other parameters with convertible energy type
    template <class OtherParametersType>
    explicit Parameters(const OtherParametersType& other) :
      binning_reference(other.binning_reference), 
      binning_width(other.binning_width), 
      energy_cutoff_lower(other.energy_cutoff_lower),
      energy_cutoff_upper(other.energy_cutoff_upper),
      use_energy_cutoff_lower(other.use_energy_cutoff_lower),
      use_energy_cutoff_upper(other.use_energy_cutoff_upper),
      flatness(other.flatness),
      modification_factor_initial(other.modification_factor_initial),
      modification_factor_final(other.modification_factor_final),
      modification_factor_multiplier(other.modification_factor_multiplier),
      sweep_steps(other.sweep_steps),
      prototype_histo(other.prototype_histo),
      reset_sweep_number(other.reset_sweep_number) { }
    
    //! Test for equality
    bool operator==(const Parameters& rhs) const
    {
      return ((binning_reference == rhs.binning_reference) &&
	      (binning_width == rhs.binning_width) &&
	      (energy_cutoff_lower == rhs.energy_cutoff_lower) && 
	      (energy_cutoff_upper == rhs.energy_cutoff_upper) &
	      (use_energy_cutoff_lower == rhs.use_energy_cutoff_lower) && 
	      (use_energy_cutoff_upper == rhs.use_energy_cutoff_upper) &&
	      (flatness == rhs.flatness) &&
	      (modification_factor_initial == rhs.modification_factor_initial) &&
	      (modification_factor_final == rhs.modification_factor_final) &&
	      (modification_factor_multiplier == rhs.modification_factor_multiplier) &&
	      (sweep_steps == rhs.sweep_steps) && 
	      (reset_sweep_number == rhs.reset_sweep_number));
    }
    
    //! Test for inequality
    bool operator!=(const Parameters& rhs) const
    {
      return !operator==(rhs);
    }
    
  private:
    //! Member variable for boost serialization
    friend class boost::serialization::access;
    //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
    template<class Archive> void serialize(Archive & ar, const unsigned int)
    {
      // serialize base class information
      ar & binning_reference;
      ar & binning_width;
      ar & energy_cutoff_lower;
      ar & energy_cutoff_upper;
      ar & use_energy_cutoff_lower;
      ar & use_energy_cutoff_upper;
      ar & flatness;
      ar & modification_factor_initial;
      ar & modification_factor_final;
      ar & modification_factor_multiplier;
      ar & sweep_steps;
      ar & prototype_histo;
    }
  };  
}

#endif
