#ifndef MOCASINNS_DETAILS_STL_EXTENSIONS_PAIR_ADDABLE_HPP
#define MOCASINNS_DETAILS_STL_EXTENSIONS_PAIR_ADDABLE_HPP

#include <map>

// Header for the serialization of the class
#include <boost/serialization/array.hpp>

namespace Mocasinns
{
  namespace Details
  {
    namespace STL_Extensions
    {
      //! Class altering an STL-pair to be addable. Defines also scalar multiplication and division. The class is used as a base class for PairObservable and PairEnergy. Uses CRTP.
      template <class T1, class T2, class Derived>
      class PairAddable
      {
      protected:
	typedef std::pair<T1,T2> data_type;
	
	//! STL-Vector containing the data
	data_type data;
      
      public:
	//! The type of the pair's first component. This is a typedef for the template parameter T1
	typedef T1 first_type;
	//! The type of the pair's first component. This is a typedef for the template parameter T1
	typedef T2 second_type;
	
	//! First entry of the pair
	first_type& first;
	//! Second entry of the pair
	second_type& second;

	//! The default constructor. It uses constructs objects of types T1 and T2 using their default constructors. This constructor may only be used if both T1 and T2 are DefaultConstructible. 
	PairAddable() : data(), first(data.first), second(data.second) { }
	//! The expand constructor. Constructs a pair so that both entries match the given one.
	template <class T>
	PairAddable(const T& value) : data(value, value), first(data.first), second(data.second) { }
	//! The pair constructor. Constructs a pair such that first is constructed from x and second is constructed from y. 
	PairAddable(const T1& first, const T2& second) :  data(first, second), first(data.first), second(data.second) { }
	//! Creates an extended pair from the standard pair
	PairAddable(const data_type& std_pair) : data(std_pair.first, std_pair.second), first(data.first), second(data.second) { }
		
	//! The assignment operator
	PairAddable<T1,T2,Derived>& operator=(const PairAddable<T1,T2,Derived>& other) 
	{ 
	  data.first = other.data.first;
	  data.second = other.data.second;
	  return *this;
	}
	//! Test the pairs component-wise for equality
	bool operator==(const PairAddable<T1,T2,Derived>& rhs) const { return (data == rhs.data); }
	//! Test the pairs component-wise for inequality
	bool operator!=(const PairAddable<T1,T2,Derived>& rhs) const { return !operator==(rhs); }
	//! Compare the pairs component-wise
	bool operator<(const PairAddable<T1,T2,Derived>& rhs) const { return (data < rhs.data); }
	//! Compare the pairs component-wise
	bool operator>(const PairAddable<T1,T2,Derived>& rhs) const { return (data != rhs.data && !(data < rhs.data)); }
	//! Compare the pairs component-wise
	bool operator<=(const PairAddable<T1,T2,Derived>& rhs) const { return !operator>(rhs); }
	//! Compare the pairs component-wise
	bool operator>=(const PairAddable<T1,T2,Derived>& rhs) const { return !operator<(rhs); }
	//! Adds a scalar to each component of this PairAddable
	template <class T>
	Derived& operator+=(const T& rhs)
	{
	  // Add the scalar component-wise
	  data.first += rhs;
	  data.second += rhs;
	  return *(static_cast<Derived*>(this));
	}
	//! Substracts a scalar of each component of this PairAddable
	template <class T>
	Derived& operator-=(const T& rhs)
	{
	  // Substracts the scalar component-wise
	  data.first -= rhs;
	  data.second -= rhs;
	  return *(static_cast<Derived*>(this));
	}
	//! Adds a PairAddable to this PairAddable
	Derived& operator+=(const PairAddable<T1,T2,Derived>& rhs)
	{
	  data.first += rhs.data.first;
	  data.second += rhs.data.second;
	  return *(static_cast<Derived*>(this));
	}
	//! Substracts a PairAddable from this PairAddable
	Derived& operator-=(const PairAddable<T1,T2,Derived>& rhs)
	{
	  data.first -= rhs.data.first;
	  data.second -= rhs.data.second;
	  return *(static_cast<Derived*>(this));
	}
	//! Multiplies this PairAddable with a scalar. 
	template <class S>
	Derived& operator*=(const S& rhs)
	{
	  // Multiply the vectors component-wise
	  data.first *= rhs;
	  data.second *= rhs;
	  return *(static_cast<Derived*>(this));
	}
	//! Devides this PairAddable by a scalar
	template <class S>
	Derived& operator/=(const S& rhs)
	{
	  // Devide the vectors component-wise
	  data.first /= rhs;
	  data.second /= rhs;
	  return *(static_cast<Derived*>(this));
	}
	
	template <class TT1, class TT2, class DDerived>
	friend std::ostream& operator<<(std::ostream& stream, const PairAddable<TT1,TT2,DDerived>& rhs);
	template <class TT1, class TT2, class DDerived>
	friend std::istream& operator>>(std::istream& stream, PairAddable<TT1,TT2,DDerived>& rhs);
	
      private:
	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive & ar, const unsigned int) 
	{
	  ar & data;
	}
      };
      
      //! Adds an PairAddable and a scalar
      template <class T1, class T2, class T, class Derived>
      const Derived operator+(const PairAddable<T1,T2,Derived>& lhs, const T& rhs) { return static_cast<Derived>(PairAddable<T1,T2,Derived>(lhs) += rhs); }
      //! Adds a scalar and an PairAddable
      template <class T1, class T2, class T, class Derived>
      const Derived operator+(const T& lhs, const PairAddable<T1,T2,Derived>& rhs) { return static_cast<Derived>(PairAddable<T1,T2,Derived>(rhs) += lhs); }
      //! Substracts an PairAddable and a scalar
      template <class T1, class T2, class T, class Derived>
      const Derived operator-(const PairAddable<T1,T2,Derived>& lhs, const T& rhs) { return static_cast<Derived>(PairAddable<T1,T2,Derived>(lhs) -= rhs); }
      
      //! Adds two PairAddables
      template <class T1, class T2, class Derived>
      const Derived operator+(const PairAddable<T1,T2,Derived>& lhs, const PairAddable<T1,T2,Derived>& rhs) { return static_cast<Derived>(PairAddable<T1,T2,Derived>(lhs) += rhs); }
      //! Substracts two PairAddables
      template <class T1, class T2, class Derived>
      const Derived operator-(const PairAddable<T1,T2,Derived>& lhs, const PairAddable<T1,T2,Derived>& rhs) { return static_cast<Derived>(PairAddable<T1,T2,Derived>(lhs) -= rhs); }
      
      //! Multiply a scalar and an PairAddable
      template <class T1, class T2, class S, class Derived>
      const Derived operator*(const S& lhs, const PairAddable<T1,T2,Derived>& rhs) { return static_cast<Derived>(PairAddable<T1,T2,Derived>(rhs) *= lhs); }
      //! Multiply a scalar and an PairAddable
      template <class T1, class T2, class S, class Derived>
      const Derived operator*(const PairAddable<T1,T2,Derived>& lhs, const S& rhs) { return static_cast<Derived>(PairAddable<T1,T2,Derived>(lhs) *= rhs); }
      //! Divide an PairAddable by a scalar
      template <class T1, class T2, class S, class Derived>
      const Derived operator/(const PairAddable<T1,T2,Derived>& lhs, const S& rhs) { return static_cast<Derived>(PairAddable<T1,T2,Derived>(lhs) /= rhs); }
      
      //! Write an addable array to a stream (seperated by spaces)
      template <class T1, class T2, class Derived>
      std::ostream& operator<<(std::ostream& stream, const PairAddable<T1,T2,Derived>& rhs)
      {
	stream << rhs.data.first << " " << rhs.data.second;
	return stream;
      }
      //! Read an addable array from a stream
      template <class T1, class T2, class Derived>
      std::istream& operator>>(std::istream& stream, PairAddable<T1,T2,Derived>& rhs)
      {
	stream >> rhs.data.first;
	stream >> rhs.data.second;
	return stream;
      }

      template <class T1, class T2>
      class PairAddablePlain : public PairAddable<T1, T2, PairAddablePlain<T1, T2> >
      {
      public:
	typedef PairAddable<T1, T2, PairAddablePlain<T1, T2> > Base;
	
	//! The default constructor. It uses constructs objects of types T1 and T2 using their default constructors. This constructor may only be used if both T1 and T2 are DefaultConstructible. 
	PairAddablePlain() : Base() { }
	//! The expand constructor. Constructs a pair so that both entries match the given one.
	template <class T>
	PairAddablePlain(const T& value) : Base(value) { }
	//! The pair constructor. Constructs a pair such that first is constructed from x and second is constructed from y. 
	PairAddable(const T1& first, const T2& second) :  Base(first, second) { }
	//! Creates an extended pair from the standard pair
	PairAddable(const data_type& std_pair) : Base(std_pair) { }
      };
    }
  }
}

#endif
