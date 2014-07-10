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
  /*!
   * The parallel tempering (or replica exchange) Monte Carlo is an extension of the Metropolis algorithm to overcome local energy barriers. 
   * The basic principle is to perform \f$N\f$ independent Metropolis simulations at inverse temperatures \f$\beta_1 < \dots < \beta_N\f$ in parallel, each on a seperate copy of the system (replica), and to exchange two inverse temperatures \f$\beta_i\f$ and \f$\beta_j\f$ after a certain number of flips with the acceptance probability
   * \f[
   * A(\beta_i \leftrightarrow \beta_2) = 
   * \begin{cases}
   * 1 & \text{if }\Delta \beta \cdot \Delta E \geq \\
   * \exp\left(\Delta \beta \cdot \Delta E\right) & \text{if }\Delta \beta \cdot \Delta E < 0
   * \end{cases}
   * \f]
   * where \f$\Delta \beta = \beta_{i+1} - \beta_i\f$ is the difference between the two neighbouring inverse temperatures and \f$\Delta E = E_{i+1} - E_i\f$ is the difference between the two replicas with the respective energies.
   * This choice ensures that the detailed balance condition also holds for the replica exchanges.
   *
   * In the <tt>mocasinns</tt> framework there are two different algorithms for doing parallel tempering simulations: 
   * - <tt>SerialTempering</tt> for doing parallel tempering on a single core, so that the Metropolis simulations for the different inverse temperatures are executed one after another
   * - <tt>ParallelTempering</tt> for parallel tempering on multiple cores, where the single Metropolis simulations are executed in parallel using OpenMP.
   * 
   * An important problem for the parallel tempering algorithm is how to choose the inverse temperatures \f$\beta_i\f$.
   * If the chosen temperature range is to wide, the acceptance probabilities \ref{eq:parallel_tempering_swap_acceptance} approach 0 and one gets basically independent Metropolis simulations which cannot escape local energy minima.
   * If the chosen temperature range is to small, computation time is wasted since simulations with neighbouring inverse temperatures produce basically the same information.
   * Especially if there are phase transitions present in the system at certain inverse temperatures, the range around these critical temperatures must be sampled mor accuratly as far away from the cricitcal regions.
   *
   * \tparam ConfigurationType \concept{ConfigurationType}
   * \tparam StepType \concept{StepType}
   * \tparam RandomNumberGenerator \concept{RandomNumberGenerator}
   */
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
    // Typedef for the base class
    typedef Simulation<ConfigurationType, RandomNumberGenerator> Base;
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
    typedef typename Base::StepNumberType StepNumberType;
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

    //! Load the data of the Parallel Tempering simulation from a serialization stream
    virtual void load_serialize(std::istream& input_stream) { Base::load_serialize(*this, input_stream); }
    //! Load the data of the Parallel Tempering simulation from a serialization file
    virtual void load_serialize(const char* filename) { Base::load_serialize(*this, filename); }
    //! Save the data of the Parallel Tempering simulation to a serialization stream
    virtual void save_serialize(std::ostream& output_stream) const { Base::save_serialize(*this, output_stream); }
    //! Save the data of the Parallel Tempering simulation to a serialization file
    virtual void save_serialize(const char* filename) const { Base::save_serialize(*this, filename); }

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

#include "src/parallel_tempering.cpp"

#endif
