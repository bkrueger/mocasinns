#ifndef SPIN_ISING_HPP
#define SPIN_ISING_HPP

#include <exception>
#include <vector>

// Header for the serialization of the class
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

typedef int value_type_ising;

/*!
 * \brief Class for Ising spins (up and down).
 * \author Benedikt Krüger
 */
class SpinIsing
{
private:
  //! Internal storage for tha value of the spin
  value_type_ising _value;

  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    ar & _value;
  }
  
public:
  //! Standard constructor, sets the value to 1
  SpinIsing();
  //! Constructor giving the value
  SpinIsing(value_type_ising value);

  //! Get-accessor for the value of the spin
  value_type_ising get_value() const { return _value; }
  //! Set-accessor for the value of the spin
  void set_value(value_type_ising value) 
  { 
    if (value >= 0) _value = 1;
    else _value = -1;
  }

  //! Operator for testing the equality between two spins
  bool operator==(const SpinIsing& other) const;
  //! Operator for testing the inequality between two spins
  bool operator!=(const SpinIsing& other) const;
  //! Operator for asigning an integer
  void operator=(const value_type_ising value);
  //! Friend operator for multiplying to spins to a double
  friend double operator*(const SpinIsing& spin_1, const SpinIsing& spin_2);

  //! Create a vector with all different values of ising spins
  std::vector<SpinIsing> all_possible_values() const;

  //! Fills the Spin with a value based on a given double between 0 and one
  void random(double random_number);

  //! Returns a spin that has value based on the random number given between 0 and 1, the value differs from the value of this spin
  SpinIsing random_differ(double random_number) const;
};

#endif
