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
  Base::initialise_empty(static_cast<HistoBase<x_value_type, other_y_value_type, Histocrete<x_value_type, other_y_value_type> > >(other));
  // Nothing else has to be done.
}

} // of namespace Histograms
} // of namespace Mocasinns

#endif
