// Include the header file for linking with Python.
#include "PyLink.h"

using namespace pybind11::literals;

PYBIND11_EMBEDDED_MODULE(PyShiftCore, m) {
    bindLayer(m);
    bindItem(m);
    bindCompItem(m);
    bindFootageItem(m);
    bindFolderItem(m);
    bindProject(m);
    bindApp(m);
    //bind the LayerFlag enum
    bindLayerEnum(m);
    bindLayerCollection(m);
    bindProjectCollection(m);
    bindItemCollection(m);
    bindAdjustmentLayerItem(m);
    bindSolidItem(m);
    bindManifest(m);

}

// Function to initialize the embedded Python interpreter and import the PyShiftCore module.
void initialize_python_module() {
    try {
        // Initialize the Python interpreter.
        py::initialize_interpreter();
        py::module_::import("PyShiftCore");
        py::object sys = py::module_::import("sys");
        py::object pyOutputStream = py::cast(new PyOutputStream());  // Create a new instance
        sys.attr("stdout") = pyOutputStream;
        sys.attr("stderr") = pyOutputStream;
    }
    catch (const py::error_already_set& e) {
        // Handle any exceptions thrown by pybind11.
        std::cerr << "Python exception: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        // Handle any standard C++ exceptions.
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }
    catch (...) {
        // Catch all other exceptions.
        std::cerr << "Unknown exception occurred" << std::endl;
    }
}

void executeFlyoutFunction(FlyoutMenuItem item) {
    try {
        py::function flyoutFunction = item.callback;
        if (flyoutFunction) {
			py::object result = flyoutFunction();
            bool answer = result.cast<bool>();
            if (answer) {
				std::cout << "True" << std::endl;
			}
            else {
				std::cout << "False" << std::endl;
			}
		}
    }
    catch (const py::error_already_set& e) {
		// Handle any exceptions thrown by pybind11.
		std::cerr << "Python exception: " << e.what() << std::endl;
	}
    catch (const std::exception& e) {
		// Handle any standard C++ exceptions.
		std::cerr << "Standard exception: " << e.what() << std::endl;
	}
    catch (...) {
		// Catch all other exceptions.
		std::cerr << "Unknown exception occurred" << std::endl;
	}
}

Manifest executeManifestFromFile(const std::string& scriptPath) {
	Manifest manifest;
    try {
		py::object result = py::eval_file(scriptPath);
        manifest = py::globals()["manifest"].cast<Manifest>();
	}
    catch (const py::error_already_set& e) {
		// Handle any exceptions thrown by pybind11.
		std::cerr << "Python exception: " << e.what() << std::endl;
		manifest = Manifest();
	}
    catch (const py::reference_cast_error& e) {
        std::cerr << "Cast error: " << e.what() << std::endl;
        manifest = Manifest();
    }
    catch (const py::cast_error& e) {
		std::cerr << "Cast error: " << e.what() << std::endl;
		manifest = Manifest();
	}
    catch (...) {
		// Catch all other exceptions.
		std::cerr << "Unknown exception occurred" << std::endl;
		manifest = Manifest();
	}
	return manifest;
}

Panel executePanelLoad(const std::string& scriptPath) {
    Panel panel;
    try {
        py::object result = py::eval_file(scriptPath);
        panel = py::globals()["panel"].cast<Panel>();
    }
    catch (const py::error_already_set& e) {
        // Handle any exceptions thrown by pybind11.
        std::cerr << "Python exception: " << e.what() << std::endl;
        panel = Panel();
    }
    catch (const py::reference_cast_error& e) {
        std::cerr << "Cast error: " << e.what() << std::endl;
        panel = Panel();
    }
    catch (const py::cast_error& e) {
        std::cerr << "Cast error: " << e.what() << std::endl;
        panel = Panel();
    }
    catch (...) {
        // Catch all other exceptions.
        std::cerr << "Unknown exception occurred" << std::endl;
        panel = Panel();
    }
    return panel;
}

std::string escape_backslashes(const std::string& input) {
    std::string output = input;
    std::replace(output.begin(), output.end(), '\\', '/');
    return output;
}

py::module_ import_module_as(const std::string& module_path, const std::string& custom_name) {
    try {
        // Escape backslashes in the module path
        std::string escaped_path = escape_backslashes(module_path);
        std::string file_name = "entry";
        // Construct Python code for importing the module
        std::string import_code =
            "import sys\n"
            "sys.path.insert(0, '" + escaped_path + "')\n"
            "import " + file_name + " as " + custom_name + "\n"
            "sys.path.pop(0)\n";

        // Execute the Python code
        py::exec(import_code.c_str());

        // Import the module in C++ using the custom name
        py::module module = py::module("PyShiftCore");
        module.add_object(custom_name.c_str(), py::module::import(file_name.c_str()));
        return module;
    }
    catch (const py::error_already_set& e) {
        // Handle Python exceptions
        std::cerr << "Python exception: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        // Handle standard C++ exceptions
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }
    catch (...) {
        // Catch all other exceptions
        std::cerr << "Unknown exception occurred" << std::endl;
    }
    return py::module_(); // Return an empty module in case of failure
}

std::string executeJSFunction(const std::string& module_path, const std::string& custom_name, JSData data)
{
    std::string funcName = data.funcName;
    std::vector<std::variant<std::string, bool, double, std::nullptr_t>> args = data.args;
    std::string result;
    try {
        py::module module = import_module_as(module_path, custom_name);
        py::function jsFunction = module.attr(custom_name.c_str()).attr(funcName.c_str());
        if (jsFunction) {
            if (args.size() == 0) {
				py::object res = jsFunction();
				result = py::str(res);
			}
            else {
				py::list list = py::cast(args);
				py::object res = jsFunction(list);
				result = py::str(res);
			}
        }
        else {
            std::cerr << "Function not found" << std::endl;
        }
    }
    catch (const py::error_already_set& e) {
		// Handle any exceptions thrown by pybind11.
		std::cerr << "Python exception: " << e.what() << std::endl;
		result = "";
	}
    catch (const py::reference_cast_error& e) {
		std::cerr << "Cast error: " << e.what() << std::endl;
		result = "";
	}
    catch (const py::cast_error& e) {
		std::cerr << "Cast error: " << e.what() << std::endl;
		result = "";
	}
    catch (...) {
		// Catch all other exceptions.
		std::cerr << "Unknown exception occurred" << std::endl;
		result = "";
	}
    return result;
}

void executeFromFile(const std::string& scriptPath) {
    try {
        py::eval_file(scriptPath);
    }
    catch (const py::error_already_set& e) {
		// Handle any exceptions thrown by pybind11.
		std::cerr << "Python exception: " << e.what() << std::endl;
	}
    catch (const std::exception& e) {
		// Handle any standard C++ exceptions.
		std::cerr << "Standard exception: " << e.what() << std::endl;
	}
    catch (...) {
		// Catch all other exceptions.
		std::cerr << "Unknown exception occurred" << std::endl;
	}
}


// Function to finalize the embedded Python interpreter.
void finalize() {
    // Finalize the Python interpreter.
    // This will clean up any resources used by Python.
    py::finalize_interpreter();
}
