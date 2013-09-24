/**
 * \file wang_landau_simulation_dump.hpp
 * \brief Contains a functor that can be used to dump the current state of the simulation into the specified dump file.
 * 
 * Do not include the file, but include <mocasinns/wang_landau.hpp>
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_WANG_LANDAU_WANG_LANDAU_SIMULATION_DUMP_HPP
#define MOCASINNS_WANG_LANDAU_WANG_LANDAU_SIMULATION_DUMP_HPP

#include "wang_landau_base.hpp"

namespace Mocasinns
{
  //! Struct used for signals to dump the whole simulation into the specified file
  template<class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  struct WangLandauBase<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, rejection_free>::SimulationDump
  {
    //! Operator for calling the SimulationDump function
    /*!
     * \param simulation Pointer to the simulation that should be dumped
     * \param outstream Output stream where the information is written in
     */
    void operator()(Simulation<ConfigurationType,RandomNumberGenerator>* simulation)
    {
      WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>* simulation_wang_landau = static_cast<WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>*>(simulation);
      simulation_wang_landau->save_serialize(simulation->get_dump_filename().c_str());
    }

  };
}

#endif
