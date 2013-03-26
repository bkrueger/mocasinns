#ifndef MOCASINNS_OBSERVABLES_VECTOR_OBSERVABLE_HPP
#define MOCASINNS_OBSERVABLES_VECTOR_OBSERVABLE_HPP

#include <vector>
#include <stdexcept>
#include <cmath>

#include <boost/accumulators/numeric/functional_fwd.hpp>

#include "../details/stl_extensions/vector_addable.hpp"

namespace Mocasinns
{
  namespace Observables
  {
    //! Class representing a vector observable with dynamic range check. Can be used like a std::vector and provides (component-wise) addition, substraction and exponentiation of the observables. For (faster) fixed range check use ArrayObservable or TupleObservable.
    template <class T>
    class VectorObservable : public Details::STL_Extensions::VectorAddable<T>
    {
    public:
      //! Base class
      typedef Details::STL_Extensions::VectorAddable<T> Base;
      
      //! The type of the object stored in the VectorObservable (T)
      typedef typename Base::value_type value_type;
      //! Pointer to T
      typedef typename Base::pointer pointer;
      //! Reference to T
      typedef typename Base::reference reference;
      //! Const-Reference to T
      typedef typename Base::const_reference const_reference;
      //! An unsigned integral type
      typedef typename Base::size_type size_type;
      //! A signed integral type
      typedef typename Base::difference_type difference_type;
      //! Iterator used to iterate through a VectorObservable
      typedef typename Base::iterator iterator;
      //! Const-Iterator used to iterate through a VectorObservable
      typedef typename Base::const_iterator const_iterator;
      //! Iterator used to iterate backwards through a VectorObservable
      typedef typename Base::reverse_iterator reverse_iterator;
      //! Const-Iterator used to iterate backwards through a VectorObservable
      typedef typename Base::const_reverse_iterator const_reverse_iterator;

      //! Creates an empty VectorObservable
      VectorObservable() : Base() {}
      //! Creates a VectorObservable with n Elements
      explicit VectorObservable(size_type n) : Base(n) {}
      //! Creates a VectorObservable with n copies of t
      VectorObservable(size_type n, const T& t) : Base(n,t) {}
      //! Creates a VectorObservable with a std::vector
      VectorObservable(const std::vector<T>& std_vector) : Base(std_vector) {}
      //! Creates a VectorObservable from a base object
      VectorObservable(const Base& base_vector) : Base(base_vector) {}
      //! Copy-Constructor
      VectorObservable(const VectorObservable<T>& other) : Base(other) {}
      //! Creates a VectorObservable with a copy of a range
      template<class InputIterator>
      VectorObservable(InputIterator first, InputIterator last) : Base(first, last) {}

      //! Multiplies this VectorObservable component-wise with another VectorObservable
      VectorObservable<T>& operator*=(const VectorObservable<T>& rhs)
      {
	// Test the sizes
	if (this->data.size() != rhs.size()) throw typename Base::SizesUnequalException();

	// Multiply the vectors component-wise
	iterator it_this = this->data.begin();
	const_iterator it_rhs = rhs.data.begin();
	for (; it_this != this->data.end(); ++it_this, ++it_rhs)
	  (*it_this) *= (*it_rhs);

	return *this;
      }
      //! Divides this VectorObservable component-wise by another VectorObservable
      VectorObservable<T>& operator/=(const VectorObservable<T>& rhs)
      {
	// Test the sizes
	if (this->data.size() != rhs.size()) throw typename Base::SizesUnequalException();

	// Divide the vectors component-wise
	iterator it_this = this->data.begin();
	const_iterator it_rhs = rhs.data.begin();
	for (; it_this != this->data.end(); ++it_this, ++it_rhs)
	  (*it_this) /= (*it_rhs);

	return *this;
      }
    };

    //! Multiply two ObservableVectors component-wise
    template <class T>
    const VectorObservable<T> operator*(const VectorObservable<T>& lhs, const VectorObservable<T>& rhs)
    {
      return VectorObservable<T>(lhs) *= rhs;
    }
    //! Divide two ObservableVectors component-wise
    template <class T>
    const VectorObservable<T> operator/(const VectorObservable<T>& lhs, const VectorObservable<T>& rhs)
    {
      return VectorObservable<T>(lhs) /= rhs;
    }

    //! Exponentiate the ObservableVector with a scalar
    template <class T, class S>
    const VectorObservable<T> pow(const VectorObservable<T>& base, const S& exponent)
    {
      VectorObservable<T> result(base.size(), T());
      typename VectorObservable<T>::const_iterator it_base = base.begin();
      typename VectorObservable<T>::iterator it_result = result.begin();
      for (; it_base != base.end(); ++it_base, ++it_result)
      {
	(*it_result) = std::pow(*it_base, exponent);
      }

      return result;
    }

    //! Takes the square root of the Observable vector component wise
    template <class T>
    const VectorObservable<T> sqrt(const VectorObservable<T>& number)
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
      // Tag type for VectorObservable
      template <typename T>
      struct VectorObservableTag;

      // Specialise tag<> for VectorObservable
      template <typename T> struct tag<Mocasinns::Observables::VectorObservable<T> >
      {
	typedef VectorObservableTag<T> type;
      };

      // Specify how to devide a VectorObservable by an integral count
      template <typename Left, typename Right, class T>
      struct average<Left, Right, VectorObservableTag<T>, void>
      {
	// Define the type of the result
	typedef Mocasinns::Observables::VectorObservable<T> result_type;
	
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
