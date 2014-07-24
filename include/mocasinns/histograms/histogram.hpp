/**
 * \file histogram.hpp
 * \brief Histogram = Histogram class storing binned values, derived from HistoBase
 * 
 * The Histogram is basically a wrapper for a std::map and is used for storing a histogram with binned x values.
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_HISTOGRAMS_HISTOGRAM_HPP
#define MOCASINNS_HISTOGRAMS_HISTOGRAM_HPP

#include "histobase.hpp"
#include "constant_width_binning.hpp"
#include "fixed_boundary_binning.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

namespace Mocasinns
{
namespace Histograms
{

//! Class for a binned histogram
  /*!
   * \details A Histogram bins the values given by the <tt>insert</tt> function or the <tt>operator[]</tt> functions using a functor that is given in the construction of the Histogram. 
   * The functor must define the member <tt>x_value_type operator(x_value_type)</tt> that maps a given x-value to the according bin.
   * 
   * In the <tt>mocasinns</tt> library there are two predefined binning functor: ConstantWidthBinning and FixedBoundaryBinning.
   * For these binning functors there are the templated typedefs HistogramConstantWidth and HistogramFixedBoundary that can be used with C++11 compliant compilers instead of giving the BinningFunctor template parameter directly.
   *
   * \tparam x_value_type Type of the x-values of the histogram
   * \tparam y_value_type Type of the y-values of the histogram
   * \tparam BinningFunctor Type of the binning functor, must define a member <tt>x_value_type operator(x_value_type)</tt> used for binning.
   */
template <class x_value_type, class y_value_type, class BinningFunctor> 
class Histogram : public HistoBase<x_value_type, y_value_type, Histogram<x_value_type, y_value_type, BinningFunctor> >
{
private:
  BinningFunctor binning;

  // Serialization stuff
  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    // serialize base class information
    ar & boost::serialization::base_object<Base>(*this);
    ar & binning;
  }

public:
  // Typedef for the base class
  typedef HistoBase<x_value_type, y_value_type, Histogram<x_value_type, y_value_type, BinningFunctor> > Base;
  // Typedefs for iterator
  // Necessary because this is a class template
  typedef typename Base::key_type key_type;
  typedef typename Base::mapped_type mapped_type;
  typedef typename Base::value_type value_type;
  typedef typename Base::key_compare key_compare;
  typedef typename Base::value_compare value_compare;
  typedef typename Base::allocator_type allocator_type;
  typedef typename Base::iterator iterator;
  typedef typename Base::const_iterator const_iterator;
  typedef typename Base::reverse_iterator reverse_iterator;
  typedef typename Base::const_reverse_iterator const_reverse_iterator;
  typedef typename Base::difference_type difference_type;
  typedef typename Base::reference reference;
  typedef typename Base::const_reference const_reference;
  typedef typename Base::pointer pointer;
  typedef typename Base::const_pointer const_pointer;
  typedef typename Base::size_type size_type;
  //! Typedef for the template parameter
  typedef BinningFunctor BinningFunctorType;

  //! Standard constructor
  Histogram() : binning() {}
  //! Constructor taking a binning functor
  Histogram(BinningFunctor binning_functor) : binning(binning_functor) {}
  //! Copy constructor
  Histogram(const Base& other) : Base(other), binning() {}

  //! Get-accessor for the binning functor
  const BinningFunctor& get_binning() const { return binning; }
  //! Set-accessor for the binning functor
  void set_binning(const BinningFunctor& value) { binning = value; }

  // Operators
  //! Increment the y-value of the given Histogram bin by one
  void operator<< (const x_value_type & bin) { this->values[binning(bin)] += 1; }
  //! Increment the y-value of the given Histocrete bin by the given y-value
  void operator<< (const value_type & xy_pair) { this->values[binning(xy_pair.first)] += xy_pair.second; }
  //! Value of the Histogram at given bin, takes binning into account
  y_value_type& operator[] (const x_value_type & bin) { return this->values[binning(bin)]; }
  //! Value of the Histogram at given bin, takes binning into account
  const y_value_type& operator[] (const x_value_type & bin) const { return this->values.find(binning(bin))->second; }

  //! Adds a given value to all bins of this Histogram
  Histogram<x_value_type, y_value_type, BinningFunctor>& operator+= (const y_value_type& scalar) { return Base::operator+=(scalar); }
  //! Substracts a given value from all bins of this Histogram
  Histogram<x_value_type, y_value_type, BinningFunctor>& operator-= (const y_value_type& scalar) { return Base::operator-=(scalar); }
  //! Multiplies a given value with all bins of this Histogram
  Histogram<x_value_type, y_value_type, BinningFunctor>& operator*= (const y_value_type& scalar) { return Base::operator*=(scalar); }
  //! Devides this Histogram binwise through a given value
  Histogram<x_value_type, y_value_type, BinningFunctor>& operator/= (const y_value_type& scalar) { return Base::operator/=(scalar); }

  //! Adds a given HistoBase to this Histogram
  template<class ArbitraryDerived>
  Histogram<x_value_type, y_value_type, BinningFunctor>& operator+=(const HistoBase<x_value_type, y_value_type, ArbitraryDerived>& rhs) { return Base::operator+=(rhs); }
  //! Substracts a given HistoBase from this Histogram
  template<class ArbitraryDerived>
  Histogram<x_value_type, y_value_type, BinningFunctor>& operator-=(const HistoBase<x_value_type, y_value_type, ArbitraryDerived>& rhs) { return Base::operator-=(rhs); }
  //! Multiplies this Histogram with given HistoBase
  template<class ArbitraryDerived>
  Histogram<x_value_type, y_value_type, BinningFunctor>& operator*=(const HistoBase<x_value_type, y_value_type, ArbitraryDerived>& rhs) { return Base::operator*=(rhs); }
  //! Divides this Histogram by given HistoBase
  template<class ArbitraryDerived>
  Histogram<x_value_type, y_value_type, BinningFunctor>& operator/=(const HistoBase<x_value_type, y_value_type, ArbitraryDerived>& rhs) { return Base::operator/=(rhs); }

  //! Bin a calue
  virtual x_value_type bin_value(x_value_type value) { return binning(value); }

  //! Initialise the Histogram with all necessary data of another Histogram, but sets all y-values to 0
  template <class other_y_value_type>
  void initialise_empty(const Histogram<x_value_type, other_y_value_type, BinningFunctor>& other);

  //! Insert element, take binning into account
  std::pair<iterator, bool> insert(const value_type& x) { return this->values.insert(value_type(binning(x.first), x.second)); }
  //! Insert element, take binning into account
  iterator insert(iterator position, const value_type& x) { return this->values.insert(position, value_type(binning(x.first), x.second)); }
  //! Insert elements, take binning into account
  template <class InputIterator> void insert(InputIterator first, InputIterator last) 
  { 
    for (InputIterator it = first; it != last; ++it)
      this->insert(*it); 
  }
};

  // Typedefs for histograms with special binnings
  // Only for simplicity of notion
  // Checks for C++11 support for template typedef
#ifndef BOOST_NO_CXX11_TEMPLATE_ALIASES
  template <class x_value_type, class y_value_type>
  using HistogramConstantWidth = Histogram<x_value_type, y_value_type, ConstantWidthBinning<x_value_type> >;

  template <class x_value_type, class y_value_type>
  using HistogramFixedBoundaries = Histogram<x_value_type, y_value_type, FixedBoundaryBinning<x_value_type> >;
#endif

} // of namespace Histogram
} // of namespace Mocasinns

#include "../src/histograms/histogram.cpp"

#endif
