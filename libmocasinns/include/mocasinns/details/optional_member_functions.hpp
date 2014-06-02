#ifndef MOCASINNS_DETAILS_OPTIONAL_MEMBER_FUNCTIONS_HPP
#define MOCASINNS_DETAILS_OPTIONAL_MEMBER_FUNCTIONS_HPP

// Header for conditional compile based on template parameters
#include <boost/utility/enable_if.hpp>
#include <boost/tti/has_function.hpp>
#include <boost/tti/has_data.hpp>

namespace Mocasinns
{
  namespace Details
  {
    BOOST_TTI_HAS_FUNCTION(is_executable)
    BOOST_TTI_HAS_FUNCTION(selection_probability_factor)
    BOOST_TTI_HAS_FUNCTION(is_serializable)

    //! Class that provides function for checking whether a type given as function template parameter has a certain member function. If the member function exists, the member function is called, otherwise a default value will be returned
    class OptionalMemberFunctions
    {
    public:

      //! /cond
      template <class StepType>
      static typename boost::enable_if_c<has_function_is_executable<StepType, bool>::value, bool>::type
      optional_is_executable(StepType& step) { return step.is_executable(); }
      template <class StepType>
      static typename boost::enable_if_c<!has_function_is_executable<StepType, bool>::value, bool>::type
      optional_is_executable(StepType&) { return true; }
      //! /endcond

      //! /cond
      template <class ConfigurationType>
      static typename boost::enable_if_c<has_function_is_serializable<ConfigurationType, bool>::value, bool>::type
      optional_is_serializable(ConfigurationType& configuration) { return configuration.is_serializable; }
      template <class ConfigurationType>
      static typename boost::enable_if_c<!has_function_is_serializable<ConfigurationType, bool>::value, bool>::type
      optional_is_serializable(ConfigurationType&) { return false; }
      //! /endcond

      //! /cond
      template <class StepType>
      static typename boost::enable_if_c<has_function_selection_probability_factor<StepType, double>::value, double>::type
      optional_selection_probability_factor(StepType& step) { return step.selection_probability_factor(); }
      template <class StepType>
      static typename boost::enable_if_c<!has_function_selection_probability_factor<StepType, double>::value, double>::type
      optional_selection_probability_factor(StepType&) { return 1.0; }
      //! /endcond

      // Code only visible for doxygen
      // Doxygen cannot deal with the enable-if structure
      #ifdef MOCASINNS_DOXYGEN_DOCUMENTATION
      //! Checks whether the given StepType has the (static) member function <tt>bool is_executable()</tt>. If this is the case, the optional function returns the value of this (static) member function, otherwise it returns true.
      template <class StepType> 
      bool optional_is_executable(StepType& step);

      //! Checks whether the given StepType has the (static) member function <tt>bool is_serializable()</tt>. If this is the case, the optional function returns the value of this (static) member function, otherwise it returns false.
      template <class ConfigurationType> 
      bool optional_is_serializable(ConfigurationType& configuration);

      //! Checks whether the given StepType has the (static) member function <tt>double selection_probability_factor()</tt>. If this is the case, the optional function returns the value of this (static) member function, otherwise it returns 1.0.
      template <class StepType> 
      double optional_selection_probability_factor(StepType& step);
      #endif
    };
  }
}

#endif
