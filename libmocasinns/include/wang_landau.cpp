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
template <class ParameterEnergyType>
WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>
::Parameters<ParameterEnergyType>::Parameters() :
  binning_reference(0), 
  binning_width(1), 
  energy_cutoff_use(false), 
  energy_cutoff(0), 
  flatness(0.8),
  modification_factor_initial(1.0),
  modification_factor_final(1e-7),
  modification_factor_multiplier(0.9),
  sweep_steps(1000),
  prototype_histo()
{}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
template <class ParameterEnergyType>
bool WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>
::Parameters<ParameterEnergyType>::operator==(const Parameters<EnergyType>& rhs) const
{
  return ((binning_reference == rhs.binning_reference) &&
	  (binning_width == rhs.binning_width) &&
	  (energy_cutoff_use == rhs.energy_cutoff_use) &&
	  (flatness == rhs.flatness) &&
	  (modification_factor_initial == rhs.modification_factor_initial) &&
	  (modification_factor_final == rhs.modification_factor_final) &&
	  (modification_factor_multiplier == rhs.modification_factor_multiplier) &&
	  (sweep_steps == rhs.sweep_steps));
}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
template <class ParameterEnergyType>
bool WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>
::Parameters<ParameterEnergyType>::operator!=(const Parameters<EnergyType>& rhs) const
{
  return !operator==(rhs);
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>
::SimulationStatus::operator()(Simulation<ConfigurationType,RandomNumberGenerator>* simulation)
{
  WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>* simulation_wang_landau = static_cast<WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>*>(simulation);
  std::cout << simulation_wang_landau->get_config_space()->get_simulation_time() << "\t";
  std::cout << simulation_wang_landau->get_modification_factor_actual() << "\t";
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
  simulation_parameters = Parameters<EnergyType>();
  initialise_with_parameters();
}
  
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::WangLandau(const Parameters<EnergyType>& params) 
  : Simulation<ConfigurationType, RandomNumberGenerator>(static_cast<ConfigurationType*>(0)), sweep_counter(0)
{
  simulation_parameters = params;
  initialise_with_parameters();
}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::WangLandau(const Parameters<EnergyType>& params, ConfigurationType* initial_configuration) 
  : Simulation<ConfigurationType, RandomNumberGenerator>(initial_configuration), sweep_counter(0)
{
  simulation_parameters = params;
  initialise_with_parameters();
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::do_wang_landau_steps(const uint32_t& number)
{
  // Variable to track the energy
  EnergyType energy = this->configuration_space->energy();

  for (uint32_t i = 0; i < number; ++i)
  {
    StepType next_step = this->configuration_space->propose_step(this->rng);

    if (next_step.is_executable())
    {
      EnergyType delta_E = next_step.delta_E();
      
      if ((!simulation_parameters.energy_cutoff_use) || (energy + delta_E < simulation_parameters.energy_cutoff))
	{
	  // Calculate the acceptance probability
	  double acceptance_probability = exp(density_of_states[energy] - density_of_states[energy + delta_E])/next_step.selection_probability_factor();

	  if (this->rng->random_double() < acceptance_probability)
	    {
	      // Do the flip
	      energy += delta_E;
	      next_step.execute();
	    }
	}
    }

    // Update the histograms
    density_of_states[energy] += modification_factor_actual;
    incidence_counter[energy]++;
  }
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
  while (modification_factor_actual > simulation_parameters.modification_factor_final)
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
    density_of_states.shift_bin_zero(density_of_states.min_x_value());
    // Decrease the modification factor
    modification_factor_actual *= simulation_parameters.modification_factor_multiplier;
  }
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::initialise_with_parameters()
{
  // Set the actual modification factor to the initial modification factor
  modification_factor_actual = simulation_parameters.modification_factor_initial;

  // Set the binnings of the density of states and the incidence counter
  density_of_states.initialise_empty(simulation_parameters.prototype_histo);
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
