// Include the header file for linking with Python.
#include "PyLink.h"

// Global or static variable to hold the App instance.

py::object pyApp;  // Global variable to hold the Python App instance

void set_app(const App& app) {
    try {
        py::module_ m = py::module_::import("PyShiftCore");
        m.attr("app") = py::cast(app, py::return_value_policy::reference);
        pyApp = m.attr("app");  // Now pyApp holds the Python object
    }
    catch (const py::error_already_set& e) {
        // This block will catch and report Python exceptions
        std::cerr << "Python error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        // This block will catch and report standard C++ exceptions
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }
    catch (...) {
        // This block will catch any other type of exception
        std::cerr << "An unknown exception occurred" << std::endl;
    }
}


PYBIND11_EMBEDDED_MODULE(PyShiftCore, m) {
    py::class_<Project>(m, "Project")
        .def_property_readonly("name", &Project::getName)
        .def_property_readonly("path", &Project::GetProjectPath);

    py::class_<App>(m, "App")
        .def_property_readonly("version", &App::getVersion)
        .def_property_readonly("project", &App::getProject, py::return_value_policy::reference_internal); 

    m.attr("app") = py::none();
    m.def("set_app", &set_app);

}


void initialize_python_module() {
    try {
        // Initialize the Python interpreter.
        py::initialize_interpreter();
        // Import the embedded module.
        // This will also call the PYBIND11_EMBEDDED_MODULE block and the setup_module function.
        py::module::import("PyShiftCore");
    }
    catch (const py::error_already_set& e) {
        // Handle pybind11-specific exceptions.
        std::cerr << "Python exception: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        // Handle standard exceptions.
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }
    catch (...) {
        // Catch any other types of exceptions.
        std::cerr << "Unknown exception occurred" << std::endl;
    }
}
// Function to execute a Python script.
std::string execute_python_script(const std::string& script) {
    std::string output;
    try {
        // Run some Python code to redirect stdout and stderr to a string stream.
        py::exec(
            "import sys\n"
            "from io import StringIO\n"
            "# Backup stdout and stderr\n"
            "_stdout, _stderr = sys.stdout, sys.stderr\n"
            "# Redirect stdout and stderr to a string stream\n"
            "sys.stdout, sys.stderr = StringIO(), StringIO()\n"
        );

        // Print sys.path to the console.
        py::exec(
            "print('Python module search path:', sys.path)\n"
        );

        // Execute the provided Python script.
        py::exec(script);

        // Run some more Python code to get the output and restore stdout and stderr.
        py::exec(
            "# Get the output and restore stdout and stderr\n"
            "output = sys.stdout.getvalue() + sys.stderr.getvalue()\n"
            "sys.stdout, sys.stderr = _stdout, _stderr\n"
        );

        // Get the 'output' variable from the Python global namespace and cast it to a std::string.
        output = py::globals()["output"].cast<std::string>();
    }
    catch (const py::error_already_set& e) {
        // Handle pybind11-specific exceptions.
        std::cerr << "Python exception: " << e.what() << std::endl;
        output = e.what();
    }
    catch (const std::exception& e) {
        // Handle standard exceptions.
        std::cerr << "Standard exception: " << e.what() << std::endl;
        output = "Standard exception: " + std::string(e.what());
    }
    catch (...) {
        // Catch any other types of exceptions.
        std::cerr << "Unknown exception occurred" << std::endl;
        output = "Unknown exception occurred";
    }
    return output;
}

std::string execute_python_script_from_file(const std::string& scriptPath) {
    // First, read the script from the file.
    std::ifstream scriptFile(scriptPath);
    if (!scriptFile) {
        std::cerr << "Failed to open script file: " << scriptPath << std::endl;
        return "Error: Failed to open script file.";
    }
    std::string script((std::istreambuf_iterator<char>(scriptFile)),
        std::istreambuf_iterator<char>());

    // Now, execute the script using the existing execute_python_script function.
    return execute_python_script(script);
}


void finalize() {
    // Finalize the Python interpreter.
    py::finalize_interpreter();
    pyApp.release();
}

/*PYTHON USAGE
name = app.project.name
version = app.version
path = app.project.path
*/
