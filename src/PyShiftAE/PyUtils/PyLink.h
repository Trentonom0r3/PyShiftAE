#pragma once

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>

#include "../CoreLib/PyCore.h"

struct JSData {
	std::string name;
	std::string	funcName;
	std::vector<std::variant<std::string, bool, double, std::nullptr_t>> args;
};

void initialize_python_module();

Manifest executeManifestFromFile(const std::string& scriptPath);
void executeFromFile(const std::string& scriptPath);
void executeFlyoutFunction(FlyoutMenuItem item);
Panel executePanelLoad(const std::string& scriptPath);
std::string executeJSFunction(const std::string& module_path, const std::string& custom_name, JSData data);

void finalize();