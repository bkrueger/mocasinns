#ifndef MOCASINNS_ENERGY_TYPES_PAIR_ENERGY_HPP
#define MOCASINNS_ENERGY_TYPES_PAIR_ENERGY_HPP

#include <cmath>

#include "../details/stl_extensions/pair_addable.hpp"

namespace Mocasinns
{
  namespace EnergyTypes
  {
    //! Class representing an pair energy. Can be used like a std::pair and provides (component-wise) addition, substraction and scalar multiplication. For using pairs of different types, use TupleObservables.
    template <class T1, class T2>
    class PairEnergy : public Details::STL_Extensions::PairAddable<T1,T2,PairEnergy<T1,T2> >
    {
    public:
      //! Type of the base class
      typedef Details::STL_Extensions::PairAddable<T1,T2,PairEnergy<T1,T2> > Base;

      //! First entry of the pair
      typedef typename Base::first_type first_type;
      //! Second entry of the pair
      typedef typename Base::second_type second_type;
      //! STL-Pair
      typedef typename Base::data_type data_type;

      //! The default constructor. It uses constructs objects of types T1 and T2 using their default constructors. This constructor may only be used if both T1 and T2 are DefaultConstructible. 
      PairEnergy() : Base() { }
      //! The expand constructor. Constructs a pair so that both entries match the given one.
      template <class T>
      PairEnergy(const T& value) : Base(value) { }
      //! The pair constructor. Constructs a pair such that first is constructed from x and second is constructed from y. 
      PairEnergy(const T1& first, const T2& second) :  Base(first, second) { }
      //! Creates an extended pair from the standard pair
      PairEnergy(const data_type& std_pair) : Base(std_pair) { }
      //! Copy constructor
      template <class Derived>
      PairEnergy(const Details::STL_Extensions::PairAddable<T1, T2, Derived >& pair_addable) : Base(pair_addable) { }
    };

    //! Multiply two ArrayEnergys component-wise
    template <class T1, class T2>
    double operator*(const PairEnergy<T1,T2>& lhs, const PairEnergy<T1,T2>& rhs)
    {
      return (lhs.first * rhs.first) + (lhs.second * rhs.second);
    }
  }
}
#endif
