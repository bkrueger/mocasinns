/*! /file libquadric_wrap.cpp
 * /brief Wrapper for using the C++ class in python
 * /author Benedikt Krüger
 */
#include <boost/python.hpp>

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "spin_ising.hpp"
#include "spin_potts.hpp"

using namespace boost::python;

BOOST_PYTHON_MODULE(libising_wrap)
{
    class_<SpinIsing>("SpinIsing")
      .def(init<int>())
      .def("get_value", &SpinIsing::get_value)
      .def("set_value", &SpinIsing::set_value)
      .def(self * self)
      .def("all_possible_values", &SpinIsing::all_possible_values)
      .def("random", &SpinIsing::random)
      .def("random_differ", &SpinIsing::random_differ)
    ;

    class_<SpinPotts>("SpinPotts")
      .def(init<int, int>())
      .def("get_value", &SpinPotts::get_value)
      .def("set_value", &SpinPotts::set_value)
      .def("get_max_value", &SpinPotts::get_value)
      .def("set_max_value", &SpinPotts::set_value)
      .def(self * self)
      .def("all_possible_values", &SpinPotts::all_possible_values)
      .def("random", &SpinPotts::random)
      .def("random_differ", &SpinPotts::random_differ)
    ;

    // Python access to stl vectors of SpinIsing
    class_< std::vector<SpinIsing> >("vectorSpinIsing")
      .def(vector_indexing_suite<std::vector<SpinIsing> >())
      ;
    // Python access to stl vectors of SpinPotts
    class_< std::vector<SpinPotts> >("vectorSpinPotts")
      .def(vector_indexing_suite<std::vector<SpinPotts> >())
      ;
}
