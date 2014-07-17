#ifndef MOCASINNS_HISTOGRAMS_FIXED_BOUNDARY_BINNING_HPP
#define MOCASINNS_HISTOGRAMS_FIXED_BOUNDARY_BINNING_HPP

#include <algorithm>
#include <limits>

#include <boost/serialization/serialization.hpp>

namespace Mocasinns
{
  namespace Histograms
  {
    //! Class for binning with fixed bin boundaries
    /*!
     * \details The boundaries of the bin are given in the constructor and are stored in a vector. The binning of a number \f$ x \f$ is done as following:
     * Let \f$ b_0, b_1, \dots b_{n-1} \f$ the bin boundaries given in the constructor, and set \f$ b_{-1} = -\infty \f$ and \f$ b_{n} = \infty \f$. 
     * Then \f$ \mathrm{binned}(x) = b_i \f$ if \f$ x \geq b_{i-1} \f$ and \f$ x < b_i \f$.
     * 
     * If the resulting value is \f$ b_n = \infty \f$ the binning functor returns <tt>std::numeric_limits<T>::infinity()</tt> if there exists a representation of infinity for the type \c T, otherwise it returns <tt>std::numeric_limits<T>::max()</tt>.
     *
     * \tparam T Type of the number to bin
     * \tparam StrictWeakOrdering The type of a function object taking two values of type T and returning wether the first one is smaller than the second one. The default is to use the < operator.
     */
    template <class T, class StrictWeakOrdering = std::less<T> > 
    class FixedBoundaryBinning
    {
    public:
      //! Constructor for fixed boundary binning
      /*!
       * \tparam InputIterator Iterator type for the range of bin boundaries.
       * \param boundaries_begin Begin of the bin boundaries range
       * \param boundaries_end End of the bin boundaries range
       * \param comp Comparator of type \c StrictWeakOrdering, the default is to use the default \c StrictWeakOrdering() (which is using the operator< for default \c StrictWeakOrdering)
       */
      template <class InputIterator>
      FixedBoundaryBinning(InputIterator boundaries_begin, InputIterator boundaries_end, StrictWeakOrdering comp = StrictWeakOrdering()) : boundaries(boundaries_begin, boundaries_end), comparator(comp)
      {
	std::sort(boundaries.begin(), boundaries.end(), comparator);
      }
      
      //! Get-accessor for the boundaries
      const std::vector<T>& get_boundaries() { return boundaries; }

      //! Functor for binning
      T operator()(const T& value)
      {
	typename std::vector<T>::const_iterator bin_iterator = std::upper_bound(boundaries.begin(), boundaries.end(), value, comparator);
	if (bin_iterator != boundaries.end())
	  return *bin_iterator;
	else
	  if (std::numeric_limits<T>::has_infinity) return std::numeric_limits<T>::infinity();
	  else return std::numeric_limits<T>::max();
      }
    private:
      //! Vector with boundaries
      std::vector<T> boundaries;
      //! Comparator for values
      StrictWeakOrdering comparator;

      friend class boost::serialization::access;
      //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
      template<class Archive> void serialize(Archive & ar, const unsigned int)
      {
	ar & boundaries;
      }

      //! Private default constructor to prohibit default construction
      FixedBoundaryBinning() {}
    };
  }
}

#endif
