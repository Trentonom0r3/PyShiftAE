#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>  // for the embedded interpreter
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>

#include "../CoreLib/PyCore.h"

namespace py = pybind11;

void initialize_python_module();
std::string execute_python_script(const std::string& script);
std::string execute_python_script_from_file(const std::string& scriptPath);
void finalize();
