#ifndef MOCASINNS_ANALYSIS_DOS_FUNCTORS_HPP
#define MOCASINNS_ANALYSIS_DOS_FUNCTORS_HPP

namespace Mocasinns
{
  namespace Analysis
  {
    template <class DosType, unsigned int moment>
    struct EnergyMoment
    {
      typedef DosType::key_type EnergyType;
      typedef DosType::mapped_type DosValueType;
      typedef EnergyType return_type;
      
      return_type operator()(const DosType& dos, const EnergyType& beta)
      {
	// Define variables storing the partition function and the unnormalized sum
	double partition_function = 0.0;
	EnergyType sum = 0.0;
	
	// Fill the partition function and the unnormalized sum
	for (typename DosType::const_iterator it = dos.begin(); it != dos.end(); ++dos)
	{
	  partition_function += it->second * exp(-beta*it->first);
	  sum += pow(it->first, moment) * it->second * exp(-beta*it->first);
	}

	// Calculate and return the result
	return sum / partition_function;
      }
    };

    template <class DosType>
    struct EnergyMoment1 : public EnergyMoment<DosType, 1>
    {
      typedef EnergyMoment<DosType,1>::return_type return_type;
    };
    template <class DosType>
    struct EnergyMoment2 : public EnergyMoment<DosType, 2>
    {
      typedef EnergyMoment<DosType,2>::return_type return_type;
    };
    template <class DosType>
    struct EnergyMoment3 : public EnergyMoment<DosType, 3>
    {
      typedef EnergyMoment<DosType,3>::return_type return_type;
    };
    template <class DosType>
    struct EnergyMoment4 : public EnergyMoment<DosType, 4>
    {
      typedef EnergyMoment<DosType,4>::return_type return_type;
    };

    template <class DosType>
    struct EnergyMean : public EnergyMoment<DosType,1>
    {
      typedef typename EnergyMoment<DosType,1>::return_type return_type;
    };

    template <class DosType>
    struct EnergyVariance
    {
      typedef typename DosType::key_type EnergyType;
      typedef EnergyType return_type;

      return_type operator()(const DosType& dos, const EnergyType beta)
      {
	EnergyMoment<DosType, 1> dos_functor_moment_1;
	EnergyMoment<DosType, 2> dos_functor_moment_2;

	return dos_functor_moment_2(dos, beta) - pow(dos_functor_moment_1(dos, beta), 2);
      }
    };

    template <class DosType> 
    struct PartitionFunction
    {
      typedef DosType::key_type EnergyType;
      typedef DosType::mapped_type DosValueType;
      typedef EnergyType return_type;
      
      return_type operator()(const DosType& dos, const EnergyType& beta)
      {
    	// Define variables storing the partition function and the unnormalized sum
	double partition_function = 0.0;
	
	// Fill the partition function and the unnormalized sum
	for (typename DosType::const_iterator it = dos.begin(); it != dos.end(); ++dos)
	  partition_function += it->second * exp(-beta*it->first);

	// Calculate and return the result
	return partition_function;
      }
    };

  }
}

#endif
