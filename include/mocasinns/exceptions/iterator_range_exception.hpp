#ifndef MOCASINNS_EXCEPTIONS_ITERATOR_RANGE_EXCEPTION_HPP
#define MOCASINNS_EXCEPTIONS_ITERATOR_RANGE_EXCEPTION_HPP

#include "mocasinns_exception.hpp"

namespace Mocasinns
{
  namespace Exceptions
  {
    //! Class for an exception occuring if using a wrong iterator range as parameter
    struct IteratorRangeException : public MocasinnsException
    {
      //! Default constructor for a general message
      IteratorRangeException() : MocasinnsException("Wrong iterator range given as parameter.") { }
      //! Constructor storing the message of the exception
      IteratorRangeException(std::string exception_message) : MocasinnsException(exception_message) { }
    };
  }
}

#endif
