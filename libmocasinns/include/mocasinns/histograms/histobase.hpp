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

/*! 
 \brief Base class for all histograms used in Mocasinns

 \details This base class provides nearly all functionality for histograms and has an interface similiar to std::map (plus additional functionality). The derived classes then provide how to accumulate data into the histogram (i.e. to which bin a new value has to be assigned, or which additional parameters have to be altered).
 
 The derived class has to be given as a template parameter and has to implement the insert-functionality and an operator[]. The derived class will be used to specify the return values of operators and to access the insert- and operator[]-functionality of the derived class to be able to have general algorithms just written once in the base class.

 \tparam x_value_type Type of the x-values of the histogram
 \tparam y_value_type Type of the y-values of the histogram
 \tparam Derived Class that will be derived of this Histobase (must implement an insert-functionality and an operator[])

 \todo Implement specialised functions for output (save_csv) and input (load_csv) of vectors, arrays, ... as x-values
*/
template <class x_value_type, class y_value_type, class Derived> class HistoBase
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
  //! Exception class for arithmetic operations with not-compatible histograms
  class ExceptionNonCompatible;
  
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
  template <class ArbitraryDerived>
  bool operator==(const HistoBase<x_value_type,y_value_type,ArbitraryDerived>& rhs) const;
  //! Operator for testing inequality
  template <class ArbitraryDerived>
  bool operator!=(const HistoBase<x_value_type,y_value_type,ArbitraryDerived>& rhs) const;

  //! Operator for adding a scalar to every bin of this HistoBase
  Derived& operator+=(const y_value_type& scalar);
  //! Operator for substracting a scalar of every bin of this HistoBase
  Derived& operator-=(const y_value_type& scalar);
  //! Operator for multiplying every bin of this HistoBase with a scalar
  Derived& operator*=(const y_value_type& scalar);
  //! Operator for dividing every bin of this HistoBase by a scalar
  Derived& operator/=(const y_value_type& scalar);

  //! Operator for adding another HistoBase to this one, using the insert functionality of the derived histogram
  template <class ArbitraryDerived>
  Derived& operator+=(const HistoBase<x_value_type,y_value_type,ArbitraryDerived>& rhs);
  //! Operator for substracting another HistoBase from this one, using the insert functionality of the derived histogram
  template <class ArbitraryDerived>
  Derived& operator-=(const HistoBase<x_value_type,y_value_type,ArbitraryDerived>& rhs);
  //! Operator for multiplying the y-values of this HistoBase with the y-values of another compatible HistoBase
  template <class ArbitraryDerived>
  Derived& operator*=(const HistoBase<x_value_type,y_value_type,ArbitraryDerived>& rhs);
  //! Operator for dividing the y-values of this Histobase by the y-values of another compatible HistoBase
  template <class ArbitraryDerived>
  Derived& operator/=(const HistoBase<x_value_type,y_value_type,ArbitraryDerived>& rhs);

  //! Return iterator to beginning
  iterator begin() { return values.begin(); }
  //! Return const_iterator to beginning
  const_iterator begin() const { return values.begin(); }

  //! Virtual function for binning a value
  virtual x_value_type bin_value(x_value_type value) { return value; }

  //! Delete all x- and y-values of the histogram
  void clear() { values.clear(); }

  //! Check whether this HistoBase and the HistoBase given as parameter have the same x-values
  template <class ArbitraryDerived>
  bool compatible(const HistoBase<x_value_type,y_value_type,ArbitraryDerived>& other) const;

  //! Count elements with a specific x-value
  size_type count(const x_value_type& x) const { return values.count(x); }
  //! Count elements with a specific y-value
  size_type count_y(const y_value_type& y) const
  {
    size_type result = 0;
    for (const_iterator it = begin(); it != end(); ++it)
      if (it->second == y) result++;
    return result;
  }

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
  template <class other_y_value_type, class ArbitraryDerived>
  void initialise_empty(const HistoBase<x_value_type, other_y_value_type, ArbitraryDerived>& other);

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
  void load_serialize(std::istream& input_stream);
  //! Load the data of the histocrete from a serialization file
  void load_serialize(const char* filename);
  //! Save the data of the histocrete to a serialization stream
  void save_serialize(std::ostream& output_stream) const;
  //! Save the data of the histocrete to a serialization file
  void save_serialize(const char* filename) const;
  //! Load the data of the histocrete from a csv stream
  void load_csv(std::istream& input_stream);
  //! Load the data of the histocrete from a csv file
  void load_csv(const char* filename);
  //! Save the data of the histocrete to a csv file
  void save_csv(std::ostream& output_stream) const;
  //! Save the data of the histocrete to a csv file
  void save_csv(const char* filename) const;
};

  template<class x_value_type, class y_value_type, class Derived>
  class HistoBase<x_value_type,y_value_type,Derived>::ExceptionNonCompatible : public std::out_of_range
  {
    ExceptionNonCompatible() : std::out_of_range("Arithmetic calculation with non-compatible histograms.") {}
  };

  //! Adds a scalar and a HistoBase
  template<class x_value_type, class y_value_type, class Derived>
  const Derived operator+(const Derived& lhs, const y_value_type& scalar);
  //! Adds a HistoBase and a scalar
  template<class x_value_type, class y_value_type, class Derived>
  const Derived operator+(const y_value_type& scalar, const Derived& rhs);
  //! Substract a scalar from a HistoBase
  template<class x_value_type, class y_value_type, class Derived>
  const Derived operator-(const Derived& lhs, const y_value_type& scalar);
  //! Multipliess a scalar and a HistoBase
  template<class x_value_type, class y_value_type, class Derived>
  const Derived operator*(const Derived& lhs, const y_value_type& scalar);
  //! Multiplies a HistoBase and a scalar
  template<class x_value_type, class y_value_type, class Derived>
  const Derived operator*(const y_value_type& scalar, const Derived& rhs);
  //! Divides a HistoBase through a scalar
  template<class x_value_type, class y_value_type, class Derived>
  const Derived operator/(const Derived& lhs, const y_value_type& scalar);

  //! Adds two histograms (the type of the left hand side determines the type of the result)
  template<class x_value_type, class y_value_type, class DerivedLeft, class DerivedRight>
  const DerivedLeft operator+(const HistoBase<x_value_type, y_value_type, DerivedLeft>& lhs, const HistoBase<x_value_type, y_value_type, DerivedRight>& rhs);
  //! Substracts two histograms (the type of the left hand side determines the type of the result)
  template<class x_value_type, class y_value_type, class DerivedLeft, class DerivedRight>
  const DerivedLeft operator-(const HistoBase<x_value_type, y_value_type, DerivedLeft>& lhs, const HistoBase<x_value_type, y_value_type, DerivedRight>& rhs);
  //! Multiplies two histograms
  template<class x_value_type, class y_value_type, class Derived>
  const Derived operator*(const HistoBase<x_value_type, y_value_type, Derived>& lhs, const HistoBase<x_value_type, y_value_type, Derived>& rhs);
  //! Divides two histograms
  template<class x_value_type, class y_value_type, class Derived>
  const Derived operator/(const HistoBase<x_value_type, y_value_type, Derived>& lhs, const HistoBase<x_value_type, y_value_type, Derived>& rhs);

  //! Writes a histogram to an output stream with format "x_value\ty_value\n"
  template<class x_value_type, class y_value_type, class Derived>
  std::ostream& operator<<(std::ostream& stream, const HistoBase<x_value_type, y_value_type, Derived>& rhs);

} // of namespace Histograms
} // of namespace Mocasinns

// Include implementation file because this is a template class
#include "histobase.cpp"

#endif
