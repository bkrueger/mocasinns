#ifdef MOCASINNS_MULTICANONICAL_COUNTING_HPP

namespace Mocasinns
{
  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator,
	    template<class,class,class,template<class,class> class,class> class AlgorithmType>
  void MulticanonicalCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, AlgorithmType>::set_log_density_of_states(const HistoType<EnergyType, double>& value)
  {
    // Typedefs for extended and normal histogram types
    typedef HistoType<ExtendedEnergyType, double> ExtendedHistoType;
    typedef HistoType<EnergyType, double> ReducedHistoType;

    // Create the result histogram and set all non-reference bins
    this->log_density_of_states.clear();
    for (typename ReducedHistoType::const_iterator it_reduced_histo = value.begin();
	 it_reduced_histo != value.end(); ++it_reduced_histo)
    {
      this->log_density_of_states[ExtendedEnergyType(it_reduced_histo->first, 0)] = it_reduced_histo->second;
    }
    // Set the value of the reference bin
    this->log_density_of_states[ExtendedEnergyType(this->get_config_space()->get_reference_configurations().begin()->first, 1)]
      = log(this->get_config_space()->count_reference_configurations());
    // Decrease the values of the normal bins
    for (typename ExtendedConfigurationType::reference_configurations_map_t::const_iterator energy_reference_configurations = this->get_config_space()->get_reference_configurations().begin();
	 energy_reference_configurations != this->get_config_space()->get_reference_configurations().end(); ++energy_reference_configurations)
    {
      const EnergyType& reference_energy = energy_reference_configurations->first;
      this->log_density_of_states[ExtendedEnergyType(reference_energy, 0)] 
	= log(exp(this->log_density_of_states[ExtendedEnergyType(reference_energy, 0)]) - energy_reference_configurations->second.size());
    }
  }

  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator,
	    template<class,class,class,template<class,class> class,class> class AlgorithmType>
  HistoType<EnergyType, double> MulticanonicalCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, AlgorithmType>::get_log_density_of_states() const
  {
    // Typedefs for extended and normal histogram types
    typedef HistoType<ExtendedEnergyType, double> ExtendedHistoType;
    typedef HistoType<EnergyType, double> ReducedHistoType;

    // Save the density of states
    ExtendedHistoType extended_log_dos = get_log_density_of_states_extended();
    
    // Find the bin with the extended energy and shift it to log(number of reference states)
    ExtendedEnergyType reference_energy(this->get_config_space()->get_reference_configurations().begin()->first, 1);
    double shift_factor = log(this->get_config_space()->count_reference_configurations()) - extended_log_dos[reference_energy];
    extended_log_dos += shift_factor;
    
    // Create the result histogram using only the non-reference bins. Then add the right number to the non-reference bin
    HistoType<EnergyType, double> result;
    for (typename ExtendedHistoType::const_iterator it_extended = extended_log_dos.begin();
    	 it_extended != extended_log_dos.end(); ++it_extended)
    {
      if (it_extended->first.get_in_ground_state() == 0)
	//	result[it_extended->first.get_original_energy()] = it_extended->second;
	result.insert(std::pair<EnergyType, double>(it_extended->first.get_original_energy(), it_extended->second));
    }

    // Go through the reference energies and update the density of states
    for (typename ExtendedConfigurationType::reference_configurations_map_t::const_iterator energy_reference_configurations = this->get_config_space()->get_reference_configurations().begin();
	 energy_reference_configurations != this->get_config_space()->get_reference_configurations().end(); ++energy_reference_configurations)
    {
      const EnergyType& reference_energy = energy_reference_configurations->first;
      // Check whether the energy is allready contained in the result density of states
      if (result.find(reference_energy) != result.end())
	result[reference_energy] = log(exp(result[reference_energy]) + this->get_config_space()->count_reference_configurations(reference_energy));
      else
	result[reference_energy] = log(this->get_config_space()->count_reference_configurations(reference_energy));
    }
    
    // Return the result
    return result;
  }

  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator,
	    template<class,class,class,template<class,class> class,class> class AlgorithmType>
  double MulticanonicalCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, AlgorithmType>::number_of_states() const
  {
    // Get the density of states
    typedef HistoType<EnergyType, double> DosType;
    DosType dos = get_log_density_of_states();
    
    // Calculate the number of states (to-do: Check whether the density of states is normalised)
    double result = 0;
    for (typename DosType::const_iterator bin = dos.begin(); bin != dos.end(); ++bin)
    {
      result += exp(bin->second);
    }
    return result;
  }
}

#endif
