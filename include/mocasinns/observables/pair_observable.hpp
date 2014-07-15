#ifndef MOCASINNS_OBSERVABLES_PAIR_OBSERVABLE_HPP
#define MOCASINNS_OBSERVABLES_PAIR_OBSERVABLE_HPP

#include <cmath>

#include <boost/accumulators/numeric/functional_fwd.hpp>

#include "../details/stl_extensions/pair_addable.hpp"

namespace Mocasinns
{
  namespace Observables
  {
    //! Class representing a pair observable. Can be used like a std::pair and provides (component-wise) addition, substraction and exponentiation of the observables.
    template <class T1, class T2>
    class PairObservable : public Details::STL_Extensions::PairAddable<T1,T2,PairObservable<T1,T2> >
    {
    public:
      //! Type of the base class
      typedef Details::STL_Extensions::PairAddable<T1,T2,PairObservable<T1,T2> > Base;

      //! First entry of the pair
      typedef typename Base::first_type first_type;
      //! Second entry of the pair
      typedef typename Base::second_type second_type;
      //! STL-Pair
      typedef typename Base::data_type data_type;

      //! The default constructor. It uses constructs objects of types T1 and T2 using their default constructors. This constructor may only be used if both T1 and T2 are DefaultConstructible. 
      PairObservable() : Base() { }
      //! The expand constructor. Constructs a pair so that both entries match the given one.
      template <class T>
      PairObservable(const T& value) : Base(value) { }
      //! The pair constructor. Constructs a pair such that first is constructed from x and second is constructed from y. 
      PairObservable(const T1& first, const T2& second) :  Base(first, second) { }
      //! Creates an extended pair from the standard pair
      PairObservable(const data_type& std_pair) : Base(std_pair) { }
      //! Copy constructor
      template <class Derived>
      PairObservable(const Details::STL_Extensions::PairAddable<T1, T2, Derived >& pair_addable) : Base(pair_addable) { }
      
      //! Multiply this PairObservable component-wise with another PairObservable
      PairObservable<T1,T2>& operator*=(const PairObservable<T1,T2>& rhs)
      {
	this->data.first *= rhs.data.first;
	this->data.second *= rhs.data.second;
	return *this;
      }
      //! Divide this PairObservable component-wise by another PairObservable
      PairObservable<T1,T2>& operator/=(const PairObservable<T1,T2>& rhs)
      {
	this->data.first /= rhs.data.first;
	this->data.second /= rhs.data.second;
	return *this;
      }
    };

    //! Multiply two ArrayObservables component-wise
    template <class T1, class T2>
    const PairObservable<T1,T2> operator*(const PairObservable<T1,T2>& lhs, const PairObservable<T1,T2>& rhs)
    {
      return PairObservable<T1,T2>(lhs) *= rhs;
    }
    //! Divide two ArrayObservables component-wise
    template <class T1, class T2>
    const PairObservable<T1,T2> operator/(const PairObservable<T1,T2>& lhs, const PairObservable<T1,T2>& rhs)
    {
      return PairObservable<T1,T2>(lhs) /= rhs;
    }

    //! Exponentiate the ArrayObservable with a scalar
    template <class T1, class T2, class S>
    const PairObservable<T1,T2> pow(const PairObservable<T1,T2>& base, const S& exponent)
    {
      PairObservable<T1,T2> result;
      result.first = std::pow(base.first, exponent);
      result.second = std::pow(base.second, exponent);

      return result;
    }

    //! Takes the square root of the ArrayObservable component wise
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

      // Specialise tag<> for ArrayObservable
      template <typename T1, typename T2> struct tag<Mocasinns::Observables::PairObservable<T1,T2> >
      {
	typedef PairObservableTag<T1,T2> type;
      };

      // Specify how to devide a ArrayObservable by an integral count
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
