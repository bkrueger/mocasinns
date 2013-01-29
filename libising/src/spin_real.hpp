#ifndef SPIN_REAL_HPP
#define SPIN_REAL_HPP

#include <exception>
#include <vector>

// Header for the serialization of the class
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace Ising
{

typedef double value_type_spin_real;

/*!
 * \brief Class for double spins (positive float numbers).
 * \author Benedikt Krüger
 */
class SpinReal
{
private:
  //! Internal storage for tha value of the spin
  value_type_spin_real _value;

  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    ar & _value;
  }
  
public:
  //! Standard constructor, sets the value to 1
  SpinReal();
  //! Constructor giving the value
  SpinReal(value_type_spin_real value);

  //! Get-accessor for the value of the spin
  value_type_spin_real get_value() const { return _value; }
  //! Set-accessor for the value of the spin
  void set_value(value_type_spin_real value) { _value = value; }

  //! Operator for testing the equality between two spins
  bool operator==(const SpinReal& other) const;
  //! Operator for testing the inequality between two spins
  bool operator!=(const SpinReal& other) const;
  //! Operator for asigning a valye
  void operator=(const value_type_spin_real value);
  //! Friend operator for multiplying to spins to a double
  friend double operator*(const SpinReal& spin_1, const SpinReal& spin_2);

  //! Create a vector with all different values of ising spins
  std::vector<SpinReal> all_possible_values() const;

  //! Fills the Spin with a value based on a given double between 0 and one
  void random(double random_number);

  //! Returns a spin that has value based on the random number given between 0 and 1, the value differs from the value of this spin
  SpinReal random_differ(double random_number) const;
};

} // of namespace Ising

#endif
