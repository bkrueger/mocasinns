/*!
 * \file optimal_ensemble_sampling.hpp
 * \brief Class for Optimal-Sampling-Monte-Carlo simulations with MoCaSinns
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_OPTIMAL_ENSEMBLE_SAMPLING_HPP
#define MOCASINNS_OPTIMAL_ENSEMBLE_SAMPLING_HPP

#include "simulation.hpp"
#include "wang_landau.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

namespace Mocasinns
{

  //! 
  /*!
    \brief Class for Optimal Ensemble Sampling
   
    \tparam ConfigurationType Type of the configuration space considered.
    \tparam StepType Type of the step considered.
    \tparam EnergyType Return type of the ConfigurationType::energy() and StepType::delta_E() functions
    \tparam HistoType Template template parameter giving the type of the histograms to use. The first template parameters specifies the type of the x-values, the second template parameter specifies the type of the y-values. Use e.g. the classes in Mocasinns::Histograms
    \tparam RandomNumberGenerator Type of the random number generator to use.

    \details Does a Optimal-Sampling simulation and determines the density of states with respect to the energy functional. Usage examples are found in the test cases. 
    
    The algorithm is based on the following two papers:
    [1] S. Trebst, D. A. Huse, M. Troyer: "Optimizing the ensemble for equilibration in broad-histogram Monte Carlo simulations", PRE 70, 046701 (2004)
    [2] S. Trebst, D. A. Huse, E. Gull, H. G. Katzgraber, U. H. E. Hansmann, M. Troyer: "Ensemble Optimization Techniques for the Simulation of Slowly Equilibrating Systems", in: Springer Proceedings in Physics Vol. 123, "Computer Simulation Studies in Condensed-Matter Physics XIX" (2007)
   */
  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
  class OptimalEnsembleSampling : public Simulation<ConfigurationType, RandomNumberGenerator>
  {
  public:
    //! Struct storing the parameters of the optimal ensemble sampling simulation
    struct Parameters;

    //! Typedef for a Wang-Landau simulation (used to estimate the weights)
    typedef WangLandau<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator> WangLandauSimulation;
    //! Typedef for the parameters of a Wang-Landau simulation (used to estimate the weights)
    typedef typename WangLandauSimulation::template Parameters<EnergyType> WangLandauParameters;

    //! Initialise a optimal ensemble sampling Monte-Carlo simulation with default parameters and default initial configuration space
    OptimalEnsembleSampling();
    //! Initialise a optimal ensemble sampling Monte-Carlo simulation with given parameters and default initial configuration space
    OptimalEnsembleSampling(const Parameters& parameters);
    //! Initialise a optimal ensemble sampling Monte-Carlo simulation with given parameters and given initial configuration space 
    OptimalEnsembleSampling(const Parameters& parameters, ConfigurationType* initial_configuration);

    //! Get-Accessor for the parameters of the simulation
    const Parameters& get_parameters() { return simulation_parameters; }
    //! Get-Accessor for the weights
    const HistoType<EnergyType, double>& get_weights() { return weights; }
    //! Get-Accessor for the incidence counter of positive labled walkers
    const HistoType<EnergyType, int>& get_incidence_counter_positive() { return incidence_counter_positive; }
    //! Get-Accessor for the incidence counter of negative labled walkers
    const HistoType<EnergyType, int>& get_incidence_counter_negative() { return incidence_counter_negative; }

    //! Find the minimal and the maximal energy of the system
    template<class TemperatureType>
    void find_minimal_maximal_energy(const TemperatureType& inverse_temperature_minimal_energy, const TemperatureType& invserse_temperature_maximal_energy, unsigned int steps);

    //! Estimate the weights from the density of states calculated in a Wang-Landau simulation with standard Wang-Landau parameters
    void estimate_weights();
    //! Estimate the weights from the density of states calculated in a Wang-Landau simulation with given Wang-Landau parameters
    void estimate_weights(const WangLandauParameters& wang_landau_parameters);

    //! Do a certain number of steps updating the density_of_states and the incidence_counter
    void do_optimal_ensemble_sampling_steps(const uint32_t& number);

    //! Do an optimal ensemble sampling simulation
    HistoType<EnergyType, double> do_optimal_ensemble_sampling_simulation();

    //! Load the data of the Optimal-Ensemble-Sampling simulation from a serialization stream
    virtual void load_serialize(std::istream& input_stream);
    //! Load the data of the Optimal-Ensemble-Sampling simulation from a serialization file
    virtual void load_serialize(const char* filename);
    //! Save the data of the Optimal-Ensemble-Sampling simulation to a serialization stream
    virtual void save_serialize(std::ostream& output_stream) const;
    //! Save the data of the Optimal-Ensemble-Sampling simulation to a serialization file
    virtual void save_serialize(const char* filename) const;

  private:
    //! Parameters of the simulation
    Parameters simulation_parameters;

    //! Histogram for the weights of the simulation
    HistoType<EnergyType, double> weights;
    //! Histogram for the incidence counter for positive labled walkers
    HistoType<EnergyType, unsigned long int> incidence_counter_positive;
    //! Histogram for the incidence counter for negative labled walkers
    HistoType<EnergyType, unsigned long int> incidence_counter_negative;

    //! Enum for the possible labels of the walker
    enum WalkerLabel { positive, negative};
    //! Flag that indicates the label of the walker
    WalkerLabel walker_label;

    //! Calculate the density of states from the current total incidence counter and the total weights
    HistoType<EnergyType, double> calculate_density_of_states() const;
    //! Initialise the histograms with the parameters
    void initialize_with_parameters();

    //! Friend class declaration for boost serialization
    friend class boost::serialization::access;
    //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
    template<class Archive> void serialize(Archive & ar, const unsigned int)
    {
      // serialize base class information
      ar & boost::serialization::base_object<Simulation<ConfigurationType, RandomNumberGenerator> >(*this);
      ar & weights;
      ar & incidence_counter_positive;
      ar & incidence_counter_negative;
      ar & walker_label;
    }

  };

  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator>
  struct OptimalEnsembleSampling<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator>::Parameters
  {
    //! Number of steps to perform for the initial try
    unsigned int initial_steps_per_iteration;

    //! Number of iterations
    unsigned int iterations;

    //! Smallest possible energy of the system, used to change the sign of the walker
    EnergyType minimal_energy;
    //! Largest possible energy of the system, used to change the sign of the walker
    EnergyType maximal_energy;

    //! Prototype histogram for all settings that the histograms of the simulation can have (e.g. binning width ...)
    HistoType<EnergyType, int> prototype_histo;

    //! Constructor setting the default values
    Parameters() : initial_steps_per_iteration(1000),
		   iterations(10),
		   minimal_energy(0),
		   maximal_energy(100),
		   prototype_histo() {}
  };
  
} // of namespace Mocasinns

#include "optimal_ensemble_sampling.cpp"

#endif
