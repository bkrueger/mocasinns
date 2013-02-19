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
  Base::initialise_empty(static_cast<HistoBase<x_value_type, other_y_value_type, Histogram<x_value_type, other_y_value_type, BinningFunctor> > >(other));

  // Copy the binning
  set_binning_width(other.get_binning_width());
  set_binning_reference(other.get_binning_reference());
}

} // of namespace Histograms
} // of namespace Mocasinns

#endif
