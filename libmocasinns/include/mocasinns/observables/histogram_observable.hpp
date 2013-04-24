#ifndef MOCASINNS_OBSERVABLES_HISTOGRAM_OBSERVABLE_HPP
#define MOCASINNS_OBSERVABLES_HISTOGRAM_OBSERVABLE_HPP

#include <stdexcept>
#include <cmath>

#include <boost/accumulators/numeric/functional_fwd.hpp>

namespace Mocasinns
{
  namespace Observables
  {
    class XValuesDoNotMatchException : public std::invalid_argument
    {
    public:
      XValuesDoNotMatchException() : std::invalid_argument("The x-values of the HistogramObservables do not match. Cannot do arithmetic operations.") {}
    };


    //! Class representing the histogram of an observable as new observable.
    /*!
      \tparam Histo Class template for a histogram, use e.g. the histograms defined in Mocasinns::Histograms. The first template parameter coresponds to the type of the x-values, the second template parameteres corresponds to the type of the y-values of the histogram.
      \tparam x_value_type Type of the x-values of the histogram
      \tparam y_value_type Type of the y-values of the histogram
     */
    template <template <class,class> class Histo, class x_value_type, class y_value_type>
    class HistogramObservable : public Histo<x_value_type, y_value_type>
    {
    public:
      typedef Histo<x_value_type, y_value_type> HistoType;

      //! Default constructor
      HistogramObservable() : HistoType () {}
      //! Constructor taking a base object
      HistogramObservable(const HistoType& base_histo) : HistoType(base_histo) {}
      //! Copy constructor
      HistogramObservable(const HistogramObservable<Histo, x_value_type, y_value_type>& other) : HistoType(static_cast<HistoType>(other)) {}

      //! Operator for adding a scalar to this HistogramObservable
      HistogramObservable& operator+=(const y_value_type rhs)
      {
	for (typename HistoType::iterator it = this->begin(); it != this->end(); ++it)
	{
	  it->second += rhs;
	}
	return *this;
      }
      //! Operator for substracting a scalar from this HistogramObservable
      HistogramObservable& operator-=(const y_value_type rhs)
      {
	for (typename HistoType::iterator it = this->begin(); it != this->end(); ++it)
	{
	  it->second -= rhs;
	}
	return *this;
      }

      //! Operator for multiplying the HistogramObservable with a scalar
      HistogramObservable& operator*=(const y_value_type rhs)
      {
	for (typename HistoType::iterator it = this->begin(); it != this->end(); ++it)
	{
	  it->second *= rhs;
	}
	return *this;
      }
      //! Operator for dividing the HistogramObservable by a scalar
      HistogramObservable& operator/=(const y_value_type rhs)
      {
	for (typename HistoType::iterator it = this->begin(); it != this->end(); ++it)
	{
	  it->second /= rhs;
	}
	return *this;
      }
      
      //! Operator for adding another HistogramObservable with this HistogramObservable
      HistogramObservable& operator+=(const HistogramObservable<Histo, x_value_type, y_value_type> rhs)
      {
	if (x_values_match(rhs) == false) throw XValuesDoNotMatchException();
                                                
	typename HistoType::iterator this_it = this->begin();
	typename HistoType::const_iterator other_it = rhs.begin();
	for (;this_it != this->end(); ++this_it, ++other_it)
	{
	  this_it->second += other_it->second;
	}

	return *this;
      }
      //! Operator for dividing another HistogramObservable from this HistogramObservable
      HistogramObservable& operator-=(const HistogramObservable<Histo, x_value_type, y_value_type> rhs)
      {
	if (x_values_match(rhs) == false) throw XValuesDoNotMatchException();

	typename HistoType::iterator this_it = this->begin();
	typename HistoType::const_iterator other_it = rhs.begin();
	for (;this_it != this->end(); ++this_it, ++other_it)
	{
	  this_it->second -= other_it->second;
	}

	return *this;
      }
      //! Operator for multiplying the HistogramObservable with another HistogramObservable
      HistogramObservable& operator*=(const HistogramObservable<Histo, x_value_type, y_value_type> rhs)
      {
	if (x_values_match(rhs) == false) throw XValuesDoNotMatchException();

	typename HistoType::iterator this_it = this->begin();
	typename HistoType::const_iterator other_it = rhs.begin();
	for (;this_it != this->end(); ++this_it, ++other_it)
	{
	  this_it->second *= other_it->second;
	}

	return *this;
      }
      //! Operator for dividing the HistogramObservable by another HistogramObservable
      HistogramObservable& operator/=(const HistogramObservable<Histo, x_value_type, y_value_type> rhs)
      {
	if (x_values_match(rhs) == false) throw XValuesDoNotMatchException();

	typename HistoType::iterator this_it = this->begin();
	typename HistoType::const_iterator other_it = rhs.begin();
	for (;this_it != this->end(); ++this_it, ++other_it)
	{
	  this_it->second /= other_it->second;
	}

	return *this;
      }

      
    private:
      bool x_values_match(const HistogramObservable<Histo, x_value_type, y_value_type>& other) const
      {
	// Check the size of the histograms
	if (this->size() != other.size()) return false;

	// Check every single entry
	for (typename HistoType::const_iterator this_it = this->begin(), other_it = other.begin();
	     this_it != this->end(); ++this_it, ++other_it)
	{
	  if (this_it->first != other_it->first) return false;
	}

	// If no check failed return true
	return true;
      } 
    };

    //! Binary operator for adding two histogram observables
    template <template <class,class> class Histo, class x_value_type, class y_value_type>
    const HistogramObservable<Histo, x_value_type, y_value_type> operator+(const HistogramObservable<Histo, x_value_type, y_value_type>& lhs, const HistogramObservable<Histo, x_value_type, y_value_type>& rhs)
    {
      return HistogramObservable<Histo, x_value_type, y_value_type>(lhs) += rhs;
    }
    //! Binary operator for substracting two histogram observables
    template <template <class,class> class Histo, class x_value_type, class y_value_type>
    const HistogramObservable<Histo, x_value_type, y_value_type> operator-(const HistogramObservable<Histo, x_value_type, y_value_type>& lhs, const HistogramObservable<Histo, x_value_type, y_value_type>& rhs)
    {
      return HistogramObservable<Histo, x_value_type, y_value_type>(lhs) -= rhs;
    }
    //! Binary operator for multiplying two histogram observables
    template <template <class,class> class Histo, class x_value_type, class y_value_type>
    const HistogramObservable<Histo, x_value_type, y_value_type> operator*(const HistogramObservable<Histo, x_value_type, y_value_type>& lhs, const HistogramObservable<Histo, x_value_type, y_value_type>& rhs)
    {
      return HistogramObservable<Histo, x_value_type, y_value_type>(lhs) *= rhs;
    }
    //! Binary operator for dividing two histogram observables
    template <template <class,class> class Histo, class x_value_type, class y_value_type>
    const HistogramObservable<Histo, x_value_type, y_value_type> operator/(const HistogramObservable<Histo, x_value_type, y_value_type>& lhs, const HistogramObservable<Histo, x_value_type, y_value_type>& rhs)
    {
      return HistogramObservable<Histo, x_value_type, y_value_type>(lhs) /= rhs;
    }

    //! Exponentiate the histogram observable with a scalar
    template <template <class, class> class Histo, class x_value_type, class y_value_type, class S>
    const HistogramObservable<Histo, x_value_type, y_value_type> pow(const HistogramObservable<Histo, x_value_type, y_value_type>& base, const S& exponent)
    {
      HistogramObservable<Histo, x_value_type, y_value_type> result;
      for (typename HistogramObservable<Histo, x_value_type, y_value_type>::const_iterator it = base.begin(); it != base.end(); ++it)
      {
	result[it->first] = std::pow(it->second, exponent);
      }
      return result;
    }
    
    // Takes the square root of a histogram observable
    template <template <class, class> class Histo, class x_value_type, class y_value_type>
    const HistogramObservable<Histo, x_value_type, y_value_type> sqrt(const HistogramObservable<Histo, x_value_type, y_value_type>& base)
    {
      return pow(base, 0.5);
    }
  }
}

namespace boost 
{ 
  namespace numeric 
  { 
    namespace functional
    {
      // Tag type for HistogramObservable
      template <template <class,class> class Histo, class x_value_type, class y_value_type>
      struct HistogramObservableTag;

      // Specialise tag<> for HistogramObservable
      template <template <class,class> class Histo, class x_value_type, class y_value_type> struct tag<Mocasinns::Observables::HistogramObservable<Histo, x_value_type, y_value_type> >
      {
	typedef HistogramObservableTag<Histo, x_value_type, y_value_type> type;
      };

      // Specify how to devide a HistogramObservable by an integral count
      template <typename Left, typename Right, template <class,class> class Histo, class x_value_type, class y_value_type>
      struct average<Left, Right, HistogramObservableTag<Histo, x_value_type, y_value_type>, void>
      {
	// Define the type of the result
	typedef Mocasinns::Observables::HistogramObservable<Histo, x_value_type, y_value_type> result_type;
	
	// Define the result operator
	result_type operator()(Left& left , Right& right) const
	{
	  return left / static_cast<double>(right);
	}
      };
    }  
  }
}

#endif
