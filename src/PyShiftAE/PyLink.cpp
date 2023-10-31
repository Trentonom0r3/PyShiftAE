#include <pybind11/pybind11.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "PyShiftCore.h"

namespace py = pybind11;

PYBIND11_MODULE(PyShiftCore, m) {
    py::class_<Project>(m, "Project")
        .def("getName", &Project::getName)
        .def("GetProjectPath", &Project::GetProjectPath)
        .def_readonly("name", &Project::name)
        .def_readonly("path", &Project::path);

    py::class_<App>(m, "App")
        .def("getVersion", &App::getVersion)
        .def("getProject", &App::getProject, py::return_value_policy::reference_internal)
        .def_readonly("version", &App::version);
}

/*PYTHON USAGE
name = app.project.name
version = app.version
path = app.project.path*/