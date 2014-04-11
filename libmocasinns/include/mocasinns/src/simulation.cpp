/*! \file simulation.cpp
 * \brief Implementation of the base class for all Monte-Carlo-Simulations in MoCaSinns
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_SIMULATION_HPP

#include <limits>

#include "../details/optional_concept_checks/step_type_has_is_executable.hpp"
#include "../details/optional_concept_checks/step_type_has_selection_probability_factor.hpp"

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::register_posix_signal_handler()
{
  // Set the signal handle
  signal(SIGTERM, handle_posix_signal);
  signal(SIGUSR1, handle_posix_signal);
  signal(SIGUSR2, handle_posix_signal);
}

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::move_dump_file()
{
  std::stringstream command_builder;
  command_builder << "mv ";
  command_builder << dump_filename << " ";
  command_builder << dump_filename << ".tmp";
  system(command_builder.str().c_str());
}
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::remove_temporary_dump_file()
{
  std::stringstream command_builder;
  command_builder << "rm ";
  command_builder << dump_filename << ".tmp";
  system(command_builder.str().c_str());
}
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::initialise_dump_file_random()
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

  /*! \fn 
   * \param signal_number Enum for the type of the signals
   */
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::handle_posix_signal(int signal_number)
{
  if (signal_number == SIGTERM) signal_number_caught = 1;
  if (signal_number == SIGUSR1) signal_number_caught = 2;
  if (signal_number == SIGUSR2) signal_number_caught = 3;
}

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
bool Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::check_for_posix_signal()
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

/*! \fn AUTO_TEMPLATE_2
 * \details Executes a number of steps using the acceptance_probability, the handle_executed_step and the handle_rejected_step functions of the derived algorithm. It can be specified whether the used step type provides the member function is_executable (otherwise the algorithm does not test the executability and executes every step) and selection_probaility_factor (otherwise the algorithm assumes that the ratio of selection probabilities is allways 1.0).
 * 
 * If the simulation is not rejection-free (specified by the template parameter of the class), one step is executed in the following way (by calling the function do_steps_generic):
 * -# The step will be proposed using the random number generator of the simulation
 * -# If the StepType provides the function is_executable and the current step is not executable, the handle_rejected_step function of the derived algorithm is executed and the next step will be proposed. Otherwise the current step will be examined further as following:
 * -# If the StepType provides the function selection_probability_factor, it is calculated for the current step.
 * -# The acceptance probability of the step is calculated using the acceptance_probability function of the Dervied algorithm. Afterwards it is devided by the selection probability ratio if applicable.
 * -# A random number is generated that deterimines whether the step will be executed. According to the outcome of the random number the step is executed and the handle_executed_step function of the Derived algorithm is called, otherwise the handle_rejected_step function of the Derived algorithm is called.
 *
 * If the simulation is rejection-free (specified by the template parameter of the class), one step is executed in the following way (by calling the function do_steps_generic_rejection_free)
 * -# All possible steps of the current configurations are proposed using the ConfigurationType::all_steps() function.
 * -# For each step it is determined whether it is executable (if the StepType does not provide an is_executable function, this is automatically assumed). If this is not the case, the acceptance probability of the step is set to 0.0
 * -# If the StepType provides the function selection_probability_factor, it is calculated for the current step.
 * -# The acceptance probability of the step is calculated using the acceptance_probability function of the Dervied algorithm. Afterwards it is devided by the selection probability ratio if applicable.
 * -# After the exception probabilities of all steps were calculated, the step to execute is determined by randomly choosing an step weighted with their acceptance probability, and the time that a non-rejection free algorithm would spent in the actual state is calculated by inverting the sum of all acceptance probabilities.
 * -# The acceptance probability of the step that will be executed is calculated again to get the acceptance_probability_parameter right
 * -# If the calculated time is infinite (this is the case if all acceptance probabilities are 0), the step is rejected and the handle_rejected_step of the Derived algorithm is called
 * -# If there is a finite time the step is executed and the handle_executed_step function of the Derived algorithm is called with the time as parameter.
 *
 * \tparam Derived Type of the derived algorithm
 * \tparam StepType Type of the steps proposed by the ConfigurationType
 * \tparam AcceptanceProbabilityParameterType Type of the data that is piped to the acceptance probability calculation of the derived algorithm
 * \param step_number Number of steps to propose (= number of executed steps + number of rejected steps)
 * \param acceptance_probability_parameter Some object or quantity that piped to the acceptance probability calculation of the derived algorithm.
 */
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
template <class Derived, class StepType, class AcceptanceProbabilityParameterType>
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::do_steps(const StepNumberType& step_number, AcceptanceProbabilityParameterType& acceptance_probability_parameter)
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

/*! \fn AUTO_TEMPLATE_2
 * \details Private function. See do_steps(step_number, acceptance_probability_parameters) for documentation of implementation details and (template) parameters.
 * 
 * \tparam STEP_HAS_IS_EXECUTABLE Boolean stating whether the StepType provides the function is_executable
 * \tparam STEP_HAS_SELECTION_PROBABILITY_FACTOR Boolean stating whether the StepType associated provides the function selection_probability_factor
 */
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
template <class Derived, class StepType, class AcceptanceProbabilityParameterType, bool STEP_HAS_IS_EXECUTABLE, bool STEP_HAS_SELECTION_PROBABILITY_FACTOR>
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::do_generic_steps(const StepNumberType& step_number, AcceptanceProbabilityParameterType& acceptance_probability_parameter)
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
	static_cast<Derived*>(this)->handle_rejected_step(next_step, 1.0, acceptance_probability_parameter);
    } // of if (next_step.is_executable())
    else
      static_cast<Derived*>(this)->handle_rejected_step(next_step, 1.0, acceptance_probability_parameter);
  }
}

/*! \fn AUTO_TEMPLATE_2
 * \details Private function. See do_steps(step_number, acceptance_probability_parameters) for documentation of implementation details and (template) parameters.
 * 
 * \tparam STEP_HAS_IS_EXECUTABLE Boolean stating whether the StepType provides the function is_executable
 * \tparam STEP_HAS_SELECTION_PROBABILITY_FACTOR Boolean stating whether the StepType associated provides the function selection_probability_factor
 */
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
template <class Derived, class StepType, class AcceptanceProbabilityParameterType, bool STEP_HAS_IS_EXECUTABLE, bool STEP_HAS_SELECTION_PROBABILITY_FACTOR>
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::do_generic_steps_rejection_free(const StepNumberType& step_number, AcceptanceProbabilityParameterType acceptance_probability_parameter)
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
      all_steps[step_index].execute();
      // Handle the executed step with 
      static_cast<Derived*>(this)->handle_executed_step(all_steps[step_index], 1.0 / cumulative_acceptance_probabilities[all_steps.size()], acceptance_probability_parameter);
    }

    //    std::cout << static_cast<Derived*>(this)->get_log_density_of_states() << std::endl;
  }
}
  
/*! \fn AUTO_TEMPLATE_1
 * \details The configuration space is allocated during the construction of the simulation.
 */
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::Simulation()
  : rng_seed(0), is_terminating(false)
{
  rng = new RandomNumberGenerator();
  rng->set_seed(rng_seed);
  configuration_space = new ConfigurationType();
  initialise_dump_file_random();
  register_posix_signal_handler();
}

/*! \fn AUTO_TEMPLATE_1
 * \param new_configuration Pointer to the configuration space onto which the algorithm will work. The configuration space is not copied in the constructor.
 */
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::Simulation(ConfigurationType* new_configuration)
  : rng_seed(0), is_terminating(false)
{
  rng = new RandomNumberGenerator();
  rng->set_seed(rng_seed);
  configuration_space = new_configuration;
  initialise_dump_file_random();
  register_posix_signal_handler();
}

template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::~Simulation()
{
  delete rng;
}

/*! \fn AUTO_TEMPLATE_1
 * \details Uses the functionality of boost::serialization.
 * \param input_stream Stream from which the simulation will be loaded.
 */
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::load_serialize(std::istream& input_stream)
{
  boost::archive::text_iarchive input_archive(input_stream);
  input_archive >> (*this);
}
/*! \fn AUTO_TEMPLATE_1
 * \details Uses the functionality of boost::serialization.
 * \param filename Filename of the stored simulation
 */
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::load_serialize(const char* filename)
{
  std::ifstream input_filestream(filename);
  load_serialize(input_filestream);
  input_filestream.close();
}
/*! \fn AUTO_TEMPLATE_1
 * \details Uses the functionality of boost::serialization.
 * \param output_stream Stream to which the simulation will be written
 */
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::save_serialize(std::ostream& output_stream) const
{
  boost::archive::text_oarchive output_archive(output_stream);
  output_archive << (*this);
}
/*! \fn AUTO_TEMPLATE_1
 * \details Uses the functionality of boost::serialization.
 * \param filename Filename of the stored simulation
 */
template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
void Mocasinns::Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::save_serialize(const char* filename) const
{
  std::ofstream output_filestream(filename);
  save_serialize(output_filestream);
  output_filestream.close();
}

#endif
