/**
 * \file histocrete.cpp
 * \brief Implementation of the Histocrete class
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */
#ifdef MOCASINNS_HISTOGRAMS_HISTOCRETE_HPP

namespace Mocasinns
{
namespace Histograms
{

template<class x_value_type, class y_value_type>
Histocrete<x_value_type,y_value_type>& Histocrete<x_value_type,y_value_type>::operator+=(HistoBase<x_value_type, y_value_type>& other_histobase)
{
  for (iterator it = other_histobase.begin(); it != other_histobase.end(); it++)
  {
    this->values[it->first] += it->second;
  }
  return *this;
}
template<class x_value_type, class y_value_type>
Histocrete<x_value_type,y_value_type>& Histocrete<x_value_type,y_value_type>::operator+=(const y_value_type& const_value)
{
  for (iterator it = this->values.begin(); it != this->values.end(); it++)
  {
    it->second += const_value;
  }
  return *this;
}
template<class x_value_type, class y_value_type>
Histocrete<x_value_type,y_value_type>& Histocrete<x_value_type,y_value_type>::operator/= (HistoBase<x_value_type, y_value_type>& other_histobase)
{
  for (iterator it = other_histobase.begin(); it != other_histobase.end(); it++)
  {
    if (!isnan(it->second) && it->second != 0)
      this->values[it->first] /= it->second;
  }
  return *this;
}
template<class x_value_type, class y_value_type>
Histocrete<x_value_type,y_value_type>& Histocrete<x_value_type,y_value_type>::operator/= (const y_value_type& const_value)
{
  if (const_value == 0) return;
  
  for (iterator it = this->values.begin(); it != this->values.end(); it++)
  {
    if (!isnan(it->second)) it->second /= const_value;
  }
  return *this;
}

/*!
  \tparam other_y_value_type Type of the y-values of the other Histocrete
  \param other Histocrete that is used to initialise the data of this Histocrete

  \details Initialises this histogram with 0 bins: The x-values are inserted into this histogram, the y-values are omitted.
 */
template<class x_value_type, class y_value_type>
template<class other_y_value_type>
void Histocrete<x_value_type, y_value_type>::initialise_empty(const Histocrete<x_value_type, other_y_value_type>& other)
{
  // Call the according HistoBase-Function
  HistoBase<x_value_type, y_value_type>::initialise_empty(static_cast<HistoBase<x_value_type, other_y_value_type> >(other));
  // Nothing else has to be done.
}

template<class x_value_type, class y_value_type>
void Histocrete<x_value_type,y_value_type>::load_serialize(std::istream& input_stream)
{
  boost::archive::text_iarchive input_archive(input_stream);
  input_archive >> (*this);
}
template<class x_value_type, class y_value_type>
void Histocrete<x_value_type,y_value_type>::load_serialize(const char* filename)
{
  std::ifstream input_filestream(filename);
  load_serialize(input_filestream);
  input_filestream.close();
}
template<class x_value_type, class y_value_type>
void Histocrete<x_value_type,y_value_type>::save_serialize(std::ostream& output_stream) const
{
  boost::archive::text_oarchive output_archive(output_stream);
  output_archive << (*this);
}
template<class x_value_type, class y_value_type>
void Histocrete<x_value_type,y_value_type>::save_serialize(const char* filename) const
{
  std::ofstream output_filestream(filename);
  save_serialize(output_filestream);
  output_filestream.close();
}

} // of namespace Histograms
} // of namespace Mocasinns

#endif
