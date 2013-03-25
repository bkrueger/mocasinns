/**
 * \file entropic_sampling.hpp
 * \brief Class for entropic sampling simulations with MoCaSinns
 * 
 * Does a entropic sampling imulation and determines the density of states with respect to the energy functional. Usage examples are found in the test cases. 
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_ENTROPIC_SAMPLING_HPP
#define MOCASINNS_ENTROPIC_SAMPLING_HPP

#include "simulation.hpp"
#include "concepts/concepts.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

namespace Mocasinns
{
  //! Class for Metropolis-Monte-Carlo simulations
  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
  class EntropicSampling : public Simulation<ConfigurationType, RandomNumberGenerator>
  {
    // Check the configuration concept
    BOOST_CONCEPT_ASSERT((Concepts::ConfigurationConcept<ConfigurationType, StepType>));
    // Check the step concept
    BOOST_CONCEPT_ASSERT((Concepts::StepConcept<StepType>));
    // Check the energy concept
    BOOST_CONCEPT_ASSERT((Concepts::EnergyConcept<EnergyType>));
    // Check the random number generator concept
    BOOST_CONCEPT_ASSERT((Concepts::RandomNumberGeneratorConcept<RandomNumberGenerator>));

  public:
    // Typedefs for integers
    typedef typename Simulation<ConfigurationType, RandomNumberGenerator>::StepNumberType StepNumberType;
    typedef uint64_t IncidenceCounterYValueType;

    // Forward declaration of the parameters for the entropic sampling simulation
    struct Parameters;
    
    //! Boost signal handler invoked after every sweep
    boost::signals2::signal<void (Simulation<ConfigurationType,RandomNumberGenerator>*)> signal_handler_sweep;

    //! Initialise an entropic sampling simulation with default parameters, default configuration space and default RandomNumberGenerator
    EntropicSampling();
    //! Initialise an entropic sampling simulation with default configuration space and default RandomNumberGenerator
    EntropicSampling(const Parameters& params);
    //! Initialise an entropic sampling simulation with given configuration space and default RandomNumberGenerator
    EntropicSampling(const Parameters& params, ConfigurationType* initial_configuration);

    //! Get-Accessor for the parameters of the simulation
    const Parameters& get_simulation_parameters() const { return simulation_parameters; }
    //! Set-Accessor for the parameters of the simulation
    void set_simulation_parameters(const Parameters& value) { simulation_parameters = value; }
    //! Get-Accessor for the estimation of the density of states
    const HistoType<EnergyType, double>& get_density_of_states() const { return density_of_states; }
    //! Set-Accessor for the estimation of the density of states
    void set_density_of_states(const HistoType<EnergyType, double>& value) { density_of_states = value; }
    //! Get-Accessor for the incidence counter
    const HistoType<EnergyType, IncidenceCounterYValueType>& get_incidence_counter() const { return incidence_counter; }
    //! Set-Accessor for the incidence counter
    void set_incidence_counter(const HistoType<EnergyType, IncidenceCounterYValueType>& value) { incidence_counter = value; }
    //! Get-Accessor for the current flatness
    const double& get_flatness_current() const { return flatness_current; }

    //! Calculate the acceptance probability of a step
    double acceptance_probability(StepType& step_to_execute, EnergyType& total_energy);
    //! Handle an accepted step
    void handle_executed_step(StepType& executed_step, EnergyType& total_energy);
    //! Handle a rejected step
    void handle_rejected_step(StepType& rejected_step, EnergyType& total_energy);
    
    //! Do a certain number of entropic sampling steps updating the incidence_counter
    void do_entropic_sampling_steps(const StepNumberType& number);
    //! Do a complete entropic sampling simulation until the desired flatness is reached
    void do_entropic_sampling_simulation();
    
    //! Load the data of the entropic sampling simulation from a serialization stream
    virtual void load_serialize(std::istream& input_stream);
    //! Load the data of the entropic sampling simulation from a serialization file
    virtual void load_serialize(const char* filename);
    //! Save the data of the entropic sampling simulation to a serialization stream
    virtual void save_serialize(std::ostream& output_stream) const;
    //! Save the data of the entropic sampling simulation to a serialization file
    virtual void save_serialize(const char* filename) const;
    
  private:
    //! Parameters of the simulation
    Parameters simulation_parameters;
    
    //! Histogram for the estimation of the density of states
    HistoType<EnergyType, double> density_of_states;
    //! Histogram for the incidence counter
    HistoType<EnergyType, IncidenceCounterYValueType> incidence_counter;

    //! Current value of the flatness of the incidence counter
    double flatness_current;
    
    //! Member variable for boost serialization
    friend class boost::serialization::access;
    //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
    template<class Archive> void serialize(Archive & ar, const unsigned int)
    {
      // serialize base class information
      ar & boost::serialization::base_object<Simulation<ConfigurationType, RandomNumberGenerator> >(*this);
      ar & simulation_parameters;
      ar & density_of_states;
      ar & incidence_counter;
    }
  };

  //! Struct for dealing with the parameters of a Wang-Landau-simulation
  template<class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
  struct EntropicSampling<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::Parameters
  {
  public:
    //! Energy value that is used as a reference point for the binning
    EnergyType binning_reference;
    //! Energy value range that is comprehended in one bin
    EnergyType binning_width;
    
    //! Flag whether to use a maximal energy cutoff, default value is false
    bool energy_cutoff_use;
    //! Value of the maximal energy cutoff
    EnergyType energy_cutoff_upper;
    //! Value of the minimal energy cutoff
    EnergyType energy_cutoff_lower;
    
    //! Flatness of the incidence counter histogram that should be reached
    double flatness;
    
    //! Number of steps to do between updating the density of states
    StepNumberType sweep_steps;
    
    //! Prototype histogram for all settings that the histograms of the simulation can have (e.g. binning width ...)
    HistoType<EnergyType, IncidenceCounterYValueType> prototype_histo;
    
    //! Constructor to set default values
    Parameters();
    
    //! Test for equality
    bool operator==(const Parameters& rhs) const;
    //! Test for inequality
    bool operator!=(const Parameters& rhs) const;
  
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
      ar & energy_cutoff_lower;
      ar & energy_cutoff_upper;
      ar & flatness;
      ar & sweep_steps;
      ar & prototype_histo;
    }
  };
    
} // of namespace Mocasinns

#include "src/entropic_sampling.cpp"

#endif
