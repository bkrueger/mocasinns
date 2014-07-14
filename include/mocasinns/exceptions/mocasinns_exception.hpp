#ifndef MOCASINNS_EXCEPTIONS_MOCASINNS_EXCEPTION_HPP
#define MOCASINNS_EXCEPTIONS_MOCASINNS_EXCEPTION_HPP

#include <exception>
#include <string>

namespace Mocasinns
{
  namespace Exceptions
  {
    //! Base class for all exceptions found in mocasinns
    class MocasinnsException : public std::exception
    {
    public:
      //! Constructor storing the message of the exception
      MocasinnsException(std::string exception_message) : std::exception(), message(exception_message) { }

      //! Give the description of the error
      const char* what() const throw () { return message.c_str(); }
      
    private:
      std::string message;
    };
  }
}

#endif
