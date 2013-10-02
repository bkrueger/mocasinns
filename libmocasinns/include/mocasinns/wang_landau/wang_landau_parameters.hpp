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

#include "../details/multicanonical/parameters_multicanonical.hpp"

#include <boost/serialization/base_object.hpp>

namespace Mocasinns
{

  //! Struct for dealing with the parameters of a Wang-Landau-simulation
  template<class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  struct WangLandauBase<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, rejection_free>::Parameters : public Details::Multicanonical::ParametersMulticanonical<EnergyType>
  {
  public:
    //! Typedef for the base class
    typedef Details::Multicanonical::ParametersMulticanonical<EnergyType> Base;

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
      Base(),
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
      Base(other),
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
      return ((static_cast<Base>(*this) == static_cast<Base>(rhs)) &&
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
      ar & boost::serialization::base_object<Base>(*this);
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
