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
// Header for checking whether the step type exposes certain functions
#include "details/optional_member_functions.hpp"

namespace Mocasinns
{

//! Base class for all Simulations in MoCaSinns
template <class ConfigurationType, class RandomNumberGenerator = Random::Boost_MT19937>
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
  ConfigurationType* get_config_space() const { return configuration_space; }
  //! Set-Accessor for the pointer to the configuration space
  void set_config_space(ConfigurationType* value) { configuration_space = value;}
  //! Get-Accessor for the seed of the RandomNumberGenerator
  int get_random_seed() const { return rng_seed; }
  //! Set-Accessor for the seed of the RandomNumberGenerator
  void set_random_seed(int seed) { rng_seed = seed; rng->set_seed(seed); }
  //! Get-Accessor for the path and name of the dumped file
  const std::string& get_dump_filename() const { return dump_filename; }
  //! Set-Accesspr for the path and name of the dumped file
  void set_dump_filename(const std::string& value) { dump_filename = value; }

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
  //! Method for serializing the class
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    serialize_generic<ConfigurationType, Archive>(ar, version);
  }

  template<class ConfigurationTypeFunction, class Archive, typename boost::enable_if_c<Details::has_function_is_serializable<ConfigurationTypeFunction, bool>::value, bool>::type = false>
  void serialize_generic(Archive & ar, const unsigned int)
  {
    ar & configuration_space;
    ar & rng_seed;
  }
  template<class ConfigurationTypeFunction, class Archive, typename boost::enable_if_c<!Details::has_function_is_serializable<ConfigurationTypeFunction, bool>::value, bool>::type = false>
  void serialize_generic(Archive & ar, const unsigned int)
  {
    ar & rng_seed;
  }

  template <class Derived, class StepType, bool rejection_free, class AcceptanceProbabilityParameterType>
  typename boost::enable_if_c<!rejection_free, void>::type // Standard version of the algorithm
  do_steps(const StepNumberType& step_number, AcceptanceProbabilityParameterType& acceptance_probability_parameter);

  template <class Derived, class StepType, bool rejection_free, class AcceptanceProbabilityParameterType>
  typename boost::enable_if_c<rejection_free, void>::type // Rejection-free version of the algorithm
  do_steps(const StepNumberType& step_number, AcceptanceProbabilityParameterType& acceptance_probability_parameter);  

private:
  //! Set the signals for POSIX signals
  void register_posix_signal_handler();
};

// Initialisation of the static member signal_number_caught
  template <class ConfigurationType, class RandomNumberGenerator>
  volatile sig_atomic_t Simulation<ConfigurationType, RandomNumberGenerator>::signal_number_caught = 0;

} // of namespace Mocasinns

#include "src/simulation.cpp"

#endif
