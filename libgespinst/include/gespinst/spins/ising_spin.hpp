#ifndef GESPINST_SPINS_ISING_SPIN_HPP
#define GESPINST_SPINS_ISING_SPIN_HPP

#include <exception>
#include <vector>

// Header for the serialization of the class
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace Ising
{

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
    /*!
     * \details Default constructor, the value of the spin is set to 1 (spin up)
     */
    SpinIsing() : _value(1) {}
    //! Constructor giving the value
    /*!
     * \details Constructor with specifying a value for the spin. Use a positive value for spin up and a negative value for spin down
     * \param value Value for the spin
     */
    SpinIsing(value_type_ising value) { set_value(value); }
    
    //! Get-accessor for the value of the spin
    value_type_ising get_value() const { return _value; }
    //! Set-accessor for the value of the spin
    void set_value(value_type_ising value) 
    { 
      if (value >= 0) _value = 1;
      else _value = -1;
    }
    
    //! Operator for testing the equality between two spins
    /*! 
     * \details Compares two Ising spins. Two spins are equal if they are both spin up or both spin down, otherwise they are not equal
     * \param other Spin to compare with
     */
    bool operator==(const SpinIsing& other) const { return (_value == other._value); }
    //! Operator for testing the inequality between two spins
    /*! 
     * \details Compares two Ising spins. Two spins are not equal if one is spin up and one is spin down, otherwise they are equal
     * \param other Spin to compare with
     */
    bool operator!=(const SpinIsing& other) const { return !operator==(other); }
    //! Operator for asigning an integer
    /*!
     * \param value SpinIsing that should be assigned to this spin
     */
    void operator=(const value_type_ising value)
    {
      if (value >= 0) _value = 1;
      else _value = -1;
    }
    //! Friend operator for multiplying to spins to a double
    friend double operator*(const SpinIsing& spin_1, const SpinIsing& spin_2);

    //! Create a vector with all different values of ising spins
    /*!
     * \details Returns a STL vector of SpinIsing that represents all possible values a spin can have. This is a vector two spins, the first spin up and the second spin down.
     */
    std::vector<SpinIsing> all_possible_values() const
    {
      std::vector<SpinIsing> result;
      result.push_back(SpinIsing(1));
      result.push_back(SpinIsing(-1));
      return result;
    }
    
    //! Fills the Spin with a value based on a given double between 0 and one
    /*!
     * \details Returns a "random" spin based on the given random double between 0 and 1. If the double is < 0.5, it will be a spin down, otherwise a spin up.
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
     * \details Returns a "random" spin based on the given random double between 0 and 1, but no spin that equals this spin. This function returns spin up if this spin is down and returns spin down if this spin is up.
     * \param random_number Double value between 0 and 1
     */
    SpinIsing random_differ(double) const
    {
      if (_value == 1) return SpinIsing(-1);
      else return SpinIsing(1);
    }
    
  };
  
  /*!
   * \details Multiplies two Ising spins. The result is 1 if the two spins are equal and -1 of the two spins are not equal.
   * \param spin_1 First Ising spin to multiply
   * \param spin_2 Second Ising spin to multiply
   */
  inline double operator*(const SpinIsing& spin_1, const SpinIsing& spin_2)
  {
    return spin_1._value * spin_2._value;
  }
  
} // of namespace Ising

#endif
