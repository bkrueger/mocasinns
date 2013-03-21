#include <vector>
#include <iostream>

// Includes from boost::accumulator
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/density.hpp>

namespace ba = boost::accumulators;

// Includes from libising
#include <spinlattice.hpp>
#include <spin_ising.hpp>
// Includes from libmocasinns
#include <mocasinns/metropolis.hpp>
#include <mocasinns/histograms/histogram.hpp>
#include <mocasinns/accumulators/histogram_accumulator.hpp>
#include <mocasinns/random/boost_random.hpp>

// Typedef for the configuration and the step
typedef Ising::SpinLattice<2, Ising::SpinIsing> ConfigurationType;
typedef Ising::Step<2, Ising::SpinIsing> StepType;

// Typedef for the Simulation
typedef Mocasinns::Metropolis<ConfigurationType, StepType, Mocasinns::Random::Boost_MT19937> Simulation;

// Class for observing the energy and the magnetization of the ising-system
struct IsingEnergyObserver
{
  typedef double observable_type;
  static observable_type observe(ConfigurationType* config)
  {
    return config->energy();
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
  simulation_parameters.measurement_number = 10000;
  simulation_parameters.steps_between_measurement = 10*10*100;

  // Create the configuration
  Simulation sim(simulation_parameters, &lattice);
  sim.set_random_seed(0);

  // Define an accumulator with an standard observable_type to copy
  Mocasinns::Accumulators::HistogramAccumulator<Mocasinns::Histograms::HistogramNumber, double> acc;
  acc.set_binning_width(2.0);
  acc.set_binning_reference(0.0);

  // Do the simulation for temperature beta = 0.1
  sim.do_metropolis_simulation<IsingEnergyObserver>(0.1, acc);

  // Extract the histogram
  Mocasinns::Histograms::HistogramNumber<double, double> histo = acc.normalized_histogram();
  for (Mocasinns::Histograms::HistogramNumber<double, double>::const_iterator it = histo.begin(); it != histo.end(); ++it)
  {
    std::cout << "Bin lower bound: " << it->first << ", value: " << it->second << std::endl;
  }

}
