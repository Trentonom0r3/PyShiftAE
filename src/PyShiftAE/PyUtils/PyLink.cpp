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
}

// Function to initialize the embedded Python interpreter and import the PyShiftCore module.
void initialize_python_module() {
    try {
        // Initialize the Python interpreter.
        py::initialize_interpreter();
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

std::string execute_python_script(const std::string& script) {
    // Variable to hold the output of the script.
    std::string output;

    try {
        // Redirect stdout and stderr to a string stream to capture them.
        py::exec(R"(
            import sys
            import traceback
            from io import StringIO
            _stdout, _stderr = sys.stdout, sys.stderr
            sys.stdout, sys.stderr = StringIO(), StringIO()
        )");

        // Try executing the provided Python script.
        py::exec(R"(
            try:
                exec(script)
                output = sys.stdout.getvalue() + sys.stderr.getvalue()
            except Exception as e:
                output = "Python exception: " + ''.join(traceback.format_exception(type(e), e, e.__traceback__))
                sys.stdout, sys.stderr = _stdout, _stderr
        )", py::globals(), py::dict("script"_a = script)
        );

        // Restore stdout and stderr after executing the script.
        py::exec(R"(
            sys.stdout, sys.stderr = _stdout, _stderr
        )");

        // Retrieve the output.
        std::string info = "All good";
        output = info;
    }
    catch (const py::error_already_set& e) {
        // Handle exceptions thrown by pybind11.
        std::cerr << "Python exception: " << e.what() << std::endl;
        output = "Python exception: " + std::string(e.what());
    }
    catch (const std::exception& e) {
        // Handle standard C++ exceptions.
        std::cerr << "Standard exception: " << e.what() << std::endl;
        output = "Standard exception: " + std::string(e.what());
    }
    catch (...) {
        // Catch all other exceptions.
        std::cerr << "Unknown exception occurred" << std::endl;
        output = "Unknown exception occurred";
    }

    return output;
}


// Function to execute a Python script from a file.
std::string execute_python_script_from_file(const std::string& scriptPath) {
    // Open the script file.
    std::ifstream scriptFile(scriptPath);
    if (!scriptFile) {
        // Handle error if the file could not be opened.
        std::cerr << "Failed to open script file: " << scriptPath << std::endl;
    }
    // Read the script from the file.
    std::string script((std::istreambuf_iterator<char>(scriptFile)),
        std::istreambuf_iterator<char>());
    // Execute the script using the existing execute_python_script function.
    return execute_python_script(script);
}

// Function to finalize the embedded Python interpreter.
void finalize() {
    // Finalize the Python interpreter.
    // This will clean up any resources used by Python.
    py::finalize_interpreter();
}
