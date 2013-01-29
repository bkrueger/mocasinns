#ifndef SPIN_POTTS_HPP
#define SPIN_POTTS_HPP

#include <exception>
#include <vector>

// Header for the serialization of the class
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace Ising
{

typedef unsigned int value_type_potts;

/*!
 * \brief Class for Potts spins (integer spins between 0 and a given maximal value).
 * \author Benedikt Krüger
 */
class SpinPotts
{
private:
  //! Internal storage for the value of the spin
  value_type_potts _value;
  //! The maximal value a Potts Spin can have, the minimal value is 0
  value_type_potts _max_value;

  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    ar & _value;
    ar & _max_value;
  }

public:
  //! Standard constructor, sets the value to 1 and the max_value to 1
  SpinPotts();
  //! Constructor giving the value and the max value
  SpinPotts(value_type_potts max_value, value_type_potts value);

  //! Get-accessor for the value of the spin
  value_type_potts get_value() const { return _value; }
  //! Set-accessor for the value of the spin
  void set_value(value_type_potts value) 
  { 
    if (value > _max_value) _value = _max_value;
    else _value = value;
  }
  //! Get-accessor for the maximal value of the spin
  value_type_potts get_max_value() const { return _max_value; }
  //! Set-accessor for the maximal value of the spin
  void set_max_value(value_type_potts max_value)
  {
    _max_value = max_value;
    if (_value > _max_value) _value = max_value;
  }

  //! Operator for testing the equality between two spins
  bool operator==(const SpinPotts& other) const;
  //! Operator for testing the inequality between two spins
  bool operator!=(const SpinPotts& other) const;
  //! Operator for assigning and int to the value
  void operator=(const value_type_potts value);
  //! Friend operator for multiplying to spins to a double
  friend double operator*(const SpinPotts& spin_1, const SpinPotts& spin_2);

  //! Create a vector with all different values of potts spins with this max_value
  std::vector<SpinPotts> all_possible_values() const;

  //! Fills the Spin with a value based on a given double between 0 and one
  void random(double random_number);

  //! Returns a spin that has value based on the random number given between 0 and 1, the value differs from the value of this spin
  SpinPotts random_differ(double random_number) const;
};

} // of namespace Ising

#endif
