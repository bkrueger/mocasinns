#ifndef MOCASINNS_OBSERVABLES_VECTOR_OBSERVABLE_HPP
#define MOCASINNS_OBSERVABLES_VECTOR_OBSERVABLE_HPP

#include <vector>
#include <stdexcept>
#include <cmath>

#include <boost/accumulators/numeric/functional_fwd.hpp>

namespace Mocasinns
{
  namespace Observables
  {
    //! Class representing a vector observable with dynamic range check. Can be used like a std::vector and provides (component-wise) addition, substraction and exponentiation of the observables. For (faster) fixed range check use ArrayObservable or TupleObservable.
    template <class T>
    class VectorObservable
    {
    private:
      //! STL-Vector containing the data
      std::vector<T> data;

    public:
      //! Exception class for non-fitting sizes
      class SizesUnequalException : public std::range_error
      {
      public:
	SizesUnequalException() : std::range_error("The sizes of the VectorObservable-objects do not match.") {}
      };

      //! The type of the object stored in the VectorObservable (T)
      typedef typename std::vector<T>::value_type value_type;
      //! Pointer to T
      typedef typename std::vector<T>::pointer pointer;
      //! Reference to T
      typedef typename std::vector<T>::reference reference;
      //! Const-Reference to T
      typedef typename std::vector<T>::const_reference const_reference;
      //! An unsigned integral type
      typedef typename std::vector<T>::size_type size_type;
      //! A signed integral type
      typedef typename std::vector<T>::difference_type difference_type;
      //! Iterator used to iterate through a VectorObservable
      typedef typename std::vector<T>::iterator iterator;
      //! Const-Iterator used to iterate through a VectorObservable
      typedef typename std::vector<T>::const_iterator const_iterator;
      //! Iterator used to iterate backwards through a VectorObservable
      typedef typename std::vector<T>::reverse_iterator reverse_iterator;
      //! Const-Iterator used to iterate backwards through a VectorObservable
      typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

      //! Creates an empty VectorObservable
      VectorObservable() : data() {}
      //! Creates a VectorObservable with n Elements
      explicit VectorObservable(size_type n) : data(n) {}
      //! Creates a VectorObservable with n copies of t
      VectorObservable(size_type n, const T& t) : data(n,t) {}
      //! Creates a VectorObservable with a std::vector
      VectorObservable(const std::vector<T>& std_vector) : data(std_vector) {}
      //! Copy-Constructor
      VectorObservable(const VectorObservable<T>& other) : data(other.data) {}
      //! Creates a VectorObservable with a copy of a range
      template<class InputIterator>
      VectorObservable(InputIterator first, InputIterator last) : data(first, last) {}

      //! The assignment operator
      VectorObservable<T>& operator=(const VectorObservable<T>& other) 
      { 
	data = other.data;
	return *this;
      }
      //! Returns the n'th element
      reference operator[](size_type n) { return data[n]; }
      //! Returns the n'th element
      const_reference operator[](size_type n) const { return data[n]; }

      template <class TT>
      friend bool operator==(const VectorObservable<TT>&, const VectorObservable<TT>&);
      template <class TT>      
      friend bool operator!=(const VectorObservable<TT>&, const VectorObservable<TT>&);
      template <class TT>
      friend bool operator<(const VectorObservable<TT>&, const VectorObservable<TT>&);
      template <class TT>
      friend bool operator<=(const VectorObservable<TT>&, const VectorObservable<TT>&);
      template <class TT>
      friend bool operator>(const VectorObservable<TT>&, const VectorObservable<TT>&);
      template <class TT>
      friend bool operator>=(const VectorObservable<TT>&, const VectorObservable<TT>&);

      //! Adds a scalar to each component of this VectorObservable
      VectorObservable<T>& operator+=(const T& rhs)
      {
	// Add the scalar component-wise
	for (iterator it_this = data.begin(); it_this != data.end(); ++it_this)
	  (*it_this) += rhs;

	return *this;
      }
      //! Substracts a scalar of each component of this VectorObservable
      VectorObservable<T>& operator-=(const T& rhs)
      {
	// Substract the scalar component-wise
	for (iterator it_this = data.begin(); it_this != data.end(); ++it_this)
	  (*it_this) -= rhs;

	return *this;
      }
      //! Adds a VectorObservable to this VectorObservable
      VectorObservable<T>& operator+=(const VectorObservable<T>& rhs)
      {
	// If the right hand side is empty, do nothing.
	// If this VectorObservable is empty, assign the rhs to this vector and return
	if (rhs.size() == 0)
	  return *this;
	if (this->size() == 0)
	{
	  data = rhs.data;
	  return *this;
	}

	// Test the sizes for other mismatch
	if (data.size() != rhs.size()) throw SizesUnequalException();

	// Add the vectors component-wise
	iterator it_this = data.begin();
	const_iterator it_rhs = rhs.data.begin();
	for (; it_this != data.end(); ++it_this, ++it_rhs)
	  (*it_this) += (*it_rhs);

	return *this;
      }
      //! Substracts a VectorObservable from this VectorObservable
      VectorObservable<T>& operator-=(const VectorObservable<T>& rhs)
      {
	// If the right hand side is empty, do nothing.
	// If this VectorObservable is empty, assign the negative rhs to this vector and return
	if (rhs.size() == 0)
	  return *this;
	if (this->size() == 0)
	{
	  for (const_iterator it_rhs = rhs.data.begin(); it_rhs != rhs.data.end(); ++it_rhs)
	    this->push_back(-(*it_rhs));
	  return *this;
	}

	// Test the sizes for other mismatch
	if (data.size() != rhs.size()) throw SizesUnequalException();

	// Substract the vectors component-wise
	iterator it_this = data.begin();
	const_iterator it_rhs = rhs.data.begin();
	for (; it_this != data.end(); ++it_this, ++it_rhs)
	  (*it_this) -= (*it_rhs);

	return *this;
      }
      //! Multiplies this VectorObservable with a scalar. 
      template<class S>
      VectorObservable<T>& operator*=(const S& rhs)
      {
	for (iterator it = data.begin(); it != data.end(); ++it)
	{
	  (*it) *= rhs;
	}
	return *this;
      }
      //! Devides this VectorObservable by a scalar
      template<class S>
      VectorObservable<T>& operator/=(const S& rhs)
      {
	for (iterator it = data.begin(); it != data.end(); ++it)
	{
	  (*it) /= rhs;
	}
	return *this;
      }
      //! Multiplies this VectorObservable component-wise with another VectorObservable
      VectorObservable<T>& operator*=(const VectorObservable<T>& rhs)
      {
	// Test the sizes
	if (data.size() != rhs.size()) throw SizesUnequalException();

	// Multiply the vectors component-wise
	iterator it_this = data.begin();
	const_iterator it_rhs = rhs.data.begin();
	for (; it_this != data.end(); ++it_this, ++it_rhs)
	  (*it_this) *= (*it_rhs);

	return *this;
      }
      //! Divides this VectorObservable component-wise by another VectorObservable
      VectorObservable<T>& operator/=(const VectorObservable<T>& rhs)
      {
	// Test the sizes
	if (data.size() != rhs.size()) throw SizesUnequalException();

	// Divide the vectors component-wise
	iterator it_this = data.begin();
	const_iterator it_rhs = rhs.data.begin();
	for (; it_this != data.end(); ++it_this, ++it_rhs)
	  (*it_this) /= (*it_rhs);

	return *this;
      }

      template<class TT, class S>
      friend VectorObservable<T>& pow(const VectorObservable<T>&, const S&);

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
      //! Number of elements for which memory has been allocated. capacity() is always greater than or equal to size().
      size_type capacity() const { return data.capacity(); }
      //! True if the VectorObservable's size() is 0
      bool is_empty() const { return data.empty(); }
      //! If n is less than or equal to capacity(), this call has no effect. Otherwise, it is a request for allocation of additional memory. If the request is successful, then capacity() is greater than or equal to n; otherwise, capacity() is unchanged. In either case, size() is unchanged.
      void reserve(size_type s) { data.reserve(s); }
      
      //! Inserts a new element at the end
      void push_back(const T& element) { data.push_back(element); }
      //! Removes the last element
      void pop_back() { data.pop_back(); }
      //! Swaps the contents of two VectorObservables
      void swap(VectorObservable<T>& other) { data.swap(other); }
      //! Inserts x before pos
      iterator insert(iterator pos, const T& x) { data.insert(pos,x); }
      //! Inserts the range [first, last) before pos. 
      template <class InputIterator>
      void insert(iterator pos, InputIterator first, InputIterator last) { data.insert(pos, first, last); }
      //! Insert n copies of x before pos
      void insert(iterator pos, size_type n, const T& x) { data.insert(pos, n, x); }
      
      //! Erases the element at position pos
      iterator erase(iterator pos) { data.erase(pos); }
      //! Erases the range [first, last)
      iterator erase(iterator first, iterator last) { data.erase(first, last); }
      //! Erases all of the elments
      void clear() { data.clear(); }
      //! Inserts or erases elements at the end such that the size becomes n. 
      void resize(size_type n, const T& element = T()) { data.resize(n, element); }
    };

    //! Tests two VectorObservables for equality
    template <class T>
    bool operator==(const VectorObservable<T>& lhs, const VectorObservable<T>& rhs) { return lhs.data == rhs.data; }
    //! Tests two VectorObservables for inequality
    template <class T>
    bool operator!=(const VectorObservable<T>& lhs, const VectorObservable<T>& rhs) { return lhs.data != rhs.data; }

    //! Lexigographic comparison
    template <class T>
    bool operator<(const VectorObservable<T>& lhs, const VectorObservable<T>& rhs) { return lhs.data < rhs.data; }
    //! Lexigographic comparison
    template <class T>
    bool operator<=(const VectorObservable<T>& lhs, const VectorObservable<T>& rhs) { return lhs.data <= rhs.data; }
    //! Lexigographic comparison
    template <class T>
    bool operator>(const VectorObservable<T>& lhs, const VectorObservable<T>& rhs) { return lhs.data > rhs.data; }
    //! Lexigographic comparison
    template <class T>
    bool operator>=(const VectorObservable<T>& lhs, const VectorObservable<T>& rhs) { return lhs.data >= rhs.data; }

    //! Adds a VectorObservable and a scalar
    template <class T>
    const VectorObservable<T> operator+(const VectorObservable<T>& lhs, const T& rhs)
    {
      return VectorObservable<T>(lhs) += rhs;
    }
    //! Adds a scalar and a VectorObservable
    template <class T>
    const VectorObservable<T> operator+(const T& lhs, const VectorObservable<T>& rhs)
    {
      return VectorObservable<T>(rhs) += lhs;
    }
    //! Substracts a VectorObservable and a scalar
    template <class T>
    const VectorObservable<T> operator-(const VectorObservable<T>& lhs, const T& rhs)
    {
      return VectorObservable<T>(lhs) -= rhs;
    }
    //! Adds two vectors
    template <class T>
    const VectorObservable<T> operator+(const VectorObservable<T>& lhs, const VectorObservable<T>& rhs)
    {
      return VectorObservable<T>(lhs) += rhs;
    }
    //! Substracts two vectors
    template <class T>
    const VectorObservable<T> operator-(const VectorObservable<T>& lhs, const VectorObservable<T>& rhs)
    {
      return VectorObservable<T>(lhs) -= rhs;
    }

    //! Multiply a scalar and a ObservableVector
    template <class T, class S>
    const VectorObservable<T> operator*(const S& lhs, const VectorObservable<T>& rhs)
    {
      return VectorObservable<T>(rhs) *= lhs;
    }
    //! Multiply a scalar and a ObservableVector
    template <class T, class S>
    const VectorObservable<T> operator*(const VectorObservable<T>& lhs, const S& rhs)
    {
      return VectorObservable<T>(lhs) *= rhs;
    }
    //! Divide the ObservableVector by a scalar
    template <class T, class S>
    const VectorObservable<T> operator/(const VectorObservable<T>& lhs, const S& rhs)
    {
      return VectorObservable<T>(lhs) /= rhs;
    }

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
