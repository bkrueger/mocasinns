/**
 * \file histogram_number.hpp
 * \brief Histogram = Histogram class storing binned values, derived from HistoBase. The binner ist for double values
 * 
 * The Histogram is basically a wrapper for a std::map and is used for storing a histogram with binned x values.
 * 
 * \author Benedikt Krüger
 */

#ifndef HISTOGRAM_NUMBER_HPP
#define HISTOGRAM_NUMBER_HPP

#include "histobase.hpp"
#include "binnings.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

//! Class for a binned histogram
template <class x_value_type, class y_value_type> 
class HistogramNumber : public HistoBase<x_value_type, y_value_type>
{
private:
  BinningNumber<x_value_type> binning;

  // Serialization stuff
  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    // serialize base class information
    ar & boost::serialization::base_object<HistoBase<x_value_type, y_value_type> >(*this);
    ar & binning;
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

  //! Constructor setting standard binning values
  HistogramNumber() : binning(1.0, 0.0) {}
  //! Constructor taking a binning values
  /*!
   * \param binning_functor Class that maps a arbitrary x-value to another x-value that corresponds to a specific binning encoded in this class using the binning_functor::operator()
   */
  HistogramNumber(x_value_type bin_width, x_value_type base_value) : binning(bin_width, base_value) {}

  //! Get-accessor for the width of the binning
  virtual x_value_type get_binning_width() { return binning.get_binning_width(); }
  //! Get-accessor for the reference of the binning
  virtual x_value_type get_binning_reference() { return binning.get_binning_reference(); }
  //! Set accessor to reset the binning, does not affect the actual values
  //! \bug Need to affect the actual values
  virtual void set_binning(x_value_type width, x_value_type reference)
  {
    binning = BinningNumber<x_value_type>(width, reference);
  }

  // Operators
  //! Increment the y-value of the given Histogram bin by one
  void operator<< (const x_value_type & bin) { this->values[binning(bin)] += 1; }
  //! Increment the y-value of the given Histocrete bin by the given y-value
  void operator<< (const value_type & xy_pair) { this->values[binning(xy_pair.first)] += xy_pair.second; }
  //! Value of the Histogram at given bin, takes binning into account
  y_value_type& operator[] (const x_value_type & bin) { return this->values[binning(bin)]; }
  //! Value of the Histogram at given bin, takes binning into account
  //const y_value_type& operator[] (const x_value_type & bin) const { return this->values[binning(bin)]; }
  //! Adds a given HistoBase (using the binning function) to this Histogram
  void operator+= (HistoBase<x_value_type, y_value_type>& other_histobase);
  //! Adds a given value to all bins of this Histogram
  void operator+= (const y_value_type& const_value);
  //! Devides this Histogram binwise through a given HistoBase
  void operator/= (HistoBase<x_value_type, y_value_type>& other_histobase);
  //! Devides this Histogram binwise through a given value
  void operator/= (const y_value_type& const_value); 

  //! Bin a value
  virtual x_value_type bin_value(x_value_type value) { return binning(value); }

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

    //! Load the data of the histocrete from a serialization stream
  virtual void load_serialize(std::istream& input_stream);
  //! Load the data of the histocrete from a serialization file
  virtual void load_serialize(const char* filename);
  //! Save the data of the histocrete to a serialization stream
  virtual void save_serialize(std::ostream& output_stream) const;
  //! Save the data of the histocrete to a serialization file
  virtual void save_serialize(const char* filename) const;
};

#include "histogram_number.cpp"

#endif
