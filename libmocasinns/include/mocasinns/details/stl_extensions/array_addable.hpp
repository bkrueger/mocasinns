#ifndef MOCASINNS_DETAILS_STL_EXTENSIONS_ARRAY_ADDABLE_HPP
#define MOCASINNS_DETAILS_STL_EXTENSIONS_ARRAY_ADDABLE_HPP

#include <boost/array.hpp>

// Header for the serialization of the class
#include <boost/serialization/array.hpp>

namespace Mocasinns
{
  namespace Details
  {
    namespace STL_Extensions
    {
       //! Class altering an STL-array or and boost::array to be addable. Defines also scalar multiplication and division. The class is used as a base class for ArrayObservable and ArrayEnergy. State "#define MOCASINNS_USE_CPLUSPLUS_11" to use the stl-version. Uses CRTP.
      template <class T, size_t N, class Derived>
      class ArrayAddable
      {
      protected:
	//! Typedef for the inner data structure
#ifdef MOCASINNS_USE_CPLUSPLUS_11
	typedef std::array<T,N> data_type;
#else
	typedef boost::array<T,N> data_type;
#endif

	//! STL-Vector containing the data
	data_type data;

      public:
	//! The type of the object stored in the ArrayAddable (T)
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
	//! Iterator used to iterate through a ArrayAddable
	typedef typename data_type::iterator iterator;
	//! Const-Iterator used to iterate through a ArrayAddable
	typedef typename data_type::const_iterator const_iterator;
	//! Iterator used to iterate backwards through a ArrayAddable
	typedef typename data_type::reverse_iterator reverse_iterator;
	//! Const-Iterator used to iterate backwards through a ArrayAddable
	typedef typename data_type::const_reverse_iterator const_reverse_iterator;
	
	//! Creates an empty ArrayAddable
	ArrayAddable() {}
	//! Creates a ArrayAddable with copies of t
	ArrayAddable(const T& t) { data.fill(t); }
	//! Creates a ArrayAddable with a std::array
	ArrayAddable(const data_type& std_array) { data = std_array; }
	//! Copy-Constructor
	ArrayAddable(const ArrayAddable<T,N,Derived>& other) { data = other.data; }

	//! The assignment operator
	ArrayAddable<T,N,Derived>& operator=(const ArrayAddable<T,N,Derived>& other) 
	{ 
	  data = other.data;
	  return *this;
	}
	//! Returns the n'th element
	reference operator[](size_type n) { return data[n]; }
	//! Returns the n'th element
	const_reference operator[](size_type n) const { return data[n]; }
	//! Test the arrays component-wise for equality
	bool operator==(const ArrayAddable<T,N,Derived>& rhs)
	{
	  for (unsigned int i = 0; i < N; ++i)
	  {
	    if (data[i] != rhs.data[i]) return false;
	  }
	  return true;
	}
	//! Test the arrays component-wise for inequality
	bool operator!=(const ArrayAddable<T,N,Derived>& rhs) { return !operator==(rhs); }
	//! Compare the arrays component-wise, starting with the first entry
	bool operator<(const ArrayAddable<T,N,Derived>& rhs) const
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
	bool operator>(const ArrayAddable<T,N,Derived>& rhs) const
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
	bool operator<=(const ArrayAddable<T,N,Derived>& rhs) const { return !operator>(rhs); }
	//! Compare the arrays component-wise, starting with the first entry
	bool operator>=(const ArrayAddable<T,N,Derived>& rhs) const { return !operator<(rhs); }
	//! Adds a scalar to each component of this ArrayAddable
	Derived& operator+=(const T& rhs)
	{
	  // Add the scalar component-wise
	  for (unsigned int i = 0; i < N; ++i)
	    data[i] += rhs;
	  
	  return *(static_cast<Derived*>(this));
	}
	//! Substracts a scalar of each component of this ArrayAddable
	Derived& operator-=(const T& rhs)
	{
	  // Substracts the scalar component-wise
	  for (unsigned int i = 0; i < N; ++i)
	    data[i] -= rhs;
	  
	  return *(static_cast<Derived*>(this));
	}
	//! Adds a ArrayAddable to this ArrayAddable
	Derived& operator+=(const ArrayAddable<T,N,Derived>& rhs)
	{
	  // Add the vectors component-wise
	  for (unsigned int i = 0; i < N; ++i)
	    data[i] += rhs.data[i];

	  return *(static_cast<Derived*>(this));
	}
	//! Substracts a ArrayAddable from this ArrayAddable
	Derived& operator-=(const ArrayAddable<T,N,Derived>& rhs)
	{
	  // Substract the vectors component-wise
	  for (unsigned int i = 0; i < N; ++i)
	    data[i] -= rhs.data[i];

	  return *(static_cast<Derived*>(this));
	}
	//! Multiplies this ArrayAddable with a scalar. 
	template <class S>
	Derived& operator*=(const S& rhs)
	{
	  // Multiply the vectors component-wise
	  for (unsigned int i = 0; i < N; ++i)
	    data[i] *= rhs;
	  
	  return *(static_cast<Derived*>(this));
	}
	//! Devides this ArrayAddable by a scalar
	template <class S>
	Derived& operator/=(const S& rhs)
	{
	  // Devide the vectors component-wise
	  for (unsigned int i = 0; i < N; ++i)
	    data[i] /= rhs;
	  
	  return *(static_cast<Derived*>(this));
	}

	template <class TT, size_t NN, class DDerived>
	friend std::ostream& operator<<(std::ostream& stream, const ArrayAddable<TT,NN,DDerived>& rhs);
	template <class TT, size_t NN, class DDerived>
	friend std::istream& operator>>(std::istream& stream, ArrayAddable<TT,NN,DDerived>& rhs);

	//! Returns an iterator pointing to the beginning of the ArrayAddable
	iterator begin() { return data.begin(); }
	//! Returns an iterator pointing to the end of the ArrayAddable
	iterator end() { return data.end(); }
	//! Returns an const_iterator pointing to the beginning of the ArrayAddable
	const_iterator begin() const { return data.begin(); }
	//! Returns an const_iterator pointing to the end of the ArrayAddable
	const_iterator end() const { return data.end(); }
	
	//! Returns a reverse_iterator pointing to the beginning of the reversed vector. 
	reverse_iterator rbegin() { return data.rbegin(); }
	//! Returns a reverse_iterator pointing to the end of the reversed vector. 
	reverse_iterator rend() { return data.rend(); }
	//! Returns a const_reverse_iterator pointing to the beginning of the reversed vector. 
	const_reverse_iterator rbegin() const { return data.rbegin(); }
	//! Returns a const_reverse_iterator pointing to the end of the reversed vector. 
	const_reverse_iterator rend() const { return data.rend(); }
      
	//! Returns a reference to the element at position n in the ArrayAddable.
	reference at (size_type n) { return data.at(n); }
	//! Returns a const-reference to the element at position n in the ArrayAddable.
	const_reference at (size_type n) const { return data.at(n); }
	//! Returns the first element
	reference front() { return data.front(); }
	//! Returns the first element
	const_reference front() const { return data.front(); }
	//! Returns the last element
	reference back() { return data.back(); }
	//! Returns the last element
	const_reference back() const { return data.back(); }

	//! Returns the size of the ArrayAddable. 
	size_type size() const { return data.size(); }
	//! Returns the largest possible size of the ArrayAddable. 
	size_type max_size() const { return data.max_size(); }
	//! True if the ArrayAddable's size() is 0
	bool is_empty() const { return data.empty(); }
	
	//! Swaps the contents of two ArrayAddables
	void swap(ArrayAddable<T,N,Derived>& other) { data.swap(other.data); }

      private:
	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive & ar, const unsigned int) 
	{
	  ar & data;
	}
      };
      
      //! Adds an ArrayAddable and a scalar
      template <class T, size_t N, class Derived>
      const Derived operator+(const ArrayAddable<T,N,Derived>& lhs, const T& rhs) { return static_cast<Derived>(ArrayAddable<T,N,Derived>(lhs) += rhs); }
      //! Adds a scalar and an ArrayAddable
      template <class T, size_t N, class Derived>
      const Derived operator+(const T& lhs, const ArrayAddable<T,N,Derived>& rhs) { return static_cast<Derived>(ArrayAddable<T,N,Derived>(rhs) += lhs); }
      //! Substracts an ArrayAddable and a scalar
      template <class T, size_t N, class Derived>
      const Derived operator-(const ArrayAddable<T,N,Derived>& lhs, const T& rhs) { return static_cast<Derived>(ArrayAddable<T,N,Derived>(lhs) -= rhs); }
    
      //! Adds two ArrayAddables
      template <class T, size_t N, class Derived>
      const Derived operator+(const ArrayAddable<T,N,Derived>& lhs, const ArrayAddable<T,N,Derived>& rhs) { return static_cast<Derived>(ArrayAddable<T,N,Derived>(lhs) += rhs); }
      //! Substracts two ArrayAddables
      template <class T, size_t N, class Derived>
      const Derived operator-(const ArrayAddable<T,N,Derived>& lhs, const ArrayAddable<T,N,Derived>& rhs) { return static_cast<Derived>(ArrayAddable<T,N,Derived>(lhs) -= rhs); }

      //! Multiply a scalar and an ArrayAddable
      template <class T, size_t N, class Derived, class S>
      const Derived operator*(const S& lhs, const ArrayAddable<T,N,Derived>& rhs) { return static_cast<Derived>(ArrayAddable<T,N,Derived>(rhs) *= lhs); }
      //! Multiply a scalar and an ArrayAddable
      template <class T, size_t N, class Derived, class S>
      const Derived operator*(const ArrayAddable<T,N,Derived>& lhs, const S& rhs) { return static_cast<Derived>(ArrayAddable<T,N,Derived>(lhs) *= rhs); }
      //! Divide an ArrayAddable by a scalar
      template <class T, size_t N, class Derived, class S>
      const Derived operator/(const ArrayAddable<T,N,Derived>& lhs, const S& rhs) { return static_cast<Derived>(ArrayAddable<T,N,Derived>(lhs) /= rhs); }

      //! Write an addable array to a stream (seperated by spaces)
      template <class T, size_t N, class Derived>
      std::ostream& operator<<(std::ostream& stream, const ArrayAddable<T,N,Derived>& rhs)
      {
	for (unsigned int i = 0; i < N; ++i)
	{
	  stream << rhs[i];
	  if (i != N - 1) 
	    stream << " ";
	}
	return stream;
      }
      //! Read an addable array from a stream
      template <class T, size_t N, class Derived>
      std::istream& operator>>(std::istream& stream, ArrayAddable<T,N,Derived>& rhs)
      {
	for (unsigned int i = 0; i < N; ++i) stream >> rhs[i];

	return stream;
      }

      template <class T, size_t N>
      class ArrayAddablePlain : public ArrayAddable<T,N,ArrayAddablePlain<T,N> >
      {
      public:
	//! Typedef for the base class
	typedef ArrayAddable<T,N,ArrayAddablePlain<T,N> > Base;
	
	//! The type of the object stored in the ArrayAddable (T)
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
	//! Iterator used to iterate through a ArrayAddable
	typedef typename Base::iterator iterator;
	//! Const-Iterator used to iterate through a ArrayAddable
	typedef typename Base::const_iterator const_iterator;
	//! Iterator used to iterate backwards through a ArrayAddable
	typedef typename Base::reverse_iterator reverse_iterator;
	//! Const-Iterator used to iterate backwards through a ArrayAddable
	typedef typename Base::const_reverse_iterator const_reverse_iterator;
	
	//! Creates an empty ArrayAddable
	ArrayAddablePlain() : Base() {}
	//! Creates a ArrayAddable with copies of t
	ArrayAddablePlain(const T& t) : Base(t) {}
	//! Creates a ArrayAddable with a std::array
	ArrayAddablePlain(const typename Base::data_type& std_array) : Base(std_array) {}
	//! Copy-Constructor
	ArrayAddablePlain(const ArrayAddablePlain<T,N>& other) : Base(other) {}
      };
    }
  }
}

#endif
