/**
 * \file histobase.cpp
 * \brief Implementation of the HistoBase class
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */
#ifdef MOCASINNS_HISTOGRAMS_HISTOBASE_HPP

#include <cmath> // INFINITY and isnan()
#include <fstream>
#include <sstream>

#include "../../exceptions/histos_not_compatible_exception.hpp"

namespace Mocasinns
{
namespace Histograms
{

  template<class x_value_type, class y_value_type, class Derived>
  template<class other_x_value_type, class OtherDerived>
  HistoBase<x_value_type, y_value_type, Derived>::HistoBase(const HistoBase<other_x_value_type, y_value_type, OtherDerived>& other)
  {
    for (typename HistoBase<other_x_value_type, y_value_type, OtherDerived>::const_iterator bin = other.begin(); bin != other.end(); ++bin)
      values.insert(std::pair<x_value_type, y_value_type>(x_value_type(bin->first), bin->second));
  }

template<class x_value_type, class y_value_type, class Derived>
template<class ArbitraryDerived>
bool HistoBase<x_value_type,y_value_type,Derived>::operator==(const HistoBase<x_value_type,y_value_type,ArbitraryDerived>& rhs) const
{
  // Test that the sizes are equivalent
  if (size() != rhs.size()) return false;

  // Test each entry
  const_iterator rhs_it = rhs.begin();
  for (const_iterator this_it = begin(); this_it != end(); ++this_it)
  {
    if (this_it->first != rhs_it->first) return false;
    if (this_it->second != rhs_it->second) return false;
    ++rhs_it;
  }

  // Everything is ok
  return true;
}
template<class x_value_type, class y_value_type, class Derived>
template<class ArbitraryDerived>
bool HistoBase<x_value_type,y_value_type,Derived>::operator!=(const HistoBase<x_value_type,y_value_type,ArbitraryDerived>& rhs) const
{
  return !operator==(rhs);
}

template<class x_value_type, class y_value_type, class Derived>
Derived& HistoBase<x_value_type, y_value_type, Derived>::operator+=(const y_value_type& scalar)
{
  for (iterator it = this->values.begin(); it != this->values.end(); it++)
    it->second += scalar;

  return *(static_cast<Derived*>(this));
}
template<class x_value_type, class y_value_type, class Derived>
Derived& HistoBase<x_value_type, y_value_type, Derived>::operator-=(const y_value_type& scalar)
{
  for (iterator it = this->values.begin(); it != this->values.end(); it++)
    it->second -= scalar;

  return *(static_cast<Derived*>(this));
}
template<class x_value_type, class y_value_type, class Derived>
Derived& HistoBase<x_value_type, y_value_type, Derived>::operator*=(const y_value_type& scalar)
{
  for (iterator it = this->values.begin(); it != this->values.end(); it++)
    it->second *= scalar;

  return *(static_cast<Derived*>(this));
}
template<class x_value_type, class y_value_type, class Derived>
Derived& HistoBase<x_value_type, y_value_type, Derived>::operator/=(const y_value_type& scalar)
{
  for (iterator it = this->values.begin(); it != this->values.end(); it++)
    it->second /= scalar;

  return *(static_cast<Derived*>(this));
}

template<class x_value_type, class y_value_type, class Derived>
template<class ArbitraryDerived>
Derived& HistoBase<x_value_type, y_value_type, Derived>::operator+=(const HistoBase<x_value_type,y_value_type,ArbitraryDerived>& rhs)
{
  typedef HistoBase<x_value_type,y_value_type,ArbitraryDerived> rhs_const_iterator;

  for (typename HistoBase<x_value_type,y_value_type,ArbitraryDerived>::const_iterator it = rhs.begin(); it != rhs.end(); it++)
  {
    static_cast<Derived*>(this)->operator[](it->first) += it->second;
  }
  return *(static_cast<Derived*>(this));
}
template<class x_value_type, class y_value_type, class Derived>
template<class ArbitraryDerived>
Derived& HistoBase<x_value_type, y_value_type, Derived>::operator-=(const HistoBase<x_value_type,y_value_type,ArbitraryDerived>& rhs)
{
  for (typename HistoBase<x_value_type,y_value_type,ArbitraryDerived>::const_iterator it = rhs.begin(); it != rhs.end(); it++)
  {
    static_cast<Derived*>(this)->operator[](it->first) -= it->second;
  }
  return *(static_cast<Derived*>(this));
}
template<class x_value_type, class y_value_type, class Derived>
template<class ArbitraryDerived>
Derived& HistoBase<x_value_type, y_value_type, Derived>::operator*=(const HistoBase<x_value_type,y_value_type,ArbitraryDerived>& rhs)
{
  if (!compatible(rhs)) throw Exceptions::HistosNotCompatibleException("Two histograms must have the same x_values in order to multiply them.");

  for (typename HistoBase<x_value_type,y_value_type,ArbitraryDerived>::const_iterator it = rhs.begin(); it != rhs.end(); it++)
  {
    static_cast<Derived*>(this)->operator[](it->first) *= it->second;
  }
  return *(static_cast<Derived*>(this));
}
template<class x_value_type, class y_value_type, class Derived>
template<class ArbitraryDerived>
Derived& HistoBase<x_value_type, y_value_type, Derived>::operator/=(const HistoBase<x_value_type,y_value_type,ArbitraryDerived>& rhs)
{
  if (!compatible(rhs)) throw Exceptions::HistosNotCompatibleException("Two histograms must have the same x_values in order to divide them.");

  for (typename HistoBase<x_value_type,y_value_type,ArbitraryDerived>::const_iterator it = rhs.begin(); it != rhs.end(); it++)
  {
    static_cast<Derived*>(this)->operator[](it->first) /= it->second;
  }
  return *(static_cast<Derived*>(this));
}

template<class x_value_type, class y_value_type, class Derived>
template<class ArbitraryDerived>
bool HistoBase<x_value_type,y_value_type,Derived>::compatible(const HistoBase<x_value_type,y_value_type,ArbitraryDerived>& other) const
{
  // Check for size match
  if (size() != other.size()) return false;

  // Check for value match
  for (const_iterator it_this = begin(), it_other = other.begin(); it_this != end(); ++it_this, ++it_other)
  {
    if (it_this->first != it_other->first) return false;
  }

  // If sizes and values match, return true
  return true;
}

template<class x_value_type, class y_value_type, class Derived>
double HistoBase<x_value_type,y_value_type,Derived>::derivative(const_iterator x) const
{
  // If the bin is the first or the last bin, use the simple formula using only one neighbour
  if (x == min_x_value())
  {
    const_iterator x_plus(x); ++x_plus;
    return static_cast<double>(x_plus->second - x->second)/static_cast<double>(x_plus->first - x->first);
  }
  else if (x == max_x_value())
  {
    const_iterator x_minus(x); --x_minus;
    return static_cast<double>(x->second - x_minus->second)/static_cast<double>(x->first - x_minus->first);
  }
  // If we have a bin in the middle, use both neighbouring points
  else
  {
    const_iterator x_plus(x); ++x_plus;
    const_iterator x_minus(x); --x_minus;
    return static_cast<double>(x_plus->second - x_minus->second)/static_cast<double>(x_plus->first - x_minus->first);
  }
}

template<class x_value_type, class y_value_type, class Derived>
double HistoBase<x_value_type,y_value_type,Derived>::flatness() const
{
  unsigned int bin_number = 0;
  y_value_type sum = 0;
  y_value_type min = values.begin()->second;

  for (const_iterator it = values.begin(); it != values.end(); it++)
  {
    bin_number++;
    sum += it->second;
    if (it->second < min) min = it->second;
  }

  // If the histo is empty or has only 0 values, return 0
  if (values.size() == 0 || sum == 0) return 0;

  double mean = static_cast<double>(sum)/static_cast<double>(bin_number);
  return static_cast<double>(min) / mean;
}

/*!
  \tparam other_y_value_type Type of the y-values of the other HistoBase
  \param other HistoBase that is used to initialise the data of this HistoBase

  \details Initialises this histogram with 0 bins: The x-values are inserted into this histogram, the y-values are omitted.
 */
template<class x_value_type, class y_value_type, class Derived>
template<class other_y_value_type, class ArbitraryDerived>
void HistoBase<x_value_type,y_value_type,Derived>::initialise_empty(const HistoBase<x_value_type, other_y_value_type, ArbitraryDerived>& other)
{
  // Clear all entries of this HistoBase
  clear();

  // Enter all x-values
  for (typename HistoBase<x_value_type, other_y_value_type, ArbitraryDerived>::const_iterator it = other.begin(); it != other.end(); ++it)
  {
    values.insert(std::pair<x_value_type, y_value_type>(it->first, y_value_type(0)));
  }
}

template<class x_value_type, class y_value_type, class Derived>
typename HistoBase<x_value_type,y_value_type,Derived>::const_iterator HistoBase<x_value_type,y_value_type,Derived>::max_y_value() const
{
  const_iterator result = begin();
  y_value_type max_value = begin()->second;
  for (const_iterator it = begin(); it != end(); ++it)
  {
    if (it->second > max_value)
    {
      max_value = it->second;
      result = it;
    }
  }
  return result;
}

template<class x_value_type, class y_value_type, class Derived>
typename HistoBase<x_value_type,y_value_type,Derived>::const_iterator HistoBase<x_value_type,y_value_type,Derived>::min_y_value() const
{
  const_iterator result = begin();
  y_value_type min_value = begin()->second;
  for (const_iterator it = begin(); it != end(); ++it)
  {
    if (it->second < min_value)
    {
      min_value = it->second;
      result = it;
    }
  }
  return result;
}

template<class x_value_type, class y_value_type, class Derived>
void HistoBase<x_value_type,y_value_type,Derived>::print() const
{
  std::cout << "Debug-Printing HistoBase data:" << std::endl;
  for (const_iterator it = values.begin(); it != values.end(); it++)
  {
    std::cout << "x: " << it->first << ", y:" << it->second << std::endl;
  }
}

template<class x_value_type, class y_value_type, class Derived>
void HistoBase<x_value_type,y_value_type,Derived>::set_all_y_values(const y_value_type& const_value)
{
  for (iterator it = values.begin(); it != values.end(); it++)
  {
    it->second = const_value;
  }
}

/*!
 * /param it Iterator pointing to the pair that should be used as a reference
 */
template<class x_value_type, class y_value_type, class Derived> 
void HistoBase<x_value_type,y_value_type,Derived>::shift_bin_zero(const_iterator it)
{
  y_value_type binValue = it->second;
  typename std::map<x_value_type, y_value_type>::iterator entry = values.begin();
  // iterator entry = values.begin();
  for (entry = values.begin(); entry != values.end(); entry++)
  {
    entry->second -= binValue;
  }
}

template<class x_value_type, class y_value_type, class Derived>
y_value_type HistoBase<x_value_type,y_value_type,Derived>::sum() const
{
  y_value_type sum = 0;
  for (const_iterator it = values.begin(); it != values.end(); it++)
  {
    sum += it->second;
  }
  return sum;
}

template<class x_value_type, class y_value_type, class Derived>
void HistoBase<x_value_type,y_value_type,Derived>::load_serialize(std::istream& input_stream)
{
  boost::archive::text_iarchive input_archive(input_stream);
  input_archive >> *static_cast<Derived*>(this);
}
template<class x_value_type, class y_value_type, class Derived>
void HistoBase<x_value_type,y_value_type,Derived>::load_serialize(const char* filename)
{
  std::ifstream input_filestream(filename);
  load_serialize(input_filestream);
  input_filestream.close();
}
template<class x_value_type, class y_value_type, class Derived>
void HistoBase<x_value_type,y_value_type,Derived>::save_serialize(std::ostream& output_stream) const
{
  boost::archive::text_oarchive output_archive(output_stream);
  output_archive << *static_cast<const Derived*>(this);
}
template<class x_value_type, class y_value_type, class Derived>
void HistoBase<x_value_type,y_value_type,Derived>::save_serialize(const char* filename) const
{
  std::ofstream output_filestream(filename);
  save_serialize(output_filestream);
  output_filestream.close();
}
template<class x_value_type, class y_value_type, class Derived>
void HistoBase<x_value_type,y_value_type,Derived>::load_csv(std::istream& input_stream)
{
  std::string line;
  this->clear();
  while (getline(input_stream, line))
  {
    line.erase(line.begin(), find_if(line.begin(), line.end(), not1(std::ptr_fun<int, int>(isspace)))); // remove leading whitespace
    if (line[0] == '#') continue; // ignore comments
    if (line.length() == 0) continue; // ignore empty lines
    line.replace(line.find("\t"), 1, " ");
    x_value_type x = atof(line.substr(0, line.find(" ")).c_str());
    y_value_type y = atof(line.substr(line.find(" ") + 1).c_str());
    static_cast<Derived*>(this)->insert(std::pair<x_value_type, y_value_type>(x,y));
  }
}
template<class x_value_type, class y_value_type, class Derived>
void HistoBase<x_value_type,y_value_type,Derived>::load_csv(const char* filename)
{
  std::ifstream input_filestream(filename);
  if(input_filestream.fail())
  {
    std::cerr << "File " << filename << " cannot be loaded! Fatal error!" << std::endl;
  }
  load_csv(input_filestream);
  input_filestream.close();
}
template<class x_value_type, class y_value_type, class Derived>
void HistoBase<x_value_type,y_value_type,Derived>::save_csv(std::ostream& output_stream) const
{
  for (const_iterator it = values.begin(); it != values.end(); it++)
  {
    output_stream << it->first << "\t" << it->second << "\n";
  }
}
template<class x_value_type, class y_value_type, class Derived>
void HistoBase<x_value_type,y_value_type,Derived>::save_csv(const char* filename) const
{
  std::ofstream output_filestream(filename);
  save_csv(output_filestream);
  output_filestream.close();
}

template<class x_value_type, class y_value_type, class Derived>
const Derived operator+(const HistoBase<x_value_type, y_value_type, Derived>& lhs, const y_value_type& scalar)
{
  return Derived(lhs) += scalar;
}
template<class x_value_type, class y_value_type, class Derived>
const Derived operator+(const y_value_type& scalar, const HistoBase<x_value_type, y_value_type, Derived>& rhs)
{
  return Derived(rhs) += scalar;
}
template<class x_value_type, class y_value_type, class Derived>
const Derived operator-(const HistoBase<x_value_type, y_value_type, Derived>& lhs, const y_value_type& scalar)
{
  return Derived(lhs) -= scalar;
}
template<class x_value_type, class y_value_type, class Derived>
const Derived operator*(const HistoBase<x_value_type, y_value_type, Derived>& lhs, const y_value_type& scalar)
{
  return Derived(lhs) *= scalar;
}
template<class x_value_type, class y_value_type, class Derived>
const Derived operator*(const y_value_type& scalar, const HistoBase<x_value_type, y_value_type, Derived>& rhs)
{
  return Derived(rhs) *= scalar;
}
template<class x_value_type, class y_value_type, class Derived>
const Derived operator/(const HistoBase<x_value_type, y_value_type, Derived>& lhs, const y_value_type& scalar)
{
  return Derived(lhs) /= scalar;
}

template<class x_value_type, class y_value_type, class DerivedLeft, class DerivedRight>
const DerivedLeft operator+(const HistoBase<x_value_type, y_value_type, DerivedLeft>& lhs, const HistoBase<x_value_type, y_value_type, DerivedRight>& rhs)
{
  return DerivedLeft(lhs) += rhs;
}
template<class x_value_type, class y_value_type, class DerivedLeft, class DerivedRight>
const DerivedLeft operator-(const HistoBase<x_value_type, y_value_type, DerivedLeft>& lhs, const HistoBase<x_value_type, y_value_type, DerivedRight>& rhs)
{
  return DerivedLeft(lhs) -= rhs;
}
template<class x_value_type, class y_value_type, class Derived>
const Derived operator*(const HistoBase<x_value_type, y_value_type, Derived>& lhs, const HistoBase<x_value_type, y_value_type, Derived>& rhs)
{
  return Derived(lhs) *= rhs;
}
template<class x_value_type, class y_value_type, class Derived>
const Derived operator/(const HistoBase<x_value_type, y_value_type, Derived>& lhs, const HistoBase<x_value_type, y_value_type, Derived>& rhs)
{
  return Derived(lhs) /= rhs;
}

template<class x_value_type, class y_value_type, class Derived>
std::ostream& operator<<(std::ostream& stream, const HistoBase<x_value_type, y_value_type, Derived>& rhs)
{
  for (typename HistoBase<x_value_type, y_value_type, Derived>::const_iterator it = rhs.begin(); it != rhs.end(); ++it)
  {
    stream << it->first << "\t" << it->second << std::endl;
  }
  return stream;
}

} // of namespace Histograms
} // of namespace Mocasinns

#endif
