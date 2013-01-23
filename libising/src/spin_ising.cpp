#include "spin_ising.hpp"

/*!
 * \details Default constructor, the value of the spin is set to 1 (spin up)
 */
SpinIsing::SpinIsing() : _value(1) {}

/*!
 * \details Constructor with specifying a value for the spin. Use a positive value for spin up and a negative value for spin down
 * \param value Value for the spin
 */
SpinIsing::SpinIsing(value_type_ising value)
{
  set_value(value);
}

/*! 
 * \details Compares two Ising spins. Two spins are equal if they are both spin up or both spin down, otherwise they are not equal
 * \param other Spin to compare with
 */
bool SpinIsing::operator==(const SpinIsing& other) const
{
  return (_value == other._value);
}
/*! 
 * \details Compares two Ising spins. Two spins are not equal if one is spin up and one is spin down, otherwise they are equal
 * \param other Spin to compare with
 */
bool SpinIsing::operator!=(const SpinIsing& other) const
{
  return !operator==(other);
}
/*!
 * \param value SpinIsing that should be assigned to this spin
 */
void SpinIsing::operator=(const value_type_ising value)
{
  if (value >= 0) _value = 1;
  else _value = -1;
}

/*!
 * \details Returns a STL vector of SpinIsing that represents all possible values a spin can have. This is a vector two spins, the first spin up and the second spin down.
 */
std::vector<SpinIsing> SpinIsing::all_possible_values() const
{
  std::vector<SpinIsing> result;
  result.push_back(SpinIsing(1));
  result.push_back(SpinIsing(-1));
  return result;
}

/*!
 * \details Returns a "random" spin based on the given random double between 0 and 1. If the double is < 0.5, it will be a spin down, otherwise a spin up.
 * \param random_number Double value between 0 and 1
 */
void SpinIsing::random(double random_number)
{
  if (random_number < 0.5)
    _value = -1;
  else
    _value = 1;
}

/*!
 * \details Returns a "random" spin based on the given random double between 0 and 1, but no spin that equals this spin. This function returns spin up if this spin is down and returns spin down if this spin is up.
 * \param random_number Double value between 0 and 1
 */
SpinIsing SpinIsing::random_differ(double random_number) const
{
  if (_value == 1) return SpinIsing(-1);
  else return SpinIsing(1);
}

/*!
 * \details Multiplies two Ising spins. The result is 1 if the two spins are equal and -1 of the two spins are not equal.
 * \param spin_1 First Ising spin to multiply
 * \param spin_2 Second Ising spin to multiply
 */
double operator*(const SpinIsing& spin_1, const SpinIsing& spin_2)
{
  return spin_1._value * spin_2._value;
}
