#include "spin_potts.hpp"

#include <cmath>
#include <iostream>

namespace Ising
{

/*!
 * \details Default constructor, the value and the maximal value of the spin are set to 1.
 */
SpinPotts::SpinPotts() : _value(1), _max_value(1) {}

/*!
 * \details Constructor with specifying a value and the maximal value for the spin. If the value is negative, it will be set to 0, if it is greater then the maximal value, it will be set to the maximal value.
 * \param value Value for the spin
 * \param max_value Maximal value for the spin
 */
SpinPotts::SpinPotts(value_type_potts max_value, value_type_potts value) : _max_value(max_value)
{
  set_value(value);
}

/*! 
 * \details Compares two Potts spins. Two spins are equal if they have the same value and the same maximal value.
 * \param other Spin to compare with
 */
bool SpinPotts::operator==(const SpinPotts& other) const
{
  return ((_value == other._value) && (_max_value == other._max_value));
}
/*! 
 * \details Compares two Potts spins. Two spins are not equal if they have different value or different maximal value.
 * \param other Spin to compare with
 */
bool SpinPotts::operator!=(const SpinPotts& other) const
{
  return !operator==(other);
}
/*!
 * \param value SpinIsing that should be assigned to this spin
 */
void SpinPotts::operator=(const value_type_potts value)
{
  set_value(value);
}

/*!
 * \details Returns a STL vector of SpinPotts that contains all possible values a spin can have. This is a vector with (maximal value + 1) spins, the first having value 0 and the last having the maximal value as value.
 */
std::vector<SpinPotts> SpinPotts::all_possible_values() const
{
  std::vector<SpinPotts> result;
  for (unsigned int value = 0; value <= _max_value; value++)
    result.push_back(SpinPotts(_max_value, value));
  return result;
}

/*!
 * \details Returns a "random" spin based on the given random double between 0 and 1. The value will be between 0 and the maximal value of this spin.
 * \param random_number Double value between 0 and 1
 */
void SpinPotts::random(double random_number)
{
  if (random_number < 0.5)
    _value = -1;
  else
    _value = 1;
}

/*!
 * \details Returns a "random" spin based on the given random double between 0 and 1, but no spin that equals this spin.
 * \param random_number Double value between 0 and 1
 */
SpinPotts SpinPotts::random_differ(double random_number) const
{
  // Generate an integer between 0 and (_max_value - 1) from the random double number
  unsigned int random_int = static_cast<unsigned int>(floor(random_number*_max_value));
  
  // If the random int is at or above the current value, return rnd_int + 1
  // If the random ist is below the current value, use the rnd_int
  if (random_int >= _value)
    return SpinPotts(_max_value, random_int + 1);
  else
    return SpinPotts(_max_value, random_int);
}

/*!
 * \details Multiplies two Potts spins. The result is 1 if the two spins are equal and 0 if the two spins are not equal.
 * \param spin_1 First Potts spin to multiply
 * \param spin_2 Second Potts spin to multiply
 */
double operator*(const SpinPotts& spin_1, const SpinPotts& spin_2)
{
  if (spin_1._value == spin_2._value)
    return 1;
  else
    return 0;
}

} // of namespace Ising
