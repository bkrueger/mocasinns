#ifndef GESPINST_SPINS_REAL_SPIN_HPP
#define GESPINST_SPINS_REAL_SPIN_HPP

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
    /*!
     * \details Default constructor, the value of the spin is set to 1 (spin up)
     */
    SpinReal() : _value(1) {}
    //! Constructor giving the value
    /*!
     * \details Constructor with specifying a value for the spin.
     * \param value Value for the spin
     */
    SpinReal(value_type_spin_real value) { set_value(value); }
    
    //! Get-accessor for the value of the spin
    value_type_spin_real get_value() const { return _value; }
    //! Set-accessor for the value of the spin
    void set_value(value_type_spin_real value) { _value = value; }
    
    //! Operator for testing the equality betwen two spins
    /*! 
     * \details Compares two real spins. Two spins are equal if the values are equal
     * \param other Spin to compare with
     */
    bool operator==(const SpinReal& other) const { return (_value == other._value); }
    //! Operator for testing the inequality between two spins
    /*! 
     * \details Compares two Ising spins. Two spins are not equal if the values are not equal
     * \param other Spin to compare with
     */
    bool operator!=(const SpinReal& other) const { return !operator==(other); }
    //! Operator for assigning a value
    /*!
     * \param value SpinReal that should be assigned to this spin
     */
    void operator=(const value_type_spin_real value) { _value = value; }
    //! Friend operator for multiplying to spins to a double
    friend double operator*(const SpinReal& spin_1, const SpinReal& spin_2);

    //! Create a vector with all different values of ising spins
    /*!
     * \details Returns a STL vector of SpinReal that represents all possible values this spin can have. This is a vector two spins, the first spin up and the second spin down with the value of this spin.
     */
    std::vector<SpinReal> all_possible_values() const
    {
      std::vector<SpinReal> result;
      result.push_back(SpinReal(_value));
      result.push_back(SpinReal(-_value));
      return result;
    }
    
    //! Fills the Spin with a value based on a given double between 0 and one
    /*!
     * \details Returns a "random" spin based on the given random double between 0 and 1. The value will be 2*random_number - 1
     * \param random_number Double value between 0 and 1
     */
    void random(double random_number)
    {
      _value = 2*random_number - 1;
    }
    
    //! Returns a spin that has value based on the random number given between 0 and 1, the value differs from the value of this spin
    /*!
     * \details Returns a "random" spin based on the given random double between 0 and 1, but no spin that equals this spin. This spin returns a spin with opposite value
     * \param random_number Double value between 0 and 1 (does not affect the routine
     */
    SpinReal random_differ(double) const
    {
      return SpinReal(-_value);
    }
    
  };
  
  /*!
   * \details Multiplies two Ising spins. The result is the product of the values.
   * \param spin_1 First Ising spin to multiply
   * \param spin_2 Second Ising spin to multiply
   */
  inline double operator*(const SpinReal& spin_1, const SpinReal& spin_2)
  {
    return spin_1._value * spin_2._value;
  }
} // of namespace Ising

#endif
