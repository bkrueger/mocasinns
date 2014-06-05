#ifndef MOCASINNS_RANDOM_BOOST_RANDOM_INTERFACE
#define MOCASINNS_RANDOM_BOOST_RANDOM_INTERFACE

#include <cstdint>

// Boost headers for distributions
#include <boost/random/uniform_01.hpp>
#include <boost/random/uniform_int_distribution.hpp>

namespace Mocasinns
{
  namespace Random
  {
    //! Class for providing and interface for a boost random number generator to be used as a random number generator in mocasinns.
    /*!
      \tparam BoostRandomNumberGenerator A random number generator that is provided by boost.
    */
    template <class BoostRandomNumberGenerator>
    class BoostRandomInterface
    {
    public:
      //! Typedef for the integer type
      typedef uint32_t RandomIntType;

      BoostRandomInterface()
      {
	rng = new BoostRandomNumberGenerator;
	rng->seed(0);
	
	double_01_distribution = new boost::random::uniform_01<double>();
	int_distribution = new boost::random::uniform_int_distribution<RandomIntType>();
      }
      ~BoostRandomInterface()
      {
	delete int_distribution;
	delete double_01_distribution;

	delete rng;
      }

      //! Set the seed of the random number generator
      void set_seed(const RandomIntType& new_seed) { rng->seed(new_seed); }
      //! Return the minimal integer that is created by \::random_uint32()
      RandomIntType get_int_min() const { return int_distribution->min(); }
      //! Return the maximal integer that is created by \::random_uint32()
      RandomIntType get_int_max() const { return int_distribution->max(); }
      //! Set the maximal integer that is created by \::random_uint32(), the minimal integer is set to 0
      void set_int_max(const RandomIntType& int_max)
      {
	delete int_distribution;
	int_distribution = new boost::random::uniform_int_distribution<RandomIntType>(0, int_max);
      }
      //! Set the range of intergers that is created by \::random_uint32()
      void set_int_range(const RandomIntType& int_min, const RandomIntType& int_max)
      {
	delete int_distribution;
	int_distribution = new boost::random::uniform_int_distribution<RandomIntType>(int_min, int_max);
      }

      //! Create an uniformly distributed double random number between 0 and 1
      double random_double()
      {
	return (*double_01_distribution)(*rng);
      }
      //! Create an uniformly distributed integer in the range [min, max], where min and max are set by the respective accessor functions
      RandomIntType random_uint32()
      {
	return (*int_distribution)(*rng);
      }
      //! Create an uniformly distributed integer in the range [min, max], where min and max are given as parameters
      RandomIntType random_uint32(const RandomIntType& min, const RandomIntType& max)
      {
	boost::random::uniform_int_distribution<RandomIntType> temp_distribution(min, max);
	return temp_distribution(*rng);
      }

    private:
      BoostRandomNumberGenerator* rng;
      
      boost::random::uniform_01<double>* double_01_distribution;
      boost::random::uniform_int_distribution<RandomIntType>* int_distribution;
    };
  }
}

#endif
