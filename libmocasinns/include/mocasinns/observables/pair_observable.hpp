#ifndef MOCASINNS_OBSERVABLES_PAIR_OBSERVABLE_HPP
#define MOCASINNS_OBSERVABLES_PAIR_OBSERVABLE_HPP

#include <cmath>

#include <boost/accumulators/numeric/functional_fwd.hpp>

#include "../details/stl_extensions/pair_addable.hpp"

namespace Mocasinns
{
  namespace Observables
  {
    //! Class representing a pair of observables. Can be used like a std::pair and provides (component-wise) addition, substraction and exponentiation of the observables.
    template <class T1, class T2>
    class PairObservable : public Details::STL_Extensions::PairAddable<T1,T2,PairObservable<T1,T2> >
    {
    public:
      //! Type of the base class
      typedef Details::STL_Extensions::PairAddable<T1,T2,PairObservable<T1,T2> > Base;

      //! Type of the first stored observable. Typedef for T1
      typedef typename Base::first_type first_type;
      //! Type of the second stored observable. Typedef for T2
      typedef typename Base::second_type second_type;

      //! Creates an empty PairObservable
      PairObservable() : Base() {}
      PairObservable(const T1& first, const T2& second) : Base(first, second) {}
      //! Creates a PairObservable with a std::pair
      PairObservable(const typename Base::data_type& std_pair) : Base(std_pair) {}
      //! Convert from ArrayAddable
      PairObservable(const Base& other) : Base(other) {}
      //! Copy-Constructor
      PairObservable(const PairObservable<T1,T2>& other) : Base(other.first, other.second) {}

      //! Multiply this PairObservable component-wise with another PairObservable
      PairObservable<T1,T2>& operator*=(const PairObservable<T1,T2>& rhs)
      {
	// Multiply the arrays component-wise
	this->data.first *= rhs.first;
	this->data.second *= rhs.second;
	return *this;
      }
      //! Divide this PairObservable component-wise by another PairObservable
      PairObservable<T1,T2>& operator/=(const PairObservable<T1,T2>& rhs)
      {
	// Divide the arrays component-wise
	this->data.first /= rhs.first;
	this->data.second /= rhs.second;
	return *this;
      }
    };

    //! Multiply two PairObservables component-wise
    template <class T1, class T2>
    const PairObservable<T1,T2> operator*(const PairObservable<T1,T2>& lhs, const PairObservable<T1,T2>& rhs)
    {
      return PairObservable<T1,T2>(lhs) *= rhs;
    }
    //! Divide two PairObservables component-wise
    template <class T1, class T2>
    const PairObservable<T1,T2> operator/(const PairObservable<T1,T2>& lhs, const PairObservable<T1,T2>& rhs)
    {
      return PairObservable<T1,T2>(lhs) /= rhs;
    }

    //! Exponentiate the PairObservable with a scalar
    template <class T1, class T2, class S>
    const PairObservable<T1,T2> pow(const PairObservable<T1,T2>& base, const S& exponent)
    {
      using std::pow;
      PairObservable<T1,T2> result;
      result.first = pow(base.first, exponent);
      result.second = pow(base.second, exponent);
      return result;
    }

    //! Takes the square root of the PairObservable component wise
    template <class T1, class T2>
    const PairObservable<T1,T2> sqrt(const PairObservable<T1,T2>& number)
    {
      return pow(number, 0.5);
    }
  }
}

namespace boost 
{ 
  namespace numeric 
  { 
    namespace functional
    {
      // Tag type for PairObservable
      template <typename T1, typename T2>
      struct PairObservableTag;

      // Specialise tag<> for PairObservable
      template <typename T1, typename T2> struct tag<Mocasinns::Observables::PairObservable<T1,T2> >
      {
	typedef PairObservableTag<T1,T2> type;
      };

      // Specify how to devide a PairObservable by an integral count
      template <typename Left, typename Right, class T1, class T2>
      struct average<Left, Right, PairObservableTag<T1,T2>, void>
      {
	// Define the type of the result
	typedef Mocasinns::Observables::PairObservable<T1,T2> result_type;
	
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
