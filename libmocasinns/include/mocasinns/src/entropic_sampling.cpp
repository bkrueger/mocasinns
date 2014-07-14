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

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>
::Parameters::Parameters() :
  Base(),
  flatness(0.9),
  sweep_steps(10000),
  prototype_histo()
{}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
template <class OtherParametersType>
EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::Parameters::Parameters(const OtherParametersType& other) :
  Base(other),
  flatness(other.flatness),
  sweep_steps(other.sweep_steps),
  prototype_histo(other.prototype_histo)
{}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
bool EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>
::Parameters::operator==(const Parameters& rhs) const
{
  return ((static_cast<Base>(*this) == static_cast<Base>(rhs)) &&
	  (flatness == rhs.flatness) &&
	  (sweep_steps == rhs.sweep_steps));
}
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
bool EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>
::Parameters::operator!=(const Parameters& rhs) const
{
  return !operator==(rhs);
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::EntropicSampling()
  : Simulation<ConfigurationType, RandomNumberGenerator>(static_cast<ConfigurationType*>(0)), simulation_parameters(Parameters()) { }
  
template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::EntropicSampling(const Parameters& params) 
  : Simulation<ConfigurationType, RandomNumberGenerator>(static_cast<ConfigurationType*>(0)), simulation_parameters(params) { } 

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::EntropicSampling(const Parameters& params, ConfigurationType* initial_configuration) 
  : Simulation<ConfigurationType, RandomNumberGenerator>(initial_configuration), simulation_parameters(params) { } 

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
double EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::acceptance_probability(StepType& step_to_execute, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
{
  // Calculate the energy difference of the step
  step_parameters.delta_E = step_to_execute.delta_E();
  EnergyType total_energy_after_step = step_parameters.total_energy + step_parameters.delta_E;

  // If an energy cutoff is used and the step would violate the energy cutoff, return 0.0
  if ((simulation_parameters.use_energy_cutoff_upper && total_energy_after_step > simulation_parameters.energy_cutoff_upper) || 
      (simulation_parameters.use_energy_cutoff_lower && total_energy_after_step < simulation_parameters.energy_cutoff_lower))
    return 0.0;

  // Calculate and return the acceptance probability
  return exp(log_density_of_states[step_parameters.total_energy] - log_density_of_states[total_energy_after_step]);
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
void EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::handle_executed_step(StepType&, double time, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
{
  // Increment the total energy
  step_parameters.total_energy += step_parameters.delta_E;
  // Update the histograms
  incidence_counter[step_parameters.total_energy] += time;
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
void EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::handle_rejected_step(StepType&, double, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
{
  // Update the histograms
  incidence_counter[step_parameters.total_energy]++;
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
void EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::do_entropic_sampling_steps(const StepNumberType& number)
{
  // Variable to track the energy
  Details::Multicanonical::StepParameter<EnergyType> step_parameters;
  step_parameters.total_energy = this->configuration_space->energy();

  // Call the generic function of Simulation
  this->template do_steps<EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>, StepType, rejection_free>(number, step_parameters);
}

template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
void EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::do_entropic_sampling_simulation()
{
  double flatness_current = 0.0;

  while (flatness_current < simulation_parameters.flatness)
  {
    // Do a number of entropic sampling steps
    do_entropic_sampling_steps(simulation_parameters.sweep_steps);

    // Update the density of states
    for (typename HistoType<EnergyType,IncidenceCounterYValueType>::const_iterator it = incidence_counter.begin(); it != incidence_counter.end(); ++it)
    {
      if (it->second != 0) log_density_of_states[it->first] += log(it->second);
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
    log_density_of_states.shift_bin_zero(log_density_of_states.min_x_value());
  }
}

// template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
// void EntropicSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator,rejection_free>::initialise_with_parameters()
// {
//   // Set the binnings of the density of states and the incidence counter
//   log_density_of_states.initialise_empty(simulation_parameters.prototype_histo);
//   incidence_counter.initialise_empty(simulation_parameters.prototype_histo);
// }

} // of namespace Mocasinns

#endif
