/**
 * \file metropolis_hastings.hpp
 * \brief Class for Metropolis-Hastings-Monte-Carlo simulations with MoCaSinns
 * 
 * Does Metropolis-Hastings steps for sampling an arbitrary probability distribution. Usage examples are found in the test cases
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_METROPOLIS_HASTINGS_HPP
#define MOCASINNS_METROPOLIS_HASTINGS_HPP

#include "metropolis.hpp"
#include "concepts/concepts.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

// Boost function types for the standard observable
#include <boost/function_types/result_type.hpp>
#include <boost/typeof/std/utility.hpp>
#include <boost/type_traits.hpp>

namespace Mocasinns
{
  /*!
   * \brief Class for the Metropolis-Hastings algorithm
   *
   * \details The Metropolis-Hastings algorithm is a generalization of the Metropolis algorithm and can be used for sampling arbitrary distributions \f$ P(E(\sigma)) \f$. 
   * The Metropolis-Hastings-choice of the acceptance probabilities is:
   * \f[
   *    A(\sigma_1 \rightarrow \sigma_2) = 
   *    \begin{cases}
   *       1 & \text{if }\beta \Delta E \leq -\ln(s) \\
   *       \frac{1}{s}\cdot \exp\left(-\beta \Delta E \right) & \text{if } \beta \Delta E > -\ln(s)
   *    \end{cases}
   * \f]
   *
   * To perform a metropolis simulation, use one of the \c Metropolis::do_metropolis_simulation() function.
   * All of them take as an optional template argument an <tt>Observator</tt> that must fulfill the \ref concept-Observator "Observator concept".
   * If no such template parameter is specified, the algorithm observes the energy of the system.
   * There are the following overrides:
   * - <tt>do_metropolis_simulation<Observator>(TemperatureType inverse_temperature)</tt>: Do a Metropolis simulation for a single temperature and return a vector of the observables measured by the <tt>Observator</tt>.
   * - <tt>do_metropolis_simulation<Observator>(Iterator inverse_temperatures_begin, Iterator inverse_temperatures_end)</tt>: Do a Metropolis simulation for a range of given temperatures and return a vector of vectors of the observables measured by the <tt>Observator</tt>. The outer vector is for the different temperatures, the inner vector for the different measurements.
   * - <tt>do_metropolis_simulation<Observator>(TemperatureType inverse_temperature, Accumulator& measurement_accumulator)</tt>: Do a Metropolis simulation for a single temperature and accumulate the measured observables in a given accumulator (that fulfills the \ref concept-Accumulator "Accumulator concept"). This can be used e.g. for calculating the mean and the variance of the observables without storing the single measurement results.
   * - <tt>do_metropolis_simulation<Observator>(TemperatureType inverse_temperature, Accumulator& measurement_accumulator)</tt>: Do a Metropolis simulation for a single temperature and accumulate the measured observables in a given accumulator (that fulfills the \ref concept-Accumulator "Accumulator concept"). This can be used e.g. for calculating the mean and the variance of the observables without storing the single measurement results.
   *
   * \tparam ConfigurationType \concept{ConfigurationType}
   * \tparam StepType \concept{StepType}
   * \tparam RandomNumberGenerator \concept{RandomNumberGenerator}
   */
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  class MetropolisHastings : public Simulation<ConfigurationType, RandomNumberGenerator>
  {
    // Check the configuration concept
    BOOST_CONCEPT_ASSERT((Concepts::ConfigurationConcept<ConfigurationType, StepType>));  
    // Check the step concept
    BOOST_CONCEPT_ASSERT((Concepts::StepConcept<StepType>));
    // Check the random number generator concept
    BOOST_CONCEPT_ASSERT((Concepts::RandomNumberGeneratorConcept<RandomNumberGenerator>));

  public:
    //! Typedef of this class
    typedef MetropolisHastings<ConfigurationType, StepType, RandomNumberGenerator> this_type;
    // Typedefs for integers
    typedef typename Simulation<ConfigurationType, RandomNumberGenerator>::StepNumberType StepNumberType;
    typedef uint32_t MeasurementNumberType;

    //! Forward declaration of the struct storing the Parameters of a Metropolis Simulation
    struct Parameters;
    //! Structure for handling the acceptance probability and the executed and rejected step handling
    template <class EnergyType, class AcceptanceProbabilityFunctor>
    struct AcceptanceProbabilityParameters
    {
      EnergyType actual_energy;
      EnergyType delta_E;
      AcceptanceProbabilityFunctor acceptance_probability_functor;
    };
    //! Forward declaration of an acceptance probability functor for the usual Boltzman weights
    template <class EnergyType> 
    struct BoltzmannAcceptanceProbability;
    //! Forward declaration of an acceptance probability functor for flat histogram weights
    template <class EnergyType, template<class,class> class HistoType> 
    struct FlatHistogramAcceptanceProbability;

    //! Typedef for the default observable
    typedef typename Metropolis<ConfigurationType, StepType, RandomNumberGenerator>::ObserveEnergy DefaultObservator;

    //! Boost signal handler invoked after every measurement
    boost::signals2::signal<void (Simulation<ConfigurationType,RandomNumberGenerator>*)> signal_handler_measurement;

    //! Initialise a Metropolis-MC simulation with default configuration space and default Parameters
    MetropolisHastings() : Simulation<ConfigurationType, RandomNumberGenerator>(), simulation_parameters() {}
    //! Initialise a Metropolis-MC simulation with default configuration space and given Parameters
    MetropolisHastings(const Parameters& params) : Simulation<ConfigurationType, RandomNumberGenerator>(), simulation_parameters(params) {}
    //! Initialise a Metropolis-MC simulation with given parameters and given configuration space
    MetropolisHastings(const Parameters& params, ConfigurationType* initial_configuration) : Simulation<ConfigurationType, RandomNumberGenerator>(initial_configuration), simulation_parameters(params) {}
    //! Initialise a Metropolis-MC simulation by copying from another simulation
    MetropolisHastings(const MetropolisHastings& other)
      : Simulation<ConfigurationType, RandomNumberGenerator>(other),
	simulation_parameters(other.simulation_parameters) {}

    //! Get-accessor for the parameters of the Metropolis simulation
    const Parameters& get_simulation_parameters() { return simulation_parameters; }
    //! Set-accessor for the parameters of the Metropolis simulation
    void set_parameters(const Parameters& value) { simulation_parameters = value; }
    
    //! Calculate the acceptance probability for a given step, must be implemented to use Simulation::do_steps
    /*!
      \param step_to_execute Step of which the acceptance probability should be calculated
      \param acceptance_probability_parameters Parameters used for calculating the energy difference
    */
    template <class AcceptanceProbabilityParameters>
    inline double acceptance_probability(StepType& step_to_execute, AcceptanceProbabilityParameters& acceptance_probability_parameters)
    {
      acceptance_probability_parameters.delta_E = step_to_execute.delta_E();
      return acceptance_probability_parameters.acceptance_probability_functor(acceptance_probability_parameters.delta_E, 
									      acceptance_probability_parameters.actual_energy);
    }
    //! Handle an executed step (do nothing, must be implemented to use Simulation::do_steps)
    template <class AcceptanceProbabilityParameters>
    inline void handle_executed_step(StepType&, double, AcceptanceProbabilityParameters& acceptance_probability_parameters) 
    /*!
      \param acceptance_probability_parameters Parameters used for calculating the energy difference
    */
    {
      acceptance_probability_parameters.actual_energy += acceptance_probability_parameters.delta_E;
    }
    //! Handle a rejected step (do nothing, must be implemented to use Simulation::do_steps)
    template <class NotImportant>
    inline void handle_rejected_step(StepType&, double, NotImportant&) {}

    //! Execute a given number of Metropolis-MC steps on the configuration at inverse temperatur beta
    /*!
      \details This function is just syntax sugar and calls the Simulation::do_steps() function
      \param number Number of Metropolis-Hastings steps that will be performed
      \param acceptance_probability_parameters Class object that defines a "double operator(EnergyType delta_E, EnergyType actual_energy)" returning the acceptance probability of the step.
    */
    template<class AcceptanceProbabilityParameters>
    void do_metropolis_hastings_steps(const StepNumberType& number, AcceptanceProbabilityParameters& acceptance_probability_parameters)
    {
      this->template do_steps<this_type, StepType, false>(number, acceptance_probability_parameters);
    }

    //! Execute a Metropolis Monte-Carlo simulation at given inverse temperature
    template<class Observator = DefaultObservator, class AcceptanceProbabilityFunctor>
    std::vector<typename Observator::observable_type> do_metropolis_hastings_simulation(const AcceptanceProbabilityFunctor& acceptance_probability_functor);

    //! Execute a Metropolis Monte-Carlo simulation at given inverse temperature with an accumulator for storing the measurements results
    template<class Observator = DefaultObservator, class Accumulator, class AcceptanceProbabilityFunctor>
    void do_metropolis_hastings_simulation(const AcceptanceProbabilityFunctor& acceptance_probability_functor, Accumulator& measurement_accumulator);

    //! Load the data of the Metropolis-Hastings simulation from a serialization stream
    virtual void load_serialize(std::istream& input_stream) { Base::load_serialize(*this, input_stream); }
    //! Load the data of the Metropolis-Hastings simulation from a serialization file
    virtual void load_serialize(const char* filename) { Base::load_serialize(*this, filename); }
    //! Save the data of the Metropolis-Hastings simulation to a serialization stream
    virtual void save_serialize(std::ostream& output_stream) const { Base::save_serialize(*this, output_stream); }
    //! Save the data of the Metropolis-Hastings simulation to a serialization file
    virtual void save_serialize(const char* filename) const { Base::save_serialize(*this, filename); }    
  private:
    //! Member variable storing the parameters of the simulation
    Parameters simulation_parameters;
    
    //! Member variable for boost serialization
    friend class boost::serialization::access;
    //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
    template<class Archive> void serialize(Archive & ar, const unsigned int)
    {
      // serialize base class information
      ar & boost::serialization::base_object<Simulation<ConfigurationType, RandomNumberGenerator> >(*this);
    }
  };

  //! Struct storing the definition of the Parameters of a Metropolis-Simulation
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  struct MetropolisHastings<ConfigurationType, StepType, RandomNumberGenerator>::Parameters
    : Metropolis<ConfigurationType, StepType, RandomNumberGenerator>::Parameters
  {
    //! Standard constructor for setting default values
    Parameters() : Metropolis<ConfigurationType, StepType, RandomNumberGenerator>::Parameters() { }
  };

  //! Struct for calculating the standard Metropolis acceptance probability
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class EnergyType>
  struct MetropolisHastings<ConfigurationType, StepType, RandomNumberGenerator>::BoltzmannAcceptanceProbability
  {
    //! Inverse temperature
    EnergyType beta;
    
    //! Functor for calculating the acceptance probability
    double operator()(const EnergyType& delta_E, const EnergyType&) { return exp( -(beta * delta_E)); }
  };

  //! Struct for calculating the standard flat histogram acceptance probability
  template <class ConfigurationType, class StepType, class RandomNumberGenerator>
  template <class EnergyType, template<class,class> class HistoType>
  struct MetropolisHastings<ConfigurationType, StepType, RandomNumberGenerator>::FlatHistogramAcceptanceProbability
  {
    //! Histogram of the log density of states
    HistoType<EnergyType, double> log_density_of_states;
    
    //! Value of the minimal energy cutoff
    EnergyType energy_cutoff_lower;
    //! Value of the maximal energy cutoff
    EnergyType energy_cutoff_upper;
    //! Flag indicating whether to use the minimal energy cutoff, default value is false
    bool use_energy_cutoff_lower;
    //! Flag indicating whether to use the maximal energy cutoff, default value is false
    bool use_energy_cutoff_upper;

    FlatHistogramAcceptanceProbability()
      : use_energy_cutoff_lower(false),
	use_energy_cutoff_upper(false) { }

    //! Functor for calculating thea acceptance probability
    double operator()(const EnergyType& delta_E, const EnergyType& actual_energy) 
    { 
      if ((use_energy_cutoff_lower && actual_energy + delta_E < energy_cutoff_lower) ||
	  (use_energy_cutoff_upper && actual_energy + delta_E > energy_cutoff_upper))
	return 0.0;

      return exp(log_density_of_states[actual_energy] - log_density_of_states[actual_energy + delta_E]);
    }
  };

} // of namespace Mocasinns

#include "src/metropolis_hastings.cpp"

#endif
