/**
 * \file binnings.hpp
 * \brief File defining several examples for binning functors that can be used as a template argument of the class Histogram.
 * 
 * There are two possible binnings defined in this file: The binning of a numerical value and the binning of a vector of the same numerical value.
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_HISTOGRAMS_BINNINGS_HPP
#define MOCASINNS_HISTOGRAMS_BINNINGS_HPP

// Header for the serialization of the class
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

namespace Mocasinns
{
namespace Histograms
{

//! Class for binning a single numerical value 
template<class T> class BinningNumber
{
private:
  //! Bin-width
  T binning_width;
  //! Zero value of the binning
  T binning_reference;

  // Serialization stuff
  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    ar & binning_width;
    ar & binning_reference;
  }
  
public:
  //! Standard constructor
  BinningNumber() : binning_width(1), binning_reference(0) {}
  //! Constructor giving the bin width
  BinningNumber(T new_binning_width) : binning_width(new_binning_width), binning_reference(0) { }
  //! Constructor giving the bin width and the base value
  BinningNumber(T new_binning_width, T new_binning_reference) : binning_width(new_binning_width), binning_reference(new_binning_reference) { }

  //! Get-accessor for the bin widht
  T get_binning_width() {return binning_width;}
  //! Get-accessor for the reference of the binning
  T get_binning_reference() {return binning_reference;}

  //! Functor for binning
  T operator()(const T& value)
  {
    return binning_reference + binning_width*(T)(floor((value - binning_reference) / static_cast<double>(binning_width)));
  }
};

//! Class for binning a vector of numerical values
template<class T> class BinningNumberVector
{
private:
  //! Bin-width
  std::vector<T> bin_widths;
  //! Zero value of the binning
  std::vector<T> base_values;

  // Serialization stuff
  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    ar & bin_widths;
    ar & base_values;
  }

  
public:
  //! Constructor giving a single bin width for all numbers
  BinningNumberVector(unsigned int dimension, T bin_width) 
    : bin_widths(dimension, bin_width), base_values(dimension, 0) {}
  //! Constructor giving a single bin width and a single base value for all numbers
  BinningNumberVector(unsigned int dimension, T bin_width, T base_value) 
    : bin_widths(dimension, bin_width), base_values(dimension, base_value) {}
  //! Constructor giving different bin widths and base values for each dimension
  BinningNumberVector(const std::vector<T>& new_bin_widths) 
    : bin_widths(new_bin_widths), base_values(bin_widths.size(), 0) {}
  //! Constructor giving different bin widths and base values for each dimension
  BinningNumberVector(const std::vector<T>& new_bin_widths, const std::vector<T>& new_base_values) 
    : bin_widths(new_bin_widths), base_values(new_base_values) {}

  //! Functor for binning
  std::vector<T> operator()(const std::vector<T>& value)
  {
    std::vector<T> result(bin_widths.size(), 0);
    for (unsigned int i = 0; i < result.size(); i++)
      result[i] = base_values[i] + bin_widths[i]*(T)(floor((value[i] - base_values[i]) / static_cast<double>(bin_widths[i])));
    return result;
  }
};

} // of namespace Histograms
} // of namespace Mocasinns

#endif
