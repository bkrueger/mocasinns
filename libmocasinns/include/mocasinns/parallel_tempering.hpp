/**
 * \file parallel_tempering.hpp
 * \brief Class for parallel tempering simulations with MoCaSinns
 * 
 * Does a parallel tempering simulation at the given temperatures and calculates observables after a certain number of steps. The number of threads can be given as a parameter.
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_PARALLEL_TEMPERING_HPP
#define MOCASINNS_PARALLEL_TEMPERING_HPP

#include "simulation.hpp"
#include "metropolis.hpp"
#include "serial_tempering.hpp"
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
  class ParallelTempering : public Simulation<ConfigurationType, RandomNumberGenerator>
  {
    // Check the configuration concept
    BOOST_CONCEPT_ASSERT((Concepts::ConfigurationConcept<ConfigurationType, StepType>));  
    // Check the step concept
    BOOST_CONCEPT_ASSERT((Concepts::StepConcept<StepType>));
    // Check the random number generator concept
    BOOST_CONCEPT_ASSERT((Concepts::RandomNumberGeneratorConcept<RandomNumberGenerator>));

  public:
    //! Typedef of this class
    typedef ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator> this_type;
    //! Typedef for a metropolis simulation with the same template paramters
    typedef Metropolis<ConfigurationType, StepType, RandomNumberGenerator> MetropolisType;
    //! Type of the default observable
    typedef typename MetropolisType::DefaultObservator DefaultObservator;
    //! Type of the replica exchange logs
    typedef std::vector<unsigned int> ReplicaExchangeLog;
    //! Type of the inverse temperature histograms
    typedef std::vector<unsigned int> InverseTemperatureHistogram;
    // Typedefs for integers
    typedef typename Simulation<ConfigurationType, RandomNumberGenerator>::StepNumberType StepNumberType;
    typedef uint32_t MeasurementNumberType;
    //! Forward declaration of the struct storing the Parameters of a Metropolis Simulation
    struct Parameters;

    template <class InverseTemperatureType> class InverseTemperatureOptimization;

    //! Boost signal handler invoked after every measurement
    boost::signals2::signal<void (Simulation<ConfigurationType,RandomNumberGenerator>*)> signal_handler_measurement;
    //! Boost signal handler invoked after every replica exchange
    boost::signals2::signal<void (Simulation<ConfigurationType,RandomNumberGenerator>*)> signal_handler_replica_exchange;

    // //! Initialise a parallel tempering simulation with given parameters, one configuration space and the number of temperatures that will be used. The configuration space will be copied.
    // ParallelTempering(const Parameters& params, ConfigurationType* configuration_space, unsigned int temperature_number);
    //! Initialise a parallel tempering simulation with given parameters and a range of pointers to configuration spaces
    template <class ConfigurationPointerIterator>
    ParallelTempering(const Parameters& params, ConfigurationPointerIterator configuration_pointers_begin, ConfigurationPointerIterator configuration_pointers_end);

    //! Get-accessor for the first configuration pointer
    ConfigurationType* get_config_space() { return get_config_space(0); }
    //! Get-accessor for the configuration pointer
    ConfigurationType* get_config_space(unsigned int index) { return metropolis_simulations[index].get_config_space(); }
    //! Get-accessor for all configuration spaces
    std::vector<ConfigurationType*> get_config_spaces()
    {
      std::vector<ConfigurationType*> result;
      for (unsigned int i = 0; i < metropolis_simulations.size(); ++i)
	result.push_back(get_config_space(i));
      return result;
    }
    //! Get-accessor for the parameters of the parallel tempering simulation
    const Parameters& get_simulation_parameters() { return simulation_parameters; }
    //! Set-accessor for the parameters of the parallel tempering simulation
    void set_simulation_parameters(const Parameters& value) { simulation_parameters = value; }

    //! Number of executed replica exchanges. 
    //! At index i the number of executed exchanges between inverse temperature index i and (i+1) is recorded
    const ReplicaExchangeLog& get_replica_exchanges_executed() const { return replica_exchange_log_executed; }
    //! Number of rejected replica exchanges. 
    //! At index i the number of rejected exchanges between inverse temperature index i and (i+1) is recorded
    const ReplicaExchangeLog get_replica_exchanges_rejected() const { return replica_exchange_log_rejected; }

    //! Histogram of temperature visits after leaving the minimal inverse temperature
    const InverseTemperatureHistogram& get_inverse_temperature_histogram_up() const { return inverse_temperature_histogram_up; }
    //! Histogram of temperature visits after leaving the maximal inverse temperature
    const InverseTemperatureHistogram& get_inverse_temperature_histogram_down() const { return inverse_temperature_histogram_down; }

    //! Execute a given number of parallel tempering steps on the configuration at inverse temperatur beta
    template <class TemperatureTypeIterator>
    void do_parallel_tempering_steps(const StepNumberType& number, TemperatureTypeIterator inverse_temperatures_begin, TemperatureTypeIterator inverse_temperatures_end);
    //! Execute a given number of parallel tempering steps on the configuration at inverse temperatur beta
    template <class NumberIterator, class TemperatureTypeIterator>
    void do_parallel_tempering_steps(NumberIterator numbers_begin, NumberIterator numbers_end, TemperatureTypeIterator inverse_temperatures_begin, TemperatureTypeIterator inverse_temperatures_end);

    //! Execute an inverse temperature exchange step
    template <class TemperatureTypeIterator>
    unsigned int do_replica_exchange(TemperatureTypeIterator inverse_temperatures_begin, TemperatureTypeIterator inverse_temperatures_end);

    //! Execute a Metropolis Monte-Carlo simulation (inverse temperatures must be provided as parameters). Returns a vector of a vector, the first index corresponds to the temperature index and the second index corresponds to the measurement number
    template<class Observator = DefaultObservator, class TemperatureTypeIterator>
    std::vector<std::vector<typename Observator::observable_type> > do_parallel_tempering_simulation(TemperatureTypeIterator inverse_temperatures_begin, 
    												   TemperatureTypeIterator inverse_temperatures_end);
   
    //! Execute a Metropolis Monte-Carlo simulation with an accumulator for storing the measurements results (inverse temperatures must be provided as parameters). Copies the accumulators and returns a vector of these accumulators.
    template<class Observator = DefaultObservator, class Accumulator, class TemperatureTypeIterator>
    std::vector<Accumulator> do_parallel_tempering_simulation(const Accumulator& measurement_accumulator,
    							    TemperatureTypeIterator inverse_temperatures_begin, TemperatureTypeIterator inverse_temperatures_end);
    //! Execute a Metropolis Monte-Carlo simulation with a vector of accumulators (for each temperature) for storing the measurement results (inverse temperatures must be provided as parameters)
    template<class Observator = DefaultObservator, class AccumulatorIterator, class TemperatureTypeIterator>
    void do_parallel_tempering_simulation(AccumulatorIterator measurement_accumulators_begin, AccumulatorIterator measurement_accumulators_end,
					TemperatureTypeIterator inverse_temperatures_begin, TemperatureTypeIterator inverse_temperatures_end);

    //! Reset the replica exchange logs
    void replica_exchanges_reset();
    
    //! Reset the inverse temperature histograms
    void inverse_temperature_histograms_reset();

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

    //! Member variable for storing the instances of the Metropolis simulations
    std::vector<MetropolisType> metropolis_simulations;

    //! Vector that stores for each configuration pointer whether the minimal inverse temperature (false) or the maximal inverse temperature (true) was left
    std::map<ConfigurationType*, bool> replica_exchange_direction;
    //! Variable for loggint the number of rejected replica exchanges (at index i the exchanges between indices i and (i+1) are logged)
    ReplicaExchangeLog replica_exchange_log_rejected;
    //! Variable for loggint the number of execyted replica exchanges (at index i the exchanges between indices i and (i+1) are logged)
    ReplicaExchangeLog replica_exchange_log_executed;
    //! Variable for storing the temperature visits after leaving the minimal inverse temperature
    InverseTemperatureHistogram inverse_temperature_histogram_up;
    //! Variable for storing the temperature visits after leaving the maximal inverse temperature
    InverseTemperatureHistogram inverse_temperature_histogram_down;

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
    }
  };
  
  //! Struct storing the definition of the Parameters of a Metropolis-Simulation
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  struct ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::Parameters
    : SerialTempering<ConfigurationType, StepType, RandomNumberGenerator>::Parameters
  {    
    //! Number of parallel processes to use. The number of considered temperatures should be devidable through this number
    unsigned int process_number;
    
    Parameters() : SerialTempering<ConfigurationType, StepType, RandomNumberGenerator>::Parameters(), process_number(2) { }
  };

  //! Class for doing inverse temperature optimization of a parallel tempering simulation
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class InverseTemperatureType>
  class ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::InverseTemperatureOptimization
  {
  public:
    typedef ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator> ParallelTemperingType;
    typedef typename ParallelTemperingType::ReplicaExchangeLog ReplicaExchangeLog;
    typedef typename ParallelTemperingType::InverseTemperatureHistogram InverseTemperatureHistogram;

    template <class InverseTemperatureIterator>
    InverseTemperatureOptimization(ParallelTemperingType* optimize_simulation, 
				   InverseTemperatureIterator inverse_temperatures_begin,
				   InverseTemperatureIterator inverse_temperatures_end) 
      : simulation(optimize_simulation), 
	original_parameters(optimize_simulation->get_simulation_parameters()),
	replica_number(optimize_simulation->get_config_spaces().size())
    { 
      std::vector<InverseTemperatureType> first_inverse_temperatures;
      for (InverseTemperatureIterator it = inverse_temperatures_begin;
	   it != inverse_temperatures_end; ++it)
	first_inverse_temperatures.push_back(*it);
      inverse_temperatures.push_back(first_inverse_temperatures);
    }

    //! Vector that stores the inverse temperatures of the replicas for each recursion step
    const std::vector<std::vector<InverseTemperatureType> >& get_inverse_temperatures() const { return inverse_temperatures; }
    //! Vector that stores the acceptance probabilities of the replica exchanges for each recursion step
    const std::vector<std::vector<double> >& get_acceptance_probabilities() const { return acceptance_probabilities;}

    //! Function for optimizing the inverse temperatures, can be used in the signal handlers.
    //! Uses the method suggested by B.A. Berg (2004, p. 304) without weights.
    void optimize_berg(unsigned int recursion_number);

    //! Function for optimizing the inverse temperatures, can be used in the signal handlers.
    //! Uses the method suggested by B.A. Berg (2004, p. 304) with weights from worst acceptance rates.
    void optimize_berg_weights_worst_acceptance(unsigned int recursion_number);

    //! Function for optimizing the inverse temperatures, can be used in the signal handlers.
    //! Uses the method suggested by B.A. Berg (2004, p. 304) with weights from statistical independent acceptance rates.
    void optimize_berg_weights_independent_acceptance(unsigned int recursion_number);

    //! Function for optimizing the inverse temperatures, can be used in the signal handlers.
    //! Uses the method suggested by H.G. Katzgraber et al. (2006)
    void optimize_katzgraber(unsigned int recursion_number);

  private:
    //! Pointer to the parallel tempering simulation
    ParallelTemperingType* simulation;
    //! Parameters of the original simulation
    typename ParallelTemperingType::Parameters original_parameters;
    //! Number of replicas in the simulations
    unsigned int replica_number;

    //! Vector that stores the inverse temperatures of the replicas for each recursion step
    std::vector<std::vector<InverseTemperatureType> > inverse_temperatures;
    //! Vector that stores the acceptance probabilities of the replica exchanges for each recursion step
    std::vector<std::vector<double> > acceptance_probabilities;

    //! Alter the parameters to perform inverse temperature optimization
    void set_parameters_optimize()
    {
      typename ParallelTemperingType::Parameters optimization_parameters(original_parameters);
      optimization_parameters.measurement_number = 1;
      optimization_parameters.relaxation_steps = 0;
      simulation->set_simulation_parameters(optimization_parameters);
    }
    //! Undo the change in the parameters
    void unset_parameters_optimize() { simulation->set_simulation_parameters(original_parameters); }

    //! Mesure the acceptance probabilities. Does simulations until each replica exchange was accepted at least once
    void measure_acceptance_probabilities();
    //! Logs the acceptance probabilities and resets the replica exchange logs
    void log_acceptance_probabilities();
    
    //! Does a single temperature optimization step using the method suggested by B.A. Berg (2004, p. 304)
    void optimize_berg_step();
    //! Does a single temperature optimization step using the method suggested by H.G. Katzgraber et al. (2006)
    void optimize_katzgraber_step();
  };
}  

#include "src/parallel_tempering.cpp"
#include "src/parallel_tempering_inverse_temperature_optimization.cpp"

#endif
