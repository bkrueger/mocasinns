/**
 * \file histobase.cpp
 * \brief Implementation of the HistoBase class
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */
#ifdef HISTOBASE_HPP

#include <cmath> // INFINITY and isnan()
#include <fstream>
#include <sstream>

namespace Mocasinns
{
namespace Histograms
{

template<class x_value_type, class y_value_type>
bool HistoBase<x_value_type,y_value_type>::operator==(const HistoBase<x_value_type,y_value_type>& rhs) const
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
template<class x_value_type, class y_value_type>
bool HistoBase<x_value_type,y_value_type>::operator!=(const HistoBase<x_value_type,y_value_type>& rhs) const
{
  return !operator==(rhs);
}

template<class x_value_type, class y_value_type>
double HistoBase<x_value_type,y_value_type>::flatness() const
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

template<class x_value_type, class y_value_type>
typename HistoBase<x_value_type, y_value_type>::const_iterator HistoBase<x_value_type,y_value_type>::max_y_value() const
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

template<class x_value_type, class y_value_type>
typename HistoBase<x_value_type, y_value_type>::const_iterator HistoBase<x_value_type,y_value_type>::min_y_value() const
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

template<class x_value_type, class y_value_type>
void HistoBase<x_value_type,y_value_type>::print() const
{
  std::cout << "Debug-Printing HistoBase data:" << std::endl;
  for (const_iterator it = values.begin(); it != values.end(); it++)
  {
    std::cout << "x: " << it->first << ", y:" << it->second << std::endl;
  }
}

template<class x_value_type, class y_value_type>
void HistoBase<x_value_type,y_value_type>::set_all_y_values(const y_value_type& const_value)
{
  for (iterator it = values.begin(); it != values.end(); it++)
  {
    it->second = const_value;
  }
}

/*!
 * /param it Iterator pointing to the pair that should be used as a reference
 */
template<class x_value_type, class y_value_type> 
void HistoBase<x_value_type, y_value_type>::shift_bin_zero(const_iterator it)
{
  y_value_type binValue = it->second;
  typename std::map<x_value_type, y_value_type>::iterator entry = values.begin();
  // iterator entry = values.begin();
  for (entry = values.begin(); entry != values.end(); entry++)
  {
    entry->second -= binValue;
  }
}

template<class x_value_type, class y_value_type>
y_value_type HistoBase<x_value_type,y_value_type>::sum()
{
  y_value_type sum = 0;
  for (iterator it = values.begin(); it != values.end(); it++)
  {
    sum += it->second;
  }
  return sum;
}

template<class x_value_type, class y_value_type>
void HistoBase<x_value_type,y_value_type>::load_serialize(std::istream& input_stream)
{
  boost::archive::text_iarchive input_archive(input_stream);
  input_archive >> (*this);
}
template<class x_value_type, class y_value_type>
void HistoBase<x_value_type,y_value_type>::load_serialize(const char* filename)
{
  std::ifstream input_filestream(filename);
  load_serialize(input_filestream);
  input_filestream.close();
}
template<class x_value_type, class y_value_type>
void HistoBase<x_value_type,y_value_type>::save_serialize(std::ostream& output_stream) const
{
  boost::archive::text_oarchive output_archive(output_stream);
  output_archive << (*this);
}
template<class x_value_type, class y_value_type>
void HistoBase<x_value_type,y_value_type>::save_serialize(const char* filename) const
{
  std::ofstream output_filestream(filename);
  save_serialize(output_filestream);
  output_filestream.close();
}
template<class x_value_type, class y_value_type>
void HistoBase<x_value_type,y_value_type>::load_csv(std::istream& input_stream)
{
  std::string line;
  this->clear();
  while (getline(input_stream, line))
  {
    x_value_type x; 
    y_value_type y;
    line.erase(line.begin(), find_if(line.begin(), line.end(), not1(std::ptr_fun<int, int>(isspace)))); // remove leading whitespace
    if(line[0] == '#') continue; // ignore comments
    std::stringstream(line) >> x >> y;
    values.insert(std::pair<x_value_type, y_value_type>(x,y));
  }
}
template<class x_value_type, class y_value_type>
void HistoBase<x_value_type,y_value_type>::load_csv(const char* filename)
{
  std::ifstream input_filestream(filename);
  if(input_filestream.fail())
  {
    std::cerr << "File " << filename << " cannot be loaded! Fatal error!" << std::endl;
  }
  load_csv(input_filestream);
  input_filestream.close();
}
template<class x_value_type, class y_value_type>
void HistoBase<x_value_type,y_value_type>::save_csv(std::ostream& output_stream) const
{
  for (const_iterator it = values.begin(); it != values.end(); it++)
  {
    output_stream << it->first << "\t" << it->second << "\n";
  }
}
template<class x_value_type, class y_value_type>
void HistoBase<x_value_type,y_value_type>::save_csv(const char* filename) const
{
  std::ofstream output_filestream(filename);
  save_csv(output_filestream);
  output_filestream.close();
}

} // of namespace Histograms
} // of namespace Mocasinns

#endif
