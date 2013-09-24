/**
 * \file wang_landau_simulation_statis.hpp
 * \brief Contains a functor that can be used to write information about the current state of the simulation to the standard output.
 * 
 * Do not include the file, but include <mocasinns/wang_landau.hpp>
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_WANG_LANDAU_WANG_LANDAU_SIMULATION_STATUS_HPP
#define MOCASINNS_WANG_LANDAU_WANG_LANDAU_SIMULATION_STATUS_HPP

#include "wang_landau_base.hpp"

namespace Mocasinns
{
  //! Struct used for signals to write the simulation status into std::cout
  template<class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  struct WangLandauBase<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, rejection_free>::SimulationStatus
  {
    //! Operator for for calling the SimulationStatus function
    /*!
     * \param simulation Pointer to the simulation of which the information should be written
     * \details Writes the following information in this order: simulation_time, modfactor_current, incidence_flatness
     */
    void operator()(Simulation<ConfigurationType,RandomNumberGenerator>* simulation)
    {
      WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>* simulation_wang_landau = static_cast<WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>*>(simulation);
      std::cout << simulation_wang_landau->get_config_space()->get_simulation_time() << "\t";
      std::cout << simulation_wang_landau->get_modification_factor_current() << "\t";
      std::cout << simulation_wang_landau->get_incidence_counter().flatness() << std::endl;
    }
  };
}

#endif
