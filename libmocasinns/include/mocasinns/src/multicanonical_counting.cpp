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

    // Get the reference energy
    ExtendedEnergyType reference_energy_reference_bin(this->get_config_space()->get_reference_configuration_energy(), 1);
    ExtendedEnergyType reference_energy_nonreference_bin(this->get_config_space()->get_reference_configuration_energy(), 0);

    // Create the result histogram setting all non-reference bins.
    // Correct the histogram by reducing the number of the reference-energy (non-reference) bin by 1
    // Include the reference bin with value 1
    this->log_density_of_states.clear();
    for (typename ReducedHistoType::const_iterator it_reduced = value.begin();
	 it_reduced != value.end(); ++it_reduced)
    {
      this->log_density_of_states[ExtendedEnergyType(it_reduced->first, 0)] = it_reduced->second;
    }
    this->log_density_of_states[reference_energy_reference_bin] = 0.0;
    this->log_density_of_states[reference_energy_nonreference_bin] = log(exp(this->log_density_of_states[reference_energy_nonreference_bin] - 1));
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
    
    // Find the bin with the extended energy and shift it to 0
    ExtendedEnergyType reference_energy(this->get_config_space()->get_reference_configuration_energy(), 1);
    extended_log_dos.shift_bin_zero(extended_log_dos.find(reference_energy));
    
    // Create the result histogram using only the non-reference bins. Then add the right number to the non-reference bin
    HistoType<EnergyType, double> result;
    for (typename ExtendedHistoType::const_iterator it_extended = extended_log_dos.begin();
    	 it_extended != extended_log_dos.end(); ++it_extended)
    {
      if (it_extended->first.get_in_ground_state() == 0)
	result[it_extended->first.get_original_energy()] = it_extended->second;
    }
    result[reference_energy.get_original_energy()] = log(exp(result[reference_energy.get_original_energy()]) + 1);

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
