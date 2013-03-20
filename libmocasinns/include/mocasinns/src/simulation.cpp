/*!
 * \file simulation.cpp
 * \brief Implementation of the base class for all Monte-Carlo-Simulations in MoCaSinns
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifdef MOCASINNS_SIMULATION_HPP

namespace Mocasinns
{

template <class ConfigurationType, class RandomNumberGenerator>
void Simulation<ConfigurationType, RandomNumberGenerator>::register_posix_signal_handler()
{
  // Set the signal handle
  signal(SIGTERM, handle_posix_signal);
  signal(SIGUSR1, handle_posix_signal);
  signal(SIGUSR2, handle_posix_signal);
}

template <class ConfigurationType, class RandomNumberGenerator>
void Simulation<ConfigurationType, RandomNumberGenerator>::move_dump_file()
{
  std::stringstream command_builder;
  command_builder << "mv ";
  command_builder << dump_filename << " ";
  command_builder << dump_filename << ".tmp";
  system(command_builder.str().c_str());
}
template <class ConfigurationType, class RandomNumberGenerator>
void Simulation<ConfigurationType, RandomNumberGenerator>::remove_temporary_dump_file()
{
  std::stringstream command_builder;
  command_builder << "rm ";
  command_builder << dump_filename << ".tmp";
  system(command_builder.str().c_str());
}
template <class ConfigurationType, class RandomNumberGenerator>
void Simulation<ConfigurationType, RandomNumberGenerator>::initialise_dump_file_random()
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

template <class ConfigurationType, class RandomNumberGenerator>
void Simulation<ConfigurationType, RandomNumberGenerator>::handle_posix_signal(int signal_number)
{
  if (signal_number == SIGTERM) signal_number_caught = 1;
  if (signal_number == SIGUSR1) signal_number_caught = 2;
  if (signal_number == SIGUSR2) signal_number_caught = 3;
}

template <class ConfigurationType, class RandomNumberGenerator>
bool Simulation<ConfigurationType, RandomNumberGenerator>::check_for_posix_signal()
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
template <class Derived, class StepType, class AcceptanceProbabilityParameterType>
void Simulation<ConfigurationType, RandomNumberGenerator>::do_steps(const StepNumberType& step_number, AcceptanceProbabilityParameterType acceptance_probability_parameter)
{
  for (StepNumberType i = 0; i < step_number; ++i)
  {
    // Propose a new step
    StepType next_step = this->configuration_space->propose_step(this->rng);
    
    // If the next step is executable, calculate the acceptance probability
    if (next_step.is_executable())
    {
      // Calculate selection probability factor and acceptance probability
      double selection_probability_factor = next_step.selection_probability_factor();
      double acceptance_probability = static_cast<Derived*>(this)->acceptance_probability(next_step, acceptance_probability_parameter);
      double step_probability = acceptance_probability / selection_probability_factor;

      // Do the step with the correct probability and call the handlers
      if (step_probability > 0.0 && (step_probability >= 1.0 || this->rng->random_double() < step_probability))
      {
	next_step.execute();
	static_cast<Derived*>(this)->handle_executed_step(next_step, acceptance_probability_parameter);
      }
      else
	static_cast<Derived*>(this)->handle_rejected_step(next_step, acceptance_probability_parameter);
    } // of if (next_step.is_executable())
    else
      static_cast<Derived*>(this)->handle_rejected_step(next_step, acceptance_probability_parameter);
  }
}
  

template <class ConfigurationType, class RandomNumberGenerator>
Simulation<ConfigurationType, RandomNumberGenerator>::Simulation()
  : rng_seed(0), is_terminating(false)
{
  rng = new RandomNumberGenerator();
  rng->set_seed(rng_seed);
  configuration_space = new ConfigurationType();
  initialise_dump_file_random();
  register_posix_signal_handler();
}

template <class ConfigurationType, class RandomNumberGenerator>
Simulation<ConfigurationType, RandomNumberGenerator>::Simulation(ConfigurationType* new_configuration)
  : rng_seed(0), is_terminating(false)
{
  rng = new RandomNumberGenerator();
  rng->set_seed(rng_seed);
  configuration_space = new_configuration;
  initialise_dump_file_random();
  register_posix_signal_handler();
}

template <class ConfigurationType, class RandomNumberGenerator>
Simulation<ConfigurationType, RandomNumberGenerator>::~Simulation()
{
  delete rng;
}

template <class ConfigurationType, class RandomNumberGenerator>
ConfigurationType* Simulation<ConfigurationType, RandomNumberGenerator>::get_config_space() const
{
  return configuration_space;
}
template <class ConfigurationType, class RandomNumberGenerator>
int Simulation<ConfigurationType, RandomNumberGenerator>::get_random_seed() const
{
  return rng_seed;
}
template <class ConfigurationType, class RandomNumberGenerator>
void Simulation<ConfigurationType, RandomNumberGenerator>::set_random_seed(int seed) 
{
  rng_seed = seed;
  rng->set_seed(seed);
}
template <class ConfigurationType, class RandomNumberGenerator>
const std::string& Simulation<ConfigurationType, RandomNumberGenerator>::get_dump_filename() const
{
  return dump_filename;
}
template <class ConfigurationType, class RandomNumberGenerator>
void Simulation<ConfigurationType, RandomNumberGenerator>::set_dump_filename(const std::string& value) 
{
  dump_filename = value;
}

template <class ConfigurationType, class RandomNumberGenerator>
void Simulation<ConfigurationType, RandomNumberGenerator>::load_serialize(std::istream& input_stream)
{
  boost::archive::text_iarchive input_archive(input_stream);
  input_archive >> (*this);
}
template <class ConfigurationType, class RandomNumberGenerator>
void Simulation<ConfigurationType, RandomNumberGenerator>::load_serialize(const char* filename)
{
  std::ifstream input_filestream(filename);
  load_serialize(input_filestream);
  input_filestream.close();
}
template <class ConfigurationType, class RandomNumberGenerator>
void Simulation<ConfigurationType, RandomNumberGenerator>::save_serialize(std::ostream& output_stream) const
{
  boost::archive::text_oarchive output_archive(output_stream);
  output_archive << (*this);
}
template <class ConfigurationType, class RandomNumberGenerator>
void Simulation<ConfigurationType, RandomNumberGenerator>::save_serialize(const char* filename) const
{
  std::ofstream output_filestream(filename);
  save_serialize(output_filestream);
  output_filestream.close();
}

} // of namespace Mocasinns

#endif
