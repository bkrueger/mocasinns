#include <vector>
#include <iostream>

// Includes from boost::accumulator
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>

namespace ba = boost::accumulators;

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

  // Define an accumulator with an standard observable_type to copy
  ba::accumulator_set<typename IsingEnergyObserver::observable_type,
		      ba::stats<ba::tag::mean, ba::tag::variance> > acc;

  // Do the simulation for temperature beta = 0.1
  sim.do_metropolis_simulation<IsingEnergyObserver>(0.1, acc);

  std::cout << "Mean energy: " << ba::mean(acc)[0] << std::endl;
  std::cout << "Energy variance: " << ba::variance(acc)[0] << std::endl;
  std::cout << "Mean magnetization: " << ba::mean(acc)[1] << std::endl;
  std::cout << "Magnetization variance: " << ba::variance(acc)[1] << std::endl;
}
