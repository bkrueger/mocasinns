/*!
 * \file multi_array_serialize.hpp
 * \brief File for providing serialization to a boost::multi_array
 * \description https://svn.boost.org/svn/boost/sandbox/statistics/detail/multi_array/boost/statistics/detail/multi_array/serialization/
 */

#ifndef MULTI_ARRAY_SERIALIZE_HPP
#define MULTI_ARRAY_SERIALIZE_HPP

#include <boost/multi_array.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_free.hpp>

namespace boost
{ 
  namespace serialization
  {
    //! Function to load a 1D boost::multi-array
    template<class Archive,class T>
    inline void load(Archive & ar,
		     boost::multi_array<T,1> & t,
		     const unsigned int) // file version omitted
    {
      typedef boost::multi_array<T,1> multi_array_;
      typedef typename multi_array_::size_type size_;
        
      size_ n0;
      ar >> BOOST_SERIALIZATION_NVP(n0);
      
      t.resize(boost::extents[n0]);
      ar >> make_array(t.data(), t.num_elements());
    }
    //! Function to load a 2D boost::multi-array
    template<class Archive,class T>
    inline void load(Archive & ar,
		     boost::multi_array<T,2> & t,
		     const unsigned int) // file version omitted
    {
      typedef boost::multi_array<T,2> multi_array_;
      typedef typename multi_array_::size_type size_;
        
      size_ n0;
      ar >> BOOST_SERIALIZATION_NVP(n0);
      size_ n1;
      ar >> BOOST_SERIALIZATION_NVP(n1);
      
      t.resize(boost::extents[n0][n1]);
      ar >> make_array(t.data(), t.num_elements());
    }

    //! Function to save a 1D boost::multi-array
    template<typename Archive,typename T>
    inline void save(Archive & ar,
		     const boost::multi_array<T,1> & t,
		     const unsigned int) // file version omitted
    {
      typedef boost::multi_array<T,1> multi_array_;
      typedef typename multi_array_::size_type size_;
      
      size_ n0 = (t.shape()[0]);
      ar << BOOST_SERIALIZATION_NVP(n0);
      ar << boost::serialization::make_array(t.data(), t.num_elements());
    }
    //! Function to save a 2D boost::multi-array
    template<typename Archive,typename T>
    inline void save(Archive & ar,
		     const boost::multi_array<T,2> & t,
		     const unsigned int) // file version omitted
    {
      typedef boost::multi_array<T,2> multi_array_;
      typedef typename multi_array_::size_type size_;
      
      size_ n0 = (t.shape()[0]);
      ar << BOOST_SERIALIZATION_NVP(n0);
      size_ n1 = (t.shape()[1]);
      ar << BOOST_SERIALIZATION_NVP(n1);
      ar << boost::serialization::make_array(t.data(), t.num_elements());
    }

    //! Function to join load and save function in a serialize function for a 1D boost::multi-array
    template<class Archive,typename T>
    inline void serialize(Archive & ar,
			  boost::multi_array<T,1>& t,
			  const unsigned int file_version) // file version omitted
    {
      split_free(ar, t, file_version);
    }
    //! Function to join load and save function in a serialize function for a 2D boost::multi-array
    template<class Archive,typename T>
    inline void serialize(Archive & ar,
			  boost::multi_array<T,2>& t,
			  const unsigned int file_version) // file version omitted
    {
      split_free(ar, t, file_version);
    }
  }
}

#endif
