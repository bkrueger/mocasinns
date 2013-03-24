#ifndef MOCASINNS_RANDOM_BOOST_RANDOM
#define MOCASINNS_RANDOM_BOOST_RANDOM

// Include the interface
#include "boost_random_interface.hpp"

// Boost headers for some random number generators that are predefined
#include <boost/random/mersenne_twister.hpp>

namespace Mocasinns
{
  namespace Random
  {
    //! Random number generator using the standard mersenne_twister
    typedef BoostRandomInterface<boost::random::mt19937> Boost_MT19937;
    //! Random number generator using a faster mersenne twister with shorter period
    typedef BoostRandomInterface<boost::random::mt11213b> Boost_MT11213B;
  }
}

#endif
