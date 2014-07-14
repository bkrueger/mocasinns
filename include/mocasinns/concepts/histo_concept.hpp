#ifndef MOCASINNS_CONCEPTS_HISTO_CONCEPT_HPP
#define MOCASINNS_CONCEPTS_HISTO_CONCEPT_HPP

/*!
  \file histo_concept.hpp

  \author Benedikt Krüger
*/

#include "boost/concept_check.hpp"

namespace Mocasinns
{
  namespace Concepts
  {

    /*!
      \brief Structure for checking a class for compatibility with the concept of a histo used in the mocasinns library.
      \tparam HistoType Class template that should be checked for compatibility with the histo concept.
    */
    template <template<class,class> class HistoType>
    struct HistoConcept : 
      boost::PairAssociativeContainer<HistoType<double,double> >,
      boost::EqualityComparable<HistoType<double, double> >
    {
    public:
      BOOST_CONCEPT_USAGE(HistoConcept)
      {
	// Arithmetic operations with scalars
	// histo = histo_other_1 + scalar;
	// histo = histo_other_1 - scalar;
	// histo = histo_other_1 * scalar;
	// histo = histo_other_1 / scalar;
	// Arithmetic operations with two histograms
	histo = histo_other_1 + histo_other_2;
	histo = histo_other_1 - histo_other_2;
	histo = histo_other_1 * histo_other_2;
	histo = histo_other_1 / histo_other_2;

	// There must be a function calculating the derivative
	// The function must take a const iterator
	// The result must be convertible to double
	derivative_result = histo.derivative(histo_const_it);

	// There must be function for calculating the flatness
	// The result must be convertible to double
	flatness_result = histo.flatness();

	// There must be a function to initialise the histo empty
	histo.initialise_empty(histo_other_1);

	// There must be functions to calculate the max and min values
	histo_const_it = histo.max_x_value();
	histo_const_it = histo.max_y_value();
	histo_const_it = histo.min_x_value();
	histo_const_it = histo.min_y_value();

	// There must be a function to shift hte y-values so that one bin is 0
	// The function must take an iterator
	histo.shift_bin_zero(histo.begin());

	// There must be a function to calculate the sum of all y-values
	histo.sum();
      }
      
    private:
      HistoType<double,double> histo;
      HistoType<double,double> histo_other_1;
      HistoType<double,double> histo_other_2;
      
      double scalar;

      typename HistoType<double,double>::const_iterator histo_const_it;

      double derivative_result;
      double flatness_result;
    };
  }
}

#endif
