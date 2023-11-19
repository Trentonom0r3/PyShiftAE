#pragma once

#include "App.h"
#include "Project.h"
#include "ItemManager.h"
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>  // for the embedded interpreter
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void bindItem(py::module_& m);
void bindCompItem(py::module_& m);
void bindFootageItem(py::module_& m);
void bindFolderItem(py::module_& m);

void bindProject(py::module_& m);
void bindApp(py::module_& m);