#ifdef MOCASINNS_MULTICANONICAL_COUNTING_HPP

namespace Mocasinns
{
  template <class ConfigurationType, class StepType, class EnergyType, template<class,class> class HistoType, class RandomNumberGenerator,
	    template<class,class,class,template<class,class> class,class> class AlgorithmType>
  double MulticanonicalCounting<ConfigurationType, StepType, EnergyType, HistoType, RandomNumberGenerator, AlgorithmType>::number_of_states() const
  {
    // Get the density of states
    typedef HistoType<ExtendedEnergyType, double> DosType;
    DosType dos = this->get_density_of_states();
    
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
