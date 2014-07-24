#ifndef MOCASINNS_EXCEPTIONS_WRONG_ENERGY_EXCEPTION_HPP
#define MOCASINNS_EXCEPTIONS_WRONG_ENERGY_EXCEPTION_HPP

#include "mocasinns_exception.hpp"

namespace Mocasinns
{
  namespace Exceptions
  {
    //! Class for an exception occuring if using the wrong energy or a configuration with the wrong energy as parameter
    struct WrongEnergyException : public MocasinnsException
    {
      //! Default constructor for a general message
      WrongEnergyException() : MocasinnsException("Wrong energy given in parameters.") { }
      //! Constructor storing the message of the exception
      WrongEnergyException(std::string exception_message) : MocasinnsException(exception_message) { }
    };
  }
}

#endif
