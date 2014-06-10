/*! \file src/wang_landau.cpp
 * \brief Implementation of the WangLandau simulation class
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_WANG_LANDAU_HPP

/*! \fn AUTO_TEMPLATE_1
 * \details Construct a Wang-Landau simulation with standard parameters and a new ConfigurationType
 */
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
Mocasinns::WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::WangLandau()
  : Simulation<ConfigurationType, RandomNumberGenerator>(static_cast<ConfigurationType*>(0)), sweep_counter(0)
{
  simulation_parameters = Parameters();
  initialise_with_parameters();
}

/*! \fn AUTO_TEMPLATE_1
 * \details Construct a Wang-Landau simulation with given parameters and a new ConfigurationType
 * \param params Wang-Landau parameter object with the parameters of the simulation.
 */
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
Mocasinns::WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::WangLandau(const Parameters& params) 
  : Simulation<ConfigurationType, RandomNumberGenerator>(static_cast<ConfigurationType*>(0)), sweep_counter(0)
{
  simulation_parameters = params;
  initialise_with_parameters();
}

/*! \fn AUTO_TEMPLATE_1
 * \details Construct a Wang-Landau simulation with given parameters and a given ConfigurationType
 * \param params Wang-Landau parameter object with the parameters of the simulation.
 * \param initial_configuration Pointer to a ConfigurationType onto which the simulation will be performed.
 */
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
Mocasinns::WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::WangLandau(const Parameters& params, ConfigurationType* initial_configuration) 
  : Simulation<ConfigurationType, RandomNumberGenerator>(initial_configuration), sweep_counter(0)
{
  simulation_parameters = params;
  initialise_with_parameters();
}

/*! \fn AUTO_TEMPLATE_2
 * \details Copies a Wang-Landau simulation from another given simulation.
 * \tparam rejection_free_other Automatically determined from parameter
 * \param other WangLandau or WangLandauRejectionFree simulation object that will be copied.
 */
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
Mocasinns::WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::WangLandau(const WangLandau<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>& other)
  : Simulation<ConfigurationType, RandomNumberGenerator>(this->configuration_space), sweep_counter(0)
{
  log_density_of_states = other.log_density_of_states;
  incidence_counter = other.incidence_counter;
  simulation_parameters = other.simulation_parameters;
  modification_factor_current = other.modification_factor_current;
  sweep_counter = other.sweep_counter;
}

/*! \fn AUTO_TEMPLATE_1
 * \details The acceptance probability of the Wang-Landau simulation is calculated using the following formula:
 * \f[
 *   A(1 \rightarrow 2) = \mathrm{min}\left(1, \frac{g(E_1)}{g(E_2)}\right)
 * \f]
 *
 * \param step_to_execute Step of which the acceptance probability will be calculated
 * \param step_parameters Structure for storing the actual energy of the system and the energy difference of the simulation. (Used for performance reasons)
 */
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
double Mocasinns::WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::acceptance_probability(StepType& step_to_execute, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
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
    return exp(log_density_of_states[step_parameters.total_energy] - new_energy_bin->second);
  else
    return 1.0;
}
  
/*! \fn AUTO_TEMPLATE_1
 * \details the following updates are performed:
 * - The total energy stored in the \c step_parameter is increased by the energy difference of the step
 * - The incidence counter at the new total energy is increase by one
 * - The density of states is increased by the current modification factor
 *
 * If a new energy bin that was not encountered before is found, it is set to the minimum of all other density of states energy bins.
 *
 * \param time Specifies the time the algorithm has been in the previous state if doing a rejection free algorithm
 * \param step_parameters Structure for storing the actual energy of the system and the energy difference of the simulation. (Used for performance reasons)
 */
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::handle_executed_step(StepType&, double time, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
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
  
/*! \fn AUTO_TEMPLATE_1
 * \details Increase the incidence histogram at the current energy of the system.
 *
 * \param time Specifies the time the algorithm has been in the previous state if doing a rejection free algorithm
 * \param step_parameters Structure for storing the actual energy of the system and the energy difference of the simulation. (Used for performance reasons)
 */
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::handle_rejected_step(StepType&, double time, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
{
  // Update the histograms
  log_density_of_states[step_parameters.total_energy] += modification_factor_current*time;
  incidence_counter[step_parameters.total_energy] += time;
}

/*! \fn AUTO_TEMPLATE_1
 * \details The incidence counter and the density of states are modified using the actual modification factor.
 * \param number Number of steps to perform.
 */
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::do_wang_landau_steps(const uint32_t& number)
{
  // Variable to track the energy
  Details::Multicanonical::StepParameter<EnergyType> step_parameters;
  step_parameters.total_energy = this->configuration_space->energy();
  
  // Call the generic function of Simulation
  this->template do_steps<WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>, StepType, rejection_free>(number, step_parameters);
}
  
/*! \fn AUTO_TEMPLATE_1
 * \details The steps are executed in portions of the sweep steps given by the parameter object. Before each sweep a check for POSIX signals takes place and the signal_handler_sweep is called.
 */
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::do_wang_landau_steps()
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

/*! \fn AUTO_TEMPLATE_1
 * \details Performs the full Wang-Landau simulation. Executes step in portions of the sweep number (given by the parameters object) and test whether the incidence counter is flat.
 *
 * If the incidence counter is flat, the modification factor will be decreased, the incidence counter will be resetted, the density of states will be normalized and the signal_handler_modfac_change is called.
 */
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::do_wang_landau_simulation()
{
  while (modification_factor_current > simulation_parameters.modification_factor_final)
  {
    // Do steps until the flatness criterion is reached
    do_wang_landau_steps();

    // If the simulation was aborted, exit the loop
    if (this->is_terminating) break;
    
    // Invoke the information signal handler
    signal_handler_modfac_change(this);
    
    // Reset the incidence counter
    incidence_counter.set_all_y_values(0);
    // Renormalize the density of states
    log_density_of_states.shift_bin_zero(log_density_of_states.min_x_value());
    // Decrease the modification factor
    modification_factor_current *= simulation_parameters.modification_factor_multiplier;
  }
}

/*! \fn AUTO_TEMPLATE_1
 * \details Performs the Belardinelli version of the Wang-Landau algorithm that can be found in PRE 75, 046701 (2007).
 *
 * The basic differences are:
 * - The flatness of the incidence histogram is not checked, it does only matter that every energy has to be visited once
 * - If the new modfication factor is smaller than 1/t (where t is the Monte-Carlo time), it is not multiplied with the modification_factor multiplier, but chosen according to 1/t.
 *
 * \param monte_carlo_time_unit Number that specifies how many single steps are one Monte-Carlo time unit
 */
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::do_wang_landau_simulation_1_t(unsigned int monte_carlo_time_unit)
{
  // Set the sweep counter to 0
  sweep_counter = 0;

  // Do the first part of the algorithm (modification factor is bigger than the inverse Monte-Carlo time)
  while (modification_factor_current > simulation_parameters.modification_factor_final &&
	 (sweep_counter == 0 || // Needed to avoid dividing by one
	  modification_factor_current > static_cast<double>(monte_carlo_time_unit)/(sweep_counter * simulation_parameters.sweep_steps)))
  {
    // Do steps until each energy has been reached at least once
    while (incidence_counter.min_y_value()->second == 0)
    {
      do_wang_landau_steps(simulation_parameters.sweep_steps);
      sweep_counter++;
    }

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

  // Do the second part of the algorithm (modification factor is smaller than the inverse Monte-Carlo time)
  unsigned long monte_carlo_time_counter = 0;
  while (modification_factor_current > simulation_parameters.modification_factor_final)
  {
    do_wang_landau_steps(monte_carlo_time_unit);
    monte_carlo_time_counter++;
    
    // Decrease the modification factor
    modification_factor_current = 1.0 / (monte_carlo_time_counter + static_cast<double>(sweep_counter * simulation_parameters.sweep_steps) / monte_carlo_time_unit);

    // Invoke the information signal handler
    signal_handler_modfac_change(this);

    // If the simulation was aborted, exit the loop
    if (this->is_terminating) break;  
  }
    
  // Renormalize the density of states
  log_density_of_states.shift_bin_zero(log_density_of_states.min_x_value());
}
  
/*! \fn AUTO_TEMPLATE_1
 * \details The modficiation_factor_current is set to the initial modification factor given by the parameters object, the histograms log_density_of_states and incidence_counter are resetted and initialised according to the prototype histogram given with the parameters object.
 *
 * If the incidence counter is flat, the modification factor will be decreased, the incidence counter will be resetted, the density of states will be normalized and the signal_handler_modfac_change is called.
 */
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::initialise_with_parameters()
{
  // Set the current modification factor to the initial modification factor
  modification_factor_current = simulation_parameters.modification_factor_initial;
  
  // Set the binnings of the density of states and the incidence counter
  log_density_of_states.initialise_empty(simulation_parameters.prototype_histo);
  incidence_counter.initialise_empty(simulation_parameters.prototype_histo);
}

#endif
