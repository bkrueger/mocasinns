// Example program for calculating the density of states of an Ising chain using the Wang-Landau algorithm
// g++ -std=c++11 -I../include wang_landau.cpp -lboost_serialization -o wang_landau

#include <iostream>
#include "simple_ising.hpp"

#include <mocasinns/wang_landau.hpp>
#include <mocasinns/random/boost_random.hpp>
#include <mocasinns/histograms/histocrete.hpp>
#include <mocasinns/metropolis_hastings.hpp>
#include <mocasinns/observables/array_observable.hpp>

typedef Mocasinns::WangLandau<IsingConfiguration, IsingStep, int, Mocasinns::Histograms::Histocrete, Mocasinns::Random::Boost_MT19937> WangLandauSimulation;
typedef Mocasinns::MetropolisHastings<IsingConfiguration, IsingStep, Mocasinns::Random::Boost_MT19937> MetropolisHastingsSimulation;

struct EnergySpecificMagnetizationObservator
{
  typedef Mocasinns::Observables::ArrayObservable<double, 2> observable_type;
  static observable_type observe(IsingConfiguration* system)
  {
    double magnetization = 0;
    for (unsigned int i = 0; i < system->spins.size(); ++i)
      magnetization += system->spins[i];
    observable_type result;
    result[0] = system->energy();
    result[1] = magnetization/system->spins.size();
    return result;
  }
};

int main()
{
  IsingConfiguration configuration(16);
  WangLandauSimulation wang_landau_simulation(WangLandauSimulation::Parameters(), &configuration);

  wang_landau_simulation.do_wang_landau_simulation();
  typedef Mocasinns::Histograms::Histocrete<int, double> dos_t; 
  dos_t log_density_of_states = wang_landau_simulation.get_log_density_of_states();
   
  MetropolisHastingsSimulation::FlatHistogramAcceptanceProbability<int, Mocasinns::Histograms::Histocrete> acceptance_probability_functor;
  acceptance_probability_functor.log_density_of_states = log_density_of_states;

  MetropolisHastingsSimulation::Parameters parameters;
  parameters.measurement_number = 1000;
  parameters.relaxation_steps = 10000;
  parameters.steps_between_measurement = 10000;
  MetropolisHastingsSimulation metropolis_hastings_simulation(parameters, &configuration);
 
  std::vector<EnergySpecificMagnetizationObservator::observable_type> energy_specific_magnetizations 
    = metropolis_hastings_simulation.do_metropolis_hastings_simulation<EnergySpecificMagnetizationObservator>(acceptance_probability_functor);
  Mocasinns::Histograms::Histocrete<int, double> specific_magnetizations_histogram;
  Mocasinns::Histograms::Histocrete<int, double> measurement_counter;
  for (unsigned int m = 0; m < energy_specific_magnetizations.size(); ++m)
  {
    if (specific_magnetizations_histogram.find(energy_specific_magnetizations[m][0]) != specific_magnetizations_histogram.end())
    {
      specific_magnetizations_histogram[energy_specific_magnetizations[m][0]] += energy_specific_magnetizations[m][1];
      measurement_counter[energy_specific_magnetizations[m][0]] += 1;
    }
    else
    {
      specific_magnetizations_histogram[energy_specific_magnetizations[m][0]] = energy_specific_magnetizations[m][1];
      measurement_counter[energy_specific_magnetizations[m][0]] = 1;
    }
  }
  std::cout << specific_magnetizations_histogram << std::endl;
  std::cout << measurement_counter << std::endl;
  specific_magnetizations_histogram /= measurement_counter;

  for (double beta = -1.0; beta < 1.05; beta += 0.1)
  {
    double partition_function = 0.0;
    double magnetization_sum = 0.0;
    
    for (dos_t::const_iterator it = log_density_of_states.begin(); it != log_density_of_states.end(); ++it)
    {
      partition_function += exp(it->second - beta*it->first);
      magnetization_sum += specific_magnetizations_histogram[it->first] * exp(it->second - beta*it->first);
    }
    std::cout << "beta = " << beta << ", <m> = " << magnetization_sum / partition_function << std::endl;
  }
}
