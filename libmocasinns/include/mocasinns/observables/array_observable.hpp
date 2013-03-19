#ifndef MOCASINNS_OBSERVABLES_ARRAY_OBSERVABLE_HPP
#define MOCASINNS_OBSERVABLES_ARRAY_OBSERVABLE_HPP

#include <boost/array.hpp>
#include <cmath>

#include <boost/accumulators/numeric/functional_fwd.hpp>

namespace Mocasinns
{
  namespace Observables
  {
    //! Class representing a array observable. Can be used like a std::array and provides (component-wise) addition, substraction and exponentiation of the observables. For using arrays of different types, use TupleObservables.
    template <class T, size_t N>
    class ArrayObservable
    {
    private:
      //! Typedef for the inner data structure
#ifdef MOCASINNS_USE_CPLUSPLUS_11
      typedef std::array<T,N> data_type;
#else
      typedef boost::array<T,N> data_type;
#endif

      //! STL-Vector containing the data
      data_type data;

    public:
      //! The type of the object stored in the ArrayObservable (T)
      typedef typename data_type::value_type value_type;
#ifdef MOCASINNS_USE_CPLUSPLUS_11
      //! Pointer to T
      typedef typename data_type::pointer pointer;
#endif
      //! Reference to T
      typedef typename data_type::reference reference;
      //! Const-Reference to T
      typedef typename data_type::const_reference const_reference;
      //! An unsigned integral type
      typedef typename data_type::size_type size_type;
      //! A signed integral type
      typedef typename data_type::difference_type difference_type;
      //! Iterator used to iterate through a ArrayObservable
      typedef typename data_type::iterator iterator;
      //! Const-Iterator used to iterate through a ArrayObservable
      typedef typename data_type::const_iterator const_iterator;
      //! Iterator used to iterate backwards through a ArrayObservable
      typedef typename data_type::reverse_iterator reverse_iterator;
      //! Const-Iterator used to iterate backwards through a ArrayObservable
      typedef typename data_type::const_reverse_iterator const_reverse_iterator;

      //! Creates an empty ArrayObservable
      ArrayObservable() {}
      //! Creates a ArrayObservable with copies of t
      ArrayObservable(const T& t) { data.fill(t); }
      //! Creates a ArrayObservable with a std::array
      ArrayObservable(const data_type& std_array) { data = std_array; }
      //! Copy-Constructor
      ArrayObservable(const ArrayObservable<T,N>& other) { data = other.data; }

      //! The assignment operator
      ArrayObservable<T,N>& operator=(const ArrayObservable<T,N>& other) 
      { 
	data = other.data;
	return *this;
      }
      //! Returns the n'th element
      reference operator[](size_type n) { return data[n]; }
      //! Returns the n'th element
      const_reference operator[](size_type n) const { return data[n]; }
      //! Test the arrays component-wise for equality
      bool operator==(const ArrayObservable<T,N>& rhs)
      {
	for (unsigned int i = 0; i < N; ++i)
	{
	  if (data[i] != rhs.data[i]) return false;
	}
	return true;
      }
      //! Test the arrays component-wise for inequality
      bool operator!=(const ArrayObservable<T,N>& rhs) { return !operator==(rhs); }
      //! Compare the arrays component-wise, starting with the first entry
      bool operator<(const ArrayObservable<T,N>& rhs)
      {
	for (unsigned int i = 0; i < N; ++i)
	{
	  // If there is a difference in the i-th component, return true or false
	  // Otherwise proceed with the next array dimension
	  if (data[i] < rhs.data[i]) return true;
	  if (data[i] > rhs.data[i]) return false;
	}
	// If no difference was found, the arrays are equal
	return false;
      }
      //! Compare the arrays component-wise, starting with the first entry
      bool operator>(const ArrayObservable<T,N>& rhs)
      {
	for (unsigned int i = 0; i < N; ++i)
	{
	  // If there is a difference in the i-th component, return true or false
	  // Otherwise proceed with the next array dimension
	  if (data[i] < rhs.data[i]) return false;
	  if (data[i] > rhs.data[i]) return true;
	}
	// If no difference was found, the arrays are equal
	return false;
      }
      //! Compare the arrays component-wise, starting with the first entry
      bool operator<=(const ArrayObservable<T,N>& rhs) { return !operator>(rhs); }
      //! Compare the arrays component-wise, starting with the first entry
      bool operator>=(const ArrayObservable<T,N>& rhs) { return !operator<(rhs); }
      //! Adds a scalar to each component of this ArrayObservable
      ArrayObservable<T,N>& operator+=(const T& rhs)
      {
	// Add the scalar component-wise
	for (unsigned int i = 0; i < N; ++i)
	  data[i] += rhs;

	return *this;
      }
      //! Substracts a scalar of each component of this ArrayObservable
      ArrayObservable<T,N>& operator-=(const T& rhs)
      {
	// Substracts the scalar component-wise
	for (unsigned int i = 0; i < N; ++i)
	  data[i] -= rhs;

	return *this;
      }
      //! Adds a ArrayObservable to this ArrayObservable
      ArrayObservable<T,N>& operator+=(const ArrayObservable<T,N>& rhs)
      {
	// Add the vectors component-wise
	for (unsigned int i = 0; i < N; ++i)
	  data[i] += rhs.data[i];

	return *this;
      }
      //! Substracts a ArrayObservable from this ArrayObservable
      ArrayObservable<T,N>& operator-=(const ArrayObservable<T,N>& rhs)
      {
	// Substract the vectors component-wise
	for (unsigned int i = 0; i < N; ++i)
	  data[i] -= rhs.data[i];

	return *this;
      }
      //! Multiplies this ArrayObservable with a scalar. 
      template <class S>
      ArrayObservable<T,N>& operator*=(const S& rhs)
      {
	// Multiply the vectors component-wise
	for (unsigned int i = 0; i < N; ++i)
	  data[i] *= rhs;

	return *this;
      }
      //! Devides this ArrayObservable by a scalar
      template <class S>
      ArrayObservable<T,N>& operator/=(const S& rhs)
      {
	// Devide the vectors component-wise
	for (unsigned int i = 0; i < N; ++i)
	  data[i] /= rhs;

	return *this;
      }
      //! Multiply this ArrayObservable component-wise with another ArrayObservable
      ArrayObservable<T,N>& operator*=(const ArrayObservable<T,N>& rhs)
      {
	// Multiply the arrays component-wise
	for (unsigned int i = 0; i < N; ++i)
	  data[i] *= rhs.data[i];

	return *this;
      }
      //! Divide this ArrayObservable component-wise by another ArrayObservable
      ArrayObservable<T,N>& operator/=(const ArrayObservable<T,N>& rhs)
      {
	// Divide the arrays component-wise
	for (unsigned int i = 0; i < N; ++i)
	  data[i] /= rhs.data[i];
	
	return *this;
      }

      //! Returns an iterator pointing to the beginning of the ArrayObservable
      iterator begin() { return data.begin(); }
      //! Returns an iterator pointing to the end of the ArrayObservable
      iterator end() { return data.end(); }
      //! Returns an const_iterator pointing to the beginning of the ArrayObservable
      const_iterator begin() const { return data.begin(); }
      //! Returns an const_iterator pointing to the end of the ArrayObservable
      const_iterator end() const { return data.end(); }

      //! Returns a reverse_iterator pointing to the beginning of the reversed vector. 
      reverse_iterator rbegin() { return data.rbegin(); }
      //! Returns a reverse_iterator pointing to the end of the reversed vector. 
      reverse_iterator rend() { return data.rend(); }
      //! Returns a const_reverse_iterator pointing to the beginning of the reversed vector. 
      const_reverse_iterator rbegin() const { return data.rbegin(); }
      //! Returns a const_reverse_iterator pointing to the end of the reversed vector. 
      const_reverse_iterator rend() const { return data.rend(); }
      
      //! Returns a reference to the element at position n in the ArrayObservable.
      reference at (size_type n) { return data.at(n); }
      //! Returns a const-reference to the element at position n in the ArrayObservable.
      const_reference at (size_type n) const { return data.at(n); }
      //! Returns the first element
      reference front() { return data.front(); }
      //! Returns the first element
      const_reference front() const { return data.front(); }
      //! Returns the last element
      reference back() { return data.back(); }
      //! Returns the last element
      const_reference back() const { return data.back(); }

      //! Returns the size of the ArrayObservable. 
      size_type size() const { return data.size(); }
      //! Returns the largest possible size of the ArrayObservable. 
      size_type max_size() const { return data.max_size(); }
      //! True if the ArrayObservable's size() is 0
      bool is_empty() const { return data.empty(); }
      
      //! Swaps the contents of two ArrayObservables
      void swap(ArrayObservable<T,N>& other) { data.swap(other.data); }

      template<class TT, class S>
      friend ArrayObservable<T,N>& pow(const ArrayObservable<T,N>&, const S&);
    };

    //! Adds an ArrayObservable and a scalar
    template <class T, size_t N>
    const ArrayObservable<T,N> operator+(const ArrayObservable<T,N>& lhs, const T& rhs)
    {
      return ArrayObservable<T,N>(lhs) += rhs;
    }
    //! Adds a scalar and an ArrayObservable
    template <class T, size_t N>
    const ArrayObservable<T,N> operator+(const T& lhs, const ArrayObservable<T,N>& rhs)
    {
      return ArrayObservable<T,N>(rhs) += lhs;
    }
      //! Substracts an ArrayObservable and a scalar
    template <class T, size_t N>
    const ArrayObservable<T,N> operator-(const ArrayObservable<T,N>& lhs, const T& rhs)
    {
      return ArrayObservable<T,N>(lhs) -= rhs;
    }
    
    //! Adds two ArrayObservables
    template <class T, size_t N>
    const ArrayObservable<T,N> operator+(const ArrayObservable<T,N>& lhs, const ArrayObservable<T,N>& rhs)
    {
      return ArrayObservable<T,N>(lhs) += rhs;
    }
    //! Substracts two ArrayObservables
    template <class T, size_t N>
    const ArrayObservable<T,N> operator-(const ArrayObservable<T,N>& lhs, const ArrayObservable<T,N>& rhs)
    {
      return ArrayObservable<T,N>(lhs) -= rhs;
    }

    //! Multiply a scalar and an ArrayObservable
    template <class T, size_t N, class S>
    const ArrayObservable<T,N> operator*(const S& lhs, const ArrayObservable<T,N>& rhs)
    {
      return ArrayObservable<T,N>(rhs) *= lhs;
    }
    //! Multiply a scalar and an ArrayObservable
    template <class T, size_t N, class S>
    const ArrayObservable<T,N> operator*(const ArrayObservable<T,N>& lhs, const S& rhs)
    {
      return ArrayObservable<T,N>(lhs) *= rhs;
    }
    //! Divide an ArrayObservable by a scalar
    template <class T, size_t N, class S>
    const ArrayObservable<T,N> operator/(const ArrayObservable<T,N>& lhs, const S& rhs)
    {
      return ArrayObservable<T,N>(lhs) /= rhs;
    }

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
