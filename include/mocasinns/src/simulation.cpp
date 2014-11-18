/*! \file simulation.cpp
 * \brief Implementation of the base class for all Monte-Carlo-Simulations in MoCaSinns
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_SIMULATION_HPP

#include <limits>

template <class ConfigurationType, class RandomNumberGenerator>
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator>::register_posix_signal_handler()
{
  // Set the signal handle
  signal(SIGTERM, handle_posix_signal);
  signal(SIGUSR1, handle_posix_signal);
  signal(SIGUSR2, handle_posix_signal);
}

/*!
 * \param signal_number Enum for the type of the signals
 */
template <class ConfigurationType, class RandomNumberGenerator>
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator>::handle_posix_signal(int signal_number)
{
  if (signal_number == SIGTERM) signal_number_caught = 1;
  if (signal_number == SIGUSR1) signal_number_caught = 2;
  if (signal_number == SIGUSR2) signal_number_caught = 3;
}

template <class ConfigurationType, class RandomNumberGenerator>
bool Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator>::check_for_posix_signal()
{
  switch(signal_number_caught)
  {
  case 1: // SIGTERM
    signal_handler_sigterm(this);
    is_terminating = true;
    return true;
  case 2: // SIGUSR1
    signal_handler_sigusr1(this);
    signal_number_caught = 0;
    return false;
  case 3: // SIGUSR2
    signal_handler_sigusr2(this);
    signal_number_caught = 0;
    return false;
  default: // Avoid compiler warning
    return false;
  }
}

template <class ConfigurationType, class RandomNumberGenerator>
template <class Derived, class StepType, bool function_rejection_free, class AcceptanceProbabilityParameterType>
typename boost::enable_if_c<!function_rejection_free, void>::type
Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator>::do_steps(const step_number_t& step_number, AcceptanceProbabilityParameterType& acceptance_probability_parameter)
{
  for (step_number_t i = 0; i < step_number; ++i)
  {
    // Propose a new step
    StepType next_step = this->configuration_space->propose_step(this->rng);
    
    // If the next step is executable, calculate the acceptance probability
    if (Details::OptionalMemberFunctions::optional_is_executable<StepType>(next_step))
    {
      // Calculate selection probability factor and acceptance probability
      double selection_probability_factor = Details::OptionalMemberFunctions::optional_selection_probability_factor<StepType>(next_step);
      double step_probability(static_cast<Derived*>(this)->acceptance_probability(next_step, acceptance_probability_parameter));
      step_probability /= selection_probability_factor;

      // Do the step with the correct probability and call the handlers
      if (step_probability > 0.0 && (step_probability >= 1.0 || this->rng->random_double() < step_probability))
      {
	next_step.execute();
#ifdef MOCASINNS_ACCEPTANCE_RATIO
	accepted_steps++;
#endif
	static_cast<Derived*>(this)->handle_executed_step(next_step, 1.0, acceptance_probability_parameter);
      }
      else
      {
#ifdef MOCASINNS_ACCEPTANCE_RATIO
	rejected_steps++;
#endif
	static_cast<Derived*>(this)->handle_rejected_step(next_step, 1.0, acceptance_probability_parameter);
      }
    } // of if (next_step.is_executable())
    else
    {
#ifdef MOCASINNS_ACCEPTANCE_RATIO
      rejected_steps++;
#endif
      static_cast<Derived*>(this)->handle_rejected_step(next_step, 1.0, acceptance_probability_parameter);
    }
  }
}
 
template <class ConfigurationType, class RandomNumberGenerator>
template <class Derived, class StepType, bool function_rejection_free, class AcceptanceProbabilityParameterType>
typename boost::enable_if_c<function_rejection_free, void>::type
Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator>::do_steps(const step_number_t& step_number, AcceptanceProbabilityParameterType& acceptance_probability_parameter)
{
  double remaining_simulation_time = step_number;

  while (remaining_simulation_time > 0)
  {
    // Propose all possible steps
    std::vector<StepType> all_steps = this->configuration_space->all_steps();
    std::vector<double> acceptance_probabilities(all_steps.size(), 0.0);

    // Iterate all steps and calculate the acceptance probabilities
    for (unsigned int s = 0; s < all_steps.size(); ++s)
    {
      // If the step is executable, calculate the acceptance probability
      // If the step is not executable, the acceptance probability remains 0
      if (Details::OptionalMemberFunctions::optional_is_executable<StepType>(all_steps[s]))
      {
	// Calculate selection probability factor and acceptance probability
	double selection_probability_factor = Details::OptionalMemberFunctions::optional_selection_probability_factor<StepType>(all_steps[s]);
	double step_probability(static_cast<Derived*>(this)->acceptance_probability(all_steps[s], acceptance_probability_parameter));
	step_probability /= selection_probability_factor;

	// Store the acceptance probability
	acceptance_probabilities[s] = std::min(1.0, step_probability);
      }
    } 

    // Calculate the cumulated acceptance probabilities
    // If a step at position r is not executable, cumulative_acceptance_probabilities[r + 1] = cumulative_acceptance_probabilities[r]
    std::vector<double> cumulative_acceptance_probabilities(all_steps.size() + 1);
    cumulative_acceptance_probabilities[0] = 0.0;
    for (unsigned int s = 1; s < all_steps.size() + 1; ++s)
    {
      cumulative_acceptance_probabilities[s] = cumulative_acceptance_probabilities[s - 1] + acceptance_probabilities[s - 1];
    }

    // Create a random number and determine which step to execute
    double rnd = rng->random_double()*cumulative_acceptance_probabilities[all_steps.size()];
    unsigned int step_index = 0;
    for (unsigned int s = 0; s < all_steps.size(); ++s)
    {
      if (cumulative_acceptance_probabilities[s] <= rnd && cumulative_acceptance_probabilities[s+1] > rnd)
	step_index = s;
    }

    // Calculate again the acceptance probability to get the acceptance probability parameter right.
    // TODO: Correct this in the future because this is not efficient
    static_cast<Derived*>(this)->acceptance_probability(all_steps[step_index], acceptance_probability_parameter);

    // Check that the maximal cumulative acceptance probability is not 0 (this means that the entropy difference of the neighbouring bins is to high for double precission and we would stay infinitesimal long in the state
    // If this is the case, do not execute the step, but handle a rejected step
    double time = 1.0 / cumulative_acceptance_probabilities[all_steps.size()];
    // ToDo: Check whether updating with another factor makes sense
    if (time == std::numeric_limits<double>::infinity() || time != time)
      static_cast<Derived*>(this)->handle_rejected_step(all_steps[step_index], 1.0, acceptance_probability_parameter);
    else
    {
      if (remaining_simulation_time - time > 0)
      {
	all_steps[step_index].execute();
	// Handle the executed step with time
	static_cast<Derived*>(this)->handle_executed_step(all_steps[step_index], time, acceptance_probability_parameter);
	remaining_simulation_time -= time;
      }
      else
      {
	// Randomly choose whether the step should be executed or not
	if (rng->random_double() < remaining_simulation_time / time)
	{
	  all_steps[step_index].execute();
	  // Handle the executed step with the remaining time
	  static_cast<Derived*>(this)->handle_executed_step(all_steps[step_index], remaining_simulation_time, acceptance_probability_parameter);
	}
	else
	{
	  // Handle the rejected step with the remaining time
	  static_cast<Derived*>(this)->handle_rejected_step(all_steps[step_index], remaining_simulation_time, acceptance_probability_parameter);
	}
	remaining_simulation_time = -1.0;
      }
    }
  } // of while (remaining_simulation_time > 0.0)
}

/*! \fn AUTO_TEMPLATE_1
 * \details The configuration space is allocated during the construction of the simulation.
 */
template <class ConfigurationType, class RandomNumberGenerator>
Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator>::Simulation()
  : rng_seed(0), is_terminating(false)
{
  rng = new RandomNumberGenerator();
  rng->set_seed(rng_seed);
  configuration_space = new ConfigurationType();
  register_posix_signal_handler();
}

/*! \fn AUTO_TEMPLATE_1
 * \param new_configuration Pointer to the configuration space onto which the algorithm will work. The configuration space is not copied in the constructor.
 */
template <class ConfigurationType, class RandomNumberGenerator>
Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator>::Simulation(ConfigurationType* new_configuration)
  : rng_seed(0), is_terminating(false)
{
  rng = new RandomNumberGenerator();
  rng->set_seed(rng_seed);
  configuration_space = new_configuration;
  register_posix_signal_handler();
}

template <class ConfigurationType, class RandomNumberGenerator>
Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator>::~Simulation()
{
  delete rng;
}

template <class ConfigurationType, class RandomNumberGenerator>
template <class Algorithm> 
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator>::load_serialize(Algorithm& simulation, std::istream& input_stream)
{
  boost::archive::text_iarchive input_archive(input_stream);
  input_archive >> simulation;
}
template <class ConfigurationType, class RandomNumberGenerator>
template <class Algorithm> 
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator>::load_serialize(Algorithm& simulation, const char* filename)
{
  std::ifstream input_filestream(filename);
  load_serialize(simulation, input_filestream);
  input_filestream.close();
}
template <class ConfigurationType, class RandomNumberGenerator>
template <class Algorithm> 
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator>::save_serialize(const Algorithm& simulation, std::ostream& output_stream)
{
  boost::archive::text_oarchive output_archive(output_stream);
  output_archive << simulation;
}
template <class ConfigurationType, class RandomNumberGenerator>
template <class Algorithm> 
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator>::save_serialize(const Algorithm& simulation, const char* filename)
{
  std::ofstream output_filestream(filename);
  save_serialize(simulation, output_filestream);
  output_filestream.close();
}


#endif
