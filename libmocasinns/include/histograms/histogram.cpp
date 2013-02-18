/**
 * \file histogram.cpp
 * \brief Implementation of the Histocrete class
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */
#ifdef MOCASINNS_HISTOGRAMS_HISTOGRAM_HPP

namespace Mocasinns
{
namespace Histograms
{

template<class x_value_type, class y_value_type, class BinningFunctor>
Histogram<x_value_type, y_value_type, BinningFunctor>& Histogram<x_value_type,y_value_type, BinningFunctor>::operator+=(HistoBase<x_value_type, y_value_type>& other_histobase)
{
  for (iterator it = other_histobase.begin(); it != other_histobase.end(); it++)
  {
    if (!isnan(it->second))
      this->values[binning(it->first)] += it->second;
    else
      this->values[binning(it->first)] = it->second;
  }
  return *this;
}
template<class x_value_type, class y_value_type, class BinningFunctor>
Histogram<x_value_type, y_value_type, BinningFunctor>& Histogram<x_value_type,y_value_type, BinningFunctor>::operator+= (const y_value_type& const_value)
{
  for (iterator it = this->values.begin(); it != this->values.end(); it++)
  {
    if (!isnan(it->second)) it->second += const_value;
  }
  return *this;
}
template<class x_value_type, class y_value_type, class BinningFunctor>
Histogram<x_value_type, y_value_type, BinningFunctor>& Histogram<x_value_type,y_value_type, BinningFunctor>::operator/= (HistoBase<x_value_type, y_value_type>& other_histobase)
{
  for (iterator it = other_histobase.begin(); it != other_histobase.end(); it++)
  {
    if (!isnan(it->second) && it->second != 0)
      this->values[binning(it->first)] /= it->second;
  }
  return *this;
}
template<class x_value_type, class y_value_type, class BinningFunctor>
Histogram<x_value_type, y_value_type, BinningFunctor>& Histogram<x_value_type,y_value_type, BinningFunctor>::operator/= (const y_value_type& const_value)
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

  \details Initialises this histogram with 0 bins: The x-values are inserted into this histogram, the y-values are omitted. The details of the binning are copied.
 */
  template<class x_value_type, class y_value_type, class BinningFunctor>
template<class other_y_value_type>
void Histogram<x_value_type, y_value_type, BinningFunctor>::initialise_empty(const Histogram<x_value_type, other_y_value_type, BinningFunctor>& other)
{
  // Call the according HistoBase-Function
  HistoBase<x_value_type, y_value_type>::initialise_empty(static_cast<HistoBase<x_value_type, other_y_value_type> >(other));

  // Copy the binning
  set_binning_width(other.get_binning_width());
  set_binning_reference(other.get_binning_reference());
}

template<class x_value_type, class y_value_type, class BinningFunctor>
void Histogram<x_value_type,y_value_type,BinningFunctor>::load_serialize(std::istream& input_stream)
{
  boost::archive::text_iarchive input_archive(input_stream);
  input_archive >> (*this);
}
template<class x_value_type, class y_value_type, class BinningFunctor>
void Histogram<x_value_type,y_value_type,BinningFunctor>::load_serialize(const char* filename)
{
  std::ifstream input_filestream(filename);
  load_serialize(input_filestream);
  input_filestream.close();
}
template<class x_value_type, class y_value_type, class BinningFunctor>
void Histogram<x_value_type,y_value_type,BinningFunctor>::save_serialize(std::ostream& output_stream) const
{
  boost::archive::text_oarchive output_archive(output_stream);
  output_archive << (*this);
}
template<class x_value_type, class y_value_type, class BinningFunctor>
void Histogram<x_value_type,y_value_type,BinningFunctor>::save_serialize(const char* filename) const
{
  std::ofstream output_filestream(filename);
  save_serialize(output_filestream);
  output_filestream.close();
}

} // of namespace Histograms
} // of namespace Mocasinns

#endif
