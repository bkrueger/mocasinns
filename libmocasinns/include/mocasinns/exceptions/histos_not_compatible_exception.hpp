#ifndef MOCASINNS_EXCEPTIONS_HISTOS_NOT_COMPATIBLE_EXCEPTION_HPP
#define MOCASINNS_EXCEPTIONS_HISTOS_NOT_COMPATIBLE_EXCEPTION_HPP

#include "mocasinns_exception.hpp"

namespace Mocasinns
{
  namespace Exceptions
  {
    //! Class for an exception occuring if using two non-compatible histograms in an operation
    struct HistosNotCompatibleException : public MocasinnsException
    {
      //! Default constructor for a general message
      HistosNotCompatibleException() : MocasinnsException("The two histograms used are not compatible.") { }
      //! Constructor storing the message of the exception
      HistosNotCompatibleException(std::string exception_message) : MocasinnsException(exception_message) { }
    };
  }
}

#endif
