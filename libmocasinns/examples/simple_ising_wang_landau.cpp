// Example program for calculating the density of states of an Ising chain using the Wang-Landau algorithm
// g++ -std=c++11 -I../include simple_ising_wang_landau.cpp -lboost_serialization -o simple_ising_wang_landau

#include <iostream>
#include "simple_ising.hpp"

#include <mocasinns/wang_landau.hpp>
#include <mocasinns/random/boost_random.hpp>
#include <mocasinns/histograms/histocrete.hpp>

typedef Mocasinns::WangLandau<IsingConfiguration, IsingStep, int, Mocasinns::Histograms::Histocrete, Mocasinns::Random::Boost_MT19937> WangLandauSimulation;

int main()
{
  IsingConfiguration configuration(16);
  WangLandauSimulation simulation(WangLandauSimulation::Parameters(), &configuration);

  simulation.do_wang_landau_simulation();
  
  typedef Mocasinns::Histograms::Histocrete<int, double> dos_t; 
  dos_t log_density_of_states = simulation.get_log_density_of_states();
  for (dos_t::const_iterator it = log_density_of_states.begin(); it != log_density_of_states.end(); ++it)
    std::cout << "ln g(E = " << it->first << ") = " << it->second << std::endl;
}
