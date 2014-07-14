#ifndef MOCASINNS_OBSERVABLES_ARRAY_OBSERVABLE_HPP
#define MOCASINNS_OBSERVABLES_ARRAY_OBSERVABLE_HPP

#include <boost/array.hpp>
#include <cmath>

#include <boost/accumulators/numeric/functional_fwd.hpp>

#include "../details/stl_extensions/array_addable.hpp"

namespace Mocasinns
{
  namespace Observables
  {
    //! Class representing a array observable. Can be used like a std::array and provides (component-wise) addition, substraction and exponentiation of the observables. For using arrays of different types, use TupleObservables.
    template <class T, size_t N>
    class ArrayObservable : public Details::STL_Extensions::ArrayAddable<T,N,ArrayObservable<T,N> >
    {
    public:
      //! Type of the base class
      typedef Details::STL_Extensions::ArrayAddable<T,N,ArrayObservable<T,N> > Base;

      //! The type of the object stored in the ArrayObservable (T)
      typedef typename Base::value_type value_type;
#ifdef MOCASINNS_USE_CPLUSPLUS_11
      //! Pointer to T
      typedef typename Base::pointer pointer;
#endif
      //! Reference to T
      typedef typename Base::reference reference;
      //! Const-Reference to T
      typedef typename Base::const_reference const_reference;
      //! An unsigned integral type
      typedef typename Base::size_type size_type;
      //! A signed integral type
      typedef typename Base::difference_type difference_type;
      //! Iterator used to iterate through a ArrayObservable
      typedef typename Base::iterator iterator;
      //! Const-Iterator used to iterate through a ArrayObservable
      typedef typename Base::const_iterator const_iterator;
      //! Iterator used to iterate backwards through a ArrayObservable
      typedef typename Base::reverse_iterator reverse_iterator;
      //! Const-Iterator used to iterate backwards through a ArrayObservable
      typedef typename Base::const_reverse_iterator const_reverse_iterator;

      //! Creates an empty ArrayObservable
      ArrayObservable() : Base() {}
      //! Creates a ArrayObservable with copies of t
      ArrayObservable(const T& t) : Base(t) {}
      //! Creates a ArrayObservable with a std::array
      ArrayObservable(const typename Base::data_type& std_array) : Base(std_array) {}
      //! Convert from ArrayAddable
      ArrayObservable(const Base& other) : Base(other) {}
      //! Copy-Constructor
      ArrayObservable(const ArrayObservable<T,N>& other) : Base(other) {}

      //! Multiply this ArrayObservable component-wise with another ArrayObservable
      ArrayObservable<T,N>& operator*=(const ArrayObservable<T,N>& rhs)
      {
	// Multiply the arrays component-wise
	for (unsigned int i = 0; i < N; ++i)
	  this->data[i] *= rhs.data[i];

	return *this;
      }
      //! Divide this ArrayObservable component-wise by another ArrayObservable
      ArrayObservable<T,N>& operator/=(const ArrayObservable<T,N>& rhs)
      {
	// Divide the arrays component-wise
	for (unsigned int i = 0; i < N; ++i)
	  this->data[i] /= rhs.data[i];
	
	return *this;
      }
    };

    //! Multiply two ArrayObservables component-wise
    template <class T, size_t N>
    const ArrayObservable<T,N> operator*(const ArrayObservable<T,N>& lhs, const ArrayObservable<T,N>& rhs)
    {
      return ArrayObservable<T,N>(lhs) *= rhs;
    }
    //! Divide two ArrayObservables component-wise
    template <class T, size_t N>
    const ArrayObservable<T,N> operator/(const ArrayObservable<T,N>& lhs, const ArrayObservable<T,N>& rhs)
    {
      return ArrayObservable<T,N>(lhs) /= rhs;
    }

    //! Exponentiate the ArrayObservable with a scalar
    template <class T, size_t N, class S>
    const ArrayObservable<T,N> pow(const ArrayObservable<T,N>& base, const S& exponent)
    {
      ArrayObservable<T,N> result;
      typename ArrayObservable<T,N>::const_iterator it_base = base.begin();
      typename ArrayObservable<T,N>::iterator it_result = result.begin();
      for (; it_base != base.end(); ++it_base, ++it_result)
      {
	(*it_result) = std::pow(*it_base, exponent);
      }

      return result;
    }

    //! Takes the square root of the ArrayObservable component wise
    template <class T, size_t N>
    const ArrayObservable<T,N> sqrt(const ArrayObservable<T,N>& number)
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
      // Tag type for ArrayObservable
      template <typename T, size_t N>
      struct ArrayObservableTag;

      // Specialise tag<> for ArrayObservable
      template <typename T, size_t N> struct tag<Mocasinns::Observables::ArrayObservable<T,N> >
      {
	typedef ArrayObservableTag<T,N> type;
      };

      // Specify how to devide a ArrayObservable by an integral count
      template <typename Left, typename Right, class T, size_t N>
      struct average<Left, Right, ArrayObservableTag<T,N>, void>
      {
	// Define the type of the result
	typedef Mocasinns::Observables::ArrayObservable<T,N> result_type;
	
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
