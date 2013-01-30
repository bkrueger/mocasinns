/**
 * \file metropolis.hpp
 * \brief Class for Metropolis-Monte-Carlo simulations with MoCaSinns
 * 
 * Does Metropolis-Steps and determines statistical averages of observables at given inverse temperatures. Usage examples are found in the test cases.
 * 
 * \author Benedikt Krüger
 */

#ifndef METROPOLIS_HPP
#define METROPOLIS_HPP

#include "simulation.hpp"

// Boost serialization for derived classes
#include <boost/serialization/base_object.hpp>

namespace Mocasinns
{

//! Class for Metropolis-Monte-Carlo simulations
template <class ConfigurationType, class StepType, class RandomNumberGenerator>
class Metropolis : public Simulation<ConfigurationType, RandomNumberGenerator>
{
private:
  //! Member variable for boost serialization
  friend class boost::serialization::access;
  //! Method to serialize this class (omitted version name to avoid unused parameter warnings)
  template<class Archive> void serialize(Archive & ar, const unsigned int)
  {
    // serialize base class information
    ar & boost::serialization::base_object<Simulation<ConfigurationType, RandomNumberGenerator> >(*this);
  }
  
public:
  //! Initialise a Metropolis-MC simulation with default configuration space and default RandomNumberGenerator
  Metropolis() : Simulation<ConfigurationType, RandomNumberGenerator>() {}
  //! Initialise a Metropolis-MC simulation with given configuration space and default RandomNumberGenerator
  Metropolis(ConfigurationType* initial_configuration) : Simulation<ConfigurationType, RandomNumberGenerator>(initial_configuration) {}

  //! Execute a given number of Metropolis-MC steps on the configuration at inverse temperatur beta
  template<class TemperaturType = double>
  void do_metropolis_steps(const uint32_t& number, const TemperaturType& beta);

  //! Load the data of the Metropolis simulation from a serialization stream
  virtual void load_serialize(std::istream& input_stream);
  //! Load the data of the Metropolis simulation from a serialization file
  virtual void load_serialize(const char* filename);
  //! Save the data of the Metropolis simulation to a serialization stream
  virtual void save_serialize(std::ostream& output_stream) const;
  //! Save the data of the Metropolis simulation to a serialization file
  virtual void save_serialize(const char* filename) const;
};

} // of namespace Mocasinns

#include "metropolis.cpp"

#endif
