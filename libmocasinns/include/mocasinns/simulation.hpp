/*!
 * \file simulation.hpp
 * \brief Base class for all Monte-Carlo-Simulations in MoCaSinns
 * 
 * Deals with the general interface (setting configurations and random number generators). Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_SIMULATION_HPP
#define MOCASINNS_SIMULATION_HPP

#include <string>
#include <sstream>
#include <cstdlib>
#include <csignal>
#include <fstream>

// Header for the serialization of the class
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
// Header for signal handling
#include <boost/signals2/signal.hpp>

// Header for the standard random number generator
#include "random/boost_random.hpp"

namespace Mocasinns
{

//! Base class for all Simulations in MoCaSinns
template <class ConfigurationType, class RandomNumberGenerator = Random::Boost_MT19937, bool rejection_free = false>
class Simulation
{
public:
  //! Typedef for the step number
  typedef uint64_t StepNumberType;

  //! Boost signal handler invoked when SIGTERM is caught
  boost::signals2::signal<void (Simulation*)> signal_handler_sigterm;
  //! Boost signal handler invoked when SIGUSR1 is caught
  boost::signals2::signal<void (Simulation*)> signal_handler_sigusr1;
  //! Boost signal handler invoked when SIGUSR1 is caught
  boost::signals2::signal<void (Simulation*)> signal_handler_sigusr2;

  //! Initialise a Simulation with default configuration space and default RandomNumberGenerator
  Simulation();
  //! Initialise a Simulation with given configuration space and default RandomNumberGenerator
  Simulation(ConfigurationType*);
  //! Virtual destructor, deletes the random number generator created in the constructors
  virtual ~Simulation();

  //! Get-Accessor for the pointer to the configuration space
  ConfigurationType* get_config_space() const;
  //! Get-Accessor for the seed of the RandomNumberGenerator
  int get_random_seed() const;
  //! Set-Accessor for the seed of the RandomNumberGenerator
  void set_random_seed(int seed);
  //! Get-Accessor for the path and name of the dumped file
  const std::string& get_dump_filename() const;
  //! Set-Accesspr for the path and name of the dumped file
  void set_dump_filename(const std::string& value);

  //! Load the data of the simulation from a serialization stream
  virtual void load_serialize(std::istream& input_stream);
  //! Load the data of the simulation from a serialization file
  virtual void load_serialize(const char* filename);
  //! Save the data of the simulation to a serialization stream
  virtual void save_serialize(std::ostream& output_stream) const;
  //! Save the data of the simulation to a serialization file
  virtual void save_serialize(const char* filename) const;

protected:
  //! Pointer to the RandomNumberGenerator used in the Simulation
  RandomNumberGenerator* rng;
  //! Pointer to the configuration space used in the Simulation
  ConfigurationType* configuration_space;
  //! Seed that is used in the simulation
  int rng_seed;

  //! Path and filename of the dumps of the simulation
  std::string dump_filename;

  //! Rename the dump file to a temporary file
  void move_dump_file();
  //! Delete renamed dump file
  void remove_temporary_dump_file();
  //! Initialise a random dump file name
  void initialise_dump_file_random();

  //! Variable that notices user and environment signals
  static volatile sig_atomic_t signal_number_caught;
  //! Function for handling signals
  static void handle_posix_signal(int signal_number);
  //! Check whether a POSIX signal was caught and call the corresponding boost signals
  //! \returns True if termination signal was caught, false otherwise
  bool check_for_posix_signal();
  //! Bool that indicates whether the simulation is terminating
  bool is_terminating;

  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method for loading this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    ar & configuration_space;
    ar & rng_seed;
  }

  //! Do a number of steps using an acceptance probability provided by the actual algorithm, decides using the template parameter rejection free whether to do rejectio free or normal steps
  template <class Derived, class StepType, class AcceptanceProbabilityParameterType>
  void do_steps(const StepNumberType& step_number, AcceptanceProbabilityParameterType& acceptance_probability_parameter);

private:
  //! Do a number of generic steps using an acceptance probability provided by the actual algorithm
  template <class Derived, class StepType, class AcceptanceProbabilityParameterType, bool step_has_is_executable, bool step_has_selection_probability_factor>
  void do_generic_steps(const StepNumberType& step_number, AcceptanceProbabilityParameterType& acceptance_probability_parameter);

  //! Do a number of generic rejection-free steps using the acceptance probability provided by the actual algorithm
  template <class Derived, class StepType, class AcceptanceProbabilityParameterType, bool step_has_is_executable, bool step_has_selection_probability_factor>
  void do_generic_steps_rejection_free(const StepNumberType& step_number, AcceptanceProbabilityParameterType acceptance_probability_parameter);

  //! Set the signals for POSIX signals
  void register_posix_signal_handler();
};

// Initialisation of the static member signal_number_caught
  template <class ConfigurationType, class RandomNumberGenerator, bool rejection_free>
  volatile sig_atomic_t Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::signal_number_caught = 0;

} // of namespace Mocasinns

#include "src/simulation.cpp"

#endif
