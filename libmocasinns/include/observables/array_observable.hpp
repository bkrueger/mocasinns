#ifndef ARRAY_OBSERVABLE_HPP
#define ARRAY_OBSERVABLE_HPP

#include <array>
#include <cmath>

namespace Mocasinns
{
  namespace Observables
  {
    //! Class representing a vector observable with dynamic range check. Can be used like a std::vector and provides (component-wise) addition, substraction and exponentiation of the observables. For (faster) fixed range check use ArrayObservable or TupleObservable.
    template <class T, size_t N>
    class ArrayObservable
    {
    private:
      //! STL-Vector containing the data
      std::array<T,N> data;

    public:
      //! The type of the object stored in the VectorObservable (T)
      typedef typename std::array<T,N>::value_type value_type;
      //! Pointer to T
      typedef typename std::array<T,N>::pointer pointer;
      //! Reference to T
      typedef typename std::array<T,N>::reference reference;
      //! Const-Reference to T
      typedef typename std::array<T,N>::const_reference const_reference;
      //! An unsigned integral type
      typedef typename std::array<T,N>::size_type size_type;
      //! A signed integral type
      typedef typename std::array<T,N>::difference_type difference_type;
      //! Iterator used to iterate through a VectorObservable
      typedef typename std::array<T,N>::iterator iterator;
      //! Const-Iterator used to iterate through a VectorObservable
      typedef typename std::array<T,N>::const_iterator const_iterator;
      //! Iterator used to iterate backwards through a VectorObservable
      typedef typename std::array<T,N>::reverse_iterator reverse_iterator;
      //! Const-Iterator used to iterate backwards through a VectorObservable
      typedef typename std::array<T,N>::const_reverse_iterator const_reverse_iterator;

      //! Creates an empty ArrayObservable
      ArrayObservable() {}
      //! Creates a ArrayObservable with copies of t
      ArrayObservable(const T& t) { data.fill(t); }
      //! Creates a VectorObservable with a std::array
      ArrayObservable(const std::array<T,N>& std_array) { data = std_array; }
      //! Copy-Constructor
      ArrayObservable(const ArrayObservable<T,N>& other) { data = other.data; }

      //! The assignment operator
      ArrayObservable<T,N>& operator=(const ArrayObservable<T,N>& other) { return data.operator=(other); }
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
      //! Adds a VectorObservable to this VectorObservable
      ArrayObservable<T,N>& operator+=(const ArrayObservable<T,N>& rhs)
      {
	// Add the vectors component-wise
	for (unsigned int i = 0; i < N; ++i)
	  data[i] += rhs.data[i];

	return *this;
      }
      //! Substracts a VectorObservable from this VectorObservable
      ArrayObservable<T,N>& operator-=(const ArrayObservable<T,N>& rhs)
      {
	// Substract the vectors component-wise
	for (unsigned int i = 0; i < N; ++i)
	  data[i] -= rhs.data[i];

	return *this;
      }
      //! Multiplies this VectorObservable with a scalar. 
      template <class S>
      ArrayObservable<T,N>& operator*=(const S& rhs)
      {
	// Multiply the vectors component-wise
	for (unsigned int i = 0; i < N; ++i)
	  data[i] *= rhs;

	return *this;
      }
      //! Devides this VectorObservable by a scalar
      template <class S>
      ArrayObservable<T,N>& operator/=(const S& rhs)
      {
	// Devide the vectors component-wise
	for (unsigned int i = 0; i < N; ++i)
	  data[i] /= rhs;

	return *this;
      }

      //! Returns an iterator pointing to the beginning of the VectorObservable
      iterator begin() { return data.begin(); }
      //! Returns an iterator pointing to the end of the VectorObservable
      iterator end() { return data.end(); }
      //! Returns an const_iterator pointing to the beginning of the VectorObservable
      const_iterator begin() const { return data.begin(); }
      //! Returns an const_iterator pointing to the end of the VectorObservable
      const_iterator end() const { return data.end(); }

      //! Returns a reverse_iterator pointing to the beginning of the reversed vector. 
      reverse_iterator rbegin() { return data.rbegin(); }
      //! Returns a reverse_iterator pointing to the end of the reversed vector. 
      reverse_iterator rend() { return data.rend(); }
      //! Returns a const_reverse_iterator pointing to the beginning of the reversed vector. 
      const_reverse_iterator rbegin() const { return data.rbegin(); }
      //! Returns a const_reverse_iterator pointing to the end of the reversed vector. 
      const_reverse_iterator rend() const { return data.rend(); }
      
      //! Returns a reference to the element at position n in the VectorObservable.
      reference at (size_type n) { return data.at(n); }
      //! Returns a const-reference to the element at position n in the VectorObservable.
      const_reference at (size_type n) const { return data.at(n); }
      //! Returns the first element
      reference front() { return data.front(); }
      //! Returns the first element
      const_reference front() const { return data.front(); }
      //! Returns the last element
      reference back() { return data.back(); }
      //! Returns the last element
      const_reference back() const { return data.back(); }

      //! Returns the size of the VectorObservable. 
      size_type size() const { return data.size(); }
      //! Returns the largest possible size of the VectorObservable. 
      size_type max_size() const { return data.max_size(); }
      //! True if the VectorObservable's size() is 0
      bool is_empty() const { return data.empty(); }
      
      //! Swaps the contents of two VectorObservables
      void swap(ArrayObservable<T,N>& other) { data.swap(other.data); }

      template<class TT, class S>
      friend ArrayObservable<T,N>& pow(const ArrayObservable<T,N>&, const S&);
    };

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
  }
}

#endif
