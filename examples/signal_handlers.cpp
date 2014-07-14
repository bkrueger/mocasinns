// Example for using signal handlers
// g++ -std=c++11 -I../include signal_handlers.cpp -lboost_serialization -o signal_handlers

#include <iostream>
#include <ctime>
#include "simple_ising.hpp"

#include <mocasinns/wang_landau.hpp>
#include <mocasinns/random/boost_random.hpp>
#include <mocasinns/histograms/histocrete.hpp>

typedef Mocasinns::Simulation<IsingConfiguration, Mocasinns::Random::Boost_MT19937> Simulation;
typedef Mocasinns::WangLandau<IsingConfiguration, IsingStep, int, Mocasinns::Histograms::Histocrete, Mocasinns::Random::Boost_MT19937> WangLandauSimulation;

// Start time
time_t start_time;

void write_information(Simulation* simulation_base)
{
  WangLandauSimulation* simulation_wl = static_cast<WangLandauSimulation*>(simulation_base);

  // Get the actual time
  const time_t actual_time = time(NULL);
  char world_time[16];
  strftime(world_time, 16, "%Y%m%d-%H%M%S", gmtime(&actual_time));

  // Write the worldtime
  std::cout << world_time << "\t";
  // Write the walltime
  std::cout << actual_time - start_time << "\t";
  // Write the simulation specific information
  std::cout << simulation_wl->get_sweep_counter() << "\t";
  std::cout << std::setprecision(8) << simulation_wl->get_modification_factor_current() << "\t";
  std::cout << simulation_wl->get_incidence_counter().flatness() << std::endl;
}

void write_histograms(Simulation* simulation_base)
{
  WangLandauSimulation* simulation_wl = static_cast<WangLandauSimulation*>(simulation_base);

  std::cout << "Incidence counter: " << std::endl;
  std::cout << simulation_wl->get_incidence_counter() << std::endl;
  std::cout << "Density of states: " << std::endl;
  std::cout << simulation_wl->get_log_density_of_states() << std::endl;
}

int main()
{
  IsingConfiguration configuration(64);
  WangLandauSimulation simulation(WangLandauSimulation::Parameters(), &configuration);
  start_time = time(NULL);

  simulation.signal_handler_modfac_change.connect(&write_information);
  simulation.signal_handler_sigusr1.connect(&write_histograms);

  simulation.do_wang_landau_simulation();
  
  typedef Mocasinns::Histograms::Histocrete<int, double> dos_t; 
  dos_t log_density_of_states = simulation.get_log_density_of_states();
  for (dos_t::const_iterator it = log_density_of_states.begin(); it != log_density_of_states.end(); ++it)
    std::cout << "ln g(E = " << it->first << ") = " << it->second << std::endl;
}
