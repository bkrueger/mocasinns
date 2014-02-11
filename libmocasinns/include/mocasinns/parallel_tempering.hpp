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
    // Typedefs for integers
    typedef typename Simulation<ConfigurationType, RandomNumberGenerator>::StepNumberType StepNumberType;
    typedef uint32_t MeasurementNumberType;
    //! Forward declaration of the struct storing the Parameters of a Metropolis Simulation
    struct Parameters;

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

    //! Class for doing inverse temperature optimization of a parallel tempering simulation
    class InverseTemperatureOptimization;

    //! Number of executed replica exchanges. 
    //! At index i the number of executed exchanges between inverse temperature index i and (i+1) is recorded
    ReplicaExchangeLog replica_exchanges_executed() const;
    //! Number of rejected replica exchanges. 
    //! At index i the number of rejected exchanges between inverse temperature index i and (i+1) is recorded
    ReplicaExchangeLog replica_exchanges_rejected() const;
    //! Number of replica exchanges (executed and rejected) going from minimal inverse temperature to maximal inverse temperature. 
    //! At index i the number of executed exchanges between inverse temperature index i and (i+1) is recorded
    ReplicaExchangeLog replica_exchanges_up() const;
    //! Number of replica exchanges (executed and rejected) going from maximal inverse temperature to maximal inverse temperature. 
    //! At index i the number of executed exchanges between inverse temperature index i and (i+1) is recorded
    ReplicaExchangeLog replica_exchanges_down() const;
    //! Number of executed replica exchanges for configurations that were last at the minimal inverse temperature
    const ReplicaExchangeLog& replica_exchanges_executed_up() const { return replica_exchange_log_executed_up; }
    //! Number of executed replica exchanges for configurations that were last at the maximal inverse temperature
    const ReplicaExchangeLog& replica_exchanges_executed_down() const { return replica_exchange_log_executed_down; }
    //! Number of rejected replica exchanges for configurations that were last at the minimal inverse temperature
    const ReplicaExchangeLog& replica_exchanges_rejected_up() const { return replica_exchange_log_rejected_up; }
    //! Number of rejected replica exchanges for configurations that were last at the maximal inverse temperature
    const ReplicaExchangeLog& replica_exchanges_rejected_down() const { return replica_exchange_log_rejected_down; }
    //! Reset the replica exchange logs
    void replica_exchanges_reset();

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
    //! Variable for logging the number of rejected replica exchanges after leaving the minimal inverse temperature
    ReplicaExchangeLog replica_exchange_log_rejected_up;
    //! Variable for logging the number of rejected replica exchanges after leaving the minimal inverse temperature
    ReplicaExchangeLog replica_exchange_log_rejected_down;
    //! Variable for logging the number of rejected replica exchanges not having met the maximal or minimal inverse temperature
    ReplicaExchangeLog replica_exchange_log_rejected_null; 
    //! Variable for logging the number of accepted replica exchanges after leaving the minimal inverse temperature
    ReplicaExchangeLog replica_exchange_log_executed_up;
    //! Variable for logging the number of accepted replica exchanges after leaving the maximal inverse temperature
    ReplicaExchangeLog replica_exchange_log_executed_down;
    //! Variable for logging the number of accepted replica exchanges not having met the maximal or minimal inverse temperature
    ReplicaExchangeLog replica_exchange_log_executed_null; 

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
  class ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator>::InverseTemperatureOptimization
  {
  public:
    typedef ParallelTempering<ConfigurationType, StepType, RandomNumberGenerator> ParallelTemperingType;

    //! Static function for optimizing the inverse temperatures, can be used in the signal handlers.
    //! Uses the method suggested by B.A. Berg (2004, p. 304) without weights.
    template <class InverseTemperatureIterator>
    static void optimize_berg(ParallelTemperingType& simulation, 
			      InverseTemperatureIterator inverse_temperatures_begin, 
			      InverseTemperatureIterator inverse_temperatures_end, 
			      unsigned int recursion_number);

    //! Static function for optimizing the inverse temperatures, can be used in the signal handlers.
    //! Uses the method suggested by B.A. Berg (2004, p. 304) with weights from worst acceptance rates.
    template <class InverseTemperatureIterator>
    static void optimize_berg_weights_worst_acceptance(ParallelTemperingType& simulation, 
						       InverseTemperatureIterator inverse_temperatures_begin, 
						       InverseTemperatureIterator inverse_temperatures_end, 
						       unsigned int recursion_number);

    //! Static function for optimizing the inverse temperatures, can be used in the signal handlers.
    //! Uses the method suggested by B.A. Berg (2004, p. 304) with weights from statistical independent acceptance rates.
    template <class InverseTemperatureIterator>
    static void optimize_berg_weights_independent_acceptance(ParallelTemperingType& simulation, 
							     InverseTemperatureIterator inverse_temperatures_begin, 
							     InverseTemperatureIterator inverse_temperatures_end, 
							     unsigned int recursion_number);

    //! Static function for optimizing the inverse temperatures, can be used in the signal handlers.
    //! Uses the method suggested by H.G. Katzgraber et al. (2006)
    template <class InverseTemperatureIterator>
    static void optimize_katzgraber(ParallelTemperingType& simulation, 
				    InverseTemperatureIterator inverse_temperatures_begin, 
				    InverseTemperatureIterator inverse_temperatures_end, 
				    unsigned int recursion_number);

  private:
    //! Does a single temperature optimization step using the method suggested by B.A. Berg (2004, p. 304)
    template <class InverseTemperatureIterator>
    static std::vector<double> optimize_berg_single(ParallelTemperingType& simulation,
						    InverseTemperatureIterator inverse_temperatures_begin,
						    InverseTemperatureIterator inverse_temperatures_end);

    //! Does the given number of optimization steps using the method suggestes by B.A. Berg (2004, p. 304) and returns the inverse temperatures and the acceptance probabilities
    template <class InverseTemperatureIterator>
    static void optimize_berg_general(ParallelTemperingType& simulation,
				      InverseTemperatureIterator inverse_temperatures_begin,
				      InverseTemperatureIterator inverse_temperatures_end,
				      unsigned int recursion_number,
				      std::vector<std::vector<double> >& acceptance_probabilities_all,
				      std::vector<std::vector<typename std::iterator_traits<InverseTemperatureIterator>::value_type> >& inverse_temperatures_all);


  };
  
} // of namespace Mocasinns

#include "src/parallel_tempering.cpp"

#endif
