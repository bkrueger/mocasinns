// Example program for calculating the mean energy of an Ising chain. Compile using
// g++ -std=c++11 -I../include analysis.cpp -lboost_serialization -o analysis

#include <iostream>
#include "simple_ising.hpp"

#include <mocasinns/metropolis.hpp>
#include <mocasinns/random/boost_random.hpp>
#include <mocasinns/analysis/binning_analysis.hpp>
#include <mocasinns/analysis/jackknife_analysis.hpp>
#include <mocasinns/analysis/bootstrap_analysis.hpp>

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

double function_exponential(const double& x) { return exp(x); }

int main()
{
  typename MetropolisSimulation::Parameters parameters;
  parameters.measurement_number = 1000;
  parameters.relaxation_steps = 10000;
  parameters.steps_between_measurement = 10000;
  
  IsingConfiguration configuration(16);
  MetropolisSimulation simulation(parameters, &configuration);
  
  std::vector<double> specific_magnetizations = simulation.do_metropolis_simulation<SpecificMagnetizationObservator>(1.0);
  
  std::pair<double,double> binning_result = Mocasinns::Analysis::BinningAnalysis<double>::analyse(specific_magnetizations.begin(), specific_magnetizations.end(), 100, function_exponential);
  std::cout << "Binning analysis result: " << std::endl;
  std::cout << binning_result.first << " +- " << binning_result.second << std::endl << std::endl;

  std::pair<double,double> jackknife_result = Mocasinns::Analysis::JackknifeAnalysis<double>::analyse(specific_magnetizations.begin(), specific_magnetizations.end(), function_exponential);
  std::cout << "Jackknife analysis result: " << std::endl;
  std::cout << jackknife_result.first << " +- " << jackknife_result.second << std::endl << std::endl;

  std::pair<double,double> bootstrap_result = Mocasinns::Analysis::BootstrapAnalysis<double>::analyse(specific_magnetizations.begin(), specific_magnetizations.end(), 100, function_exponential);
  std::cout << "Bootstrap analysis result: " << std::endl;
  std::cout << bootstrap_result.first << " +- " << bootstrap_result.second << std::endl << std::endl;
}
