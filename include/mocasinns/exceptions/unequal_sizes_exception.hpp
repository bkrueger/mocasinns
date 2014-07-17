#ifndef MOCASINNS_EXCEPTIONS_UNEQUAL_SIZES_EXCEPTION_HPP
#define MOCASINNS_EXCEPTIONS_UNEQUAL_SIZES_EXCEPTION_HPP

#include "mocasinns_exception.hpp"

namespace Mocasinns
{
  namespace Exceptions
  {
    //! Class for an exception occuring if using containers with unequal sizes as function arguments
    struct UnequalSizesException : public MocasinnsException
    {
      //! Default constructor for a general message
      UnequalSizesException() : MocasinnsException("Containers with unequal sizes given as parameter.") { }
      //! Constructor storing the message of the exception
      UnequalSizesException(std::string exception_message) : MocasinnsException(exception_message) { }
    };
  }
}

#endif
