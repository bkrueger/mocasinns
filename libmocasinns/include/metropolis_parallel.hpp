/**
 * \file metropolis_parallel.hpp
 * \brief Class for parallel Metropolis-Monte-Carlo simulations with MoCaSinns
 * 
 * Does Metropolis-Steps and determines statistical averages of observables at given inverse temperatures. Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_METROPOLIS_PARALLEL_HPP
#define MOCASINNS_METROPOLIS_PARALLEL_HPP

#include "simulation.hpp"
#include "metropolis.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

namespace Mocasinns
{

//! Class for parallel Metropolis-Monte-Carlo simulations
template <class ConfigurationType, class StepType, class RandomNumberGenerator>
class MetropolisParallel : public Simulation<ConfigurationType, RandomNumberGenerator>
{
private:
  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    // serialize base class information
    ar & boost::serialization::base_object<Simulation<ConfigurationType, RandomNumberGenerator> >(*this);
  }
  
public:
  // Forward declaration of the parameters used for a parallel Metropolis Simulation
  struct Parameters;

  //! Typedef for the serial Metropolis
  typedef Metropolis<ConfigurationType, StepType, RandomNumberGenerator> MetropolisSerial;

  //! Boost signal handler invoked after every measurement
  boost::signal<void (Simulation<ConfigurationType,RandomNumberGenerator>*)> signal_handler_measurement;
  //! Boost signal handler invoked after every run
  boost::signal<void (Simulation<ConfigurationType,RandomNumberGenerator>*)> signal_handler_run;

  //! Initialise a Metropolis-MC simulation with default configuration space and default RandomNumberGenerator
  MetropolisParallel() : Simulation<ConfigurationType, RandomNumberGenerator>() {}
  //! Initialise a Metropolis-MC simulation with given configuration space and default RandomNumberGenerator
  MetropolisParallel(ConfigurationType* initial_configuration) : Simulation<ConfigurationType, RandomNumberGenerator>(initial_configuration) {}

  //! Execute a Metropolis Monte-Carlo simulation with given parameters at given inverse temperature
  template<class Observable, class TemperatureType = double>
  std::vector<typename Observable::observable_type> do_parallel_metropolis_simulation(const Parameters& parameters, const TemperatureType& beta);
  //! Execute a parallel Metropolis Monte-Carlo simulation with given parameters at the given range of inverse temperatures
  template<class Observable, class InputIterator>
  std::vector<std::vector<typename Observable::observable_type> > do_parallel_metropolis_simulation(const Parameters& parameters, InputIterator beta_begin, InputIterator beta_end);

  //! Execute a Metropolis Monte-Carlo simulation with given parameters at given inverse temperature with an accumulator for storing the measurements results
  template<class Observable, class Accumulator, class TemperatureType = double>
  void do_parallel_metropolis_simulation(const Parameters& parameters, const TemperatureType& beta, Accumulator& measurement_accumulator);
  //! Execute a Metropolis Monte-Carlo simulation with given parameters at given range of invere temperatures with an range of accumulators for storing the measurements results
  template<class Observable, class AccumulatorIterator, class InverseTemperatureIterator>
  void do_parallel_metropolis_simulation(const Parameters& parameters, InverseTemperatureIterator beta_begin, InverseTemperatureIterator beta_end, AccumulatorIterator measurement_accumulator_begin, AccumulatorIterator measurement_accumulator_end);

  //! Load the data of the Metropolis simulation from a serialization stream
  virtual void load_serialize(std::istream& input_stream);
  //! Load the data of the Metropolis simulation from a serialization file
  virtual void load_serialize(const char* filename);
  //! Save the data of the Metropolis simulation to a serialization stream
  virtual void save_serialize(std::ostream& output_stream) const;
  //! Save the data of the Metropolis simulation to a serialization file
  virtual void save_serialize(const char* filename) const;
};

//! Struct storing the parameters of a parallel Metropolis Monte-Carlo Simulation
template <class ConfigurationType, class StepType, class RandomNumberGenerator>
struct MetropolisParallel<ConfigurationType, StepType, RandomNumberGenerator>::Parameters 
  : Metropolis<ConfigurationType, StepType, RandomNumberGenerator>::Parameters
{
  //! Number of independent runs to perform
  unsigned int run_number;
  //! Number of processes to use
  unsigned int process_number;
  
  //! Standard constructor for setting default values
  Parameters() : MetropolisSerial::Parameters(),
		 run_number(2),
		 process_number(2) {}
};


} // of namespace Mocasinns

#include "metropolis_parallel.cpp"

#endif
