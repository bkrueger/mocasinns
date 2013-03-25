/**
 * \file wang_landau.hpp
 * \brief Class for Wang-Landau-Monte-Carlo simulations with MoCaSinns
 * 
 * Does a Wang-Landau-Simulation and determines the density of states with respect to the energy functional. Usage examples are found in the test cases. 
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_WANG_LANDAU_HPP
#define MOCASINNS_WANG_LANDAU_HPP

#include "simulation.hpp"
#include "concepts/concepts.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

namespace Mocasinns
{
//! Class for Metropolis-Monte-Carlo simulations
template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
class WangLandau : public Simulation<ConfigurationType, RandomNumberGenerator>
{
  // Check the configuration concept
  BOOST_CONCEPT_ASSERT((Concepts::ConfigurationConcept<ConfigurationType, StepType>));
  // Check the step concept
  BOOST_CONCEPT_ASSERT((Concepts::StepConcept<StepType>));
  // Check the energy concept
  BOOST_CONCEPT_ASSERT((Concepts::EnergyConcept<EnergyType>));
  // Check the histo concept
  BOOST_CONCEPT_ASSERT((Concepts::HistoConcept<HistoType>));
  // Check the random number generator concept
  BOOST_CONCEPT_ASSERT((Concepts::RandomNumberGeneratorConcept<RandomNumberGenerator>));
  
public:
  // Typedefs for integers
  typedef typename Simulation<ConfigurationType, RandomNumberGenerator>::StepNumberType StepNumberType;
  typedef uint64_t IncidenceCounterYValueType;

  // Forward declaration of the parameters for the WangLandau-Simulation
  template <class ParameterEnergyType = EnergyType> struct Parameters;
  // Forward declaration of struct used in signals to write the simulation status into std::cout
  struct SimulationStatus;
  // Forward declaration of struct used in signals to dump the whole simulation into the specified file
  struct SimulationDump;

  //! Boost signal handler invoked after every sweep
  boost::signals2::signal<void (Simulation<ConfigurationType,RandomNumberGenerator>*)> signal_handler_sweep;
  //! Boost signal handler invoked if the current modification factor is altered by the simulation
  boost::signals2::signal<void (Simulation<ConfigurationType,RandomNumberGenerator>*)> signal_handler_modfac_change;

  //! Initialise a WangLanday-MC simulation with default parameters, default configuration space and default RandomNumberGenerator
  WangLandau();
  //! Initialise a WangLandau-MC simulation with default configuration space and default RandomNumberGenerator
  WangLandau(const Parameters<EnergyType>& params);
  //! Initialise a WangLandau-MC simulation with given configuration space and default RandomNumberGenerator
  WangLandau(const Parameters<EnergyType>& params, ConfigurationType* initial_configuration);

  //! Get-Accessor for the parameters of the simulation
  const Parameters<EnergyType>& get_simulation_parameters() const { return simulation_parameters; }
  //! Set-Accessor for the parameters of the simulation
  void set_simulation_parameters(const Parameters<EnergyType>& value) { simulation_parameters = value; initialise_with_parameters(); }
  //! Get-Accessor for the current modification factor
  double get_modification_factor_current() const { return modification_factor_current; }
  //! Set-Accessor for the current modification factor
  void set_modification_factor_current(double value) { modification_factor_current = value; }
  //! Get-Accessor for the estimation of the density of states
  const HistoType<EnergyType, double>& get_density_of_states() const { return density_of_states; }
  //! Set-Accessor for the estimation of the density of states
  void set_density_of_states(const HistoType<EnergyType, double>& value) { density_of_states = value; }
  //! Get-Accessor for the incidence counter
  const HistoType<EnergyType, IncidenceCounterYValueType>& get_incidence_counter() const { return incidence_counter; }
  //! Set-Accessor for the incidence counter
  void set_incidence_counter(const HistoType<EnergyType, IncidenceCounterYValueType>& value) { incidence_counter = value; }
  //! Get-Accessor for the sweep counter
  StepNumberType get_sweep_counter() const { return sweep_counter; }

  //! Calculate the acceptance probability of a step
  double acceptance_probability(StepType& step_to_execute, EnergyType& total_energy);
  //! Handle an accepted step
  void handle_executed_step(StepType& executed_step, EnergyType& total_energy);
  //! Handle a rejected step
  void handle_rejected_step(StepType& rejected_step, EnergyType& total_energy);

  //! Do a certain number of wang-landau steps updating the density_of_states and the incidence_counter at the current modification factor
  void do_wang_landau_steps(const uint32_t& number);
  //! Do wang-landau steps until the incidence counter is flat
  void do_wang_landau_steps();
  //! Do a complete wang-landau simulation until the final modification factor is reached
  void do_wang_landau_simulation();

  //! Load the data of the Wang-Landau simulation from a serialization stream
  virtual void load_serialize(std::istream& input_stream);
  //! Load the data of the Wang-Landau simulation from a serialization file
  virtual void load_serialize(const char* filename);
  //! Save the data of the Wang-Landau simulation to a serialization stream
  virtual void save_serialize(std::ostream& output_stream) const;
  //! Save the data of the Wang-Landau simulation to a serialization file
  virtual void save_serialize(const char* filename) const;

private:
  //! Parameters of the simulation
  Parameters<EnergyType> simulation_parameters;

  //! Current value of the modification factor
  double modification_factor_current;

  //! Histogram for the estimation of the density of states
  HistoType<EnergyType, double> density_of_states;
  //! Histogram for the incidence counter
  HistoType<EnergyType, IncidenceCounterYValueType> incidence_counter;

  //! Counter for the number of sweeps
  StepNumberType sweep_counter;

  //! Set the class properties that depend on the parameters, this function can be called each time the parameters will be updated
  void initialise_with_parameters();

  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    // serialize base class information
    ar & boost::serialization::base_object<Simulation<ConfigurationType, RandomNumberGenerator> >(*this);
    ar & simulation_parameters;
    ar & modification_factor_current;
    ar & density_of_states;
    ar & incidence_counter;
  }
};

//! Struct for dealing with the parameters of a Wang-Landau-simulation
template<class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
template<class ParameterEnergyType>
struct WangLandau<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::Parameters
{
public:
  //! Energy value that is used as a reference point for the binning
  ParameterEnergyType binning_reference;
  //! Energy value range that is comprehended in one bin
  ParameterEnergyType binning_width;
  
  //! Flag whether to use a maximal energy cutoff, default value is false
  bool energy_cutoff_use;
  //! Value of the maximal energy cutoff
  EnergyType energy_cutoff;
  
  //! Flatness below that the modification factor is changed.
  double flatness;
  
  //! Modification factor for the entropy histogram at the beginning of the simulation
  double modification_factor_initial;
  //! Modification factor for the entropy histogram at the end of the simulation, triggers the exit
  double modification_factor_final;
  //! Factor to modify the multiplication factor after the desired flatness has been reached
  double modification_factor_multiplier;
    
  //! Number of steps to take before checking again the flatness
  StepNumberType sweep_steps;

  //! Prototype histogram for all settings that the histograms of the simulation can have (e.g. binning width ...)
  HistoType<EnergyType, IncidenceCounterYValueType> prototype_histo;
    
  //! Constructor to set default values
  Parameters();
    
  //! Test for equality
  bool operator==(const Parameters<EnergyType>& rhs) const;
  //! Test for inequality
  bool operator!=(const Parameters<EnergyType>& rhs) const;
  
private:
  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    // serialize base class information
    ar & binning_reference;
    ar & binning_width;
    ar & energy_cutoff_use;
    ar & energy_cutoff;
    ar & flatness;
    ar & modification_factor_initial;
    ar & modification_factor_final;
    ar & modification_factor_multiplier;
    ar & sweep_steps;
    ar & prototype_histo;
  }
};

//! Struct used for signals to write the simulation status into std::cout
template<class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
struct WangLandau<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::SimulationStatus
{
  //! Operator for for calling the SimulationStatus function
  /*!
   * \param simulation Pointer to the simulation of which the information should be written
   * \details Writes the following information in this order: simulation_time, modfactor_current, incidence_flatness
     */
  void operator()(Simulation<ConfigurationType,RandomNumberGenerator>*);
};

//! Struct used for signals to dump the whole simulation into the specified file
template<class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
struct WangLandau<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::SimulationDump
{
  //! Operator for calling the SimulationDump function
  /*!
   * \param simulation Pointer to the simulation that should be dumped
   * \param outstream Output stream where the information is written in
   */
  void operator()(Simulation<ConfigurationType,RandomNumberGenerator>*);
};

} // of namespace Mocasinns

#include "src/wang_landau.cpp"

#endif
