#include "random_boost_mt19937.hpp"

Boost_MT19937::Boost_MT19937()
{
  rng = new boost::random::mt19937;
  rng->seed(0);
  
  double_01_distribution = new boost::random::uniform_01<double>();
  int_distribution = new boost::random::uniform_int_distribution<uint32_t>();
}

void Boost_MT19937::set_seed(const uint32_t& new_seed)
{
  rng->seed(new_seed);
}

uint32_t Boost_MT19937::get_int_max() const
{
  return int_distribution->max();
}

void Boost_MT19937::set_int_max(const uint32_t& new_int_max)
{
  delete int_distribution;
  int_distribution = new boost::random::uniform_int_distribution<uint32_t>(0, new_int_max);
}

double Boost_MT19937::random_double()
{
  return (*double_01_distribution)(*rng);
}

uint32_t Boost_MT19937::random_uint32()
{
  return (*int_distribution)(*rng);
}

uint32_t Boost_MT19937::random_uint32(const uint32_t& min, const uint32_t& max)
{
  boost::random::uniform_int_distribution<uint32_t> temp_distribution(min, max);
  return temp_distribution(*rng);
}
