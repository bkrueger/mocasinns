#ifndef MOCASINNS_HISTOGRAMS_CONSTANT_WIDTH_BINNING_HPP
#define MOCASINNS_HISTOGRAMS_CONSTANT_WIDTH_BINNING_HPP

#include <boost/serialization/serialization.hpp>

// Energy types for specialization
#include "../energy_types/vector_energy.hpp"
#include "../energy_types/array_energy.hpp"
#include "../energy_types/pair_energy.hpp"

namespace Mocasinns
{
  namespace Histograms
  {
    //! Common base class for all ConstantWidthBinning classes.
    //! \details This class cannot be used as a binning functor, because it has no binning operator.
    template <class T> 
    class ConstantWidthBinningBase
    {
    public:
      //! Constructor giving the bin width and the binning reference
      ConstantWidthBinningBase(T new_binning_width, T new_binning_reference) : binning_width(new_binning_width), binning_reference(new_binning_reference) {}

      //! Get-accessor for the bin width
      T get_binning_width() const {return binning_width;}
      //! Set-accessor for the bin width
      void set_binning_width(T value) { binning_width = value; }
      //! Get-accessor for the reference point of the binning
      T get_binning_reference() const {return binning_reference;}
      //! Set-accessor for the reference point of the binning
      void set_binning_reference(T value) { binning_reference = value; }
      
    protected:
      T binning_width;
      T binning_reference;

    private:
      friend class boost::serialization::access;
      //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
      template<class Archive> void serialize(Archive & ar, const unsigned int)
      {
	ar & binning_width;
	ar & binning_reference;
      }

      //! Private default constructor to prohibit default construction
      ConstantWidthBinningBase();
    };

    //! Class for binning with constant binwidth
    /*!
     * \details Binning with constant binwidth needs as arguments the width of the bin \f$ \Delta b \f$ and a reference point \f$ b_0 \f$. 
     * A value \f$ x \f$ is binned into \f$ b_i := b_0 + i\cdot \Delta b \f$ if \f$ b_i \leq x < b_{i+1}\f$.
     * 
     * There are specializations of this binning for all energy types defined in the <tt>mocasinns</tt>-library, so that one can bin the values component-wise for VectorEnergy, ArrayEnergy or PairEnergy.
     *
     * \tparam T Type of the values to bin
     */
    template <class T> 
    class ConstantWidthBinning : public ConstantWidthBinningBase<T>
    {
    public:
      typedef ConstantWidthBinningBase<T> Base;

      //! Standard constructor, sets the reference point to 0 and the width to 1
      ConstantWidthBinning() : Base(1, 0) {}
      //! Constructor giving the bin width, setting the reference point to 0
      ConstantWidthBinning(T new_binning_width) : Base(new_binning_width, 0) {}
      //! Constructor giving the bin width and the binning reference
      ConstantWidthBinning(T new_binning_width, T new_binning_reference) : Base(new_binning_width, new_binning_reference) {}

      //! Functor for binning
      T operator()(const T& value)
      {
	return this->binning_reference + this->binning_width*(T)(floor((value - this->binning_reference) / static_cast<double>(this->binning_width)));
      }
    };

    //! \cond
    template <class T>
    class ConstantWidthBinning<EnergyTypes::VectorEnergy<T> > : public ConstantWidthBinningBase<EnergyTypes::VectorEnergy<T> >
    {
    public:
      typedef EnergyTypes::VectorEnergy<T> BinType;
      typedef ConstantWidthBinningBase<BinType> Base;

      //! Constructor setting bin width 1 and reference 0 in all given dimensions
      ConstantWidthBinning(unsigned int dimension) 
	: Base(BinType(dimension, 1), BinType(dimension, 0)) {}
      //! Constructor setting the given bin width and reference 0 in all given dimensions
      ConstantWidthBinning(unsigned int dimension, T bin_width) 
	: Base(BinType(dimension, bin_width), BinType(dimension, 0)) {}
      //! Constructor setting the given bin width and reference in all given dimensions
      ConstantWidthBinning(unsigned int dimension, T bin_width, T bin_reference) 
	: Base(BinType(dimension, bin_width), BinType(dimension, bin_reference)) {}
      //! Constructor setting individual bin width and reference 0 in all dimensions
      ConstantWidthBinning(const BinType& bin_widthes)
	: Base(bin_widthes, BinType(bin_widthes.size(), 0)) {}
      //! Constructor setting individual bin width and references
      ConstantWidthBinning(const BinType& bin_widthes, const BinType& bin_references)
	: Base(bin_widthes, bin_references) {}

      //! Functor for binning
      BinType operator()(const BinType& value)
      {
	BinType result(this->binning_width.size(), 0);
	for (unsigned int i = 0; i < result.size(); i++)
	  result[i] = this->binning_reference[i] + this->binning_width[i]*(T)(floor((value[i] - this->binning_reference[i]) / static_cast<double>(this->binning_width[i])));
	return result;
      }
    };
    
    template <class T, unsigned int N>
    class ConstantWidthBinning<EnergyTypes::ArrayEnergy<T,N> > : public ConstantWidthBinningBase<EnergyTypes::ArrayEnergy<T,N> >
    {
    public:
      typedef EnergyTypes::ArrayEnergy<T,N> BinType;
      typedef ConstantWidthBinningBase<BinType> Base;

      //! Constructor setting bin width 1 and reference 0 in all given dimensions
      ConstantWidthBinning() 
	: Base(BinType(1), BinType(0)) {}
      //! Constructor setting the given bin width and reference 0 in all given dimensions
      ConstantWidthBinning(T bin_width) 
	: Base(BinType(bin_width), BinType(0)) {}
      //! Constructor setting the given bin width and reference in all given dimensions
      ConstantWidthBinning(T bin_width, T bin_reference) 
	: Base(BinType(bin_width), BinType(bin_reference)) {}
      //! Constructor setting individual bin width and reference 0 in all dimensions
      ConstantWidthBinning(const BinType& bin_widthes)
	: Base(bin_widthes, BinType(0)) {}
      //! Constructor setting individual bin width and references
      ConstantWidthBinning(const BinType& bin_widthes, const BinType& bin_references)
	: Base(bin_widthes, bin_references) {}

      //! Functor for binning
      BinType operator()(const BinType& value)
      {
	BinType result;
	for (unsigned int i = 0; i < N; i++)
	  result[i] = this->binning_reference[i] 
	    + this->binning_width[i]*(T)(floor((value[i] - this->binning_reference[i]) / static_cast<double>(this->binning_width[i])));
	return result;
      }
    };

    template <class T1, class T2>
    class ConstantWidthBinning<EnergyTypes::PairEnergy<T1,T2> > : public ConstantWidthBinningBase<EnergyTypes::PairEnergy<T1,T2> >
    {
      typedef EnergyTypes::PairEnergy<T1,T2> BinType;
      typedef ConstantWidthBinningBase<BinType> Base;

      //! Constructor setting bin width 1 and reference 0 in first and second component
      ConstantWidthBinning() 
	: Base(BinType(1,1), BinType(0,0)) {}
      //! Constructor setting individual bin width and reference 0 in first and second component
      ConstantWidthBinning(const BinType& bin_widthes)
	: Base(bin_widthes, BinType(0,0)) {}
      //! Constructor setting individual bin width and references inf first and second component
      ConstantWidthBinning(const BinType& bin_widthes, const BinType& bin_references)
	: Base(bin_widthes, bin_references) {}

      //! Functor for binning
      BinType operator()(const BinType& value)
      {
	BinType result;
	result.first = this->binning_reference.first 
	  + this->binning_width.first*(T1)(floor((value.first - this->binning_reference.first) / static_cast<double>(this->binning_width.first)));
	result.second = this->binning_reference.second 
	  + this->binning_width.second*(T1)(floor((value.second - this->binning_reference.second) / static_cast<double>(this->binning_width.second)));
	return result;
      }
    };
    //! \endcond
  }
}

#endif
