#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>  // for the embedded interpreter
#include <pybind11/numpy.h>
#include "PyShiftCore.h"

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <AEGP_SuiteHandler.h>


namespace py = pybind11;
void set_app(const App& app);
void initialize_python_module();
std::string execute_python_script(const std::string& script);
std::string execute_python_script_from_file(const std::string& scriptPath);
void finalize();