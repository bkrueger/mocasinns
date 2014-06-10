#ifndef MOCASINNS_RANDOM_BOOST_RANDOM
#define MOCASINNS_RANDOM_BOOST_RANDOM

// Include the interface
#include "boost_random_interface.hpp"

// Boost headers for some random number generators that are predefined
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/ranlux.hpp>
#include <boost/random/lagged_fibonacci.hpp>

namespace Mocasinns
{
  namespace Random
  {
    //! Random number generator using the standard mersenne_twister boost::random::mt19937
    typedef BoostRandomInterface<boost::random::mt19937> Boost_MT19937;
    //! Random number generator using the faster mersenne twister boost::random::mt11213b with shorter period
    typedef BoostRandomInterface<boost::random::mt11213b> Boost_MT11213B;

    //! Random number generator using the boost::random::ranlux3 generator
    typedef BoostRandomInterface<boost::random::ranlux3> Boost_RANLUX3;
    //! Random number generator using the boost::random::ranlux4 generator
    typedef BoostRandomInterface<boost::random::ranlux4> Boost_RANLUX4;

    //! Random number generatore using the boost::lagged_fibonacci607 generator
    typedef BoostRandomInterface<boost::random::lagged_fibonacci607> Boost_LaggedFibonacci607;
    //! Random number generatore using the boost::lagged_fibonacci1279 generator
    typedef BoostRandomInterface<boost::random::lagged_fibonacci1279> Boost_LaggedFibonacci1279;
    //! Random number generatore using the boost::lagged_fibonacci2281 generator
    typedef BoostRandomInterface<boost::random::lagged_fibonacci2281> Boost_LaggedFibonacci2281;
    //! Random number generatore using the boost::lagged_fibonacci3217 generator
    typedef BoostRandomInterface<boost::random::lagged_fibonacci3217> Boost_LaggedFibonacci3217;
    //! Random number generatore using the boost::lagged_fibonacci4423 generator
    typedef BoostRandomInterface<boost::random::lagged_fibonacci4423> Boost_LaggedFibonacci4423;
    //! Random number generatore using the boost::lagged_fibonacci9689 generator
    typedef BoostRandomInterface<boost::random::lagged_fibonacci9689> Boost_LaggedFibonacci9689;
    //! Random number generatore using the boost::lagged_fibonacci19937 generator
    typedef BoostRandomInterface<boost::random::lagged_fibonacci19937> Boost_LaggedFibonacci19937;
    //! Random number generatore using the boost::lagged_fibonacci23209 generator
    typedef BoostRandomInterface<boost::random::lagged_fibonacci23209> Boost_LaggedFibonacci23209;
    //! Random number generatore using the boost::lagged_fibonacci44497 generator
    typedef BoostRandomInterface<boost::random::lagged_fibonacci44497> Boost_LaggedFibonacci44497;
  }
}

#endif
