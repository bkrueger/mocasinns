/*!
 * \file optimal_ensemble_sampling.cpp
 * \brief Implementation of the Optimal Ensemble Sampling simulation class
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_OPTIMAL_ENSEMBLE_SAMPLING_HPP

#include "metropolis.hpp"

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

  /*!
    \details Constructs a metropolis simulation and does a lot of steps to find the maximal and the minimal energy of the system. Sets this minimal and maximal energy in the parameters of the Simulation.

    \tparam Type of the temperature to use. There must be an operator* defined for multiplying the inverse temperature with the energy of the system.
    \param inverse_temperature_minimal_energy Inverse temperature to be used for searching the minimal energy of the system
    \param inverse_temperature_maximal_energy Inverse temperature to be used for seraching the maximal energy of the system
    \param steps Number of steps to perform for searching each extremum of the energies of the system

    \todo After the SimulatedAnnealing simulation was implemented, use this instead of the Metropolis simulation
   */
  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
  template <class TemperatureType>
  void OptimalEnsembleSampling<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::find_minimal_maximal_energy(const TemperatureType& inverse_temperature_minimal_energy, const TemperatureType& inverse_temperature_maximal_energy, unsigned int steps)
  {
    typedef Metropolis<ConfigurationType, StepType, RandomNumberGenerator> MetropolisSimulation;
    
    // Define a metropolis simulation that executes the steps
    MetropolisSimulation metropolis_simulation(typename MetropolisSimulation::Parameters(), this->get_config_space());

    // Do the steps searching the minimal energy
    EnergyType minimal_energy = this->get_config_space()->energy();
    for (unsigned int i = 0; i < steps; ++i)
    {
      metropolis_simulation->do_metropolis_steps(1, inverse_temperature_minimal_energy);
      if (this->get_config_space()->energy() < minimal_energy)
	minimal_energy = this->get_config_space()->energy();
    }

    // Do the steps searching the minimal energy
    EnergyType maximal_energy = this->get_config_space()->energy();
    for (unsigned int i = 0; i < steps; ++i)
    {
      metropolis_simulation->do_metropolis_steps(1, inverse_temperature_maximal_energy);
      if (this->get_config_space()->energy() > maximal_energy)
	maximal_energy = this->get_config_space()->energy();
    }

    // Set the maximal and the minimal energy
    simulation_parameters.maximal_energy = maximal_energy;
    simulation_parameters.minimal_energy = minimal_energy;
  }

  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
  void OptimalEnsembleSampling<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::estimate_weights()
  {
    // Set parameters for a fast relaxation
    WangLandauParameters wl_parameters;
    wl_parameters.prototype_histo = simulation_parameters.prototype_histo;
    wl_parameters.sweep_steps = simulation_parameters.initial_steps_per_iteration;
    wl_parameters.modification_factor_initial = 1;
    wl_parameters.modification_factor_final = 1e-6;
    wl_parameters.modification_factor_multiplier = 0.5;
    wl_parameters.flatness = 0.8;

    // Call the general function
    estimate_weights(wl_parameters);
  }
  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
  void OptimalEnsembleSampling<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::estimate_weights(const WangLandauParameters& wang_landau_parameters)
  {
    // Create a WangLandau simulation
    WangLandauSimulation wl_simulation(wang_landau_parameters, this->get_config_space());

    // Perform the WangLandau simulation
    wl_simulation.do_wang_landau_simulation();
    // Extract the density of states
    HistoType<EnergyType, double> density_of_states = wl_simulation.get_density_of_states();

    // Set the weights to the logarithm of the inverse density of states
    for (typename HistoType<EnergyType, double>::const_iterator dos = density_of_states.begin();
	 dos != density_of_states.end(); ++dos)
    {
      weights[dos->first] = - dos->second;
    }
  }

  /*!
    \details Performs a number of steps with the acceptance probability given by the weights. Records the step in the positive and negative incidence counter histograms and resets the walker label if necessary.
    \param number Number of steps to perform.
   */
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  void OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::do_optimal_ensemble_sampling_steps(const uint32_t& number)
  {
    // Variable to track the energy
    EnergyType energy = this->configuration_space->energy();

    for (uint32_t i = 0; i < number; ++i)
    {
      StepType next_step = this->configuration_space->propose_step(this->rng);
      
      if (next_step.is_executable())
      {
	EnergyType delta_E = next_step.delta_E();
	
	// Calculate the acceptance probability
	double acceptance_probability = exp(weights[energy + delta_E] - weights[energy])/next_step.selection_probability_factor();
	
	if (this->rng->random_double() < acceptance_probability)
	{
	  // Do the flip
	  energy += delta_E;
	  next_step.execute();
	}

	// If the negative energy barrier was reached, set the walker_label
	if (energy == simulation_parameters.minimal_energy)
	  walker_label = negative;
	// If the positive energy barrier was reached, set the walker_label
	if (energy == simulation_parameters.maximal_energy)
	  walker_label = positive;
      }

      // Update the counting histograms
      if (walker_label == positive)
	incidence_counter_positive[energy]++;
      else
	incidence_counter_negative[energy]++;
    }  
  }

  /*!
    \returns Histogram with the logarithmic density of states associated to each energy.
   */
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  HistoType<EnergyType, double> OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::do_optimal_ensemble_sampling_simulation()
  {
    // Create the fraction histogram
    HistoType<EnergyType, double> fraction_histogram;

    // Do the iterations
    for (unsigned int iteration = 0; iteration < simulation_parameters.iterations; ++iteration)
    {
      // Reset the positive and the negative incidence counter as well as the fraction histogram
      incidence_counter_positive.initialise_empty(weights);
      incidence_counter_negative.initialise_empty(weights);
      fraction_histogram.initialise_empty(weights);

      // Do the steps until all values in the incidence counter have been reached
      bool incidence_counters_vanish = false;
      bool incidence_counters_zero_y_values = false;
      bool fraction_derivative_negative = false;
      do
      {
	// Do the sampling steps
	do_optimal_ensemble_sampling_steps(pow(2,iteration) * simulation_parameters.initial_steps_per_iteration);

	// Check for signals and return if simulation should be terminated
	if (this->check_for_posix_signal()) return HistoType<EnergyType, double>();

	// Fill the fraction histogram
	for (typename HistoType<EnergyType, unsigned long int>::const_iterator it = incidence_counter_positive.begin();
	     it != incidence_counter_positive.end(); ++it)
	{	  
	  fraction_histogram[it->first] = static_cast<double>(it->second) 
	    / static_cast<double>(it->second + incidence_counter_negative[it->first]);
	}
	
	// Set the flags to false
	incidence_counters_vanish = false;
	incidence_counters_zero_y_values = false;
	fraction_derivative_negative = false;

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
      } while (incidence_counters_vanish || incidence_counters_zero_y_values || fraction_derivative_negative);

      // Create and fill the fraction histogram
      HistoType<EnergyType, double> fraction_histogram;
      fraction_histogram.initialise_empty(weights);
      for (typename HistoType<EnergyType, unsigned long int>::const_iterator it = incidence_counter_positive.begin();
	   it != incidence_counter_positive.end(); ++it)
      {
	EnergyType energy = it->first;
	unsigned long int positive_incidence = it->second;
	unsigned long int negative_incidence = incidence_counter_negative[it->first];
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

      // Invoke the signal handler after iteration
      signal_handler_iteration(this);
    }
      
    // Calculate and return the density of states
    return calculate_density_of_states();
  }

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  void OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::load_serialize(std::istream& input_stream)
  {
    boost::archive::text_iarchive input_archive(input_stream);
    input_archive >> (*this);
  }
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  void OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::load_serialize(const char* filename)
  {
    std::ifstream input_filestream(filename);
    load_serialize(input_filestream);
    input_filestream.close();
  }
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  void OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::save_serialize(std::ostream& output_stream) const
  {
    boost::archive::text_oarchive output_archive(output_stream);
    output_archive << (*this);
  }
  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  void OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::save_serialize(const char* filename) const
  {
    std::ofstream output_filestream(filename);
    save_serialize(output_filestream);
    output_filestream.close();
  }

  template <class ConfigurationType, class StepType, class EnergyType, template <class,class> class HistoType, class RandomNumberGenerator>
  HistoType<EnergyType, double> OptimalEnsembleSampling<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>::calculate_density_of_states() const
  {
    HistoType<EnergyType, double> density_of_states;
    density_of_states.initialise_empty(weights);
    for (typename HistoType<EnergyType, double>::const_iterator energy_weight = weights.begin();
	 energy_weight != weights.end(); ++energy_weight)
    {
      EnergyType energy = energy_weight->first;
      double weight = energy_weight->second;
      double positive_incidence = incidence_counter_positive.find(energy)->second;
      double negative_incidence = incidence_counter_negative.find(energy)->second;
      density_of_states[energy] = log(static_cast<double>(positive_incidence + negative_incidence)) - weight;
    }

    density_of_states.shift_bin_zero(density_of_states.min_y_value());

    return density_of_states;
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
