/* PyShiftAE.cpp */

#include "PyShiftAE.h"
#include "AETK/AEGP/Core/PyFx.hpp"
#include "Python.h"
#include "windows.h"
#include <commdlg.h> // For file dialog
#include <memory>
#include <mutex>
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

// Plugin ID
AEGP_PluginID myID = 3927L;

// Thread-safe shared variables
std::shared_ptr<std::string> script_path = std::make_shared<std::string>("");
std::mutex script_mutex;
std::atomic<bool> running = false;
std::thread py_thread;

/**
 * Helper function to display a file dialog and select a Python script.
 * Uses the Windows API to open a file selection dialog.
 */
std::string PyShiftAE::selectScriptPath() {
  char filePath[MAX_PATH] = "";

  OPENFILENAME ofn = {0};
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = NULL;
  ofn.lpstrFilter = "Python Scripts (*.py)\0*.py\0All Files (*.*)\0*.*\0";
  ofn.lpstrFile = filePath;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (GetOpenFileName(&ofn)) {
    return std::string(filePath);
  } else {
    // Return an empty string if the user cancels the dialog
    return "";
  }
}

/**
 * Initializes and starts the Python thread.
 */
void PyShiftAE::startPythonThread() {
  py_thread = std::thread([]() {
    Py_Initialize();
    if (Py_IsInitialized()) {
        py::module::import("PyFx");
	}
    while (true) {
      py::gil_scoped_release release; // Release GIL to allow other threads to run
      if (running) {
        std::string path;
        {
          std::lock_guard<std::mutex> lock(script_mutex);
          path = *script_path;
        }
        if (!path.empty()) {
          try {
            py::gil_scoped_acquire acquire;
            py::eval_file(path.c_str());
          } catch (const std::exception &e) {
            App::Alert(e.what());
          }
          {
            std::lock_guard<std::mutex> lock(script_mutex);
            script_path->clear(); // Clear script path after execution
          }
        }
      }
      std::this_thread::sleep_for(
          std::chrono::milliseconds(10)); // Prevent excessive CPU usage
    }
  });
  py_thread.detach();
}

/**
 * Executes the command to set the Python script path and start the processing.
 */
void PyShiftAECommand::execute() {
  std::string path = PyShiftAE::selectScriptPath();
  if (!path.empty()) {
    std::lock_guard<std::mutex> lock(script_mutex);
    *script_path = path;
    running.store(true);
  }
}

/**
 * Updates the plugin menu state.
 */
void PyShiftAECommand::updateMenu() {
  SuiteManager::GetInstance()
      .GetSuiteHandler()
      .CommandSuite1()
      ->AEGP_EnableCommand(getCommand());
}

/**
 * Plugin initialization.
 */
void PyShiftAE::onInit() {
  // Register plugin commands and hooks
  addCommand(std::make_unique<PyShiftAECommand>());
  registerCommandHook();
  registerUpdateMenuHook();
  registerIdleHook();

  // Start Python execution thread
  startPythonThread();
}

/**
 * Plugin cleanup.
 */
void PyShiftAE::onDeath() {
  running.store(false); // Stop the Python thread
  Py_Finalize();        // Finalize Python interpreter
}

/**
 * Idle state handler for the plugin.
 */
void PyShiftAE::onIdle() { ae::TaskScheduler::GetInstance().ExecuteTask(); }

// Declare the plugin entry point
DECLARE_ENTRY(PyShiftAE, myID)
