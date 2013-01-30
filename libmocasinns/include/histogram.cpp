/**
 * \file histogram.cpp
 * \brief Implementation of the Histocrete class
 * 
 * Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */
#ifdef HISTOGRAM_HPP

namespace Mocasinns
{
namespace Histograms
{

template<class x_value_type, class y_value_type, class BinningFunctor>
void Histogram<x_value_type,y_value_type, BinningFunctor>::operator+=(HistoBase<x_value_type, y_value_type>& other_histobase)
{
  for (iterator it = other_histobase.begin(); it != other_histobase.end(); it++)
  {
    if (!isnan(it->second))
      this->values[binning(it->first)] += it->second;
    else
      this->values[binning(it->first)] = it->second;
  }
}
template<class x_value_type, class y_value_type, class BinningFunctor>
void Histogram<x_value_type,y_value_type, BinningFunctor>::operator+= (const y_value_type& const_value)
{
  for (iterator it = this->values.begin(); it != this->values.end(); it++)
  {
    if (!isnan(it->second)) it->second += const_value;
  }
}
template<class x_value_type, class y_value_type, class BinningFunctor>
void Histogram<x_value_type,y_value_type, BinningFunctor>::operator/= (HistoBase<x_value_type, y_value_type>& other_histobase)
{
  for (iterator it = other_histobase.begin(); it != other_histobase.end(); it++)
  {
    if (!isnan(it->second) && it->second != 0)
      this->values[binning(it->first)] /= it->second;
  }
}
template<class x_value_type, class y_value_type, class BinningFunctor>
void Histogram<x_value_type,y_value_type, BinningFunctor>::operator/= (const y_value_type& const_value)
{
  if (const_value == 0) return;
  
  for (iterator it = this->values.begin(); it != this->values.end(); it++)
  {
    if (!isnan(it->second)) it->second /= const_value;
  }
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
