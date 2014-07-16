/**
 * \file metropolis.hpp
 * \brief Contains the class for doing Metropolis simulations
 * 
 * \author Benedikt Krüger
 * \author Johannes Knauf
 */

#ifndef MOCASINNS_METROPOLIS_HPP
#define MOCASINNS_METROPOLIS_HPP

#include "simulation.hpp"
#include "concepts/concepts.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

// Boost function types for the standard observable
#include <boost/function_types/result_type.hpp>
#include <boost/typeof/std/utility.hpp>
#include <boost/type_traits.hpp>

namespace Mocasinns
{
  /*!
   * \brief Class for the Metropolis algorithm
   *
   * \details The Metropolis algorithm was the first Monte-Carlo algorithm to be applied to a physical system (system of hard discs). 
   * It chooses the probabilites \f$ P(\sigma) \f$ of the state \f$ \sigma \f$ according to its Boltzmann probability
   * \f[
   *    P_{\mathrm{Metropolis}} = \exp\left( -\beta E[\sigma] \right)
   * \f]
   * Calculating the canonical average of an observable reduces to the arithmetic mean of the observables at each state
   * \f[
   *    \langle\mathcal Q\rangle = \frac{1}{k} \sum_{i=1}^k \mathcal O(\sigma_i)
   * \f]
   * The Metropolis-choice of the acceptance probabilities is:
   * \f[
   *    A(\sigma_1 \rightarrow \sigma_2) = 
   *    \begin{cases}
   *       1 & \text{if }\beta \Delta E \leq -\ln(s) \\
   *       \frac{1}{s}\cdot \exp\left(-\beta \Delta E \right) & \text{if } \beta \Delta E > -\ln(s)
   *    \end{cases}
   * \f]
   *
   * To perform a metropolis simulation, use one of the \c Metropolis::do_metropolis_simulation() function.
   * All of them take as an optional template argument an <tt>Observator</tt> that must fulfill the \ref concept-Observator "Observator concept".
   * If no such template parameter is specified, the algorithm observes the energy of the system.
   * There are the following overrides:
   * - <tt>do_metropolis_simulation<Observator>(TemperatureType inverse_temperature)</tt>: Do a Metropolis simulation for a single temperature and return a vector of the observables measured by the <tt>Observator</tt>.
   * - <tt>do_metropolis_simulation<Observator>(Iterator inverse_temperatures_begin, Iterator inverse_temperatures_end)</tt>: Do a Metropolis simulation for a range of given temperatures and return a vector of vectors of the observables measured by the <tt>Observator</tt>. The outer vector is for the different temperatures, the inner vector for the different measurements.
   * - <tt>do_metropolis_simulation<Observator>(TemperatureType inverse_temperature, Accumulator& measurement_accumulator)</tt>: Do a Metropolis simulation for a single temperature and accumulate the measured observables in a given accumulator (that fulfills the \ref concept-Accumulator "Accumulator concept"). This can be used e.g. for calculating the mean and the variance of the observables without storing the single measurement results.
   * - <tt>do_metropolis_simulation<Observator>(TemperatureType inverse_temperature, Accumulator& measurement_accumulator)</tt>: Do a Metropolis simulation for a single temperature and accumulate the measured observables in a given accumulator (that fulfills the \ref concept-Accumulator "Accumulator concept"). This can be used e.g. for calculating the mean and the variance of the observables without storing the single measurement results.
   *
   * \signalhandlers
   * \signalhandler{signal_handler_measurement,This handler is called before every measurement.}
   * \signalhandler{signal_handler_sig...., The check for <tt>POSIX</tt> signals (<tt>SIGTERM</tt>\, <tt>SIGUSR1</tt> and <tt>SIGUSR2</tt>) after every measurment.}
   * \endsignalhandlers
   *
   * \tparam ConfigurationType \concept{ConfigurationType}
   * \tparam StepType \concept{StepType}
   * \tparam RandomNumberGenerator \concept{RandomNumberGenerator}
   *
   * \references
   * \reference{1, Metropolis N. et al.\, J. Chem. Phys. 21 (1953) 1087}
   * \reference{2, Metropolis N. and Ulam S.\, J. Amer. Statist. Assoc. 44 (1949) 335}
   * \endreferences
   */
  template <class ConfigurationType, class StepType, class RandomNumberGenerator, bool rejection_free = false>
  class Metropolis : public Simulation<ConfigurationType, RandomNumberGenerator>
  {
    // Check the configuration concept
    BOOST_CONCEPT_ASSERT((Concepts::ConfigurationConcept<ConfigurationType, StepType>));  
    // Check the step concept
    BOOST_CONCEPT_ASSERT((Concepts::StepConcept<StepType>));
    // Check the random number generator concept
    BOOST_CONCEPT_ASSERT((Concepts::RandomNumberGeneratorConcept<RandomNumberGenerator>));
    
  public:
    // Typedef for the base class
    typedef Simulation<ConfigurationType, RandomNumberGenerator> Base;
    //! Typedef of this class
    typedef Metropolis<ConfigurationType, StepType, RandomNumberGenerator, rejection_free> this_type;
    // Typedefs for integers
    typedef typename Base::step_number_t step_number_t;
    typedef uint32_t measurement_number_t;
    //! Forward declaration of the struct storing the Parameters of a Metropolis Simulation
    struct Parameters;
    
    //! Standard class for observing the energy of the system
    struct ObserveEnergy
    {
      // typedef decltype(((ConfigurationType*)nullptr)->energy()) observable_type;
      typedef BOOST_TYPEOF(&ConfigurationType::energy) energy_function_type;
      typedef typename boost::function_types::result_type<energy_function_type>::type observable_type;
      //    typedef typename boost::function_traits<energy_function_type>::result_type observable_type;
      static observable_type observe(ConfigurationType* config) { return config->energy(); }
    };
    //! Typedef for the default observable
    typedef typename Metropolis<ConfigurationType, StepType, RandomNumberGenerator, rejection_free>::ObserveEnergy DefaultObservator;
    
    //! Boost signal handler invoked after every measurement
    boost::signals2::signal<void (Simulation<ConfigurationType,RandomNumberGenerator>*)> signal_handler_measurement;
    
    //! Initialise a Metropolis-MC simulation with default configuration space and default Parameters
    Metropolis() : Simulation<ConfigurationType, RandomNumberGenerator>(), simulation_parameters() {}
    //! Initialise a Metropolis-MC simulation with default configuration space and given Parameters
    Metropolis(const Parameters& params) : Simulation<ConfigurationType, RandomNumberGenerator>(), simulation_parameters(params) {}
    //! Initialise a Metropolis-MC simulation with given parameters and given configuration space
    Metropolis(const Parameters& params, ConfigurationType* initial_configuration) 
      : Simulation<ConfigurationType, RandomNumberGenerator>(initial_configuration), 
	simulation_parameters(params) {}
    //! Initialise a Metropolis-MC simulation by copying from another one
    Metropolis(const Metropolis& other) 
      : Simulation<ConfigurationType, RandomNumberGenerator>(other.configuration_space),
	simulation_parameters(other.simulation_parameters) {}
    
    //! Assignment operator
    this_type& operator=(const Metropolis& other)
    {
      if (this != &other)
      {
	this->configuration_space = other.get_config_space();
	this->simulation_parameters = other.simulation_parameters;
      }
      return *this;
    }

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
      return exp( -(beta * step_to_execute.delta_E()));
    }
    //! Handle an executed step (do nothing, must be implemented to use Simulation::do_steps)
    template <class NotImportant>
    inline void handle_executed_step(StepType&, double, NotImportant) {}
    //! Handle a rejected step (do nothing, must be implemented to use Simulation::do_steps)
    template <class NotImportant>
    inline void handle_rejected_step(StepType&, double, NotImportant) {}
    
    //! Execute a given number of Metropolis-MC steps on the configuration at inverse temperatur beta
    /*!
      \details This function is just syntax sugar and calls the Simulation::do_steps() function
      \param number Number of Metropolis steps that will be performed
      \param beta Inverse temperature that will be used for calculation of the acceptance probability of the Metropolis steps.
    */
    template<class TemperaturType = double>
    void do_metropolis_steps(const step_number_t& number, const TemperaturType& beta = 0.0)
    {
      this->template do_steps<this_type, StepType, rejection_free>(number, beta);
    }
    
    //! Execute a Metropolis Monte-Carlo simulation at given inverse temperature
    template<class Observator = DefaultObservator, class TemperatureType = double>
    std::vector<typename Observator::observable_type> do_metropolis_simulation(const TemperatureType& beta);
    //! Execute a Metropolis Monte-Carlo simulation at the given range of inverse temperatures
    template<class Observator = DefaultObservator, class InputIterator>
    std::vector<std::vector<typename Observator::observable_type> > do_metropolis_simulation(InputIterator beta_begin, InputIterator beta_end);
    
    //! Execute a Metropolis Monte-Carlo simulation at given inverse temperature with an accumulator for storing the measurements results
    template<class Observator = DefaultObservator, class Accumulator, class TemperatureType = double>
    void do_metropolis_simulation(const TemperatureType& beta, Accumulator& measurement_accumulator);
    //! Execute a Metropolis Monte-Carlo simulation at given range of invere temperatures with an range of accumulators for storing the measurements results
    template<class Observator = DefaultObservator, class AccumulatorIterator, class InverseTemperatureIterator>
    void do_metropolis_simulation(InverseTemperatureIterator beta_begin, InverseTemperatureIterator beta_end, AccumulatorIterator measurement_accumulator_begin, AccumulatorIterator measurement_accumulator_end);
        
    //! Load the data of the Metropolis simulation from a serialization stream
    virtual void load_serialize(std::istream& input_stream) { Base::load_serialize(*this, input_stream); }
    //! Load the data of the Metropolis simulation from a serialization file
    virtual void load_serialize(const char* filename) { Base::load_serialize(*this, filename); }
    //! Save the data of the Metropolis simulation to a serialization stream
    virtual void save_serialize(std::ostream& output_stream) const { Base::save_serialize(*this, output_stream); }
    //! Save the data of the Metropolis simulation to a serialization file
    virtual void save_serialize(const char* filename) const { Base::save_serialize(*this, filename); }

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

  // Typedef for Metropolis rejection-free.
  // Checks for C++11 support for template typedef
#ifndef BOOST_NO_CXX11_TEMPLATE_ALIASES
  template <class ConfigurationType, class StepType, class RandomNumberGenerator> using MetropolisRejectionFree = Metropolis<ConfigurationType, StepType, RandomNumberGenerator, true>;
#endif

  //! Struct storing the definition of the Parameters of a Metropolis-Simulation
  template <class ConfigurationType, class StepType, class RandomNumberGenerator, bool rejection_free>
  struct Metropolis<ConfigurationType, StepType, RandomNumberGenerator, rejection_free>::Parameters
  {
    //! Number of steps to perform before taking data
    step_number_t relaxation_steps;
    //! Number of data points per temperature
    measurement_number_t measurement_number;
    //! Number of steps to perform between two data measurements
    step_number_t steps_between_measurement;
    //! Number of measurements to perform before calling each signal
    unsigned int measurements_per_signal;
    
    //! Standard constructor for setting default values
    Parameters() : relaxation_steps(1000),
		   measurement_number(100),
		   steps_between_measurement(100),
		   measurements_per_signal(1) {}
  };
}

/*!
 * \example simple_ising_metropolis.cpp
 * Example for the simplest possible usage of a Metropolis simulation. Since no observator is given, a default observator that measures the energy is used. The measurment results are given as a vector.
 */

/*!
 * \example simple_ising_metropolis_observator.cpp
 * Example for using a Metropolis simulation with an user-provided observator that measures the energy and the magnetization of the system at once.
 */

#include "src/metropolis.cpp"

#endif
