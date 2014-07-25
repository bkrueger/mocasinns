/*!
 * \file wang_landau_replica_exchange.cpp
 *
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_WANG_LANDAU_REPLICA_EXCHANGE_HPP

#include <sstream>
#include <iterator>
#include <omp.h>

#include "../exceptions/iterator_range_exception.hpp"
#include "../exceptions/wrong_energy_exception.hpp"

namespace Mocasinns
{
  /*!
    \details This constructor takes the parameters object and a range of pointers to configurations to work on.
    The size of this range must equal the product of energy range number \f$ h \f$ and simulations per energy range \f$ m \f$ specified in the parameters region.
    The configurations must be in energy-range-first order, so that the \f$ m \f$ first configurations correspond to the first energy range and so on.
    If the current energy of the given configurations is not inside of the given energy range, an exception will be thrown

    \exception IteratorRangeException The size of the given iterator range is not equal to \f$ h \cdot m \f$, the product of energy range numbers and simulations per energy range
    \exception WrongEnergyException The energy of a given configuration is outside of the respective energy range
    
    \tparam ConfigurationPointerIterator Iterator of a range of pointers to configurations to work on
    \param params Parameters of the simulation
    \param configuration_pointers_begin Begin of the range of pointers to configurations to work on
    \param configuration_pointers_end End of the range of pointers to configurations to work on
  */    
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  template <class ConfigurationPointerIterator>
  WangLandauReplicaExchange<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::WangLandauReplicaExchange(const Parameters& params, 
																  ConfigurationPointerIterator configuration_pointers_begin, 
																  ConfigurationPointerIterator configuration_pointers_end) 
    : simulation_parameters(params),
      modification_factor_current(params.modification_factor_initial),
      replica_exchange_log_rejected(params.energy_ranges.size() - 1, 0),
      replica_exchange_log_executed(params.energy_ranges.size() - 1, 0)
  {
    // Check that the number of simulations matches the parameters
    if (static_cast<unsigned int>(std::distance(configuration_pointers_begin, configuration_pointers_end)) != params.energy_ranges.size() * params.simulations_per_replica)
    {
      std::stringstream error_string;
      error_string << "The size of the given iterators (here: " << std::distance(configuration_pointers_begin, configuration_pointers_end);
      error_string << ") must equal the product of energy range number (here: " << params.energy_ranges.size();
      error_string << ") and the simulations per replica (here: " << params.simulations_per_replica;
      error_string << ") specified in the parameters.";
      throw Exceptions::IteratorRangeException(error_string.str());
    }

    // Go through all configurations, store the pointers and set up a WangLandau simulation
    ConfigurationPointerIterator configuration_pointers_it = configuration_pointers_begin;
    for (unsigned int e = 0; e < params.energy_ranges.size(); ++e)
    {
      typename WangLandauType::Parameters base_parameters(params);
      base_parameters.use_energy_cutoff_lower = true;
      base_parameters.use_energy_cutoff_upper = true;
      base_parameters.energy_cutoff_lower = simulation_parameters.energy_ranges[e].first;
      base_parameters.energy_cutoff_upper = simulation_parameters.energy_ranges[e].second;

      for (unsigned int r = 0; r < params.simulations_per_replica; ++r)
      {
	// Check that the energy of the configuration is in the correct range
	if ((*configuration_pointers_it)->energy() < simulation_parameters.energy_ranges[e].first ||
	    (*configuration_pointers_it)->energy() > simulation_parameters.energy_ranges[e].second)
	{
	  std::stringstream error_string;
	  error_string << "The energy of configuration " << e*params.simulations_per_replica + r;
	  error_string << ", E = " << (*configuration_pointers_it)->energy();
	  error_string << " is outside of the given energy range " << e << ", ";
	  error_string << simulation_parameters.energy_ranges[e].first << " <= E <= " << simulation_parameters.energy_ranges[e].second << ".";
	  throw Exceptions::WrongEnergyException(error_string.str());
	}

	// Create the Wang-Landau simulations
	wang_landau_simulations.push_back(WangLandauType(base_parameters, *configuration_pointers_it));
	// Add the replica index to the random seed to avoid sampling with the same sedd
	wang_landau_simulations.back().set_random_seed(this->get_random_seed());
	configuration_pointers_it++;
      }
    }

    // Set up the density of states vector
    log_density_of_states = std::vector<LogDensityOfStatesType>(params.energy_ranges.size());
  }
  
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  void WangLandauReplicaExchange<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::do_wang_landau_sweeps(const step_number_t& sweep_number)
  {  
    // OpenMP cannot return in a parallelised loop, so a simple return on a catched POSIX signal is not possible
    // So define a boolean that is set to true on catched POSIX signals and only execute the sweeps if this boolean is false
    bool posix_signal_caught = false;

    omp_set_num_threads(simulation_parameters.process_number);
#pragma omp parallel for
    for (unsigned int i = 0; i < wang_landau_simulations.size(); ++i)
    {
      if (!posix_signal_caught)
      {
	step_number_t sweep_counter = 0;
	while (wang_landau_simulations[i].get_incidence_counter().flatness() < simulation_parameters.flatness &&
	       sweep_counter < sweep_number)
	{
	  wang_landau_simulations[i].do_wang_landau_steps(simulation_parameters.sweep_steps);
	  sweep_counter++;
	  
	  // Check for signals and return if simulation should be terminated
	  posix_signal_caught = this->check_for_posix_signal();
	}
      }
    }
  }

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  void WangLandauReplicaExchange<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::do_replica_exchange()
  {
    if (simulation_parameters.energy_ranges.size() == 1)
    {
      replica_exchange_log_rejected[0]++;
      return;
    }

    // Select randomly the index of the energy ranges to exchange
    unsigned int exchange_index = this->rng->random_int32(0, simulation_parameters.energy_ranges.size() - 2);
    // Select randomly the index of the simulation to exchange
    unsigned int simulation_index_1, simulation_index_2;
    if (simulation_parameters.simulations_per_replica == 1)
    {
      simulation_index_1 = exchange_index;
      simulation_index_2 = exchange_index + 1;
    }
    else
    {
      simulation_index_1 = exchange_index*simulation_parameters.simulations_per_replica 
	+ this->rng->random_int32(1, simulation_parameters.simulations_per_replica - 1);
      simulation_index_2 = (exchange_index + 1)*simulation_parameters.simulations_per_replica 
	+ this->rng->random_int32(1, simulation_parameters.simulations_per_replica - 1);
    }
    
    // Get the density of states and the energies of the selected walkers
    const HistoType<EnergyType, double>& log_dos_1 = wang_landau_simulations[simulation_index_1].get_log_density_of_states();
    const HistoType<EnergyType, double>& log_dos_2 = wang_landau_simulations[simulation_index_2].get_log_density_of_states();
    EnergyType energy_1 = wang_landau_simulations[simulation_index_1].get_config_space()->energy();
    EnergyType energy_2 = wang_landau_simulations[simulation_index_2].get_config_space()->energy();

    // Check that the energies of the selected walkers are contained in the energy ranges
    if (energy_1 < simulation_parameters.energy_ranges[exchange_index + 1].first ||
	energy_2 > simulation_parameters.energy_ranges[exchange_index].second)
    {
      replica_exchange_log_rejected[exchange_index]++;
      return;
    }

    // Calculate the acceptance probability
    double acceptance_probability = exp(log_dos_1[energy_1] - log_dos_1[energy_2] + 
					log_dos_2[energy_2] - log_dos_2[energy_1]);

    // Do the step with the acceptance probability
    if (acceptance_probability < 1 && this->rng->random_double() < acceptance_probability)
    {
      // Exchange the configurations in the metropolis simulations
      ConfigurationType* temp(wang_landau_simulations[simulation_index_1].get_config_space());
      wang_landau_simulations[simulation_index_1].set_config_space(wang_landau_simulations[simulation_index_2].get_config_space());
      wang_landau_simulations[simulation_index_2].set_config_space(temp);

      replica_exchange_log_executed[exchange_index]++;
    }
    else
      replica_exchange_log_rejected[exchange_index]++;
  }

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  void WangLandauReplicaExchange<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::average_density_of_states()
  {
    // If there is only one simulation per energy range, do only the dos shift
    if (simulation_parameters.simulations_per_replica == 1) 
    {
      for (unsigned int e = 0; e < simulation_parameters.energy_ranges.size(); ++e)
      {
	LogDensityOfStatesType replica_dos = wang_landau_simulations[e].get_log_density_of_states();
	replica_dos.shift_bin_zero(replica_dos.min_x_value());
	wang_landau_simulations[e].set_log_density_of_states(replica_dos);
	log_density_of_states[e] = replica_dos;
      }
      return;
    }

    for (unsigned int e = 0; e < simulation_parameters.energy_ranges.size(); ++e)
    {
      // Define the averaged_dos
      LogDensityOfStatesType averaged_dos;
      averaged_dos.initialise_empty(wang_landau_simulations[e*simulation_parameters.simulations_per_replica].get_log_density_of_states());
      
      // Sum up
      for (unsigned int r = 0; r < simulation_parameters.simulations_per_replica; ++r)
      {
	LogDensityOfStatesType replica_dos = wang_landau_simulations[e*simulation_parameters.simulations_per_replica + r].get_log_density_of_states();
	replica_dos.shift_bin_zero(replica_dos.min_x_value());
	for (typename LogDensityOfStatesType::iterator dos_it = replica_dos.begin(); dos_it != replica_dos.end(); ++dos_it)
	  averaged_dos[dos_it->first] += exp(dos_it->second) / simulation_parameters.simulations_per_replica;
      }

      // Calculate the logarithm
      for (typename LogDensityOfStatesType::iterator dos_it = averaged_dos.begin(); dos_it != averaged_dos.end(); ++dos_it)
	dos_it->second = log(dos_it->second);

      // Assign the averaged dos
      for (unsigned int r = 0; r < simulation_parameters.simulations_per_replica; ++r)
	wang_landau_simulations[e*simulation_parameters.simulations_per_replica + r].set_log_density_of_states(averaged_dos);
      log_density_of_states[e] = averaged_dos;
    }
  }

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  void WangLandauReplicaExchange<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::clear_logs()
  {
    // Clear the replica exchange logs
    for (typename ReplicaExchangeLog::iterator log_it = replica_exchange_log_executed.begin();
	 log_it != replica_exchange_log_executed.end(); ++log_it)
      *log_it = 0;
    for (typename ReplicaExchangeLog::iterator log_it = replica_exchange_log_rejected.begin();
	 log_it != replica_exchange_log_rejected.end(); ++log_it)
      *log_it = 0;
  }

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  void WangLandauReplicaExchange<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::do_wang_landau_replica_exchange_simulation()
  {
    // Log the start of the simulation
    this->simulation_start_log();

    // Use the member density of states to initialise the density of states of the single simulations
    for (unsigned int e = 0; e < simulation_parameters.energy_ranges.size(); ++e)
    {
      for (unsigned int r = 0; r < simulation_parameters.simulations_per_replica; ++r)
	wang_landau_simulations[e*simulation_parameters.simulations_per_replica + r].set_log_density_of_states(log_density_of_states[e]);
    }

    // Do the actual simulation
    while(modification_factor_current > simulation_parameters.modification_factor_final)
    {
      bool all_incidence_counters_flat = false;
      while (!all_incidence_counters_flat)
      {
	// Do sweeps
	do_wang_landau_sweeps(simulation_parameters.sweeps_per_replica_exchange);
	// Do a replica exchange
	do_replica_exchange();
	// Invoke the information signal handler
	signal_handler_replica_exchange(this);
	// Check flatness
	all_incidence_counters_flat = true;
	for (unsigned int i = 0; i < wang_landau_simulations.size(); ++i)
	  if (wang_landau_simulations[i].get_incidence_counter().flatness() < simulation_parameters.flatness) { all_incidence_counters_flat = false; break; }

	// If the simulation was aborted, exit the loop
	if (this->is_terminating) break;
      }
      
      // If the simulation was aborted, exit the loop
      if (this->is_terminating) break;

      // Invoke the information signal handler
      signal_handler_modfac_change(this);

      // Reset the incidence counters, normalize and average the density of states
      modification_factor_current *= simulation_parameters.modification_factor_multiplier;	
      for (unsigned int i = 0; i < wang_landau_simulations.size(); ++i)
      {
	wang_landau_simulations[i].set_incidence_counter();
	wang_landau_simulations[i].set_modification_factor_current(modification_factor_current);
      }
      average_density_of_states();
    }
  }
}
#endif
