// Example program for calculating the mean energy of an Ising chain. Compile using
// g++ -std=c++11 -I../include simple_ising_metropolis_observator.cpp -lboost_serialization -o simple_ising_metropolis_observator

#include <iostream>
#include "simple_ising.hpp"

#include <mocasinns/metropolis.hpp>
#include <mocasinns/random/boost_random.hpp>

struct SpecificMagnetizationObservator
{
  typedef double observable_type;
  static observable_type observe(IsingConfiguration* system)
  {
    double result = 0;
    for (unsigned int i = 0; i < system->spins.size(); ++i)
      result += system->spins[i];
    return result/system->spins.size();
  }
};

typedef Mocasinns::Metropolis<IsingConfiguration, IsingStep, Mocasinns::Random::Boost_MT19937> MetropolisSimulation;

int main()
{
  typename MetropolisSimulation::Parameters parameters;
  parameters.measurement_number = 1000;
  parameters.relaxation_steps = 10000;
  parameters.steps_between_measurement = 10000;
  
  IsingConfiguration configuration(16);
  MetropolisSimulation simulation(parameters, &configuration);
  
  std::vector<double> specific_magnetizations = simulation.do_metropolis_simulation<SpecificMagnetizationObservator>(1.0);
  
  double mean_magnetization = 0.0;
  for (unsigned int i = 0; i < parameters.measurement_number; ++i)
    mean_magnetization += static_cast<double>(specific_magnetizations[i])/parameters.measurement_number;
  
  std::cout << "Mean specific magnetization: " << mean_magnetization << std::endl;
}
