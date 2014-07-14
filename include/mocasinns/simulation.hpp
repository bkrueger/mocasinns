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
#include <fstream>
#include <cstdlib>
#include <csignal>
#include <ctime>

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
  typedef uint64_t step_number_t;
  //! Typedef for incidence counter y values (for multicanonical algorithms)
  typedef double incidence_counter_y_value_t;

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

  //! Calculate the real time (in seconds) that passed since the start of the simulation
  int simulation_time_real() const { return time(NULL) - simulation_start; }

  //! Load the data of the simulation from a serialization stream
  virtual void load_serialize(std::istream& input_stream) { load_serialize(*this, input_stream); }
  //! Load the data of the simulation from a serialization file
  virtual void load_serialize(const char* filename) { load_serialize(*this, filename); }
  //! Save the data of the simulation to a serialization stream
  virtual void save_serialize(std::ostream& output_stream) const { save_serialize(*this, output_stream); }
  //! Save the data of the simulation to a serialization file
  virtual void save_serialize(const char* filename) const { save_serialize(*this, filename); }

protected:
  //! Pointer to the RandomNumberGenerator used in the Simulation
  RandomNumberGenerator* rng;
  //! Pointer to the configuration space used in the Simulation
  ConfigurationType* configuration_space;
  //! Seed that is used in the simulation
  int rng_seed;

  //! Time of the simulation start
  time_t simulation_start;

  //! Path and filename of the dumps of the simulation
  std::string dump_filename;

  //! Variable that notices user and environment signals
  static volatile sig_atomic_t signal_number_caught;
  //! Function for handling signals
  static void handle_posix_signal(int signal_number);
  //! Check whether a POSIX signal was caught and call the corresponding boost signals
  //! \returns True if termination signal was caught, false otherwise
  bool check_for_posix_signal();
  //! Bool that indicates whether the simulation is terminating
  bool is_terminating;

  //! \cond
  template <class Derived, class StepType, bool rejection_free, class AcceptanceProbabilityParameterType>
  typename boost::enable_if_c<!rejection_free, void>::type // Standard version of the algorithm
  do_steps(const step_number_t& step_number, AcceptanceProbabilityParameterType& acceptance_probability_parameter);

  template <class Derived, class StepType, bool rejection_free, class AcceptanceProbabilityParameterType>
  typename boost::enable_if_c<rejection_free, void>::type // Rejection-free version of the algorithm
  do_steps(const step_number_t& step_number, AcceptanceProbabilityParameterType& acceptance_probability_parameter);  
  //! \endcond

  // Doxygen-Documentation for the do-steps routine
  /*!
   * \details Executes a number of steps using the acceptance_probability, the handle_executed_step and the handle_rejected_step functions of the derived algorithm. It can be specified whether the used step type provides the member function is_executable (otherwise the algorithm does not test the executability and executes every step) and selection_probaility_factor (otherwise the algorithm assumes that the ratio of selection probabilities is allways 1.0).
   * 
   * If the simulation is not rejection-free (specified by the template parameter of the function), one step is executed in the following way (by calling the function do_steps_generic):
   * -# The step will be proposed using the random number generator of the simulation
   * -# If the StepType provides the function is_executable and the current step is not executable, the handle_rejected_step function of the derived algorithm is executed and the next step will be proposed. Otherwise the current step will be examined further as following:
   * -# If the StepType provides the function selection_probability_factor, it is calculated for the current step.
   * -# The acceptance probability of the step is calculated using the acceptance_probability function of the Dervied algorithm. Afterwards it is devided by the selection probability ratio if applicable.
   * -# A random number is generated that deterimines whether the step will be executed. According to the outcome of the random number the step is executed and the handle_executed_step function of the Derived algorithm is called, otherwise the handle_rejected_step function of the Derived algorithm is called.
   *
   * If the simulation is rejection-free (specified by the template parameter of the function), one step is executed in the following way (by calling the function do_steps_generic_rejection_free)
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
   * \tparam rejection_free Boolean template parameter specifying whether to execute rejection free steps.
   * \tparam AcceptanceProbabilityParameterType Type of the data that is piped to the acceptance probability calculation of the derived algorithm
   * \param step_number Number of steps to propose (= number of executed steps + number of rejected steps)
   * \param acceptance_probability_parameter Some object or quantity that piped to the acceptance probability calculation of the derived algorithm.
   */
#ifdef MOCASINNS_DOXYGEN_DOCUMENTATION
  template <class Derived, class StepType, bool rejection_free, class AcceptanceProbabilityParameterType>
  void do_steps(const step_number_t& step_number, AcceptanceProbabilityParameterType& acceptance_probability_parameter);
#endif

  //! Function to log the simulation start, stores the time of start of the simulation
  void simulation_start_log() { simulation_start = time(NULL); }

  //! Load a serialized simulation from a stream
  template <class Algorithm> static void load_serialize(Algorithm& simulation, std::istream& input_stream);
  //! Load a serialized simulation from a file
  template <class Algorithm> static void load_serialize(Algorithm& simulation, const char* filename);
  //! Save a simulation to a stream using serialization
  template <class Algorithm> static void save_serialize(const Algorithm& simulation, std::ostream& output_stream);
  //! Save a simulation to a file using serialization
  template <class Algorithm> static void save_serialize(const Algorithm& simulation, const char* filename);

private:
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
    ar & simulation_start;
  }
  template<class ConfigurationTypeFunction, class Archive, typename boost::enable_if_c<!Details::has_function_is_serializable<ConfigurationTypeFunction, bool>::value, bool>::type = false>
  void serialize_generic(Archive & ar, const unsigned int)
  {
    ar & rng_seed;
    ar & simulation_start;
  }

  //! Set the signals for POSIX signals
  void register_posix_signal_handler();
};

// Initialisation of the static member signal_number_caught
  template <class ConfigurationType, class RandomNumberGenerator>
  volatile sig_atomic_t Simulation<ConfigurationType, RandomNumberGenerator>::signal_number_caught = 0;


} // of namespace Mocasinns

#include "src/simulation.cpp"

#endif
