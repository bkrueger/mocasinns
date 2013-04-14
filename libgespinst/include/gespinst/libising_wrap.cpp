/*! /file libquadric_wrap.cpp
 * /brief Wrapper for using the C++ class in python
 * /author Benedikt Krüger
 */
#include <boost/python.hpp>

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "spin_ising.hpp"
#include "spin_potts.hpp"

using namespace boost::python;
using namespace Ising;

BOOST_PYTHON_MODULE(libising_wrap)
{
    class_<IsingSpin>("SpinIsing")
      .def(init<int>())
      .def("get_value", &IsingSpin::get_value)
      .def("set_value", &IsingSpin::set_value)
      .def(self * self)
      .def("all_possible_values", &IsingSpin::all_possible_values)
      .def("random", &IsingSpin::random)
      .def("random_differ", &IsingSpin::random_differ)
    ;

    class_<PottsSpin>("SpinPotts")
      .def(init<int, int>())
      .def("get_value", &PottsSpin::get_value)
      .def("set_value", &PottsSpin::set_value)
      .def("get_max_value", &PottsSpin::get_value)
      .def("set_max_value", &PottsSpin::set_value)
      .def(self * self)
      .def("all_possible_values", &PottsSpin::all_possible_values)
      .def("random", &PottsSpin::random)
      .def("random_differ", &PottsSpin::random_differ)
    ;

    // Python access to stl vectors of IsingSpin
    class_< std::vector<IsingSpin> >("vectorSpinIsing")
      .def(vector_indexing_suite<std::vector<IsingSpin> >())
      ;
    // Python access to stl vectors of PottsSpin
    class_< std::vector<PottsSpin> >("vectorSpinPotts")
      .def(vector_indexing_suite<std::vector<PottsSpin> >())
      ;
}
