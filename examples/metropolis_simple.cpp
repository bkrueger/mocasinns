#include <vector>
#include <iostream>

// Includes from libising
#include <spinlattice.hpp>
#include <spin_ising.hpp>
// Includes from libmocasinns
#include <metropolis.hpp>
#include <observables/array_observable.hpp>
// Includes from librandom
#include <random_boost_mt19937.hpp>

// Typedef for the configuration and the step
typedef Ising::SpinLattice<2, Ising::SpinIsing> ConfigurationType;
typedef Ising::Step<2, Ising::SpinIsing> StepType;

// Typedef for the Simulation
typedef Mocasinns::Metropolis<ConfigurationType, StepType, Boost_MT19937> Simulation;

// Class for observing the energy and the magnetization of the ising-system
struct IsingEnergyObserver
{
  typedef Mocasinns::Observables::ArrayObservable<double,2> observable_type;
  static observable_type observe(ConfigurationType* config)
  {
    observable_type result;
    result[0] = config->energy();
    result[1] = config->magnetization();
    return result;
  }
};

int main()
{
  // Create a configuration of size 10x10
  std::vector<unsigned int> size(2, 10);
  ConfigurationType lattice(size);
  std::cout << "Groundstate energy: " << lattice.energy() << std::endl;
  std::cout << "Groundstate magnetization: " << lattice.magnetization() << std::endl;

  // Set the parameters of the simulation
  Simulation::Parameters simulation_parameters;
  simulation_parameters.relaxation_steps = 10*10*1000;
  simulation_parameters.measurement_number = 1000;
  simulation_parameters.steps_between_measurement = 10*10*100;

  // Create the configuration
  Simulation sim(simulation_parameters, &lattice);
  sim.set_random_seed(0);

  // Do the simulation for temperature beta = 0.1
  std::vector<typename IsingEnergyObserver::observable_type> results = sim.do_metropolis_simulation<IsingEnergyObserver>(0.1);

  // Calculate the mean energy and the mean magnetization
  double mean_energy = 0.0;
  double mean_magnetization = 0.0;
  for (unsigned int m = 0; m < simulation_parameters.measurement_number; ++m)
  {
    mean_energy += results[m][0]/static_cast<double>(simulation_parameters.measurement_number);
    mean_magnetization += results[m][1]/static_cast<double>(simulation_parameters.measurement_number);
  }
  
  std::cout << "Mean energy: " << mean_energy << std::endl;
  std::cout << "Mean magnetization: " << mean_magnetization << std::endl;
}
