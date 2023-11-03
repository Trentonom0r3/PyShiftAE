// Include the header file for linking with Python.
#include "PyLink.h"

// Global or static variable to hold the Python App instance.

void set_app(const App& app) {
    try {
        py::module_ m = py::module_::import("PyShiftCore");  // Import the Python module 'PyShiftCore'.
        m.attr("app") = py::cast(app, py::return_value_policy::reference);  // Set the 'app' attribute of the module to the App instance, using a reference policy to avoid copying the object.
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

// Define a Python module using pybind11. This will expose the C++ classes to Python.
PYBIND11_EMBEDDED_MODULE(PyShiftCore, m) {
    py::class_<Item>(m, "Item")
        .def_property_readonly("name", &Item::getName);


    py::class_<CompItem, Item>(m, "CompItem")
        .def("frameAtTime", [](CompItem& self, float time) {
            ImageData image_data = self.frameAtTime(time);
            py::array_t<uint8_t> result({ image_data.height, image_data.width, image_data.channels }, image_data.data.data());
            return result;
            })
        
        .def("replaceFrameAtTime", [](CompItem& self, py::array_t<uint8_t> img, float time) {
            // Ensure the NumPy array is C-style contiguous
            if (!img.flags() & py::array::c_style) {
                throw std::runtime_error("Input array must be a C-style contiguous array");
            }

            // Check the number of dimensions
            if (img.ndim() != 3) {
                throw std::runtime_error("Input array must have three dimensions");
            }

            // Convert the NumPy array to ImageData
            ImageData new_img;
            new_img.width = img.shape(1);
            new_img.height = img.shape(0);
            new_img.channels = img.shape(2);
            new_img.data.assign(img.data(), img.data() + img.size());

            // Call the C++ method
            self.replaceFrameAtTime(new_img, time);
            });



    // Expose the Project class to Python as "Project".
    py::class_<Project>(m, "Project")
        // Expose the Project::getName method to Python as a read-only property "name".
        .def_property_readonly("name", &Project::getName)
        // Expose the Project::GetProjectPath method to Python as a read-only property "path".
        .def_property_readonly("path", &Project::GetProjectPath)
        // Expose the Project::getActiveItem method to return the Item Object (ActiveItem)
        .def_property_readonly("activeItem", &Project::getActiveItem, py::return_value_policy::reference);

    // Expose the App class to Python as "App".
    py::class_<App>(m, "App")
        // Expose the App::getVersion method to Python as a read-only property "version".
        .def_property_readonly("version", &App::getVersion)
        // Expose the App::getProject method to Python as a read-only property "project".
        .def_property_readonly("project", &App::getProject, py::return_value_policy::reference_internal)
        // Expose the App::startUndoGroup method to Python.
        .def("beginUndoGroup", &App::startUndoGroup, py::arg("undo_name") = "Default Undo Group Name")
        // Expose the App::endUndoGroup method to Python.
        .def("endUndoGroup", &App::endUndoGroup)

        .def("executeCommand", &App::executeCommand, py::arg("commandId"));
    // Initially set the 'app' attribute of the module to None.
    // This will later be replaced by the actual App instance in the set_app function.
    m.attr("app") = py::none();
    // Expose the set_app function to Python.
    m.def("set_app", &set_app);


}

// Function to initialize the embedded Python interpreter and import the PyShiftCore module.
void initialize_python_module() {
    try {
        // Initialize the Python interpreter.
        py::initialize_interpreter();
        // Import the embedded PyShiftCore module.
        // This will execute the code in the PYBIND11_EMBEDDED_MODULE block above.
        py::module::import("PyShiftCore");
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

// Function to execute a provided Python script.
std::string execute_python_script(const std::string& script) {
    // Variable to hold the output of the script.
    std::string output;
    try {
        // Run some Python code to redirect stdout and stderr to a string stream.
        // This will capture any print statements or errors generated by the script.
        py::exec(
            "import sys\n"
            "from io import StringIO\n"
            "# Backup stdout and stderr\n"
            "_stdout, _stderr = sys.stdout, sys.stderr\n"
            "# Redirect stdout and stderr to a string stream\n"
            "sys.stdout, sys.stderr = StringIO(), StringIO()\n"
        );
        // Print the Python module search path to the console.
        // This can be helpful for debugging.
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
        // Handle any exceptions thrown by pybind11.
        std::cerr << "Python exception: " << e.what() << std::endl;
        output = e.what();
    }
    catch (const std::exception& e) {
        // Handle any standard C++ exceptions.
        std::cerr << "Standard exception: " << e.what() << std::endl;
        output = "Standard exception: " + std::string(e.what());
    }
    catch (...) {
        // Catch all other exceptions.
        std::cerr << "Unknown exception occurred" << std::endl;
        output = "Unknown exception occurred";
    }
    // Return the output.
    return output;
}

// Function to execute a Python script from a file.
std::string execute_python_script_from_file(const std::string& scriptPath) {
    // Open the script file.
    std::ifstream scriptFile(scriptPath);
    if (!scriptFile) {
        // Handle error if the file could not be opened.
        std::cerr << "Failed to open script file: " << scriptPath << std::endl;
        return "Error: Failed to open script file.";
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

/*PYTHON USAGE
name = app.project.name
version = app.version
path = app.project.path
items = app.project.items # get the items 
for item in items:
    if item(isinstance FootageItem):
        img = item.getFrame(TIME)

*/
