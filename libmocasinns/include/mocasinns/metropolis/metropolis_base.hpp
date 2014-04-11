/**
 * \file metropolis_base.hpp
 * \brief Contains the declaration of the base class of the two Metropolis algorithms (normal and rejection free)
 * 
 * Do not include the file, but include <mocasinns/metropolis.hpp>
 * 
 * \author Benedikt Krüger
 * \author Johannes Knauf
 */

#ifndef MOCASINNS_METROPOLIS_METROPOLIS_BASE_HPP
#define MOCASINNS_METROPOLIS_METROPOLIS_BASE_HPP

#include "../simulation.hpp"
#include "../concepts/concepts.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

// Boost function types for the standard observable
#include <boost/function_types/result_type.hpp>
#include <boost/typeof/std/utility.hpp>
#include <boost/type_traits.hpp>

namespace Mocasinns
{
  //! Base class for Metropolis-Monte-Carlo simulations
  /*!
   * \tparam ConfigurationType \concept{ConfigurationType}
   */
  template <class ConfigurationType, class StepType, class RandomNumberGenerator, bool rejection_free>
  class MetropolisBase : public Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>
  {
    // Check the configuration concept
    BOOST_CONCEPT_ASSERT((Concepts::ConfigurationConcept<ConfigurationType, StepType>));  
    // Check the step concept
    BOOST_CONCEPT_ASSERT((Concepts::StepConcept<StepType>));
    // Check the random number generator concept
    BOOST_CONCEPT_ASSERT((Concepts::RandomNumberGeneratorConcept<RandomNumberGenerator>));
    
  public:
    //! Typedef of this class
    typedef MetropolisBase<ConfigurationType, StepType, RandomNumberGenerator, rejection_free> this_type;
    // Typedefs for integers
    typedef typename Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::StepNumberType StepNumberType;
    typedef uint32_t MeasurementNumberType;
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
    typedef typename MetropolisBase<ConfigurationType, StepType, RandomNumberGenerator, rejection_free>::ObserveEnergy DefaultObservator;
    
    //! Boost signal handler invoked after every measurement
    boost::signals2::signal<void (Simulation<ConfigurationType,RandomNumberGenerator, rejection_free>*)> signal_handler_measurement;
    
    //! Initialise a Metropolis-MC simulation with default configuration space and default Parameters
    MetropolisBase() : Simulation<ConfigurationType, RandomNumberGenerator>(), simulation_parameters() {}
    //! Initialise a Metropolis-MC simulation with default configuration space and given Parameters
    MetropolisBase(const Parameters& params) : Simulation<ConfigurationType, RandomNumberGenerator>(), simulation_parameters(params) {}
    //! Initialise a Metropolis-MC simulation with given parameters and given configuration space
    MetropolisBase(const Parameters& params, ConfigurationType* initial_configuration) 
      : Simulation<ConfigurationType, RandomNumberGenerator>(initial_configuration), 
	simulation_parameters(params) {}
    //! Initialise a Metropolis-MC simulation by copying from another one
    MetropolisBase(const MetropolisBase& other) 
      : Simulation<ConfigurationType, RandomNumberGenerator>(other.configuration_space),
	simulation_parameters(other.simulation_parameters) {}
    
    //! Assignment operator
    this_type& operator=(const MetropolisBase& other)
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
    void do_metropolis_steps(const StepNumberType& number, const TemperaturType& beta = 0.0)
    {
      this->template do_steps<this_type, StepType>(number, beta);
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
    
    //! Measure the autocorrelation function of the system with respect to an observable
    template<class Observator = DefaultObservator, class TemperatureType = double>
    std::vector<typename Observator::observable_type> autocorrelation_function(const TemperatureType& beta, unsigned int maximal_time, unsigned int considered_time_factor = 5);
    //! Calculate the integrated autocorrelation time of the system with respect to an observable
    template<class Observator = DefaultObservator, class TemperatureType = double>
    typename Observator::observable_type integrated_autocorrelation_time(const TemperatureType& beta, unsigned int maximal_time, unsigned int considered_time_factor = 5);
    
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
      ar & boost::serialization::base_object<Simulation<ConfigurationType, RandomNumberGenerator, rejection_free> >(*this);
    }
  };
}

#include "../src/metropolis.cpp"

#endif
