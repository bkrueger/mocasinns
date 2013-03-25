/**
 * \file metropolis.hpp
 * \brief Class for Metropolis-Monte-Carlo simulations with MoCaSinns
 * 
 * Does Metropolis-Steps and determines statistical averages of observables at given inverse temperatures. Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_METROPOLIS_HPP
#define MOCASINNS_METROPOLIS_HPP

#include "simulation.hpp"
#include "concepts/concepts.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

namespace Mocasinns
{

//! Class for Metropolis-Monte-Carlo simulations
template <class ConfigurationType, class StepType, class RandomNumberGenerator>
class Metropolis : public Simulation<ConfigurationType, RandomNumberGenerator>
{
  // Check the configuration concept
  BOOST_CONCEPT_ASSERT((Concepts::ConfigurationConcept<ConfigurationType, StepType>));  
  // Check the step concept
  BOOST_CONCEPT_ASSERT((Concepts::StepConcept<StepType>));
  // Check the random number generator concept
  BOOST_CONCEPT_ASSERT((Concepts::RandomNumberGeneratorConcept<RandomNumberGenerator>));

public:
  //! Typedef of this class
  typedef Metropolis<ConfigurationType, StepType, RandomNumberGenerator> this_type;
  // Typedefs for integers
  typedef typename Simulation<ConfigurationType, RandomNumberGenerator>::StepNumberType StepNumberType;
  typedef uint32_t MeasurementNumberType;
  //! Forward declaration of the struct storing the Parameters of a Metropolis Simulation
  struct Parameters;

  //! Standard class for observing the energy of the system
  struct ObserveEnergy
  {
    typedef decltype(((ConfigurationType*)nullptr)->energy()) observable_type;
    static observable_type observe(ConfigurationType* config) { return config->energy(); }
  };
  //! Typedef for the default observable
  typedef typename Metropolis<ConfigurationType, StepType, RandomNumberGenerator>::ObserveEnergy DefaultObservable;

  //! Boost signal handler invoked after every measurement
  boost::signals2::signal<void (Simulation<ConfigurationType,RandomNumberGenerator>*)> signal_handler_measurement;

  //! Initialise a Metropolis-MC simulation with default configuration space and default Parameters
  Metropolis() : Simulation<ConfigurationType, RandomNumberGenerator>(), simulation_parameters() {}
  //! Initialise a Metropolis-MC simulation with default configuration space and given Parameters
  Metropolis(const Parameters& params) : Simulation<ConfigurationType, RandomNumberGenerator>(), simulation_parameters(params) {}
  //! Initialise a Metropolis-MC simulation with given parameters and given configuration space
  Metropolis(const Parameters& params, ConfigurationType* initial_configuration) : Simulation<ConfigurationType, RandomNumberGenerator>(initial_configuration), simulation_parameters(params) {}

  //! Get-accessor for the parameters of the Metropolis simulation
  const Parameters& get_simulation_parameters() { return simulation_parameters; }
  //! Set-accessor for the parameters of the Metropolis simulation
  void set_parameters(const Parameters& value) { simulation_parameters = value; }

  //! Calculate the acceptance probability for a given step, must be implemented to use Simulation::do_steps
  /*!
    \tparam TemperatureType Type of the temperatures
    \param step_to_execute Step of which the acceptance probability should be calculated
    \param beta Temperature at which the step is done
   */
  template <class TemperatureType>
  inline double acceptance_probability(StepType& step_to_execute, const TemperatureType& beta = 0)
  {
    return exp( -beta * step_to_execute.delta_E());
  }
  //! Handle an executed step (do nothing, must be implemented to use Simulation::do_steps)
  template <class NotImportant>
  inline void handle_executed_step(StepType&, NotImportant) {}
  //! Handle a rejected step (do nothing, must be implemented to use Simulation::do_steps)
  template <class NotImportant>
  inline void handle_rejected_step(StepType&, NotImportant) {}

  //! Execute a given number of Metropolis-MC steps on the configuration at inverse temperatur beta
  /*!
    \details This function is just syntax sugar and calls the Simulation::do_steps() function
    \param number Number of Metropolis steps that will be performed
    \param beta Inverse temperature that will be used for calculation of the acceptance probability of the Metropolis steps.
   */
  template<class TemperaturType = double>
  void do_metropolis_steps(const StepNumberType& number, const TemperaturType& beta = 0.0)
  {
    this->template do_steps<this_type, StepType>(number, beta);
  }

  //! Execute a Metropolis Monte-Carlo simulation at given inverse temperature
  template<class Observable = DefaultObservable, class TemperatureType = double>
  std::vector<typename Observable::observable_type> do_metropolis_simulation(const TemperatureType& beta);
  //! Execute a Metropolis Monte-Carlo simulation at the given range of inverse temperatures
  template<class Observable = DefaultObservable, class InputIterator>
  std::vector<std::vector<typename Observable::observable_type> > do_metropolis_simulation(InputIterator beta_begin, InputIterator beta_end);

  //! Execute a Metropolis Monte-Carlo simulation at given inverse temperature with an accumulator for storing the measurements results
  template<class Observable = DefaultObservable, class Accumulator, class TemperatureType = double>
  void do_metropolis_simulation(const TemperatureType& beta, Accumulator& measurement_accumulator);
  //! Execute a Metropolis Monte-Carlo simulation at given range of invere temperatures with an range of accumulators for storing the measurements results
  template<class Observable = DefaultObservable, class AccumulatorIterator, class InverseTemperatureIterator>
  void do_metropolis_simulation(InverseTemperatureIterator beta_begin, InverseTemperatureIterator beta_end, AccumulatorIterator measurement_accumulator_begin, AccumulatorIterator measurement_accumulator_end);

  //! Measure the autocorrelation function of the system with respect to an observable
  template<class Observable = DefaultObservable, class TemperatureType = double>
  std::vector<typename Observable::observable_type> autocorrelation_function(const TemperatureType& beta, unsigned int maximal_time, unsigned int considered_time_factor = 5);
  //! Calculate the integrated autocorrelation time of the system with respect to an observable
  template<class Observable = DefaultObservable, class TemperatureType = double>
  typename Observable::observable_type integrated_autocorrelation_time(const TemperatureType& beta, unsigned int maximal_time, unsigned int considered_time_factor = 5);

  //! Load the data of the Metropolis simulation from a serialization stream
  virtual void load_serialize(std::istream& input_stream);
  //! Load the data of the Metropolis simulation from a serialization file
  virtual void load_serialize(const char* filename);
  //! Save the data of the Metropolis simulation to a serialization stream
  virtual void save_serialize(std::ostream& output_stream) const;
  //! Save the data of the Metropolis simulation to a serialization file
  virtual void save_serialize(const char* filename) const;

private:
  //! Member variable storing the parameters of the simulation
  Parameters simulation_parameters;

  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    // serialize base class information
    ar & boost::serialization::base_object<Simulation<ConfigurationType, RandomNumberGenerator> >(*this);
  }
};

//! Struct storing the definition of the Parameters of a Metropolis-Simulation
template <class ConfigurationType, class StepType, class RandomNumberGenerator>
struct Metropolis<ConfigurationType, StepType, RandomNumberGenerator>::Parameters
{
  //! Number of steps to perform before taking data
  StepNumberType relaxation_steps;
  //! Number of data points per temperature
  MeasurementNumberType measurement_number;
  //! Number of steps to perform between two data measurements
  StepNumberType steps_between_measurement;
  
  //! Standard constructor for setting default values
  Parameters() : relaxation_steps(1000),
		 measurement_number(100),
		 steps_between_measurement(100) {}
};
  
} // of namespace Mocasinns

#include "src/metropolis.cpp"

#endif
