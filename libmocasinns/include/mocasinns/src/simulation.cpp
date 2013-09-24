/*!
 * \file simulation.cpp
 * \brief Implementation of the base class for all Monte-Carlo-Simulations in MoCaSinns
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_SIMULATION_HPP

#include "../details/optional_concept_checks/step_type_has_is_executable.hpp"
#include "../details/optional_concept_checks/step_type_has_selection_probability_factor.hpp"

namespace Mocasinns
{

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::register_posix_signal_handler()
{
  // Set the signal handle
  signal(SIGTERM, handle_posix_signal);
  signal(SIGUSR1, handle_posix_signal);
  signal(SIGUSR2, handle_posix_signal);
}

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::move_dump_file()
{
  std::stringstream command_builder;
  command_builder << "mv ";
  command_builder << dump_filename << " ";
  command_builder << dump_filename << ".tmp";
  system(command_builder.str().c_str());
}
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::remove_temporary_dump_file()
{
  std::stringstream command_builder;
  command_builder << "rm ";
  command_builder << dump_filename << ".tmp";
  system(command_builder.str().c_str());
}
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::initialise_dump_file_random()
{
  // Try as long as the dump file name does not exist
  bool file_exists = true;
  while (file_exists)
  {
    char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    rng->set_int_max(61);
    std::stringstream ss;
    for (int i = 0; i < 16; i++)
      ss << letters[rng->random_uint32()];

    ss << ".dat";
    dump_filename = ss.str();

    // Determine whether the file exists
    std::ifstream test_file(dump_filename.c_str());
    if (!test_file) file_exists = false;
  }
}

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::handle_posix_signal(int signal_number)
{
  if (signal_number == SIGTERM) signal_number_caught = 1;
  if (signal_number == SIGUSR1) signal_number_caught = 2;
  if (signal_number == SIGUSR2) signal_number_caught = 3;
}

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
bool Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::check_for_posix_signal()
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

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
template <class Derived, class StepType, class AcceptanceProbabilityParameterType>
void Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::do_steps(const StepNumberType& step_number, AcceptanceProbabilityParameterType acceptance_probability_parameter)
{
  // Call the generic step function depending on the rejection free template parameter
  if (rejection_free)
  {
    do_generic_steps_rejection_free<Derived, StepType, AcceptanceProbabilityParameterType, 
				    Details::OptionalConceptChecks::StepTypeHasIsExecutable<StepType>::value, 
				    Details::OptionalConceptChecks::StepTypeHasSelectionProbabilityFactor<StepType>::value>
      (step_number, acceptance_probability_parameter);
  }
  else
  {
    do_generic_steps<Derived, StepType, AcceptanceProbabilityParameterType, 
		     Details::OptionalConceptChecks::StepTypeHasIsExecutable<StepType>::value, 
		     Details::OptionalConceptChecks::StepTypeHasSelectionProbabilityFactor<StepType>::value>
      (step_number, acceptance_probability_parameter);
  }
}

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
template <class Derived, class StepType, class AcceptanceProbabilityParameterType, bool STEP_HAS_IS_EXECUTABLE, bool STEP_HAS_SELECTION_PROBABILITY_FACTOR>
void Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::do_generic_steps(const StepNumberType& step_number, AcceptanceProbabilityParameterType acceptance_probability_parameter)
{
  for (StepNumberType i = 0; i < step_number; ++i)
  {
    // Propose a new step
    StepType next_step = this->configuration_space->propose_step(this->rng);
    
    // If the next step is executable, calculate the acceptance probability
    if (STEP_HAS_IS_EXECUTABLE && next_step.is_executable())
    {
      // Calculate selection probability factor and acceptance probability
      double selection_probability_factor = 1.0;
      if (STEP_HAS_SELECTION_PROBABILITY_FACTOR) selection_probability_factor = next_step.selection_probability_factor();
      double step_probability(static_cast<Derived*>(this)->acceptance_probability(next_step, acceptance_probability_parameter));
      step_probability /= selection_probability_factor;

      // Do the step with the correct probability and call the handlers
      if (step_probability > 0.0 && (step_probability >= 1.0 || this->rng->random_double() < step_probability))
      {
	next_step.execute();
	static_cast<Derived*>(this)->handle_executed_step(next_step, 1.0, acceptance_probability_parameter);
      }
      else
	static_cast<Derived*>(this)->handle_rejected_step(next_step, acceptance_probability_parameter);
    } // of if (next_step.is_executable())
    else
      static_cast<Derived*>(this)->handle_rejected_step(next_step, acceptance_probability_parameter);
  }
}

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
template <class Derived, class StepType, class AcceptanceProbabilityParameterType, bool STEP_HAS_IS_EXECUTABLE, bool STEP_HAS_SELECTION_PROBABILITY_FACTOR>
void Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::do_generic_steps_rejection_free(const StepNumberType& step_number, AcceptanceProbabilityParameterType acceptance_probability_parameter)
{
  for (StepNumberType i = 0; i < step_number; ++i)
  {
    // Propose all possible steps
    std::vector<StepType> all_steps = this->configuration_space->all_steps();
    std::vector<double> acceptance_probabilities(all_steps.size(), 0.0);

    // Iterate all steps and calculate the acceptance probabilities
    for (unsigned int s = 0; s < all_steps.size(); ++s)
    {
      // If the step is executable, calculate the acceptance probability
      // If the step is not executable, the acceptance probability remains 0
      if (STEP_HAS_IS_EXECUTABLE && all_steps[s].is_executable())
      {
	// Calculate selection probability factor and acceptance probability
	double selection_probability_factor = 1.0;
	if (STEP_HAS_SELECTION_PROBABILITY_FACTOR) selection_probability_factor = all_steps[s].selection_probability_factor();
	double step_probability(static_cast<Derived*>(this)->acceptance_probability(all_steps[s], acceptance_probability_parameter));
	step_probability /= selection_probability_factor;

	// Store the acceptance probability
	acceptance_probabilities[s] = step_probability;
      }
    } 

    // Calculate the cumulated acceptance probabilities
    // If a step at position r is not executable, cumulative_acceptance_probabilities[r + 1] = cumulative_acceptance_probabilities[r]
    std::vector<double> cumulative_acceptance_probabilities(all_steps.size() + 1);
    cumulative_acceptance_probabilities[0] = 0.0;
    for (unsigned int s = 0; s < all_steps.size() + 1; ++s)
    {
      cumulative_acceptance_probabilities[s] = cumulative_acceptance_probabilities[s - 1] + acceptance_probabilities[s - 1];
    }

    // Create a random number and determine which step to execute
    double rnd = rng->random_double()*cumulative_acceptance_probabilities[all_steps.size()];
    unsigned int step_index;
    for (unsigned int s = 0; s < all_steps.size(); ++s)
    {
      if (cumulative_acceptance_probabilities[s] <= rnd && cumulative_acceptance_probabilities[s+1] > rnd)
	step_index = s;
    }
    all_steps[step_index].execute();

    // Handle the executed step with 
    static_cast<Derived*>(this)->handle_executed_step(all_steps[step_index], -log(rng->random_double()) / cumulative_acceptance_probabilities[all_steps.size()], acceptance_probability_parameter);
  }
}
  

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::Simulation()
  : rng_seed(0), is_terminating(false)
{
  rng = new RandomNumberGenerator();
  rng->set_seed(rng_seed);
  configuration_space = new ConfigurationType();
  initialise_dump_file_random();
  register_posix_signal_handler();
}

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::Simulation(ConfigurationType* new_configuration)
  : rng_seed(0), is_terminating(false)
{
  rng = new RandomNumberGenerator();
  rng->set_seed(rng_seed);
  configuration_space = new_configuration;
  initialise_dump_file_random();
  register_posix_signal_handler();
}

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::~Simulation()
{
  delete rng;
}

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
ConfigurationType* Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::get_config_space() const
{
  return configuration_space;
}
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
int Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::get_random_seed() const
{
  return rng_seed;
}
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::set_random_seed(int seed) 
{
  rng_seed = seed;
  rng->set_seed(seed);
}
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
const std::string& Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::get_dump_filename() const
{
  return dump_filename;
}
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::set_dump_filename(const std::string& value) 
{
  dump_filename = value;
}

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::load_serialize(std::istream& input_stream)
{
  boost::archive::text_iarchive input_archive(input_stream);
  input_archive >> (*this);
}
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::load_serialize(const char* filename)
{
  std::ifstream input_filestream(filename);
  load_serialize(input_filestream);
  input_filestream.close();
}
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::save_serialize(std::ostream& output_stream) const
{
  boost::archive::text_oarchive output_archive(output_stream);
  output_archive << (*this);
}
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::save_serialize(const char* filename) const
{
  std::ofstream output_filestream(filename);
  save_serialize(output_filestream);
  output_filestream.close();
}

} // of namespace Mocasinns

#endif
