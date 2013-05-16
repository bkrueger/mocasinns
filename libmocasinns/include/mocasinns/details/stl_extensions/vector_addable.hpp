#ifndef MOCASINNS_DETAILS_STL_EXTENSIONS_VECTOR_ADDABLE_HPP
#define MOCASINNS_DETAILS_STL_EXTENSIONS_VECTOR_ADDABLE_HPP

#include <vector>
#include <stdexcept>

// Header for the serialization of the class
#include <boost/serialization/vector.hpp>

namespace Mocasinns
{
  namespace Details
  {
    namespace STL_Extensions
    {
      //! Class altering an STL-vector to be addable. Defines also scalar multiplication and division. The class is used as a base class for VectorObservable and VectorEnergy. Uses the CRTP
      template <class T, class Derived>
      class VectorAddable
      {
      protected:
	//! STL-Vector containing the data
	std::vector<T> data;
	
      public:
	//! Exception class for non-fitting sizes
	class SizesUnequalException : public std::range_error
	{
	public:
	  SizesUnequalException() : std::range_error("The sizes of the VectorAddable-objects do not match.") {}
	};

	//! The type of the object stored in the VectorAddable (T)
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
	//! Iterator used to iterate through a VectorAddable
	typedef typename std::vector<T>::iterator iterator;
	//! Const-Iterator used to iterate through a VectorAddable
	typedef typename std::vector<T>::const_iterator const_iterator;
	//! Iterator used to iterate backwards through a VectorAddable
	typedef typename std::vector<T>::reverse_iterator reverse_iterator;
	//! Const-Iterator used to iterate backwards through a VectorAddable
	typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

	//! Creates an empty VectorAddable
	VectorAddable() : data() {}
	//! Creates a VectorAddable with n Elements
	explicit VectorAddable(size_type n) : data(n) {}
	//! Creates a VectorAddable with n copies of t
	VectorAddable(size_type n, const T& t) : data(n,t) {}
	//! Creates a VectorAddable with a std::vector
	VectorAddable(const std::vector<T>& std_vector) : data(std_vector) {}
	//! Copy-Constructor
	VectorAddable(const VectorAddable<T, Derived>& other) : data(other.data) {}
	//! Creates a VectorAddable with a copy of a range
	template<class InputIterator>
	VectorAddable(InputIterator first, InputIterator last) : data(first, last) {}

	//! The assignment operator
	VectorAddable<T, Derived>& operator=(const VectorAddable<T, Derived>& other) 
	{ 
	  data = other.data;
	  return *this;
	}
	//! Returns the n'th element
	reference operator[](size_type n) { return data[n]; }
	//! Returns the n'th element
	const_reference operator[](size_type n) const { return data[n]; }

	template <class TT, class OtherDerived>
	friend bool operator==(const VectorAddable<TT,OtherDerived>&, const VectorAddable<TT,OtherDerived>&);
	template <class TT, class OtherDerived>
	friend bool operator!=(const VectorAddable<TT,OtherDerived>&, const VectorAddable<TT,OtherDerived>&);
	template <class TT, class OtherDerived>
	friend bool operator<(const VectorAddable<TT,OtherDerived>&, const VectorAddable<TT,OtherDerived>&);
	template <class TT, class OtherDerived>
	friend bool operator<=(const VectorAddable<TT,OtherDerived>&, const VectorAddable<TT,OtherDerived>&);
	template <class TT, class OtherDerived>
	friend bool operator>(const VectorAddable<TT,OtherDerived>&, const VectorAddable<TT,OtherDerived>&);
	template <class TT, class OtherDerived>
	friend bool operator>=(const VectorAddable<TT,OtherDerived>&, const VectorAddable<TT,OtherDerived>&);
	
	//! Adds a scalar to each component of this VectorAddable
	Derived& operator+=(const T& rhs)
	{
	  // Add the scalar component-wise
	  for (iterator it_this = data.begin(); it_this != data.end(); ++it_this)
	    (*it_this) += rhs;
	  
	  return *(static_cast<Derived*>(this));
	}
	//! Substracts a scalar of each component of this VectorAddable
	Derived& operator-=(const T& rhs)
	{
	  // Substract the scalar component-wise
	  for (iterator it_this = data.begin(); it_this != data.end(); ++it_this)
	    (*it_this) -= rhs;
	  
	  return *(static_cast<Derived*>(this));
	}
	//! Adds a VectorAddable to this VectorAddable
	Derived& operator+=(const VectorAddable<T, Derived>& rhs)
	{
	  // If the right hand side is empty, do nothing.
	  // If this VectorAddable is empty, assign the rhs to this vector and return
	  if (rhs.size() == 0)
	    return *(static_cast<Derived*>(this));
	  if (this->size() == 0)
	  {
	    data = rhs.data;
	    return *(static_cast<Derived*>(this));
	  }

	  // Test the sizes for other mismatch
	  if (data.size() != rhs.size()) throw SizesUnequalException();
	  
	  // Add the vectors component-wise
	  iterator it_this = data.begin();
	  const_iterator it_rhs = rhs.data.begin();
	  for (; it_this != data.end(); ++it_this, ++it_rhs)
	    (*it_this) += (*it_rhs);
	  
	  return *(static_cast<Derived*>(this));
	}
	//! Substracts a VectorAddable from this VectorAddable
	Derived& operator-=(const VectorAddable<T, Derived>& rhs)
	{
	  // If the right hand side is empty, do nothing.
	  // If this VectorAddable is empty, assign the negative rhs to this vector and return
	  if (rhs.size() == 0)
	    return *(static_cast<Derived*>(this));
	  if (this->size() == 0)
	  {
	    for (const_iterator it_rhs = rhs.data.begin(); it_rhs != rhs.data.end(); ++it_rhs)
	      this->push_back(-(*it_rhs));
	    return *(static_cast<Derived*>(this));
	  }
	  
	  // Test the sizes for other mismatch
	  if (data.size() != rhs.size()) throw SizesUnequalException();

	  // Substract the vectors component-wise
	  iterator it_this = data.begin();
	  const_iterator it_rhs = rhs.data.begin();
	  for (; it_this != data.end(); ++it_this, ++it_rhs)
	    (*it_this) -= (*it_rhs);
	  
	  return *(static_cast<Derived*>(this));
	}
	//! Multiplies this VectorAddable with a scalar. 
	template<class S>
	Derived& operator*=(const S& rhs)
	{
	  for (iterator it = data.begin(); it != data.end(); ++it)
	  {
	    (*it) *= rhs;
	  }
	  return *(static_cast<Derived*>(this));
	}
	//! Devides this VectorAddable by a scalar
	template<class S>
	Derived& operator/=(const S& rhs)
	{
	  for (iterator it = data.begin(); it != data.end(); ++it)
	  {
	    (*it) /= rhs;
	  }
	  return *(static_cast<Derived*>(this));
	}

	//! Returns an iterator pointing to the beginning of the VectorAddable
	iterator begin() { return data.begin(); }
	//! Returns an iterator pointing to the end of the VectorAddable
	iterator end() { return data.end(); }
	//! Returns an const_iterator pointing to the beginning of the VectorAddable
	const_iterator begin() const { return data.begin(); }
	//! Returns an const_iterator pointing to the end of the VectorAddable
	const_iterator end() const { return data.end(); }
	
	//! Returns a reverse_iterator pointing to the beginning of the reversed vector. 
	reverse_iterator rbegin() { return data.rbegin(); }
	//! Returns a reverse_iterator pointing to the end of the reversed vector. 
	reverse_iterator rend() { return data.rend(); }
	//! Returns a const_reverse_iterator pointing to the beginning of the reversed vector. 
	const_reverse_iterator rbegin() const { return data.rbegin(); }
	//! Returns a const_reverse_iterator pointing to the end of the reversed vector. 
	const_reverse_iterator rend() const { return data.rend(); }
	
	//! Returns a reference to the element at position n in the VectorAddable.
	reference at (size_type n) { return data.at(n); }
	//! Returns a const-reference to the element at position n in the VectorAddable.
	const_reference at (size_type n) const { return data.at(n); }
	//! Returns the first element
	reference front() { return data.front(); }
	//! Returns the first element
	const_reference front() const { return data.front(); }
	//! Returns the last element
	reference back() { return data.back(); }
	//! Returns the last element
	const_reference back() const { return data.back(); }

	//! Returns the size of the VectorAddable. 
	size_type size() const { return data.size(); }
	//! Returns the largest possible size of the VectorAddable. 
	size_type max_size() const { return data.max_size(); }
      //! Number of elements for which memory has been allocated. capacity() is always greater than or equal to size().
	size_type capacity() const { return data.capacity(); }
	//! True if the VectorAddable's size() is 0
	bool is_empty() const { return data.empty(); }
	//! If n is less than or equal to capacity(), this call has no effect. Otherwise, it is a request for allocation of additional memory. If the request is successful, then capacity() is greater than or equal to n; otherwise, capacity() is unchanged. In either case, size() is unchanged.
	void reserve(size_type s) { data.reserve(s); }
	
	//! Inserts a new element at the end
	void push_back(const T& element) { data.push_back(element); }
	//! Removes the last element
	void pop_back() { data.pop_back(); }
	//! Swaps the contents of two VectorAddables
	void swap(VectorAddable<T, Derived>& other) { data.swap(other); }
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

      private:
	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive & ar, const unsigned int) 
	{
	  ar & data;
	}
      };
      
      //! Tests two VectorAddables for equality
      template <class T, class Derived>
      bool operator==(const VectorAddable<T,Derived>& lhs, const VectorAddable<T,Derived>& rhs) { return lhs.data == rhs.data; }
      //! Tests two VectorAddables for inequality
      template <class T, class Derived>
      bool operator!=(const VectorAddable<T,Derived>& lhs, const VectorAddable<T,Derived>& rhs) { return lhs.data != rhs.data; }
      
      //! Lexigographic comparison
      template <class T, class Derived>
      bool operator<(const VectorAddable<T,Derived>& lhs, const VectorAddable<T,Derived>& rhs) { return lhs.data < rhs.data; }
      //! Lexigographic comparison
      template <class T, class Derived>
      bool operator<=(const VectorAddable<T,Derived>& lhs, const VectorAddable<T,Derived>& rhs) { return lhs.data <= rhs.data; }
      //! Lexigographic comparison
      template <class T, class Derived>
      bool operator>(const VectorAddable<T,Derived>& lhs, const VectorAddable<T,Derived>& rhs) { return lhs.data > rhs.data; }
      //! Lexigographic comparison
      template <class T, class Derived>
      bool operator>=(const VectorAddable<T,Derived>& lhs, const VectorAddable<T,Derived>& rhs) { return lhs.data >= rhs.data; }

      //! Adds a VectorAddable and a scalar
      template <class T, class Derived>
      const Derived operator+(const VectorAddable<T,Derived>& lhs, const T& rhs)
      {
	return static_cast<Derived>(VectorAddable<T,Derived>(lhs) += rhs);
      }
      //! Adds a scalar and a VectorAddable
      template <class T, class Derived>
      const Derived operator+(const T& lhs, const VectorAddable<T,Derived>& rhs)
      {
	return static_cast<Derived>(VectorAddable<T,Derived>(rhs) += lhs);
      }
      //! Substracts a VectorAddable and a scalar
      template <class T, class Derived>
      const Derived operator-(const VectorAddable<T,Derived>& lhs, const T& rhs)
      {
	return static_cast<Derived>(VectorAddable<T,Derived>(lhs) -= rhs);
      }
      //! Adds two vectors
      template <class T, class Derived>
      const Derived operator+(const VectorAddable<T,Derived>& lhs, const VectorAddable<T,Derived>& rhs)
      {
	return static_cast<Derived>(VectorAddable<T,Derived>(lhs) += rhs);
      }
      //! Substracts two vectors
      template <class T, class Derived>
      const Derived operator-(const VectorAddable<T,Derived>& lhs, const VectorAddable<T,Derived>& rhs)
      {
	return static_cast<Derived>(VectorAddable<T,Derived>(lhs) -= rhs);
      }
      
      //! Multiply a scalar and a ObservableVector
      template <class T, class Derived, class S>
      const Derived operator*(const S& lhs, const VectorAddable<T,Derived>& rhs)
      {
	return static_cast<Derived>(VectorAddable<T,Derived>(rhs) *= lhs);
      }
      //! Multiply a scalar and a ObservableVector
      template <class T, class Derived, class S>
      const Derived operator*(const VectorAddable<T,Derived>& lhs, const S& rhs)
      {
	return static_cast<Derived>(VectorAddable<T,Derived>(lhs) *= rhs);
      }
      //! Divide the ObservableVector by a scalar
      template <class T, class Derived, class S>
      const Derived operator/(const VectorAddable<T,Derived>& lhs, const S& rhs)
      {
	return static_cast<Derived>(VectorAddable<T,Derived>(lhs) /= rhs);
      }

      //! Class representing a plain VectorAddable that is not used as a base class
      template <class T>
      class VectorAddablePlain : public VectorAddable<T, VectorAddablePlain<T> >
      {
      public:
	//! Definition for the base class
	typedef VectorAddable<T, VectorAddablePlain> Base;

	//! The type of the object stored in the VectorAddable (T)
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
	//! Iterator used to iterate through a VectorAddable
	typedef typename Base::iterator iterator;
	//! Const-Iterator used to iterate through a VectorAddable
	typedef typename Base::const_iterator const_iterator;
	//! Iterator used to iterate backwards through a VectorAddable
	typedef typename Base::reverse_iterator reverse_iterator;
	//! Const-Iterator used to iterate backwards through a VectorAddable
	typedef typename Base::const_reverse_iterator const_reverse_iterator;

	//! Creates an empty VectorAddable
	VectorAddablePlain() : Base() {}
	//! Creates a VectorAddable with n Elements
	explicit VectorAddablePlain(size_type n) : Base(n) {}
	//! Creates a VectorAddable with n copies of t
	VectorAddablePlain(size_type n, const T& t) : Base(n,t) {}
	//! Creates a VectorAddable with a std::vector
	VectorAddablePlain(const std::vector<T>& std_vector) : Base(std_vector) {}
	//! Copy-Constructor
	VectorAddablePlain(const VectorAddablePlain<T>& other) : Base(other.data) {}
	//! Creates a VectorAddable with a copy of a range
	template<class InputIterator>
	VectorAddablePlain(InputIterator first, InputIterator last) : Base(first, last) {}
      };
       
    }
  }
}

#endif
