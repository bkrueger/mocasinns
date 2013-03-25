/**
 * \file histocrete.hpp
 * \brief HistoCrete = Histogram class storing discrete values, derived from HistoBase
 * 
 * The Histocrete is basically a wrapper for a std::map and is used for storing a histogram with discrete x values without binning.
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_HISTOGRAMS_HISTOCRETE_HPP
#define MOCASINNS_HISTOGRAMS_HISTOCRETE_HPP

#include "histobase.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

namespace Mocasinns
{
namespace Histograms
{

template <class x_value_type, class y_value_type> 
class Histocrete : public HistoBase<x_value_type, y_value_type, Histocrete<x_value_type, y_value_type> >
{
private:
  // Serialization stuff
  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    // serialize base class information
    ar & boost::serialization::base_object<Base>(*this);
  }

public:
  // Typedef for base class
  typedef HistoBase<x_value_type, y_value_type, Histocrete<x_value_type, y_value_type> > Base;
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

  //! Standard constructor
  Histocrete() { }
  //! Copy constructor
  Histocrete(const Base& other) : Base(other) {}

  // Operators
  //! Increment the y-value of the given Histocrete bin by one
  void operator<< (const x_value_type & bin) { this->values[bin] += 1; }
  //! Increment the y-value of the given Histocrete bin by the given y-value
  void operator<< (const value_type & xy_pair) { this->values[xy_pair.first] += xy_pair.second; }
  //! Value of the Histocrete at given bin
  y_value_type& operator[] (const x_value_type & bin) { return this->values[bin]; }
  //! Value of the Histocrete at given bin
  const y_value_type& operator[] (const x_value_type & bin) const { return this->values[bin]; }

  //! Adds a given value to all bins of this Histocrete
  Histocrete<x_value_type, y_value_type>& operator+= (const y_value_type& scalar) { return Base::operator+=(scalar); }
  //! Substracts a given value from all bins of this Histocrete
  Histocrete<x_value_type, y_value_type>& operator-= (const y_value_type& scalar) { return Base::operator-=(scalar); }
  //! Multiplies a given value with all bins of this Histocrete
  Histocrete<x_value_type, y_value_type>& operator*= (const y_value_type& scalar) { return Base::operator*=(scalar); }
  //! Devides this Histocrete binwise through a given value
  Histocrete<x_value_type, y_value_type>& operator/= (const y_value_type& scalar) { return Base::operator/=(scalar); }

  //! Adds a given HistoBase to this Histocrete
  template<class ArbitraryDerived>
  Histocrete<x_value_type, y_value_type>& operator+=(const HistoBase<x_value_type, y_value_type, ArbitraryDerived>& rhs) { return Base::operator+=(rhs); }
  //! Substracts a given HistoBase from this Histocrete
  template<class ArbitraryDerived>
  Histocrete<x_value_type, y_value_type>& operator-=(const HistoBase<x_value_type, y_value_type, ArbitraryDerived>& rhs) { return Base::operator-=(rhs); }
  //! Multiplies this Histocrete with given HistoBase
  template<class ArbitraryDerived>
  Histocrete<x_value_type, y_value_type>& operator*=(const HistoBase<x_value_type, y_value_type, ArbitraryDerived>& rhs) { return Base::operator*=(rhs); }
  //! Divides this Histocrete by given HistoBase
  template<class ArbitraryDerived>
  Histocrete<x_value_type, y_value_type>& operator/=(const HistoBase<x_value_type, y_value_type, ArbitraryDerived>& rhs) { return Base::operator/=(rhs); }

  //! Initialise the Histocrete with all necessary data of another Histocrete, but sets all y-values to 0
  template <class other_y_value_type>
  void initialise_empty(const Histocrete<x_value_type, other_y_value_type>& other);

  //! Insert element
  std::pair<iterator, bool> insert(const value_type& x) { return this->values.insert(x); }
  //! Insert element
  iterator insert(iterator position, const value_type& x) { return this->values.insert(position, x); }
  //! Insert elements
  template <class InputIterator> void insert(InputIterator first, InputIterator last) { this->values.insert(first, last); }
};

} // of namespace Histograms
} // of namespace Mocasinns

#include "../src/histograms/histocrete.cpp"

#endif
