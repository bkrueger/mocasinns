#ifndef MOCASINNS_ENERGY_TYPES_ARRAY_ENERGY_HPP
#define MOCASINNS_ENERGY_TYPES_ARRAY_ENERGY_HPP

#include <boost/array.hpp>
#include <cmath>

#include "../details/stl_extensions/array_addable.hpp"

namespace Mocasinns
{
  namespace EnergyTypes
  {
    //! Class representing an array energy. Can be used like a std::array and provides (component-wise) addition, substraction and scalar multiplication. For using arrays of different types, use TupleObservables.
    template <class T, size_t N>
    class ArrayEnergy : public Details::STL_Extensions::ArrayAddable<T,N,ArrayEnergy<T,N> >
    {
    public:
      //! Type of the base class
      typedef Details::STL_Extensions::ArrayAddable<T,N,ArrayEnergy<T,N> > Base;

      //! The type of the object stored in the ArrayEnergy (T)
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
      //! Iterator used to iterate through a ArrayEnergy
      typedef typename Base::iterator iterator;
      //! Const-Iterator used to iterate through a ArrayEnergy
      typedef typename Base::const_iterator const_iterator;
      //! Iterator used to iterate backwards through a ArrayEnergy
      typedef typename Base::reverse_iterator reverse_iterator;
      //! Const-Iterator used to iterate backwards through a ArrayEnergy
      typedef typename Base::const_reverse_iterator const_reverse_iterator;

      //! Creates an empty ArrayEnergy
      ArrayEnergy() : Base() {}
      //! Creates a ArrayEnergy with copies of t
      ArrayEnergy(const T& t) : Base(t) {}
      //! Creates a ArrayEnergy with a std::array
      ArrayEnergy(const typename Base::data_type& std_array) : Base(std_array) {}
      //! Convert from ArrayAddable
      ArrayEnergy(const Base& other) : Base(other) {}
      //! Copy-Constructor
      ArrayEnergy(const ArrayEnergy<T,N>& other) : Base(other) {}
    };

    //! Multiply two ArrayEnergys component-wise
    template <class T, size_t N>
    const T operator*(const ArrayEnergy<T,N>& lhs, const ArrayEnergy<T,N>& rhs)
    {
      T result(0);
      for (unsigned int i = 0; i < N; ++i)
	result += lhs[i]*rhs[i];
      return result;
    }
  }
}
#endif
