#ifdef MOCASINNS_MULTICANONICAL_COUNTING_HPP

namespace Mocasinns
{
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
