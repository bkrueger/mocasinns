/*!
 * \file wang_landau.cpp
 * \brief Implementation of the WangLandau simulation class
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_WANG_LANDAU_WANG_LANDAU_BASE_HPP

namespace Mocasinns
{

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::WangLandauBase()
    : Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>(static_cast<ConfigurationType*>(0)), sweep_counter(0)
  {
    simulation_parameters = Parameters();
    initialise_with_parameters();
  }
  
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::WangLandauBase(const Parameters& params) 
    : Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>(static_cast<ConfigurationType*>(0)), sweep_counter(0)
  {
    simulation_parameters = params;
    initialise_with_parameters();
  }

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::WangLandauBase(const Parameters& params, ConfigurationType* initial_configuration) 
    : Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>(initial_configuration), sweep_counter(0)
  {
    simulation_parameters = params;
    initialise_with_parameters();
  }

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  template <bool rejection_free_other>
  WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::WangLandauBase(const WangLandauBase<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, rejection_free_other>& other)
    : Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>(this->configuration_space), sweep_counter(0)
  {
    log_density_of_states = other.log_density_of_states;
    incidence_counter = other.incidence_counter;
    simulation_parameters = other.simulation_parameters;
    modification_factor_current = other.modification_factor_current;
    sweep_counter = other.sweep_counter;
  }

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  double WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::acceptance_probability(StepType& step_to_execute, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
  {
    // Calculate the energy difference of the step
    step_parameters.delta_E = step_to_execute.delta_E();
    
    // If an energy cutoff is used and the step would violate the energy cutoff, return 0.0
    if ((simulation_parameters.use_energy_cutoff_lower && step_parameters.total_energy + step_parameters.delta_E < simulation_parameters.energy_cutoff_lower) ||
	(simulation_parameters.use_energy_cutoff_upper && step_parameters.total_energy + step_parameters.delta_E > simulation_parameters.energy_cutoff_upper))
      return 0.0;
    
    // Calculate and return the acceptance probability
    // If the new energy is not contained in the density of the states, return an acceptance probability of 1.0
    typename HistoType<EnergyType, double>::iterator new_energy_bin = log_density_of_states.find(step_parameters.total_energy + step_parameters.delta_E);
    if (new_energy_bin != log_density_of_states.end())
    {
      if (new_energy_bin->first == step_parameters.total_energy)
	return 0.1;
      else
	return exp(log_density_of_states[step_parameters.total_energy] - new_energy_bin->second);
    }
    else
      return 1.0;
  }
  
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  void WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::handle_executed_step(StepType&, double time, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
  {
    // Increment the total energy
    step_parameters.total_energy += step_parameters.delta_E;
    
    // If the according bin does not exist in the density of states, initialise the density of states with the minimum dos + the current modification factor
    // If the according bin does exist, add the current modification factor to the density of states
    typename HistoType<EnergyType, double>::iterator update_position = log_density_of_states.find(step_parameters.total_energy);
    if (update_position == log_density_of_states.end())
      log_density_of_states.insert(std::pair<EnergyType, double>(step_parameters.total_energy, log_density_of_states.min_y_value()->second + modification_factor_current*time));
    else
      update_position->second += std::min(1.0, modification_factor_current*time);
    
    // Update the incidence counter
    incidence_counter[step_parameters.total_energy] += std::min(1.0, time);
  }
  
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  void WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::handle_rejected_step(StepType&, double time, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
  {
    // Update the histograms
    log_density_of_states[step_parameters.total_energy] += modification_factor_current*time;
    incidence_counter[step_parameters.total_energy] += time;
  }
  
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  void WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::do_wang_landau_steps(const uint32_t& number)
  {
    // Variable to track the energy
    Details::Multicanonical::StepParameter<EnergyType> step_parameters;
    step_parameters.total_energy = this->configuration_space->energy();
    
    // Call the generic function of Simulation
    this->template do_steps<WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>, StepType>(number, step_parameters);
  }
  
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  void WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::do_wang_landau_steps()
  {
    // Set a local sweep counter
    unsigned int modfac_sweep_counter = 0;
    
    // While the flatness is below the desired flatness, do sweep_steps wang landau steps
    while (incidence_counter.flatness() < simulation_parameters.flatness)
    {
      // Check for signals and return if simulation should be terminated
      if (this->check_for_posix_signal()) return;
      // Handle the sweep signal handler
      signal_handler_sweep(this);
      
      do_wang_landau_steps(simulation_parameters.sweep_steps);
      modfac_sweep_counter++;
      sweep_counter++;
      
      // If the modfac_sweep_counter reached the reset number, reset the incidence counter
      if (modfac_sweep_counter == simulation_parameters.reset_sweep_number)
      {
	modfac_sweep_counter = 0;
	incidence_counter.set_all_y_values(0);
      }
    }
  }
  
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  void WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::do_wang_landau_simulation()
  {
    while (modification_factor_current > simulation_parameters.modification_factor_final)
    {
      // Do steps until the flatness criterion is reached
      do_wang_landau_steps();
      
      // Invoke the information signal handler
      signal_handler_modfac_change(this);
      // If the simulation was aborted, exit the loop
      if (this->is_terminating) break;

      // Reset the incidence counter
      incidence_counter.set_all_y_values(0);
      // Renormalize the density of states
      log_density_of_states.shift_bin_zero(log_density_of_states.min_x_value());
      // Decrease the modification factor
      modification_factor_current *= simulation_parameters.modification_factor_multiplier;
    }
  }
  
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  void WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::initialise_with_parameters()
  {
    // Set the current modification factor to the initial modification factor
    modification_factor_current = simulation_parameters.modification_factor_initial;
    
    // Set the binnings of the density of states and the incidence counter
    log_density_of_states.initialise_empty(simulation_parameters.prototype_histo);
    incidence_counter.initialise_empty(simulation_parameters.prototype_histo);
  }
  
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  void WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::load_serialize(std::istream& input_stream)
  {
    boost::archive::text_iarchive input_archive(input_stream);
    input_archive >> (*this);
  }

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  void WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::load_serialize(const char* filename)
  {
    std::ifstream input_filestream(filename);
    load_serialize(input_filestream);
    input_filestream.close();
  }

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  void WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::save_serialize(std::ostream& output_stream) const
  {
    boost::archive::text_oarchive output_archive(output_stream);
    output_archive << (*this);
  }
  
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  void WangLandauBase<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::save_serialize(const char* filename) const
  {
    std::ofstream output_filestream(filename);
    save_serialize(output_filestream);
    output_filestream.close();
  }

} // of namespace Mocasinns

#endif
