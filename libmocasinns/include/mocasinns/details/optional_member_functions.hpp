#ifndef MOCASINNS_DETAILS_OPTIONAL_MEMBER_FUNCTIONS_HPP
#define MOCASINNS_DETAILS_OPTIONAL_MEMBER_FUNCTIONS_HPP

// Header for conditional compile based on template parameters
#include <boost/utility/enable_if.hpp>
#include <boost/tti/has_member_function.hpp>

namespace Mocasinns
{
  namespace Details
  {
    BOOST_TTI_HAS_MEMBER_FUNCTION(is_executable)
    BOOST_TTI_HAS_MEMBER_FUNCTION(selection_probability_factor)

    //! Class that provides function for checking whether a type given as function template parameter has a certain member function. If the member function exists, the member function is called, otherwise a default value will be returned
    class OptionalMemberFunctions
    {
    public:
      
      template <class StepType>
      static typename boost::enable_if_c<has_member_function_is_executable<StepType, bool>::value, bool>::type
      optional_is_executable(StepType& step) { return step.is_executable(); }
      
      template <class StepType>
      static typename boost::enable_if_c<!has_member_function_is_executable<StepType, bool>::value, bool>::type
      optional_is_executable(StepType& step) { return true; }
      
      template <class StepType>
      static typename boost::enable_if_c<has_member_function_selection_probability_factor<StepType, double>::value, double>::type
      optional_selection_probability_factor(StepType& step) { return step.selection_probability_factor(); }
      
      template <class StepType>
      static typename boost::enable_if_c<!has_member_function_selection_probability_factor<StepType, double>::value, double>::type
      optional_selection_probability_factor(StepType& step) { return 1.0; }
    };
  }
}

#endif
