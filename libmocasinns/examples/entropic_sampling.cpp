// Example program for calculating the density of states of an Ising chain using the entropic sampling algorithm
// g++ -std=c++11 -I../include entropic_sampling.cpp -lboost_serialization -o entropic_sampling

#include <iostream>
#include "simple_ising.hpp"

#include <mocasinns/entropic_sampling.hpp>
#include <mocasinns/random/boost_random.hpp>
#include <mocasinns/histograms/histocrete.hpp>

typedef Mocasinns::EntropicSampling<IsingConfiguration, IsingStep, int, Mocasinns::Histograms::Histocrete, Mocasinns::Random::Boost_MT19937> EntropicSamplingSimulation;

int main()
{
  IsingConfiguration configuration(16);
  EntropicSamplingSimulation simulation(EntropicSamplingSimulation::Parameters(), &configuration);

  simulation.do_entropic_sampling_simulation();
  
  typedef Mocasinns::Histograms::Histocrete<int, double> dos_t; 
  dos_t log_density_of_states = simulation.get_log_density_of_states();
  for (dos_t::const_iterator it = log_density_of_states.begin(); it != log_density_of_states.end(); ++it)
    std::cout << "ln g(E = " << it->first << ") = " << it->second << std::endl;
}
