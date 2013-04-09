/*!
 * \file entropic_sampling.cpp
 * \brief Implementation of the entropic simulation class
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_ENTROPIC_SAMPLING_HPP

namespace Mocasinns
{

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>
::Parameters::Parameters() :
  binning_reference(0), 
  binning_width(1), 
  energy_cutoff_use(false), 
  energy_cutoff_upper(0), 
  energy_cutoff_lower(0),
  flatness(0.95),
  sweep_steps(1000),
  prototype_histo()
{}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
bool EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>
::Parameters::operator==(const Parameters& rhs) const
{
  return ((binning_reference == rhs.binning_reference) &&
	  (binning_width == rhs.binning_width) &&
	  (energy_cutoff_use == rhs.energy_cutoff_use) &&
	  (energy_cutoff_lower == rhs.energy_cutoff_lower) &&
	  (energy_cutoff_upper == rhs.energy_cutoff_upper) &&
	  (flatness == rhs.flatness) &&
	  (sweep_steps == rhs.sweep_steps));
}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
bool EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>
::Parameters::operator!=(const Parameters& rhs) const
{
  return !operator==(rhs);
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::EntropicSampling()
  : Simulation<ConfigurationType, RandomNumberGenerator>(static_cast<ConfigurationType*>(0)), simulation_parameters(Parameters()) { }
  
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::EntropicSampling(const Parameters& params) 
  : Simulation<ConfigurationType, RandomNumberGenerator>(static_cast<ConfigurationType*>(0)), simulation_parameters(params) { } 

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::EntropicSampling(const Parameters& params, ConfigurationType* initial_configuration) 
  : Simulation<ConfigurationType, RandomNumberGenerator>(initial_configuration), simulation_parameters(params) { } 

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
double EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::acceptance_probability(StepType& step_to_execute, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
{
  // Calculate the energy difference of the step
  step_parameters.delta_E = step_to_execute.delta_E();
  EnergyType total_energy_after_step = step_parameters.total_energy + step_parameters.delta_E;

  // If an energy cutoff is used and the step would violate the energy cutoff, return 0.0
  if (simulation_parameters.energy_cutoff_use && 
      (total_energy_after_step > simulation_parameters.energy_cutoff_upper || 
       total_energy_after_step < simulation_parameters.energy_cutoff_lower))
    return 0.0;

  // Calculate and return the acceptance probability
  return exp(density_of_states[step_parameters.total_energy] - density_of_states[total_energy_after_step]);
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::handle_executed_step(StepType&, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
{
  // Increment the total energy
  step_parameters.total_energy += step_parameters.delta_E;
  // Update the histograms
  incidence_counter[step_parameters.total_energy]++;
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::handle_rejected_step(StepType&, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
{
  // Update the histograms
  incidence_counter[step_parameters.total_energy]++;
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::do_entropic_sampling_steps(const StepNumberType& number)
{
  // Variable to track the energy
  Details::Multicanonical::StepParameter<EnergyType> step_parameters;
  step_parameters.total_energy = this->configuration_space->energy();

  // Call the generic function of Simulation
  this->template do_steps<EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>, StepType>(number, step_parameters);
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::do_entropic_sampling_simulation()
{
  double flatness_current = 0.0;

  while (flatness_current < simulation_parameters.flatness)
  {
    // Do a number of entropic sampling steps
    do_entropic_sampling_steps(simulation_parameters.sweep_steps);

    // Update the density of states
    for (typename HistoType<EnergyType,IncidenceCounterYValueType>::const_iterator it = incidence_counter.begin(); it != incidence_counter.end(); ++it)
    {
      if (it->second != 0) density_of_states[it->first] += log(it->second);
    }

    // Calculate the flatness
    flatness_current = incidence_counter.flatness();

    // Check for signals and return if simulation should be terminated
    if (this->check_for_posix_signal()) return;
    // Handle the sweep signal handler
    signal_handler_sweep(this);

    // Reset the incidence counter
    incidence_counter.set_all_y_values(0);
    // Renormalize the density of states
    density_of_states.shift_bin_zero(density_of_states.min_x_value());
  }
}

// template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
// void EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::initialise_with_parameters()
// {
//   // Set the binnings of the density of states and the incidence counter
//   density_of_states.initialise_empty(simulation_parameters.prototype_histo);
//   incidence_counter.initialise_empty(simulation_parameters.prototype_histo);
// }

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::load_serialize(std::istream& input_stream)
{
  boost::archive::text_iarchive input_archive(input_stream);
  input_archive >> (*this);
}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::load_serialize(const char* filename)
{
  std::ifstream input_filestream(filename);
  load_serialize(input_filestream);
  input_filestream.close();
}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::save_serialize(std::ostream& output_stream) const
{
  boost::archive::text_oarchive output_archive(output_stream);
  output_archive << (*this);
}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
void EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::save_serialize(const char* filename) const
{
  std::ofstream output_filestream(filename);
  save_serialize(output_filestream);
  output_filestream.close();
}

} // of namespace Mocasinns

#endif
