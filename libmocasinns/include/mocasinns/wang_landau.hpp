/**
 * \file wang_landau.hpp
 * \brief Contains the declaration of the base class of the two Wang-Landau algorithm (normal and rejection free)
 * 
 * Do not include the file, but include <mocasinns/wang_landau.hpp>
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_WANG_LANDAU_HPP
#define MOCASINNS_WANG_LANDAU_HPP

#include "simulation.hpp"
#include "concepts/concepts.hpp"
#include "details/multicanonical/step_parameter.hpp"
#include "details/multicanonical/parameters_multicanonical.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

namespace Mocasinns
{
  /*!
   * \brief Class for the Wang-Landau algorithm
   *
   * \details The Wang-Landau algorithm is an improvement of entropic sampling and uses the same acceptance probability
   * \f[
   *   A(\sigma_1 \rightarrow \sigma_2) = \min\left(1, \frac{g(E(\sigma_1))}{g(E(\sigma_2))} \right)
   * \f]. 
   * to sample according to the probability distribution \f$ P(\sigma) = g(E(\sigma))^{-1}\f$.
   * In contrast to entropic sampling the density of states is not updated after a certain number of steps, but after each step, as well as the incidence counter \f$H(E) \f$. 
   * If the system has energy $E$ after a step (independent on whether the step has been accepted or not), the incidence counter and the density of states are modified according to:
   * \f[
   * \begin{split}
   *   H(E) &\rightarrow H(E) + 1 \\
   *   g(E) &\rightarrow f\cdot g(E)
   * \end{split}
   * \f]
   * After a certain number of steps the flatness of the incidence counter \f$\mathrm{flat}(H) \f$ is tested. 
   * If the flatness is bigger than an user-defined value, the modification factor is lowered by applying
   * \f[
   * f_{n+1} = f_n^m\quad m\in [0;1]
   * \f]
   * The algorithm stops if \f$f_n\f$ becomes smaller than a final value \f$f_e \f$. 
   * Commonly one uses the parameters \f$ f_0 = \exp(1) \f$ and \f$ f_e = \exp\left(10^{-8}\right) \f$ and chooses \f$ m=0.5 \f$ which make the recurrsion relation to \f$ f_{n+1} = \sqrt{f_n} \f$.
   *
   * As in the entropic sampling algorithm the logarithm of the density of states is stored, and one adds the logarithm of the modification factor to this entropy in each step. 
   * So also the parameters for the modification factors must be provided in logarithmic form by the user.
   *
   * \signalhandlers
   * \signalhandler{signal_handler_modfac_change,This handler is called if the flatness criterion was reached and the current modification factor will be decreased.}
   * \signalhandler{signal_handler_sweep, This handler is called after every sweep of \c Parameters::sweep_steps steps.}
   * \signalhandler{signal_handler_sig...., The check for \c POSIX signals (SIGTERM\, SIGUSR1 and SIGUSR2) is performed after every sweep of \c Parameters::sweep_steps steps.}
   * \endsignalhandlers
   *
   * \tparam ConfigurationType \concept{ConfigurationType}
   * \tparam StepType \concept{StepType}
   * \tparam EnergyType \concept{EnergyType}
   * \tparam HistoType \concept{HistoType}
   * \tparam RandomNumberGenerator \concept{RandomNumberGenerator}
   */
  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator, bool rejection_free = false>
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
    typedef double IncidenceCounterYValueType;

    // Forward declaration of the parameters for the WangLandau-Simulation
    struct Parameters;
    // Forward declaration of struct used in signals to write the simulation status into std::cout
    struct SimulationStatus;
    // Forward declaration of struct used in signals to dump the whole simulation into the specified file
    struct SimulationDump;
    
    //! Boost signal handler invoked after every sweep
    boost::signals2::signal<void (Simulation<ConfigurationType, RandomNumberGenerator>*)> signal_handler_sweep;
    //! Boost signal handler invoked if the current modification factor is altered by the simulation
    boost::signals2::signal<void (Simulation<ConfigurationType, RandomNumberGenerator>*)> signal_handler_modfac_change;
    
    //! Initialise a WangLandau-MC simulation with default parameters, default configuration space and default RandomNumberGenerator
    WangLandau();
    //! Initialise a WangLandau-MC simulation with default configuration space and default RandomNumberGenerator
    WangLandau(const Parameters& params);
    //! Initialise a WangLandau-MC simulation with given configuration space and default RandomNumberGenerator
    WangLandau(const Parameters& params, ConfigurationType* initial_configuration);
    //! Copy from a Wang-Landau simulation with possible different rejection free parameter
    WangLandau(const WangLandau<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>& other);
    
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
    void handle_executed_step(StepType&, double time, Details::Multicanonical::StepParameter<EnergyType>& step_parameters);
    //! Handle a rejected step
    void handle_rejected_step(StepType&, double time, Details::Multicanonical::StepParameter<EnergyType>& step_parameters);
    
    //! Do a given number of wang-landau steps
    void do_wang_landau_steps(const uint32_t& number);
    //! Do wang-landau steps until the incidence counter is flat
    void do_wang_landau_steps();
    //! Do a complete wang-landau simulation until the final modification factor is reached
    void do_wang_landau_simulation();
    //! Do a complete wang-landau simulation using a 1/t change in the modification factor until the final modifcation factor is reached
    void do_wang_landau_simulation_1_t(unsigned int monte_carlo_time_unit);
    
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
    
    friend class boost::serialization::access;
    //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
    template<class Archive> void serialize(Archive & ar, const unsigned int)
    {
      // serialize base class information
      ar & boost::serialization::base_object<Simulation<ConfigurationType, RandomNumberGenerator> >(*this);
      ar & simulation_parameters;
      ar & modification_factor_current;
      ar & log_density_of_states;
      ar & incidence_counter;
    }
  };

  // Typedef for Wang Landau rejection-free.
  // Checks for C++11 support for template typedef
  #ifndef BOOST_NO_CXX11_TEMPLATE_ALIASES
  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator> 
  using WangLandauRejectionFree = WangLandau<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, true>;
  #endif

  //! Struct for dealing with the parameters of a Wang-Landau-simulation
  template<class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  struct WangLandau<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, rejection_free>::Parameters : public Details::Multicanonical::ParametersMulticanonical<EnergyType>
  {
  public:
    //! Typedef for the base class
    typedef Details::Multicanonical::ParametersMulticanonical<EnergyType> Base;

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
    
    //! Number of sweeps that should be executed before a reset of the incidence counter and recalculation. Choose 0 (default) for no resets.
    unsigned int reset_sweep_number;
    
    //! Constructor to set default values
    Parameters() :
      Base(),
      flatness(0.8),
      modification_factor_initial(1.0),
      modification_factor_final(1e-7),
      modification_factor_multiplier(0.9),
      sweep_steps(1000),
      prototype_histo(),
      reset_sweep_number(0) { }
    
    //! Constructor to copy parameters from other parameters with convertible energy type
    template <class OtherParametersType>
    explicit Parameters(const OtherParametersType& other) :
      Base(other),
      flatness(other.flatness),
      modification_factor_initial(other.modification_factor_initial),
      modification_factor_final(other.modification_factor_final),
      modification_factor_multiplier(other.modification_factor_multiplier),
      sweep_steps(other.sweep_steps),
      prototype_histo(other.prototype_histo),
      reset_sweep_number(other.reset_sweep_number) { }
    
    //! Test for equality
    bool operator==(const Parameters& rhs) const
    {
      return ((static_cast<Base>(*this) == static_cast<Base>(rhs)) &&
	      (flatness == rhs.flatness) &&
	      (modification_factor_initial == rhs.modification_factor_initial) &&
	      (modification_factor_final == rhs.modification_factor_final) &&
	      (modification_factor_multiplier == rhs.modification_factor_multiplier) &&
	      (sweep_steps == rhs.sweep_steps) && 
	      (reset_sweep_number == rhs.reset_sweep_number));
    }
    
    //! Test for inequality
    bool operator!=(const Parameters& rhs) const
    {
      return !operator==(rhs);
    }
    
  private:
    friend class boost::serialization::access;
    //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
    template<class Archive> void serialize(Archive & ar, const unsigned int)
    {
      // serialize base class information
      ar & boost::serialization::base_object<Base>(*this);
      ar & flatness;
      ar & modification_factor_initial;
      ar & modification_factor_final;
      ar & modification_factor_multiplier;
      ar & sweep_steps;
      ar & prototype_histo;
    }
  };  

  //! Struct used for signals to write the simulation status into std::cout
  template<class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  struct WangLandau<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, rejection_free>::SimulationStatus
  {
    //! Operator for for calling the SimulationStatus function
    /*!
     * \param simulation Pointer to the simulation of which the information should be written
     * \details Writes the following information in this order: simulation_time, modfactor_current, incidence_flatness
     */
    void operator()(Simulation<ConfigurationType,RandomNumberGenerator>* simulation)
    {
      WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>* simulation_wang_landau = static_cast<WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>*>(simulation);
      std::cout << simulation_wang_landau->get_config_space()->get_simulation_time() << "\t";
      std::cout << simulation_wang_landau->get_modification_factor_current() << "\t";
      std::cout << simulation_wang_landau->get_incidence_counter().flatness() << std::endl;
    }
  };

  //! Struct used for signals to dump the whole simulation into the specified file
  template<class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator, bool rejection_free>
  struct WangLandau<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, rejection_free>::SimulationDump
  {
    //! Operator for calling the SimulationDump function
    /*!
     * \param simulation Pointer to the simulation that should be dumped
     */
    void operator()(Simulation<ConfigurationType,RandomNumberGenerator>* simulation)
    {
      WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>* simulation_wang_landau = static_cast<WangLandau<ConfigurationType,StepType,EnergyType,HistoType,RandomNumberGenerator>*>(simulation);
      simulation_wang_landau->save_serialize(simulation->get_dump_filename().c_str());
    }

  };
}

#include "src/wang_landau.cpp"

#endif
