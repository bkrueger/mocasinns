#ifndef GESPINST_SPINS_POTTS_SPIN_HPP
#define GESPINST_SPINS_POTTS_SPIN_HPP

#include <exception>
#include <vector>

// Header for the serialization of the class
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace Gespinst
{
  typedef unsigned int value_type_potts;
  
  /*!
   * \brief Class for Potts spins (integer spins between 0 and a given maximal value).
   * \author Benedikt Krüger
   */
  class PottsSpin
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
    /*!
     * \details Default constructor, the value and the maximal value of the spin are set to 1.
     */
    PottsSpin() : _value(1), _max_value(1) {}
    //! Constructor giving the value and the max value
    /*!
     * \details Constructor with specifying a value and the maximal value for the spin. If the value is negative, it will be set to 0, if it is greater then the maximal value, it will be set to the maximal value.
     * \param value Value for the spin
     * \param max_value Maximal value for the spin
     */
    PottsSpin(value_type_potts max_value, value_type_potts value) : _max_value(max_value) { set_value(value); }
    
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
    /*! 
     * \details Compares two Potts spins. Two spins are equal if they have the same value and the same maximal value.
     * \param other Spin to compare with
     */
    bool operator==(const PottsSpin& other) const { return ((_value == other._value) && (_max_value == other._max_value)); }
    //! Operator for testing the inequality between two spins
    /*! 
     * \details Compares two Potts spins. Two spins are not equal if they have different value or different maximal value.
     * \param other Spin to compare with
     */
    bool operator!=(const PottsSpin& other) const { return !operator==(other); }
    //! Operator for assigning and int to the value
    /*!
     * \param value IsingSpin that should be assigned to this spin
     */
    void operator=(const value_type_potts value) { set_value(value); }
    //! Friend operator for multiplying to spins to a double
    friend double operator*(const PottsSpin& spin_1, const PottsSpin& spin_2);

    //! Create a vector with all different values of potts spins with this max_value
    /*!
     * \details Returns a STL vector of PottsSpin that contains all possible values a spin can have. This is a vector with (maximal value + 1) spins, the first having value 0 and the last having the maximal value as value.
     */
    std::vector<PottsSpin> all_possible_values() const
    {
      std::vector<PottsSpin> result;
      for (unsigned int value = 0; value <= _max_value; value++)
	result.push_back(PottsSpin(_max_value, value));
      return result;
    }
    
    //! Fills the Spin with a value based on a given double between 0 and one
    /*!
     * \details Returns a "random" spin based on the given random double between 0 and 1. The value will be between 0 and the maximal value of this spin.
     * \param random_number Double value between 0 and 1
     */
    void random(double random_number)
    {
      if (random_number < 0.5)
	_value = -1;
      else
	_value = 1;
    }
    
    //! Returns a spin that has value based on the random number given between 0 and 1, the value differs from the value of this spin
    /*!
     * \details Returns a "random" spin based on the given random double between 0 and 1, but no spin that equals this spin.
     * \param random_number Double value between 0 and 1
     */
    PottsSpin random_differ(double random_number) const
    {
      // Generate an integer between 0 and (_max_value - 1) from the random double number
      unsigned int random_int = static_cast<unsigned int>(floor(random_number*_max_value));
      
      // If the random int is at or above the current value, return rnd_int + 1
      // If the random ist is below the current value, use the rnd_int
      if (random_int >= _value)
	return PottsSpin(_max_value, random_int + 1);
      else
	return PottsSpin(_max_value, random_int);
    }
  };
  
  /*!
   * \details Multiplies two Potts spins. The result is 1 if the two spins are equal and 0 if the two spins are not equal.
   * \param spin_1 First Potts spin to multiply
   * \param spin_2 Second Potts spin to multiply
   */
  inline double operator*(const PottsSpin& spin_1, const PottsSpin& spin_2)
  {
    if (spin_1._value == spin_2._value)
      return 1;
    else
      return 0;
  }
} // of namespace Ising

#endif
