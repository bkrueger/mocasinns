/**
 * \file metropolis_parameters.hpp
 * \brief Contains the declaration and the definition of the parameter class of the Metropolis algorithms
 * 
 * Do not include the file, but include <mocasinns/wang_landau.hpp>
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_METROPOLIS_METROPOLIS_PARAMETERS_HPP
#define MOCASINNS_METROPOLIS_METROPOLIS_PARAMETERS_HPP

#include "metropolis_base.hpp"

namespace Mocasinns
{
  //! Struct storing the definition of the Parameters of a Metropolis-Simulation
  template <class ConfigurationType, class StepType, class RandomNumberGenerator, bool rejection_free>
  struct MetropolisBase<ConfigurationType, StepType, RandomNumberGenerator, rejection_free>::Parameters
  {
    //! Number of steps to perform before taking data
    StepNumberType relaxation_steps;
    //! Number of data points per temperature
    MeasurementNumberType measurement_number;
    //! Number of steps to perform between two data measurements
    StepNumberType steps_between_measurement;
    //! Number of measurements to perform before calling each signal
    unsigned int measurements_per_signal;
    
    //! Standard constructor for setting default values
    Parameters() : relaxation_steps(1000),
		   measurement_number(100),
		   steps_between_measurement(100),
		   measurements_per_signal(1) {}
  };

}

#endif
