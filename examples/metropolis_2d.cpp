// Example program for calculating the mean energy of an Ising chain. Compile using
// g++ -std=c++11 -I../include metropolis_2d.cpp -lboost_serialization -o metropolis_2d

#include <iostream>
#include "simple_ising_2d.hpp"

#include <mocasinns/metropolis.hpp>
#include <mocasinns/random/boost_random.hpp>

typedef Mocasinns::Simulation<IsingConfiguration2d,  Mocasinns::Random::Boost_MT19937> Simulation;
typedef Mocasinns::Metropolis<IsingConfiguration2d, IsingStep2d, Mocasinns::Random::Boost_MT19937> MetropolisSimulation;

void measure(Simulation* simulation_base)
{
  MetropolisSimulation* simulation_metropolis = static_cast<MetropolisSimulation*>(simulation_base);

  unsigned int spin_number = simulation_metropolis->get_config_space()->size_x * simulation_metropolis->get_config_space()->size_y;
  std::cout << static_cast<double>(simulation_metropolis->get_config_space()->energy()) / spin_number << "\t" << static_cast<double>(simulation_metropolis->get_config_space()->magnetization()) / spin_number << std::endl;
}

int main(int argc, char* argv[])
{
  // Check and read command line arguments
  if (argc != 4)
  {
    std::cerr << "ERROR: Use three command line parameters: size, inverse temperature and external field" << std::endl;
    return 1;
  }
  unsigned int size = atoi(argv[1]);
  double beta = atof(argv[2]);
  double H = atof(argv[3]);

  typename MetropolisSimulation::Parameters parameters;
  parameters.measurement_number = 1000;
  parameters.relaxation_steps = 1000*size*size;
  parameters.steps_between_measurement = 100*size*size;
  
  IsingConfiguration2d configuration(size, size);
  configuration.external_field = H;
  MetropolisSimulation simulation(parameters, &configuration);
  simulation.signal_handler_measurement.connect(&measure);
  
  simulation.do_metropolis_simulation(beta);
}
