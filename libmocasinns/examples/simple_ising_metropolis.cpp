// Example program for calculating the mean energy of an Ising chain. Compile using
// g++ -std=c++11 -I../include simple_ising_metropolis.cpp -lboost_serialization -o simple_ising_metropolis

#include <iostream>
#include "simple_ising.hpp"

#include <mocasinns/metropolis.hpp>
#include <mocasinns/random/boost_random.hpp>

typedef Mocasinns::Metropolis<IsingConfiguration, IsingStep, Mocasinns::Random::Boost_MT19937> MetropolisSimulation;

int main()
{
  typename MetropolisSimulation::Parameters parameters;
  parameters.measurement_number = 1000;
  parameters.relaxation_steps = 10000;
  parameters.steps_between_measurement = 10000;
  
  IsingConfiguration configuration(16);
  MetropolisSimulation simulation(parameters, &configuration);
  
  std::vector<int> energies = simulation.do_metropolis_simulation(1.0);
  
  double mean_energy = 0.0;
  for (unsigned int i = 0; i < parameters.measurement_number; ++i)
    mean_energy += static_cast<double>(energies[i])/parameters.measurement_number;
  
  std::cout << "Mean energy: " << mean_energy << std::endl;
}
