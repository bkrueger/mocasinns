/**
 * \file histocrete.hpp
 * \brief HistoCrete = Histogram class storing discrete values, derived from HistoBase
 * 
 * The Histocrete is basically a wrapper for a std::map and is used for storing a histogram with discrete x values without binning.
 * 
 * \author Benedikt Krüger
 */

#ifndef HISTOCRETE_HPP
#define HISTOCRETE_HPP

#include "histobase.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

namespace Mocasinns
{
namespace Histograms
{

template <class x_value_type, class y_value_type> 
class Histocrete : public HistoBase<x_value_type, y_value_type>
{
private:
  // Serialization stuff
  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    // serialize base class information
    ar & boost::serialization::base_object<HistoBase<x_value_type, y_value_type> >(*this);
  }

public:
  // Typedefs for iterator
  // Necessary because this is a class template
  typedef typename HistoBase<x_value_type, y_value_type>::iterator iterator;
  typedef typename HistoBase<x_value_type, y_value_type>::const_iterator const_iterator;
  typedef typename HistoBase<x_value_type, y_value_type>::reverse_iterator reverse_iterator;
  typedef typename HistoBase<x_value_type, y_value_type>::const_reverse_iterator const_reverse_iterator;
  typedef typename HistoBase<x_value_type, y_value_type>::value_type value_type;
  typedef typename HistoBase<x_value_type, y_value_type>::size_type size_type;

  //! Standard constructor
  Histocrete() { }

  // Operators
  //! Increment the y-value of the given Histocrete bin by one
  void operator<< (const x_value_type & bin) { this->values[bin] += 1; }
  //! Increment the y-value of the given Histocrete bin by the given y-value
  void operator<< (const value_type & xy_pair) { this->values[xy_pair.first] += xy_pair.second; }
  //! Value of the Histocrete at given bin
  y_value_type& operator[] (const x_value_type & bin) { return this->values[bin]; }
  //! Value of the Histocrete at given bin
  const y_value_type& operator[] (const x_value_type & bin) const { return this->values[bin]; }
  //! Adds a given HistoBase to this Histocrete
  void operator+= (HistoBase<x_value_type, y_value_type>& other_histobase);
  //! Adds a given value to all bins of this Histocrete
  void operator+= (const y_value_type& const_value);
  //! Devides this Histocrete binwise through a given HistoBase
  void operator/= (HistoBase<x_value_type, y_value_type>& other_histobase);
  //! Devides this Histocrete binwise through a given value
  void operator/= (const y_value_type& const_value); 

  //! Insert element
  std::pair<iterator, bool> insert(const value_type& x) { return this->values.insert(x); }
  //! Insert element
  iterator insert(iterator position, const value_type& x) { return this->values.insert(position, x); }
  //! Insert elements
  template <class InputIterator> void insert(InputIterator first, InputIterator last) { this->values.insert(first, last); }

  //! Load the data of the histocrete from a serialization stream
  virtual void load_serialize(std::istream& input_stream);
  //! Load the data of the histocrete from a serialization file
  virtual void load_serialize(const char* filename);
  //! Save the data of the histocrete to a serialization stream
  virtual void save_serialize(std::ostream& output_stream) const;
  //! Save the data of the histocrete to a serialization file
  virtual void save_serialize(const char* filename) const;
};

} // of namespace Histograms
} // of namespace Mocasinns

#include "histocrete.cpp"

#endif
