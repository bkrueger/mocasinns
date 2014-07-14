/**
 * \file serial_tempering.hpp
 * \brief Class for parallel tempering simulations on one cpu with MoCaSinns
 * 
 * Does a parallel tempering simulation at the given temperatures and calculates observables after a certain number of steps. Only uses one thread (therefor the name serial tempering)
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_SERIAL_TEMPERING_HPP
#define MOCASINNS_SERIAL_TEMPERING_HPP

#include "simulation.hpp"
#include "metropolis.hpp"
#include "concepts/concepts.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

// Boost function types for the standard observable
#include <boost/function_types/result_type.hpp>
#include <boost/typeof/std/utility.hpp>
#include <boost/type_traits.hpp>

namespace Mocasinns
{
  //! Class for parallel tempering simulations
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  class SerialTempering : public Simulation<ConfigurationType, RandomNumberGenerator>
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
    typedef SerialTempering<ConfigurationType, StepType, RandomNumberGenerator> this_type;
    //! Typedef for a metropolis simulation with the same template paramters
    typedef Metropolis<ConfigurationType, StepType, RandomNumberGenerator> MetropolisType;
    // Typedefs for integers
    typedef typename Base::step_number_t step_number_t;
    typedef uint32_t measurement_number_t;
    //! Type of the default observable
    typedef typename MetropolisType::DefaultObservator DefaultObservator;
    //! Forward declaration of the struct storing the Parameters of a Metropolis Simulation
    struct Parameters;

    //! Boost signal handler invoked after every measurement
    boost::signals2::signal<void (Simulation<ConfigurationType,RandomNumberGenerator>*)> signal_handler_measurement;

    //! Initialise a parallel tempering simulation on one thread with given parameters and a range of pointers to configuration spaces
    template <class ConfigurationPointerIterator>
    SerialTempering(const Parameters& params, ConfigurationPointerIterator configuration_pointers_begin, ConfigurationPointerIterator configuration_pointers_end);

    //! Get-accessor for the first configuration pointer
    ConfigurationType* get_config_space() { return get_config_space(0); }
    //! Get-accessor for the configuration pointer
    ConfigurationType* get_config_space(unsigned int index) { return configuration_pointers[index]; }
    //! Get-accessor for the parameters of the parallel tempering simulation
    const Parameters& get_simulation_parameters() { return simulation_parameters; }
    //! Set-accessor for the parameters of the parallel tempering simulation
    void set_simulation_parameters(const Parameters& value) { simulation_parameters = value; }
    //! Get-accessor for the log of the executed replica exchanges. At index 0 the number of non-executed replica exchanges is stored, at index i the number of executed exchanges between inverse temperature index (i-1) and i.
    const std::vector<unsigned int>& get_replica_exchange_log() { return replica_exchange_log; }

    //! Execute a given number of parallel tempering steps on the configuration at inverse temperatur beta
    template <class TemperatureTypeIterator>
    void do_serial_tempering_steps(const step_number_t& number, TemperatureTypeIterator inverse_temperatures_begin, TemperatureTypeIterator inverse_temperatures_end);
    //! Execute a given number of parallel tempering steps on the configuration at inverse temperatur beta
    template <class NumberIterator, class TemperatureTypeIterator>
    void do_serial_tempering_steps(NumberIterator numbers_begin, NumberIterator numbers_end, TemperatureTypeIterator inverse_temperatures_begin, TemperatureTypeIterator inverse_temperatures_end);

    //! Execute an inverse temperature exchange step
    template <class TemperatureTypeIterator>
    unsigned int do_replica_exchange(TemperatureTypeIterator inverse_temperatures_begin, TemperatureTypeIterator inverse_temperatures_end);

    //! Execute a Metropolis Monte-Carlo simulation (inverse temperatures must be provided as parameters). Returns a vector of a vector, the first index corresponds to the temperature index and the second index corresponds to the measurement number
    template<class Observator = DefaultObservator, class TemperatureTypeIterator>
    std::vector<std::vector<typename Observator::observable_type> > do_serial_tempering_simulation(TemperatureTypeIterator inverse_temperatures_begin, 
    												   TemperatureTypeIterator inverse_temperatures_end);
    
    //! Execute a Metropolis Monte-Carlo simulation with an accumulator for storing the measurements results (inverse temperatures must be provided as parameters). Copies the accumulators and returns a vector of these accumulators.
    template<class Observator = DefaultObservator, class Accumulator, class TemperatureTypeIterator>
    std::vector<Accumulator> do_serial_tempering_simulation(const Accumulator& measurement_accumulator,
    							    TemperatureTypeIterator inverse_temperatures_begin, TemperatureTypeIterator inverse_temperatures_end);
    //! Execute a Metropolis Monte-Carlo simulation with a vector of accumulators (for each temperature) for storing the measurement results (inverse temperatures must be provided as parameters)
    template<class Observator = DefaultObservator, class AccumulatorIterator, class TemperatureTypeIterator>
    void do_serial_tempering_simulation(AccumulatorIterator measurement_accumulators_begin, AccumulatorIterator measurement_accumulators_end,
					TemperatureTypeIterator inverse_temperatures_begin, TemperatureTypeIterator inverse_temperatures_end);

    //! Load the data of the Serial Tempering simulation from a serialization stream
    virtual void load_serialize(std::istream& input_stream) { Base::load_serialize(*this, input_stream); }
    //! Load the data of the Serial Tempering simulation from a serialization file
    virtual void load_serialize(const char* filename) { Base::load_serialize(*this, filename); }
    //! Save the data of the Serial Tempering simulation to a serialization stream
    virtual void save_serialize(std::ostream& output_stream) const { Base::save_serialize(*this, output_stream); }
    //! Save the data of the Serial Tempering simulation to a serialization file
    virtual void save_serialize(const char* filename) const { Base::save_serialize(*this, filename); }

  private:
    //! Member variable storing the parameters of the simulation
    Parameters simulation_parameters;

    //! Member variable for storing the pointers to the different instances of the configuration space
    std::vector<ConfigurationType*> configuration_pointers;
    //! Member variable for storing the instances of the Metropolis simulations
    std::vector<MetropolisType> metropolis_simulations;
    //! Member variable for storing the done replica exchanges. 
    std::vector<unsigned int> replica_exchange_log;

    //! Private function to check whether a given temperature range has the same size as the configuration pointers and throw an error if necessary.
    template <class TemperatureTypeIterator>
    void check_temperature_range(TemperatureTypeIterator inverse_temperatures_begin, TemperatureTypeIterator inverse_temperatures_end);
    
    //! Member variable for boost serialization
    friend class boost::serialization::access;
    //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
    template<class Archive> void serialize(Archive & ar, const unsigned int)
    {
      // serialize base class information
      ar & boost::serialization::base_object<Simulation<ConfigurationType, RandomNumberGenerator> >(*this);

      // Serialize the class members
      ar & configuration_pointers;
      ar & metropolis_simulations;
      ar & replica_exchange_log;
    }
  };
  
  //! Struct storing the definition of the Parameters of a Metropolis-Simulation
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  struct SerialTempering<ConfigurationType, StepType, RandomNumberGenerator>::Parameters
    : Metropolis<ConfigurationType, StepType, RandomNumberGenerator>::Parameters
  {
    //! Number of steps between the exchange of a replice. This number should be divisor of the steps_between_measurements
    step_number_t steps_between_replica_exchange;

    Parameters() : Metropolis<ConfigurationType, StepType, RandomNumberGenerator>::Parameters(), steps_between_replica_exchange(10) { }
  };
  
} // of namespace Mocasinns

#include "src/serial_tempering.cpp"

#endif
