// Includes from libising
#include "simple_ising.hpp"
// Includes from libmocasinns
#include <mocasinns/metropolis.hpp>
#include <mocasinns/observables/array_observable.hpp>
// Includes from librandom
#include <random_boost_mt19937.hpp>

// Typedef for the Simulation
typedef Mocasinns::Metropolis<SimpleIsingSystem, SimpleIsingStep, Boost_MT19937> Simulation;

int main()
{
  SimpleIsingSystem lattice(10);
  std::cout << "Groundstate energy: " << lattice.energy() << std::endl;

  // Set the parameters of the simulation
  Simulation::Parameters simulation_parameters;
  simulation_parameters.relaxation_steps = 10*10*1000;
  simulation_parameters.measurement_number = 1000;
  simulation_parameters.steps_between_measurement = 10*10*100;

  // Create the simulation
  Simulation sim(simulation_parameters, &lattice);
  sim.set_random_seed(0);

  // Do the simulation for temperature beta = 0.1
  std::vector<int> results = sim.do_metropolis_simulation(0.1);

  // Calculate the mean energy and the mean magnetization
  double mean_energy = 0.0;
  for (unsigned int m = 0; m < simulation_parameters.measurement_number; ++m)
  {
    mean_energy += results[m]/static_cast<double>(simulation_parameters.measurement_number);
  }
  
  std::cout << "Mean energy: " << mean_energy << std::endl;
}
