#ifndef MOCASINNS_OBSERVABLES_VECTOR_ENERGY_HPP
#define MOCASINNS_OBSERVABLES_VECTOR_ENERGY_HPP

#include <vector>
#include <stdexcept>
#include <cmath>

#include "../exceptions/unequal_sizes_exception.hpp"
#include "../details/stl_extensions/vector_addable.hpp"

namespace Mocasinns
{
  namespace EnergyTypes
  {
    //! Class representing a multidimensional energy with dynamic range check. Can be used like a std::vector and provides (component-wise) addition, substraction as well as scalar multiplication. For (faster) fixed range check use ArrayEnergy or TupleEnergy.
    template <class T>
    class VectorEnergy : public Details::STL_Extensions::VectorAddable<T, VectorEnergy<T> >
    {
    public:
      //! Base class
      typedef Details::STL_Extensions::VectorAddable<T, VectorEnergy<T> > Base;
      
      //! The type of the object stored in the VectorEnergy (T)
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
      //! Iterator used to iterate through a VectorEnergy
      typedef typename Base::iterator iterator;
      //! Const-Iterator used to iterate through a VectorEnergy
      typedef typename Base::const_iterator const_iterator;
      //! Iterator used to iterate backwards through a VectorEnergy
      typedef typename Base::reverse_iterator reverse_iterator;
      //! Const-Iterator used to iterate backwards through a VectorEnergy
      typedef typename Base::const_reverse_iterator const_reverse_iterator;

      //! Creates an empty VectorEnergy
      VectorEnergy() : Base() {}
      //! Creates a VectorEnergy with n Elements
      explicit VectorEnergy(size_type n) : Base(n) {}
      //! Creates a VectorEnergy with n copies of t
      VectorEnergy(size_type n, const T& t) : Base(n,t) {}
      //! Creates a VectorEnergy with a std::vector
      VectorEnergy(const std::vector<T>& std_vector) : Base(std_vector) {}
      //! Creates a VectorEnergy from a base object
      VectorEnergy(const Base& base_vector) : Base(base_vector) {}
      //! Copy-Constructor
      VectorEnergy(const VectorEnergy<T>& other) : Base(other) {}
      //! Creates a VectorEnergy with a copy of a range
      template<class InputIterator>
      VectorEnergy(InputIterator first, InputIterator last) : Base(first, last) {}
    };

    //! Multiply two VectorEnergy-objects as scalar product
    template <class T>
    const T operator*(const VectorEnergy<T>& lhs, const VectorEnergy<T>& rhs)
    {
      // Test the sizes
      if (lhs.size() != rhs.size()) throw Exceptions::UnequalSizesException("Scalar multiplication of two VectorEnergy objects works only for vectors of the same size.");

      // Multiply the vectors as a scalar product
      T result(0);
      for (typename VectorEnergy<T>::const_iterator it_lhs = lhs.begin(), it_rhs = rhs.begin();
	   it_lhs != lhs.end(); ++it_lhs, ++it_rhs)
	result += (*it_lhs) * (*it_rhs);

      return result;
    }
  }
}

#endif
