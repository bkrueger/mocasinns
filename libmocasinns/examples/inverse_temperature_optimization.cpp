// Example program for calculating the mean energy of an Ising chain using parallel tempering. Compile using
// g++ -std=c++11 -I../include inverse_temperature_optimization.cpp -lboost_serialization -fopenmp -o inverse_temperature_optimization

#include <iostream>
#include "simple_ising.hpp"

#include <mocasinns/parallel_tempering.hpp>
#include <mocasinns/details/parallel_tempering/inverse_temperature_optimization_equal_acceptance_probabilities.hpp>
#include <mocasinns/random/boost_random.hpp>

typedef Mocasinns::ParallelTempering<IsingConfiguration, IsingStep, Mocasinns::Random::Boost_MT19937> ParallelTemperingSimulation;
typedef Mocasinns::Details::ParallelTempering::InverseTemperatureOptimizationEqualAcceptanceProbabilities<ParallelTemperingSimulation, double, Mocasinns::Details::ParallelTempering::WeightWorstAcceptance> InverseTemperatureOptimizationType;

int main()
{
  typename ParallelTemperingSimulation::Parameters parameters;
  parameters.measurement_number = 1000;
  parameters.relaxation_steps = 10000;
  parameters.steps_between_measurement = 10000;
  parameters.steps_between_replica_exchange = 100;
  parameters.process_number = 4;

  std::vector<IsingConfiguration*> configurations;
  for (unsigned int c = 0; c < 8; ++c) configurations.push_back(new IsingConfiguration(16));
  ParallelTemperingSimulation simulation(parameters, configurations.begin(), configurations.end());
  
  std::vector<double> inverse_temperatures;
  inverse_temperatures.push_back(-0.35); inverse_temperatures.push_back(-0.25); inverse_temperatures.push_back(-0.15); inverse_temperatures.push_back(-0.05); 
  inverse_temperatures.push_back(0.05); inverse_temperatures.push_back(0.15); inverse_temperatures.push_back(0.25); inverse_temperatures.push_back(0.35); 

  InverseTemperatureOptimizationType inverse_temperature_optimization(&simulation, 100);
  inverse_temperature_optimization.optimize(inverse_temperatures.begin(), inverse_temperatures.end());

  std::vector<std::vector<int> > energies = simulation.do_parallel_tempering_simulation(inverse_temperatures.begin(), inverse_temperatures.end());
  
  std::vector<double> mean_energies(inverse_temperatures.size(), 0.0);
  for (unsigned int b = 0; b < inverse_temperatures.size(); ++b)
    for (unsigned int i = 0; i < parameters.measurement_number; ++i)
      mean_energies[b] += static_cast<double>(energies[b][i])/parameters.measurement_number;
  
  std::cout << "Mean energies: " << std::endl;
  for (unsigned int b = 0; b < inverse_temperatures.size(); ++b)
	 std::cout << "beta = " << inverse_temperatures[b] << ":\t" << mean_energies[b] << std::endl;
}
