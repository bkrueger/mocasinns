#include "spin_real.hpp"

/*!
 * \details Default constructor, the value of the spin is set to 1 (spin up)
 */
SpinReal::SpinReal() : _value(1) {}

/*!
 * \details Constructor with specifying a value for the spin.
 * \param value Value for the spin
 */
SpinReal::SpinReal(value_type_spin_real value)
{
  set_value(value);
}

/*! 
 * \details Compares two real spins. Two spins are equal if the values are equal
 * \param other Spin to compare with
 */
bool SpinReal::operator==(const SpinReal& other) const
{
  return (_value == other._value);
}
/*! 
 * \details Compares two Ising spins. Two spins are not equal if the values are not equal
 * \param other Spin to compare with
 */
bool SpinReal::operator!=(const SpinReal& other) const
{
  return !operator==(other);
}
/*!
 * \param value SpinReal that should be assigned to this spin
 */
void SpinReal::operator=(const value_type_spin_real value)
{
  _value = value;
}

/*!
 * \details Returns a STL vector of SpinReal that represents all possible values this spin can have. This is a vector two spins, the first spin up and the second spin down with the value of this spin.
 */
std::vector<SpinReal> SpinReal::all_possible_values() const
{
  std::vector<SpinReal> result;
  result.push_back(SpinReal(_value));
  result.push_back(SpinReal(-_value));
  return result;
}

/*!
 * \details Returns a "random" spin based on the given random double between 0 and 1. The value will be 2*random_number - 1
 * \param random_number Double value between 0 and 1
 */
void SpinReal::random(double random_number)
{
  _value = 2*random_number - 1;
}

/*!
 * \details Returns a "random" spin based on the given random double between 0 and 1, but no spin that equals this spin. This spin returns a spin with opposite value
 * \param random_number Double value between 0 and 1 (does not affect the routine
 */
SpinReal SpinReal::random_differ(double random_number) const
{
  return SpinReal(-_value);
}

/*!
 * \details Multiplies two Ising spins. The result is the product of the values.
 * \param spin_1 First Ising spin to multiply
 * \param spin_2 Second Ising spin to multiply
 */
double operator*(const SpinReal& spin_1, const SpinReal& spin_2)
{
  return spin_1._value * spin_2._value;
}
