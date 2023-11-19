#pragma once

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>

#include "../CoreLib/PyCore.h"


void initialize_python_module();
std::string execute_python_script(const std::string& script);
std::string execute_python_script_from_file(const std::string& scriptPath);
void finalize();
