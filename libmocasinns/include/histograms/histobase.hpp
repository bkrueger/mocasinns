/**
 * \file histobase.hpp
 * \brief HistoBase = Base class for all types of histograms used in the Wang-Landau algorithm
 * 
 * The HistoBase is basically a wrapper for a std::map and is used for storing a histogram.
 * 
 * \author Benedikt Krüger
 */

#ifndef MOCASINNS_HISTOGRAMS_HISTOBASE_HPP
#define MOCASINNS_HISTOGRAMS_HISTOBASE_HPP

#include <map>

// Header for the serialization of the class
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>

namespace Mocasinns
{
namespace Histograms
{

//! Class for a histogram used in the Wang-Landau-Algorithm
template <class x_value_type, class y_value_type> class HistoBase
{
protected:
  // Private typedefs of the class  
  typedef std::map<x_value_type, y_value_type> histobase_container;

  //! Values of the histogram
  histobase_container values;

  // Serialization stuff
  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    ar & values;
  }

public:
  // Typedefs for iterator
  typedef typename histobase_container::iterator iterator;
  typedef typename histobase_container::const_iterator const_iterator;
  typedef typename histobase_container::reverse_iterator reverse_iterator;
  typedef typename histobase_container::const_reverse_iterator const_reverse_iterator;
  typedef typename histobase_container::value_type value_type;
  typedef typename histobase_container::size_type size_type;

  //! Standard constructor, initialises a HistoBase without values
  HistoBase() { }

  //! Operator for testing equality
  virtual bool operator==(const HistoBase<x_value_type,y_value_type>& rhs) const;
  //! Operator for testing inequality
  virtual bool operator!=(const HistoBase<x_value_type,y_value_type>& rhs) const;
  
  //! Return iterator to beginning
  iterator begin() { return values.begin(); }
  //! Return const_iterator to beginning
  const_iterator begin() const { return values.begin(); }

  //! Virtual function for binning a value
  virtual x_value_type bin_value(x_value_type value) { return value; }

  //! Delete all x- and y-values of the histogram
  void clear() { values.clear(); }

  //! Count elements with a specific x-value
  size_type count(const x_value_type& x) const { return values.count(x); }

  //! Calculate the derivative at given x_value_type
  double derivative(const_iterator x) const;
  
  //! Test whether container is empty
  bool empty() const { return values.empty(); }

  //! Return iterator to end
  iterator end() { return values.end(); }
  //! Return const_iterator to end
  const_iterator end() const { return values.end(); }

  //! Erase one element given by the iterator position
  void erase(iterator position) { values.erase(position); }
  //! Erase one element given by the x-value
  //! \return Number of elements that are erased (0 or 1, because the Histogram is unique)
  size_type erase(const x_value_type& x) { return values.erase(x); }
  //! Erase elements in the range between the given iterators
  void erase(iterator first, iterator last) { values.erase(first, last); }

  //! Check whether a xvalue exists
  bool exists(const x_value_type& bin) const { return values.find(bin) != values.end(); }

  //! Get iterator to element
  iterator find(const x_value_type& bin) { return values.find(bin); }
  //! Get iterator to element
  const_iterator find(const x_value_type& bin) const { return values.find(bin); }

  //! Calculates the flatness of the histogram
  double flatness() const;

  //! Initialise the HistoBase with all necessary data of another HistoBase, but sets all y-values to 0
  template <class other_y_value_type>
  void initialise_empty(const HistoBase<x_value_type, other_y_value_type>& other);

  //! Maximal size of the HistoBase given trough implementation
  size_type max_size() const { return values.max_size(); }
  
  //! Return the iterator to the maximal x-value
  const_iterator max_x_value() const { return --end(); }
  //! Return the iterator to the maximal y-value
  const_iterator max_y_value() const;
  //! Return the iterator to the minimal x-value
  const_iterator min_x_value() const { return begin(); }
  //! Return the iterator to the minimal y-value
  const_iterator min_y_value() const;

  //! Print the HistoBase into the console
  void print() const;

  //! Return reverse iterator to reverse beginning
  reverse_iterator rbegin() { return values.rbegin(); }
  //! Return reverse iterator to reverse beginning
  const_reverse_iterator rbegin() const { return values.rbegin(); }
  //! Return reverse iterator to reverse end
  reverse_iterator rend() { return values.rend(); }
  //! Return reverse iterator to reverse end
  const_reverse_iterator rend() const { return values.rend(); }

  //! Set all y-values to the given constant value
  void set_all_y_values(const y_value_type& const_value);

  //! Shifts the values of the histogram by substracting the y-value of the given iterator position
  //! After the shift the bin y-value of the given iterator is zero
  void shift_bin_zero(const_iterator it);

  //! Returns container size
  size_type size() const { return values.size(); }
  
  //! Returns the sum of the histocrete entries
  y_value_type sum() const;

  //! Load the data of the histocrete from a serialization stream
  virtual void load_serialize(std::istream& input_stream);
  //! Load the data of the histocrete from a serialization file
  virtual void load_serialize(const char* filename);
  //! Save the data of the histocrete to a serialization stream
  virtual void save_serialize(std::ostream& output_stream) const;
  //! Save the data of the histocrete to a serialization file
  virtual void save_serialize(const char* filename) const;
  //! Load the data of the histocrete from a csv stream
  virtual void load_csv(std::istream& input_stream);
  //! Load the data of the histocrete from a csv file
  virtual void load_csv(const char* filename);
  //! Save the data of the histocrete to a csv file
  virtual void save_csv(std::ostream& output_stream) const;
  //! Save the data of the histocrete to a csv file
  virtual void save_csv(const char* filename) const;
};

} // of namespace Histograms
} // of namespace Mocasinns

// Include implementation file because this is a template class
#include "histobase.cpp"

#endif
