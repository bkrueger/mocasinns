#include <cstdint>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#ifndef RNG_BOOST_MT
#define RNG_BOOST_MT

class Boost_MT19937
{
private:
  boost::random::mt19937* rng;
  
  boost::random::uniform_01<double>* double_01_distribution;
  boost::random::uniform_int_distribution<uint32_t>* int_distribution;
public:
  Boost_MT19937();

  void set_seed(const uint32_t&);
  uint32_t get_int_max() const;
  void set_int_max(const uint32_t&);
  
  double random_double();
  uint32_t random_uint32();
  uint32_t random_uint32(const uint32_t& min, const uint32_t& max);
};

#endif
