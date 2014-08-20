/*!
 * \file optimal_ensemble_sampling.cpp
 * \brief Implementation of the Optimal Ensemble Sampling simulation class
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_OPTIMAL_ENSEMBLE_SAMPLING_HPP

#include "../metropolis.hpp"

namespace Mocasinns
{
  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
  OptimalEnsembleSampling<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::OptimalEnsembleSampling()
    : Simulation<ConfigurationType, RandomNumberGenerator>(), simulation_parameters()
  { 
    initialize_with_parameters();
  }

  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
  OptimalEnsembleSampling<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::OptimalEnsembleSampling(const Parameters& parameters)
    : Simulation<ConfigurationType, RandomNumberGenerator>(), simulation_parameters(parameters)
  {
    initialize_with_parameters();
  }

  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
  OptimalEnsembleSampling<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::OptimalEnsembleSampling(const Parameters& parameters, ConfigurationType* initial_configuration)
    : Simulation<ConfigurationType, RandomNumberGenerator>(initial_configuration), simulation_parameters(parameters)
  {
    initialize_with_parameters();
  }

  /*! \fn AUTO_TEMPLATE_1
   * \details The acceptance probability of the Optimal Ensemble simulation is calculated using the following formula:
   * \f[
   *   A(E_1 \rightarrow E_2) = \mathrm{min}\left(1, \frac{P(E_2)}{P(E_1)}\right)
   * \f]
   * where \f$ P(E) \f$ are the current weights at energy level \f$E\f$.
   *
   * If the step leads to an energy wich is lower than the current minimal energy or higher than the current maximal energy of the simulation, the parameter is updated and the weight of the new state is set to the weight of the old extremal state.
   *
   * \param step_to_execute Step of which the acceptance probability will be calculated
   * \param step_parameters Structure for storing the actual energy of the system and the energy difference of the simulation. (Used for performance reasons)
   */
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  double OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::acceptance_probability(StepType& step_to_execute, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
  {
    // Calculate the energy difference of the step
    step_parameters.delta_E = step_to_execute.delta_E();
    EnergyType total_energy_after_step = step_parameters.total_energy + step_parameters.delta_E;

    // If an energy cutoff is used and the step would violate the energy cutoff, return 0.0
    if ((simulation_parameters.use_energy_cutoff_lower && step_parameters.total_energy + step_parameters.delta_E < simulation_parameters.energy_cutoff_lower) ||
	(simulation_parameters.use_energy_cutoff_upper && step_parameters.total_energy + step_parameters.delta_E > simulation_parameters.energy_cutoff_upper))
      return 0.0;
    
    // Check whether one leaves the energy range (then allways do the step)
    if (total_energy_after_step > simulation_parameters.maximal_energy)
    {
      // Set the weight of the new bin to the weight of the maximal bin
      weights[total_energy_after_step] = weights[simulation_parameters.maximal_energy];
      // Reset the maximal energy parameter
      simulation_parameters.maximal_energy = total_energy_after_step;
      // Execute the step 
      return 1.0;
    }
    if (total_energy_after_step < simulation_parameters.minimal_energy)
    {
      // Set the weight of the new bin to the weight of the minimal bin
      weights[total_energy_after_step] = weights[simulation_parameters.minimal_energy];
      // Reset the minimal energy parameter
      simulation_parameters.minimal_energy = total_energy_after_step;
      // Execute the step
      return 1.0;
    }
    
    // calculate the normal acceptance probability
    return exp(weights[total_energy_after_step] - weights[step_parameters.total_energy]);
  }
  
/*! \fn AUTO_TEMPLATE_1
 * \details the following updates are performed:
 * - The total energy stored in the \c step_parameter is increased by the energy difference of the step
 * - If the total energy equals the minimal or the maximal energy of the system, the label of the walker, which determines which incidence counter to fill, is adjusted
 * - The positive or the negative incidence counter (which one depends on the actual label of the walker) at the new total energy is increase by one
 *
 * \param time Specifies the time the algorithm has been in the previous state if doing a rejection free algorithm
 * \param step_parameters Structure for storing the actual energy of the system and the energy difference of the simulation. (Used for performance reasons)
 */
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  void OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::handle_executed_step(StepType&, double time, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
  {
    // Increment the total energy
    step_parameters.total_energy += step_parameters.delta_E;

    // If the negative energy barrier was reached, set the walker_label
    if (step_parameters.total_energy == simulation_parameters.minimal_energy)
      walker_label = negative;
    // If the positive energy barrier was reached, set the walker_label
    if (step_parameters.total_energy == simulation_parameters.maximal_energy)
      walker_label = positive;

    // Update the counting histograms
    if (walker_label == positive)
      incidence_counter_positive[step_parameters.total_energy] += time;
    else
      incidence_counter_negative[step_parameters.total_energy] += time;
  }
  
  /*! \fn AUTO_TEMPLATE_1
   * \details the following updates are performed:
   * - The positive or the negative incidence counter (which one depends on the actual label of the walker) at the current total energy is increase by one
   *
   * \param time Specifies the time the algorithm has been in the previous state if doing a rejection free algorithm
   * \param step_parameters Structure for storing the actual energy of the system and the energy difference of the simulation. (Used for performance reasons)
   */
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  void OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::handle_rejected_step(StepType&, double, Details::Multicanonical::StepParameter<EnergyType>& step_parameters)
  {
    // Update the counting histograms
    if (walker_label == positive)
      incidence_counter_positive[step_parameters.total_energy]++;
    else
      incidence_counter_negative[step_parameters.total_energy]++;
  }

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  bool OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::weights_recalculable()
  {
    // Fill the fraction histogram
    HistoType<EnergyType, double> fraction_histogram;
    for (typename HistoType<EnergyType, incidence_counter_y_value_t>::const_iterator it = incidence_counter_positive.begin();
	 it != incidence_counter_positive.end(); ++it)
    {	  
      fraction_histogram[it->first] = static_cast<double>(it->second) 
	/ static_cast<double>(it->second + incidence_counter_negative[it->first]);
    }
	
    // Set the flags to false
    bool incidence_counters_vanish = false;
    bool incidence_counters_zero_y_values = false;
    bool fraction_derivative_negative = false;

    // Test whether new sampling steps have to be done
    if (incidence_counter_positive.size() == 0 || incidence_counter_negative.size() == 0)
      incidence_counters_vanish = true;
    if (incidence_counter_positive.count_y(0) > 1 || incidence_counter_negative.count_y(0) > 1)
      incidence_counters_zero_y_values = true;
    for (typename HistoType<EnergyType, double>::const_iterator it = fraction_histogram.begin();
	 it != fraction_histogram.end(); ++it)
    {
      if (fraction_histogram.derivative(it) < 0)
	fraction_derivative_negative = true;
    }
    
    return !(incidence_counters_vanish || incidence_counters_zero_y_values || fraction_derivative_negative);
  }

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  void OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::recalculate_weights()
  {
    // Create and fill the fraction histogram
    HistoType<EnergyType, double> fraction_histogram;
    fraction_histogram.initialise_empty(weights);
    for (typename HistoType<EnergyType, incidence_counter_y_value_t>::const_iterator it = incidence_counter_positive.begin();
	 it != incidence_counter_positive.end(); ++it)
    {
      EnergyType energy = it->first;
      incidence_counter_y_value_t positive_incidence = it->second;
      incidence_counter_y_value_t negative_incidence = incidence_counter_negative[it->first];
      fraction_histogram[energy] = static_cast<double>(positive_incidence) / static_cast<double>(positive_incidence + negative_incidence);
    }
      
    // Calculate the new weights out of the old ones
    for (typename HistoType<EnergyType, double>::iterator energy_weight = weights.begin();
	 energy_weight != weights.end(); ++energy_weight)
    {
      EnergyType energy = energy_weight->first;
      energy_weight->second += 0.5 * (log(fraction_histogram.derivative(fraction_histogram.find(energy))) - log(incidence_counter_positive[energy] + incidence_counter_negative[energy]));
    }

    // Rescale the weights
    weights.shift_bin_zero(weights.min_y_value());
  }

  /*!
    \details Performs a number of steps with the acceptance probability given by the weights. Records the step in the positive and negative incidence counter histograms and resets the walker label if necessary.
    \param number Number of steps to perform.
   */
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  void OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::do_optimal_ensemble_sampling_steps(const uint32_t& number)
  {
    // Variable to track the energy
    Details::Multicanonical::StepParameter<EnergyType> step_parameters;
    step_parameters.total_energy = this->configuration_space->energy();
    
    // Call the generic method
    this->template do_steps<OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>,StepType, false>(number, step_parameters);
  }

  /*!
    \returns Histogram with the logarithmic density of states associated to each energy.
   */
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  HistoType<EnergyType, double> OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::do_optimal_ensemble_sampling_simulation()
  {
    // Log the start of the simulation
    this->simulation_start_log();

    // Create the fraction histogram
    HistoType<EnergyType, double> fraction_histogram;

    // Do the iterations
    for (unsigned int iteration = 0; iteration < simulation_parameters.iterations; ++iteration)
    {
      // Reset the positive and the negative incidence counter as well as the fraction histogram
      incidence_counter_positive.initialise_empty(weights);
      incidence_counter_negative.initialise_empty(weights);
      fraction_histogram.initialise_empty(weights);

      // Test whether the weights can be recalculated
      while(!weights_recalculable())
      {
	// Do the sampling steps
	do_optimal_ensemble_sampling_steps(pow(2,iteration) * simulation_parameters.initial_steps_per_iteration);

	// Check for signals and return if simulation should be terminated
	if (this->check_for_posix_signal()) return HistoType<EnergyType, double>();
      }
      // Recalculate the weights based on the data accumulated in the incidence counters
      recalculate_weights();

      // Invoke the signal handler after iteration
      signal_handler_iteration(this);
    }
      
    // Calculate and return the density of states
    return calculate_log_density_of_states();
  }

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  HistoType<EnergyType, double> OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::calculate_log_density_of_states() const
  {
    HistoType<EnergyType, double> log_density_of_states;
    log_density_of_states.initialise_empty(weights);
    for (typename HistoType<EnergyType, double>::const_iterator energy_weight = weights.begin();
	 energy_weight != weights.end(); ++energy_weight)
    {
      EnergyType energy = energy_weight->first;
      double weight = energy_weight->second;
      double positive_incidence = incidence_counter_positive.find(energy)->second;
      double negative_incidence = incidence_counter_negative.find(energy)->second;
      log_density_of_states[energy] = log(static_cast<double>(positive_incidence + negative_incidence)) - weight;
    }

    log_density_of_states.shift_bin_zero(log_density_of_states.min_y_value());

    return log_density_of_states;
  }

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  void OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::initialize_with_parameters()
  {
    weights.initialise_empty(simulation_parameters.prototype_histo);
    incidence_counter_positive.initialise_empty(simulation_parameters.prototype_histo);
    incidence_counter_negative.initialise_empty(simulation_parameters.prototype_histo);
  }
  
} // of namespace Mocasinns

#endif
