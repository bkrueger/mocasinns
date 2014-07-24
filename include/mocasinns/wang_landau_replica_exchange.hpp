/**
 * \file wang_landau_replica_exchange.hpp
 * \brief Class for parallel tempering simulations with MoCaSinns
 * 
 * Do a parallel replica exchange Wang-Landau simulation described by T. Vogel et al, PRL 110 (2013) 210603
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_WANG_LANDAU_REPLICA_EXCHANGE_HPP
#define MOCASINNS_WANG_LANDAU_REPLICA_EXCHANGE_HPP

#include "simulation.hpp"
#include "wang_landau.hpp"
#include "concepts/concepts.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

// Boost function types for the standard observable
#include <boost/function_types/result_type.hpp>
#include <boost/typeof/std/utility.hpp>
#include <boost/type_traits.hpp>

namespace Mocasinns
{
  //! Class for replica exchange Wang-Landau simulations
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  class WangLandauReplicaExchange : public Simulation<ConfigurationType, RandomNumberGenerator>
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
    typedef WangLandauReplicaExchange<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator> this_type;
    //! Typedef for a Wang-Landau simulation with the same template paramters
    typedef WangLandau<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator> WangLandauType;
    // Typedefs for integers
    typedef typename Base::step_number_t step_number_t;
    typedef typename Base::incidence_counter_y_value_t incidence_counter_y_value_t;
    // Typedefs for histograms
    typedef HistoType<EnergyType, incidence_counter_y_value_t> IncidenceCounterType;
    typedef HistoType<EnergyType, double> LogDensityOfStatesType;
    //! Forward declaration of the struct storing the Parameters of a Metropolis Simulation
    struct Parameters;

    //! Boost signal handler invoked if a replica exchange is tried 
    boost::signals2::signal<void (Base*)> signal_handler_replica_exchange;
    //! Boost signal handler invoked if the current modification factor is altered by the simulation
    boost::signals2::signal<void (Base*)> signal_handler_modfac_change;

    //! Initialise a parallel tempering simulation with given parameters and a range of pointers to configuration spaces
    template <class ConfigurationPointerIterator>
    WangLandauReplicaExchange(const Parameters& params, ConfigurationPointerIterator configuration_pointers_begin, ConfigurationPointerIterator configuration_pointers_end);

    //! Get-accessor for the first configuration pointer
    ConfigurationType* get_config_space() { return get_config_space(0); }
    //! Get-accessor for the configuration pointer
    ConfigurationType* get_config_space(unsigned int index) { return wang_landau_simulations[index].get_config_space(); }
    //! Get-accessor for all configuration spaces
    std::vector<ConfigurationType*> get_config_spaces()
    {
      std::vector<ConfigurationType*> result;
      for (unsigned int i = 0; i < wang_landau_simulations.size(); ++i)
	result.push_back(get_config_space(i));
      return result;
    }
    //! Get-accessor for the parameters of the parallel tempering simulation
    const Parameters& get_simulation_parameters() { return simulation_parameters; }
    //! Set-accessor for the parameters of the parallel tempering simulation
    void set_simulation_parameters(const Parameters& value) { simulation_parameters = value; }
    //! Get-accessor for the current modification factor
    double get_modification_factor_current() { return modification_factor_current; }
    //! Set-accessor for the current modification factor
    void set_modification_factor_current(double value) { modification_factor_current = value; }
    //! Get-accessor for the density of states of the energy ranges
    const std::vector<LogDensityOfStatesType>& get_log_density_of_states() { return log_density_of_states; }
    //! Set-accessor for the density of states of the energy ranges
    void set_log_density_of_states(const std::vector<LogDensityOfStatesType>& value) { log_density_of_states = value; }

    //! Execute a given number of parallel tempering steps on the configuration at inverse temperatur beta
    void do_wang_landau_sweeps(const step_number_t& sweep_number);

    //! Execute an replica exchange step
    unsigned int do_replica_exchange();
    //! Average the density of states of the simulations in the same energy range
    void average_density_of_states();

    //! Execute a replica exchange Wang-Landau simulation
    void do_wang_landau_replica_exchange_simulation();

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
    //! Member storing the overall current modification factor
    double modification_factor_current;

    //! Member variable for storing the instances of the Metropolis simulations
    std::vector<WangLandauType> wang_landau_simulations;

    //! Member variable for storing the density of states for the energy ranges
    std::vector<LogDensityOfStatesType> log_density_of_states;

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
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  struct WangLandauReplicaExchange<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::Parameters
    : WangLandau<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::Parameters
  {    
    //! Number of sweeps to execute before to try a replica exchange
    typename WangLandauReplicaExchange<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::step_number_t sweeps_per_replica_exchange;

    //! Number of independent simulations per energy range
    unsigned int simulations_per_replica;

    //! Number of parallel processes to use.
    unsigned int process_number;

    //! Vector of energy ranges
    std::vector<std::pair<EnergyType, EnergyType> > energy_ranges;
    
    Parameters() 
      : WangLandau<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::Parameters(), 
	sweeps_per_replica_exchange(10),
	simulations_per_replica(1),
	process_number(2) {}
  };
}
#include "src/wang_landau_replica_exchange.cpp"

#endif
