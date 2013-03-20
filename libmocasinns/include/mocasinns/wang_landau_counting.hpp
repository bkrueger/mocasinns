/*!
 * \file wang_landau_counting.hpp
 * \brief Class for counting the number of states using the Wang-Landau algorithm with additional ground state bin
 * 
 * Extends the given configuration space to a new configuration space with modified energy and delta E that determines whether the configuration is in the ground state or not. Usage examples are found in the test cases. 
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_WANG_LANDAU_COUNTING_HPP
#define MOCASINNS_WANG_LANDAU_COUNTING_HPP

#include "wang_landau.hpp"

//! Class extending the energy type
/*!
 * \bug For the class extended configuration type one has to do the following design decision: 1. Make ConfigurationTypeExtended a derived class from the ConfigurationType (pro: The functions of ConfigurationType that need not to be modified must not be repeated, contra: One has to do a copy of the given configuration space in the constructor, so the algorithm does not work directly on the configuration space as expected) 2. Make ConfigurationTypeExtended contain a pointer to a object of class ConfigurationType (pro and contra are interchanged) 3. Force the user to use an extended ConfigurationSpace (contra: the user should not know about this extensions.
 */
template <class EnergyType>
class EnergyTypeExtended
{
private:
  //! Member specifying the original energy
  EnergyType original_energy;
  //! Member specifying whether we are in ground state (1) or not (0)
  int in_ground_state;

public:
  //! Constructor specifying the original energy and whether we are in ground state
  EnergyTypeExtended(EnergyType orig_energy, int in_gs);
  //! Copy-Constructor
  EnergyTypeExtended(const EnergyTypeExtended& other);

  //! Get-Accessor for the original energy
  const EnergyType& get_original_energy() { return original_energy; }
  //! Get-Accessor for the int specifying we are in ground state
  int get_in_ground_state() { return in_ground_state; }

  //! Assignment operator
  EnergyTypeExtended<EnergyType>& operator=(const EnergyTypeExtended& rhs);
  //! Operator for testing equality
  bool operator==(const EnergyTypeExtended& rhs) const;
  //! Operator for testing inequality
  bool operator!=(const EnergyTypeExtended& rhs) const;
  //! Operator for comparing two extended energy types
  bool operator<(const EnergyTypeExtended& rhs) const;
  
  //! Operator for adding another EnergyTypeExtended to this one
  void operator+=(const EnergyTypeExtended& rhs);
  //! Operator for substracting another EnergyTypeExtended from this one
  void operator-=(const EnergyTypeExtended& rhs);
};
//! Add two extended energy types
template <class EnergyType>
EnergyTypeExtended<EnergyType> operator+(const EnergyTypeExtended<EnergyType>& lhs, const EnergyTypeExtended<EnergyType>& rhs);
//! Substract two simplex numbers
template <class EnergyType>
EnergyTypeExtended<EnergyType> operator-(const EnergyTypeExtended<EnergyType>& lhs, const EnergyTypeExtended<EnergyType>& rhs);

// Forward declaration of the extended step type
template <class ConfigurationType, class StepType, class EnergyType>
class StepTypeExtended;

//! Class extending the ConfigurationType to a class with modified energy
template <class ConfigurationType, class StepType, class EnergyType>
class ConfigurationTypeExtended
{
private:
  //! Configuration that is used in the simulation
  ConfigurationType* work_configuration;
  //! Configuration that is used as reference
  ConfigurationType* reference_configuration;
  //! Energy of the reference configuration
  EnergyType reference_configuration_energy;
  //! Saved energy of the configuration (to not calculate the energy again and again ...)
  EnergyType current_energy;
  //! Flag that indicates whether this configuration is the reference configuration
  int is_reference_configuration;

public:
  //! Constructor for start configuration equal the reference configuration
  ConfigurationTypeExtended(ConfigurationType* start_and_reference_configuration);
  //! Constructor for start configuration different from reference configuration
  ConfigurationTypeExtended(ConfigurationType* start_configuration, ConfigurationType* new_reference_configuration);
  //! Destructor, deletes the copied reference_configuration
  ~ConfigurationTypeExtended();

  //! Get-Accessor for the work configuration
  ConfigurationType* get_original_configuration() { return work_configuration; }
  //! Get-Accessor for the reference configuration
  ConfigurationType* get_reference_configuration() { return reference_configuration; }
  //! Get-Accessor for the energy of the reference configuration
  const EnergyType& get_reference_configuration_energy() { return reference_configuration_energy; }
  //! Get-Accessor for the flag is reference configuration
  bool get_is_reference_configuration() { return (is_reference_configuration == 1); }

  //! Function overriding the commit function of the original configuration, sets the flag whether the system is on reference configuration
  void commit(StepTypeExtended<ConfigurationType, StepType, EnergyType>& step_to_commit);

  //! Function overriding the energy function of the original configuration
  EnergyTypeExtended<EnergyType> energy();

  //! Function overriding the propose step functionality
  template <class RandomNumberGenerator>
  StepTypeExtended<ConfigurationType, StepType, EnergyType> propose_step(RandomNumberGenerator* rng);
};

//! Class extending the StepType to a class with modified delta_E
template <class ConfigurationType, class StepType, class EnergyType>
class StepTypeExtended
{
private:
  //! Original step that will be considered
  StepType* work_step;
  //! Extended configuration to which this step belongs to
  ConfigurationTypeExtended<ConfigurationType,StepType,EnergyType>* extended_configuration_space;
  //! Flag that indicates whether this step was executed from outside
  bool was_executed;
  //! Flag that indicates whether this step was executed for reference testing from the inside
  bool was_executed_testwise;
  //! Integer part of the extended energy of the step
  int groundstate_delta_E;
  //! Flag that indicates whether the groundstate_delta_E was calculated;
  bool groundstate_delta_E_calculated;

public:
  //! Constructor
  StepTypeExtended(StepType* original_step, ConfigurationTypeExtended<ConfigurationType,StepType,EnergyType>* proposing_configuration_space);
 
  //! Destructor, undoing a possibly done step if the step is destructed before beeing executed
  ~StepTypeExtended();

  //! Function overriding the delta_E function of the original step
  EnergyTypeExtended<EnergyType> delta_E();

  //! Function overriding the execute function of the original step
  void execute();
};

//! Class for Metropolis-Monte-Carlo simulations
template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
class WangLandauCounting 
  : public WangLandau<ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType>, 
		      StepTypeExtended<ConfigurationType, StepType, EnergyType>, 
		      EnergyTypeExtended<EnergyType>, 
		      HistoType, RandomNumberGenerator>
{
  typedef ConfigurationTypeExtended<ConfigurationType, StepType, EnergyType> ConfTypeExt;
  typedef StepTypeExtended<ConfigurationType, StepType, EnergyType> StpTypeExt;
  typedef WangLandau<ConfTypeExt, StpTypeExt, EnergyTypeExtended<EnergyType>, HistoType, RandomNumberGenerator> WangLandauType;

  typedef typename WangLandauType::template Parameters<EnergyType> ParametersOriginalType;
  typedef typename WangLandauType::template Parameters<EnergyTypeExtended<EnergyType> > ParametersExtendedType;

private:
  //! Convert parameters with different energy types
  static ParametersExtendedType convert_parameters(const ParametersOriginalType& user_parameters);

public:
  //! Initialise a WangLandauCounting-MC simulation with default initial configuration and default reference configuration
  WangLandauCounting(const ParametersOriginalType& params);
  //! Initialise a WangLandauCounting-MC simulation with given initial configuration space that will be also used as a reference configuration
  WangLandauCounting(const ParametersOriginalType& params, ConfigurationType* initial_configuration);
  //! Initialise a WangLandauCounting-MC simulation with given initial configuration and given reference configuration
  WangLandauCounting(const ParametersOriginalType& params, ConfigurationType* initial_configuration, ConfigurationType* reference_configuration);

  //! Do a complete wang-landau simulation to count the number of states
  double do_wang_landau_counting();
};

#include "src/wang_landau_counting.cpp"

#endif
