#ifndef MOCASINNS_DETAILS_MULTICANONICAL_PARAMETERS_MULTICANONICAL_HPP
#define MOCASINNS_DETAILS_MULTICANONICAL_PARAMETERS_MULTICANONICAL_HPP

namespace Mocasinns
{
  namespace Details
  {
   namespace Multicanonical
   {
     template <class EnergyType>
     struct DefaultEnergyTypeComparator
     {
     public:
       int operator()(const EnergyType& energy_1, const EnergyType& energy_2)
       {
	 if (energy_1 == energy_2) 
	   return 0;
	 else if (energy_1 < energy_2)
	   return -1;
	 else 
	   return 1;
       }
     };

     template <class EnergyType, class LowerComparator = std::greater_equal<EnergyType>, class UpperComparator = std::less_equal<EnergyType> >
     class ParametersMulticanonical
     {
     public:
       //! Energy value that is used as a reference point for the binning
       EnergyType binning_reference;
       //! Energy value range that is comprehended in one bin
       EnergyType binning_width;
       
       //! Value of the minimal energy cutoff
       EnergyType energy_cutoff_lower;
       //! Value of the maximal energy cutoff
       EnergyType energy_cutoff_upper;
       //! Flag indicating whether to use the minimal energy cutoff, default value is false
       bool use_energy_cutoff_lower;
       //! Flag indicating whether to use the maximal energy cutoff, default value is false
       bool use_energy_cutoff_upper;

       //! Comparator object for comparing with the lower energy cutoff
       LowerComparator lower_comparator;
       //! Comparator object for comparing with the upper energy_cutoff
       UpperComparator upper_comparator;

       //! Comparator object
       //       EnergyTypeComparator energy_type_comparator;

       //! Constructor to set default values
       ParametersMulticanonical() :
	 binning_reference(0), 
	 binning_width(1), 
	 energy_cutoff_lower(0),
	 energy_cutoff_upper(0),
	 use_energy_cutoff_lower(false),
	 use_energy_cutoff_upper(false),
	 lower_comparator(LowerComparator()),
	 upper_comparator(UpperComparator()) {}

       //! Constructor to copy parameters from other parameters with convertible energy type
       template <class EnergyTypeOther>
       explicit ParametersMulticanonical(const ParametersMulticanonical<EnergyTypeOther>& other) :
	 binning_reference(other.binning_reference), 
	 binning_width(other.binning_width), 
	 energy_cutoff_lower(other.energy_cutoff_lower),
	 energy_cutoff_upper(other.energy_cutoff_upper),
	 use_energy_cutoff_lower(other.use_energy_cutoff_lower),
	 use_energy_cutoff_upper(other.use_energy_cutoff_upper),
	 lower_comparator(other.lower_comparator),
	 upper_comparator(other.upper_comparator) { }

       //! Test for equality
       bool operator==(const ParametersMulticanonical<EnergyType>& rhs) const
       {
	 return ((binning_reference == rhs.binning_reference) &&
		 (binning_width == rhs.binning_width) &&
		 (energy_cutoff_lower == rhs.energy_cutoff_lower) && 
		 (energy_cutoff_upper == rhs.energy_cutoff_upper) &
		 (use_energy_cutoff_lower == rhs.use_energy_cutoff_lower) && 
		 (use_energy_cutoff_upper == rhs.use_energy_cutoff_upper));
       }
       //! Test for inequality
       bool operator!=(const ParametersMulticanonical<EnergyType>& rhs) const { return !operator==(rhs); }
       
       //! Test whether a given energy is in the range given by this parameter
       bool energy_in_range(const EnergyType& energy) const
       {
	 // Check the upper cutoff
	 if (use_energy_cutoff_upper && !upper_comparator(energy, energy_cutoff_upper))
	   return false;
	 // Check the lower cutoff
	 if (use_energy_cutoff_lower && !lower_comparator(energy, energy_cutoff_lower))
	   return false;
	 // No cutoff violated
	 return true;
       }
       
     private:
       //! Member variable for boost serialization
       friend class boost::serialization::access;
       //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
       template<class Archive> void serialize(Archive & ar, const unsigned int)
       {
	 // serialize base class information
	 ar & binning_reference;
	 ar & binning_width;
	 ar & energy_cutoff_lower;
	 ar & energy_cutoff_upper;
	 ar & use_energy_cutoff_lower;
	 ar & use_energy_cutoff_upper;
       }  
     };
   } 
  }
}

#endif
