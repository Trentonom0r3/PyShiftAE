// Include the header file for linking with Python.
#include "PyLink.h"

// Global or static variable to hold the App instance.
std::unique_ptr<App> global_app;

void set_app_instance(AEGP_SuiteHandler& suites) {
    // Create the App instance and store it in the global variable.
    // std::make_unique is a helper function that creates a new unique_ptr.
    global_app = std::make_unique<App>(suites);
}

void setup_module(py::module& m) {
    // Check if the global App instance has been created.
    if (global_app) {
        // Wrap the Project class for Python.
        py::class_<Project>(m, "Project")
            // Create read-only properties for the name and path.
            .def_property_readonly("name", &Project::getName)
            .def_property_readonly("path", &Project::GetProjectPath);

        // Wrap the App class for Python.
        py::class_<App>(m, "App")
            // Create read-only properties for the version and project.
            .def_property_readonly("version", &App::getVersion)
            .def_property_readonly("project", &App::getProject, py::return_value_policy::reference_internal);

        // Set a module-level variable 'app' to hold the global App instance.
        m.attr("app") = py::cast(global_app.get());
    }
    else {
        // Throw an exception if the App instance hasn't been created.
        throw std::runtime_error("App instance has not been set");
    }
}

// Define the embedded Python module.
PYBIND11_EMBEDDED_MODULE(PyShiftCore, m) {
    // Set up the module using the previously defined setup_module function.
    setup_module(m);
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
    py::gil_scoped_acquire acquire;  // Acquire the GIL
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
    global_app.reset();
}

/*PYTHON USAGE
name = app.project.name
version = app.version
path = app.project.path
*/
