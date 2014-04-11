/**
 * \file wang_landau_base.hpp
 * \brief Contains the declaration of the base class of the two Wang-Landau algorithm (normal and rejection free)
 * 
 * Do not include the file, but include <mocasinns/wang_landau.hpp>
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_WANG_LANDAU_WANG_LANDAU_BASE_HPP
#define MOCASINNS_WANG_LANDAU_WANG_LANDAU_BASE_HPP

#include "../simulation.hpp"
#include "../concepts/concepts.hpp"
#include "../details/multicanonical/step_parameter.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

namespace Mocasinns
{

  //! Base class for the two Wang-Landau simulations. Do not use directly, but use Mocasinns::WangLandau or Mocasinns::WangLandauRejectionFree
  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  class WangLandauBase : public Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>
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
    typedef typename Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>::StepNumberType StepNumberType;
    typedef double IncidenceCounterYValueType;

    // Forward declaration of the parameters for the WangLandau-Simulation
    struct Parameters;
    // Forward declaration of struct used in signals to write the simulation status into std::cout
    struct SimulationStatus;
    // Forward declaration of struct used in signals to dump the whole simulation into the specified file
    struct SimulationDump;
    
    //! Boost signal handler invoked after every sweep
    boost::signals2::signal<void (Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>*)> signal_handler_sweep;
    //! Boost signal handler invoked if the current modification factor is altered by the simulation
    boost::signals2::signal<void (Simulation<ConfigurationType, RandomNumberGenerator, rejection_free>*)> signal_handler_modfac_change;
    
    //! Initialise a WangLanday-MC simulation with default parameters, default configuration space and default RandomNumberGenerator
    WangLandauBase();
    //! Initialise a WangLandau-MC simulation with default configuration space and default RandomNumberGenerator
    WangLandauBase(const Parameters& params);
    //! Initialise a WangLandau-MC simulation with given configuration space and default RandomNumberGenerator
    WangLandauBase(const Parameters& params, ConfigurationType* initial_configuration);
    //! Copy from a Wang-Landau simulation with possible different rejection free parameter
    template <bool rejection_free_other>
    WangLandauBase(const WangLandauBase<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, rejection_free_other>& other);
    
    //! Get-Accessor for the parameters of the simulation
    const Parameters& get_simulation_parameters() const { return simulation_parameters; }
    //! Set-Accessor for the parameters of the simulation
    void set_simulation_parameters(const Parameters& value) { simulation_parameters = value; initialise_with_parameters(); }
    //! Get-Accessor for the current modification factor
    double get_modification_factor_current() const { return modification_factor_current; }
    //! Set-Accessor for the current modification factor
    void set_modification_factor_current(double value) { modification_factor_current = value; }
    //! Get-Accessor for the estimation of the density of states
    const HistoType<EnergyType, double>& get_log_density_of_states() const { return log_density_of_states; }
    //! Set-Accessor for the estimation of the density of states.
    void set_log_density_of_states(const HistoType<EnergyType, double>& value) { log_density_of_states = value; }
    //! Get-Accessor for the incidence counter
    const HistoType<EnergyType, IncidenceCounterYValueType>& get_incidence_counter() const { return incidence_counter; }
    //! Set-Accessor for an empty incidence counter (initialises an empty incidence counter that looks like the density of states)
    void set_incidence_counter() { incidence_counter.initialise_empty(log_density_of_states); }
    //! Set-Accessor for the incidence counter
    void set_incidence_counter(const HistoType<EnergyType, IncidenceCounterYValueType>& value) { incidence_counter = value; }
    //! Get-Accessor for the sweep counter
    StepNumberType get_sweep_counter() const { return sweep_counter; }
    
    //! Calculate the acceptance probability of a step
    double acceptance_probability(StepType& step_to_execute, Details::Multicanonical::StepParameter<EnergyType>& step_parameters);
    //! Handle an accepted step
    void handle_executed_step(StepType& executed_step, double time, Details::Multicanonical::StepParameter<EnergyType>& step_parameters);
    //! Handle a rejected step
    void handle_rejected_step(StepType& rejected_step, double time, Details::Multicanonical::StepParameter<EnergyType>& step_parameters);
    
    //! Do a given number of wang-landau steps
    void do_wang_landau_steps(const uint32_t& number);
    //! Do wang-landau steps until the incidence counter is flat
    void do_wang_landau_steps();
    //! Do a complete wang-landau simulation until the final modification factor is reached
    void do_wang_landau_simulation();
    
    virtual void load_serialize(std::istream& input_stream);
    virtual void load_serialize(const char* filename);
    virtual void save_serialize(std::ostream& output_stream) const;
    virtual void save_serialize(const char* filename) const;
    
  protected:
    //! Histogram for the estimation of the density of states
    HistoType<EnergyType, double> log_density_of_states;
    //! Histogram for the incidence counter
    HistoType<EnergyType, IncidenceCounterYValueType> incidence_counter;
    
  private:
    //! Parameters of the simulation
    Parameters simulation_parameters;
    
    //! Current value of the modification factor
    double modification_factor_current;
    
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
      ar & boost::serialization::base_object<Simulation<ConfigurationType, RandomNumberGenerator, rejection_free> >(*this);
      ar & simulation_parameters;
      ar & modification_factor_current;
      ar & log_density_of_states;
      ar & incidence_counter;
    }
  };
}

#include "../src/wang_landau.cpp"

#endif
