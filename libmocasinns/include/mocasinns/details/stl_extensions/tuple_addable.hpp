#ifndef MOCASINNS_DETAILS_STL_EXTENSIONS_TUPLE_ADDABLE_HPP
#define MOCASINNS_DETAILS_STL_EXTENSIONS_TUPLE_ADDABLE_HPP

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/tuple/tuple_io.hpp>

namespace Mocasinns
{
  namespace Details
  { 
    namespace STL_Extensions
    {
      template <class... Types>
      class TupleAddable
      {
      private:
	//! Tuple defining the internal data members
	std::tuple<Types...> data;
	//! Size of the tuples
	const size_t tuple_size = std::tuple_size<decltype(data)>::value;
	
	//! Private subclass to apply a given arithmetic operations to a tuple
	template<template<class,class> class Functor, size_t element_number>
	class apply_arithmetics
	{
	  static void apply(TupleAddable<Types...>& lhs, const TupleAddable<Types...>& rhs)
	  {
	    typedef typename std::tuple_element<element_number, decltype(lhs.data)>::type lhs_type;
	    typedef typename std::tuple_element<element_number, decltype(rhs.data)>::type rhs_type;
	    Functor<lhs_type,rhs_type>(std::get<element_number>(lhs.data), std::get<element_number>(rhs.data));
	    apply_arithmetics<Functor,element_number-1>::apply(lhs, rhs);
	  }

	  template<class S>
	  static void apply(TupleAddable<Types...>& lhs, const S& rhs)
	  {
	    typedef typename std::tuple_element<element_number, decltype(lhs.data)>::type lhs_type;
	    Functor<lhs_type,S>(std::get<element_number>(lhs.data), rhs);
	    apply_arithmetics<Functor,element_number-1>::apply(lhs, rhs);
	  }
	};
	// Overload to break recurssion
	template<template<class,class> class Functor> 
	class apply_arithmetics<Functor, 0>
	{
	  static void apply(TupleAddable<Types...>& lhs, const TupleAddable<Types...>& rhs) { }
	  template<class S>
	  static void apply(TupleAddable<Types...>& lhs, const S& rhs) { }
	};

	//! Private subclass representing an  to a tuple
	template<class Type_LHS, class Type_RHS>
	class arithmetic_add_to
	{
	  void operator()(Type_LHS& value_lhs, const Type_RHS& value_rhs) { value_lhs += value_rhs; }
	};
	//! Private subclass representing an substraction from a tuple
	template<class Type_LHS, class Type_RHS>
	class arithmetic_subtract_from
	{
	  void operator()(Type_LHS& value_lhs, const Type_RHS& value_rhs) { value_lhs -= value_rhs; }
	};
	//! Private subclass representing a multiplication with a tuple
	template<class Type_LHS, class Type_RHS>
	class arithmetic_multiply_with
	{
	  void operator()(Type_LHS& value_lhs, const Type_RHS& value_rhs) { value_lhs *= value_rhs; }
	};
	//! Private subclass representing a division of a tuple by a scalar
	template<class Type_LHS, class Type_RHS>
	class arithmetic_divide_by
	{
	  void operator()(Type_LHS& value_lhs, const Type_RHS& value_rhs) { value_lhs /= value_rhs; }
	};

	//! Private subclass to compare two given Tuples with a comparison operator
	template<template<class> class Functor, bool equality_result, size_t element_number, size_t element_index>
	class apply_comparison
	{
	  static bool compare(TupleAddable<Types...>& lhs, const TupleAddable<Types...>& rhs)
	  {
	    typedef typename std::tuple_element<element_index, decltype(lhs.data)>::type element_type;
	    bool this_element_result = Functor<element_type>(std::get<element_index>(lhs.data), std::get<element_index>(rhs.data));
	    
	    if (this_element_result == false)
	      return false;
	    else
	      return apply_comparison<Functor,equality_result,element_number,element_index+1>::compare(lhs, rhs);
	  }
	};
	// Overload to break recurssion
	template<template<class> class Functor, bool equality_result, size_t element_number> 
	class apply_comparison<Functor, equality_result, element_number, element_number>
	{
	  static bool compare(TupleAddable<Types...>& lhs, const TupleAddable<Types...>& rhs) { return equality_result; }
	};

	//! Private subclass representing a smaller comparison
	template<class Type>
	class comparison_smaller
	{
	  bool operator()(Type& value_lhs, const Type& value_rhs) { return (value_lhs < value_rhs); }
	};
	//! Private subclass representing a greater comparison
	template<class Type>
	class comparison_greater
	{
	  bool operator()(Type& value_lhs, const Type& value_rhs) { return (value_lhs > value_rhs); }
	};

      public:
	//! Default constructor
	TupleAddable() : data() {}
	//! Copy constructor
	TupleAddable(const TupleAddable<Types...>& other) : data(other.data) {}
	//! Initialisation with members
	TupleAddable(const Types&... elements) : data(elements...) {}
	//! Initialisation with tuple
	TupleAddable(const std::tuple<Types...> tup) : data(tup) {}
	
	//! Assignment operator
	TupleAddable& operator=(const TupleAddable<Types...>& other) { data = other.data; }
	
	//! Operator for comparing two tuples for equality
	bool operator==(const TupleAddable<Types...>& other)
	{
	  return (data == other.data);
	}
	//! Operator for comparing two tuples for inequality
	bool operator!=(const TupleAddable<Types...>& other)
	{
	  return !operator==(other);
	}

	//! Operator for comparing two tuples for smaller
	bool operator<(const TupleAddable<Types...>& other)
	{
	  return apply_comparison<comparison_smaller, false, tuple_size, 0>::compare(*this, other);
	}
	//! Operator for comparing two tuples for smaller or equality
	bool operator<=(const TupleAddable<Types...>& other)
	{
	  return apply_comparison<comparison_smaller, true, tuple_size, 0>::compare(*this, other);
	}
	//! Operator for comparing two tuples for greater
	bool operator>(const TupleAddable<Types...>& other)
	{
	  return apply_comparison<comparison_greater, false, tuple_size, 0>::compare(*this, other);
	}
	//! Operator for comparing two tuples for smaller or equality
	bool operator>=(const TupleAddable<Types...>& other)
	{
	  return apply_comparison<comparison_greater, true, tuple_size, 0>::compare(*this, other);
	}

	//! Operator for adding a scalar to this tuple
	template <class S>
	TupleAddable& operator+=(const S& scalar)
	{
	  apply_arithmetics<arithmetic_add_to, tuple_size>::apply(*this, scalar);
	  return *this;
	}
	//! Operator for substracting a scalar from this tuple
	template <class S>
	TupleAddable& operator-=(const S& scalar)
	{
	  apply_arithmetics<arithmetic_subtract_from, tuple_size>::apply(*this, scalar);
	  return *this;
	}
	//! Operator for adding the values of a tuple with the same types to this tuple
	TupleAddable& operator+=(const TupleAddable<Types...>& other)
	{
	  apply_arithmetics<arithmetic_add_to, tuple_size>::apply(*this, other);
	  return *this;
	}
	//! Operator for subtracting the values of a tuple with the same types from this tuple
	TupleAddable& operator-=(const TupleAddable<Types...>& other)
	{
	  apply_arithmetics<arithmetic_subtract_from, tuple_size>::apply(*this, other);
	  return *this;
	}
	//! Operator for multiplying a scalar with this tuple
	template <class S>
	TupleAddable& operator*=(const S& scalar)
	{
	  apply_arithmetics<arithmetic_multiply_with, tuple_size>::apply(*this, scalar);
	  return *this;
	}
	//! Operator for dividing this tuple by a scalar
	template <class S>
	TupleAddable& operator/=(const S& scalar)
	{
	  apply_arithmetics<arithmetic_divide_by, tuple_size>::apply(*this, scalar);
	  return *this;
	}

	template <size_t I> 
	const typename std::tuple_element<I, decltype(data)>::type& get() const
	{
	  return std::get<I>(data);
	}
	template <size_t I>
	typename std::tuple_element<I, decltype(data)>::type& get()
	{
	  return std::get<I>(data);
	}
      };

      //! Binary operator for adding a TupleAddable and a scalar
      template <class... Types, class S>
      const TupleAddable<Types...> operator+(const TupleAddable<Types...> lhs, const S& rhs) { return TupleAddable<Types...>(lhs) += rhs; }
      //! Binary operator for adding a scalar and a TupleAddable
      template <class... Types, class S>
      const TupleAddable<Types...> operator+(const S& lhs, const TupleAddable<Types...> rhs) { return TupleAddable<Types...>(rhs) += lhs; }
      //! Binary operator for adding two TupleAddable
      template <class... Types>
      const TupleAddable<Types...> operator+(const TupleAddable<Types...> lhs, const TupleAddable<Types...>& rhs) { return TupleAddable<Types...>(lhs) += rhs; }

      //! Binary operator for substracting a TupleAddable and a scalar
      template <class... Types, class S>
      const TupleAddable<Types...> operator-(const TupleAddable<Types...> lhs, const S& rhs) { return TupleAddable<Types...>(lhs) -= rhs; }
      //! Binary operator for substracting two TupleAddable
      template <class... Types>
      const TupleAddable<Types...> operator-(const TupleAddable<Types...> lhs, const TupleAddable<Types...>& rhs) { return TupleAddable<Types...>(lhs) -= rhs; }

      //! Binary operator for multiplying a TupleAddable and a scalar
      template <class... Types, class S>
      const TupleAddable<Types...> operator*(const TupleAddable<Types...> lhs, const S& rhs) { return TupleAddable<Types...>(lhs) *= rhs; }
      //! Binary operator for multiplying a scalar and a TupleAddable
      template <class... Types, class S>
      const TupleAddable<Types...> operator*(const S& lhs, const TupleAddable<Types...> rhs) { return TupleAddable<Types...>(rhs) *= lhs; }

      //! Binary operator for dividing a TupleAddable by a scalar
      template <class... Types, class S>
      const TupleAddable<Types...> operator/(const TupleAddable<Types...> lhs, const S& rhs) { return TupleAddable<Types...>(lhs) /= rhs; }
    }
  }
}

namespace std
{
  template <size_t I, class... Types>
  typename std::tuple_element<I, decltype(std::tuple<Types...>()) >::type& get(Mocasinns::Details::STL_Extensions::TupleAddable<Types...>& tpl)
  {
    return tpl.get<I>();
  }
  template <size_t I, class... Types>
  typename std::tuple_element<I, decltype(std::tuple<Types...>()) >::type&& get(Mocasinns::Details::STL_Extensions::TupleAddable<Types...>&& tpl)
  {
    return tpl.get<I>();
  }
  template <size_t I, class... Types>
  typename std::tuple_element<I, decltype(std::tuple<Types...>())>::type const& get(const Mocasinns::Details::STL_Extensions::TupleAddable<Types...>& tpl)
  {
    return tpl.get<I>();
  }
}

#endif
