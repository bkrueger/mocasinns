// Example program for calculating the mean energy of an Ising chain. Compile using
// g++ -std=c++11 -I../include accumulator.cpp -lboost_serialization -o accumulator

#include <iostream>
#include "simple_ising.hpp"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>

#include <mocasinns/metropolis.hpp>
#include <mocasinns/random/boost_random.hpp>
#include <mocasinns/accumulators/file_accumulator.hpp>

namespace ba = boost::accumulators;

typedef Mocasinns::Metropolis<IsingConfiguration, IsingStep, Mocasinns::Random::Boost_MT19937> MetropolisSimulation;

int main()
{
  typename MetropolisSimulation::Parameters parameters;
  parameters.measurement_number = 1000;
  parameters.relaxation_steps = 10000;
  parameters.steps_between_measurement = 10000;
  
  IsingConfiguration configuration(16);
  MetropolisSimulation simulation(parameters, &configuration);

  ba::accumulator_set<int, ba::stats<ba::tag::mean> > boost_accumulator;
  Mocasinns::Accumulators::FileAccumulator<int> file_accumulator("filename.dat");
  
  simulation.do_metropolis_simulation(1.0, boost_accumulator);
  simulation.do_metropolis_simulation(1.0, file_accumulator);
  
  std::cout << "Mean energy: " << ba::mean(boost_accumulator) << std::endl;
}
