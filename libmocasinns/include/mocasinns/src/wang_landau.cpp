/*!
 * \file wang_landau.cpp
 * \brief Implementation of the WangLandau simulation class
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_WANG_LANDAU_HPP

namespace Mocasinns
{

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::Parameters::Parameters() :
  binning_reference(0), 
  binning_width(1), 
  energy_cutoff_lower(0),
  energy_cutoff_upper(0),
  use_energy_cutoff_lower(false),
  use_energy_cutoff_upper(false),
  flatness(0.8),
  modification_factor_initial(1.0),
  modification_factor_final(1e-7),
  modification_factor_multiplier(0.9),
  sweep_steps(1000),
  prototype_histo()
{}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
template <class OtherParametersType>
WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::Parameters::Parameters(const OtherParametersType& other) :
  binning_reference(other.binning_reference), 
  binning_width(other.binning_width), 
  energy_cutoff_lower(other.energy_cutoff_lower),
  energy_cutoff_upper(other.energy_cutoff_upper),
  use_energy_cutoff_lower(other.use_energy_cutoff_lower),
  use_energy_cutoff_upper(other.use_energy_cutoff_upper),
  flatness(other.flatness),
  modification_factor_initial(other.modification_factor_initial),
  modification_factor_final(other.modification_factor_final),
  modification_factor_multiplier(other.modification_factor_multiplier),
  sweep_steps(other.sweep_steps),
  prototype_histo(other.prototype_histo)
{}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
bool WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::Parameters::operator==(const Parameters& rhs) const
{
  return ((binning_reference == rhs.binning_reference) &&
	  (binning_width == rhs.binning_width) &&
	  (energy_cutoff_lower == rhs.energy_cutoff_lower) && 
	  (energy_cutoff_upper == rhs.energy_cutoff_upper) &
	  (use_energy_cutoff_lower == rhs.use_energy_cutoff_lower) && 
	  (use_energy_cutoff_upper == rhs.use_energy_cutoff_upper) &&
	  (flatness == rhs.flatness) &&
	  (modification_factor_initial == rhs.modification_factor_initial) &&
	  (modification_factor_final == rhs.modification_factor_final) &&
	  (modification_factor_multiplier == rhs.modification_factor_multiplier) &&
	  (sweep_steps == rhs.sweep_steps));
}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
bool WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::Parameters::operator!=(const Parameters& rhs) const
{
  return !operator==(rhs);
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>
::SimulationStatus::operator()(Simulation<ConfigurationType,RandomNumberGenerator>* simulation)
{
  WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>* simulation_wang_landau = static_cast<WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>*>(simulation);
  std::cout << simulation_wang_landau->get_config_space()->get_simulation_time() << "\t";
  std::cout << simulation_wang_landau->get_modification_factor_current() << "\t";
  std::cout << simulation_wang_landau->get_incidence_counter().flatness() << std::endl;
}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>
::SimulationDump::operator()(Simulation<ConfigurationType,RandomNumberGenerator>* simulation)
{
  WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>* simulation_wang_landau = static_cast<WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>*>(simulation);
  simulation_wang_landau->save_serialize(simulation->get_dump_filename().c_str());
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::WangLandau()
  : Simulation<ConfigurationType, RandomNumberGenerator>(static_cast<ConfigurationType*>(0)), sweep_counter(0)
{
  simulation_parameters = Parameters();
  initialise_with_parameters();
}
  
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::WangLandau(const Parameters& params) 
  : Simulation<ConfigurationType, RandomNumberGenerator>(static_cast<ConfigurationType*>(0)), sweep_counter(0)
{
  simulation_parameters = params;
  initialise_with_parameters();
}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::WangLandau(const Parameters& params, ConfigurationType* initial_configuration) 
  : Simulation<ConfigurationType, RandomNumberGenerator>(initial_configuration), sweep_counter(0)
{
  simulation_parameters = params;
  initialise_with_parameters();
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
double WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::acceptance_probability(StepType& step_to_execute, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
{
  // Calculate the energy difference of the step
  step_parameters.delta_E = step_to_execute.delta_E();

  // If an energy cutoff is used and the step would violate the energy cutoff, return 0.0
  if ((simulation_parameters.use_energy_cutoff_lower && step_parameters.total_energy + step_parameters.delta_E < simulation_parameters.energy_cutoff_lower) ||
      (simulation_parameters.use_energy_cutoff_upper && step_parameters.total_energy + step_parameters.delta_E > simulation_parameters.energy_cutoff_upper))
    return 0.0;

  // Calculate and return the acceptance probability
  return exp(log_density_of_states[step_parameters.total_energy] - log_density_of_states[step_parameters.total_energy + step_parameters.delta_E]);
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::handle_executed_step(StepType&, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
{
  // Increment the total energy
  step_parameters.total_energy += step_parameters.delta_E;
  
  // If the according bin does not exist in the density of states, initialise the density of states with the minimum dos + the current modification factor
  // If the according bin does exist, add the current modification factor to the density of states
  typename HistoType<EnergyType, double>::iterator update_position = log_density_of_states.find(step_parameters.total_energy);
  if (update_position == log_density_of_states.end())
    log_density_of_states.insert(std::pair<EnergyType, double>(step_parameters.total_energy, log_density_of_states.min_y_value()->second + modification_factor_current));
  else
    update_position->second += modification_factor_current;
  
  // Update the incidence counter
  incidence_counter[step_parameters.total_energy]++;
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::handle_rejected_step(StepType&, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
{
  // Update the histograms
  log_density_of_states[step_parameters.total_energy] += modification_factor_current;
  incidence_counter[step_parameters.total_energy]++;
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::do_wang_landau_steps(const uint32_t& number)
{
  // Variable to track the energy
  Details::Multicanonical::StepParameter<EnergyType> step_parameters;
  step_parameters.total_energy = this->configuration_space->energy();

  // Call the generic function of Simulation
  this->template do_steps<WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>, StepType>(number, step_parameters);
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::do_wang_landau_steps()
{
  // While the flatness is below the desired flatness, do sweep_steps wang landau steps
  while (incidence_counter.flatness() < simulation_parameters.flatness)
  {
    // Check for signals and return if simulation should be terminated
    if (this->check_for_posix_signal()) return;
    // Handle the sweep signal handler
    signal_handler_sweep(this);

    do_wang_landau_steps(simulation_parameters.sweep_steps);
    sweep_counter++;
  }
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::do_wang_landau_simulation()
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

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::initialise_with_parameters()
{
  // Set the current modification factor to the initial modification factor
  modification_factor_current = simulation_parameters.modification_factor_initial;

  // Set the binnings of the density of states and the incidence counter
  log_density_of_states.initialise_empty(simulation_parameters.prototype_histo);
  incidence_counter.initialise_empty(simulation_parameters.prototype_histo);
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::load_serialize(std::istream& input_stream)
{
  boost::archive::text_iarchive input_archive(input_stream);
  input_archive >> (*this);
}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::load_serialize(const char* filename)
{
  std::ifstream input_filestream(filename);
  load_serialize(input_filestream);
  input_filestream.close();
}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::save_serialize(std::ostream& output_stream) const
{
  boost::archive::text_oarchive output_archive(output_stream);
  output_archive << (*this);
}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::save_serialize(const char* filename) const
{
  std::ofstream output_filestream(filename);
  save_serialize(output_filestream);
  output_filestream.close();
}

} // of namespace Mocasinns

#endif
