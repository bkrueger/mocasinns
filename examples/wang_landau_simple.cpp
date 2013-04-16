// Includes from libising
#include <gespinst/spin_lattice.hpp>
#include <gespinst/spins/ising_spin.hpp>
// Includes from libmocasinns
#include <mocasinns/wang_landau.hpp>
#include <mocasinns/histograms/histocrete.hpp>
#include <mocasinns/random/boost_random.hpp>

// Typedef for the configuration and the step
typedef Gespinst::SpinLattice<2, Gespinst::IsingSpin> ConfigurationType;
typedef Gespinst::SpinLatticeStep<2, Gespinst::IsingSpin> StepType;

// Typedef for the Simulation
typedef Mocasinns::WangLandau<ConfigurationType, StepType, int, Mocasinns::Histograms::Histocrete, Mocasinns::Random::Boost_MT19937> Simulation;

int main()
{
  // Create a configuration of size 10x10
  std::vector<unsigned int> size(2, 10);
  ConfigurationType lattice(size);

  // Set the parameters of the simulation
  Simulation::Parameters simulation_parameters;
  simulation_parameters.modification_factor_final = 1e-6;
  simulation_parameters.modification_factor_multiplier = 0.9;
  simulation_parameters.flatness = 0.8;

  // Create the simulation
  Simulation sim(simulation_parameters, &lattice);
  sim.set_random_seed(0);

  // Do the Wang-Landau-simulation
  sim.do_wang_landau_simulation();

  // Extract the density of states
  Mocasinns::Histograms::Histocrete<int, double> dos_log = sim.get_density_of_states();
  std::cout << "E\tg(E)" << std::endl;
  for (Mocasinns::Histograms::Histocrete<int, double>::const_iterator it = dos_log.begin(); it != dos_log.end(); ++it)
  {
    std::cout << it->first << "\t" << exp(it->second) << std::endl;
  }
}
