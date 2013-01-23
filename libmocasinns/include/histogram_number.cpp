/**
 * \file histogram_number.cpp
 * \brief Implementation of the HistogramNumber class
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */
#ifdef HISTOGRAM_NUMBER_HPP

template<class x_value_type, class y_value_type>
void HistogramNumber<x_value_type,y_value_type>::operator+=(HistoBase<x_value_type, y_value_type>& other_histobase)
{
  for (iterator it = other_histobase.begin(); it != other_histobase.end(); it++)
  {
    this->values[binning(it->first)] += it->second;
  }
}
template<class x_value_type, class y_value_type>
void HistogramNumber<x_value_type,y_value_type>::operator+= (const y_value_type& const_value)
{
  for (iterator it = this->values.begin(); it != this->values.end(); it++)
  {
    it->second += const_value;
  }
}
template<class x_value_type, class y_value_type>
void HistogramNumber<x_value_type,y_value_type>::operator/= (HistoBase<x_value_type, y_value_type>& other_histobase)
{
  for (iterator it = other_histobase.begin(); it != other_histobase.end(); it++)
  {
    if (it->second != 0)
      this->values[binning(it->first)] /= it->second;
  }
}
template<class x_value_type, class y_value_type>
void HistogramNumber<x_value_type,y_value_type>::operator/= (const y_value_type& const_value)
{
  if (const_value == 0) return;
  
  for (iterator it = this->values.begin(); it != this->values.end(); it++)
  {
    it->second /= const_value;
  }
}

template<class x_value_type, class y_value_type>
void HistogramNumber<x_value_type,y_value_type>::load_serialize(std::istream& input_stream)
{
  boost::archive::text_iarchive input_archive(input_stream);
  input_archive >> (*this);
}
template<class x_value_type, class y_value_type>
void HistogramNumber<x_value_type,y_value_type>::load_serialize(const char* filename)
{
  std::ifstream input_filestream(filename);
  load_serialize(input_filestream);
  input_filestream.close();
}
template<class x_value_type, class y_value_type>
void HistogramNumber<x_value_type,y_value_type>::save_serialize(std::ostream& output_stream) const
{
  boost::archive::text_oarchive output_archive(output_stream);
  output_archive << (*this);
}
template<class x_value_type, class y_value_type>
void HistogramNumber<x_value_type,y_value_type>::save_serialize(const char* filename) const
{
  std::ofstream output_filestream(filename);
  save_serialize(output_filestream);
  output_filestream.close();
}

#endif
