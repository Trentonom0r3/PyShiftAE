#include "PyShiftCore.h"

// PyShiftCore.Cpp
/*
This is the core Implementation File. May Refactor into individual component files as
the library expands, but keeping everything here for now while I figure it all out.
*/
// Implementation of the Project class.
Project::Project(AEGP_SuiteHandler* suites, AEGP_ProjectH& projH)
    : suites_(suites), projH_(projH) {  // Initializes member variables with constructor arguments.
    populateAttributes();  // Calls method to populate attribute values.
}

void Project::populateAttributes() {
    name = getName();  // Gets the project name using the getName method and sets it as the name attribute.
    path = GetProjectPath();  // Gets the project path using the GetProjectPath method and sets it as the path attribute.
}

std::string Project::getName() const {  // Method to get the project name.
    A_char nameZ[AEGP_MAX_PROJ_NAME_SIZE];  // Array to hold the project name.
    A_Err err = suites_->ProjSuite6()->AEGP_GetProjectName(projH_, nameZ);  // Calls the AEGP_GetProjectName function to get the project name.
    if (err) {
        throw std::runtime_error("Error obtaining project name");  // Throws an exception if there's an error.
    }
    return std::string(nameZ);  // Converts the project name to a std::string and returns it.
}

std::string Project::GetProjectPath() const {  // Method to get the project path.
    AEGP_MemHandle unicode_pathPH = NULL;  // Handle for memory allocation.
    AEGP_MemSize resultSize = 0;  // Variable to hold the size of the allocated memory.
    A_UTF16Char* resultStr = NULL;  // Pointer to hold the project path string.

    A_Err err = suites_->ProjSuite6()->AEGP_GetProjectPath(projH_, &unicode_pathPH);  // Calls the AEGP_GetProjectPath function to get the project path.

    if (err) {
        return "";  // Returns an empty string if there's an error.
    }

    // Gets the size of the memory handle.
    suites_->MemorySuite1()->AEGP_GetMemHandleSize(unicode_pathPH, &resultSize);

    // Locks the memory handle to make it safe to dereference.
    if (suites_->MemorySuite1()->AEGP_LockMemHandle(unicode_pathPH, (void**)&resultStr) == A_Err_NONE) {
        // Converts the UTF-16 string to a UTF-8 std::string.
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        std::string utf8Result = convert.to_bytes((char16_t*)resultStr, (char16_t*)(resultStr + resultSize / sizeof(A_UTF16Char)));

        // Unlocks the memory handle.
        suites_->MemorySuite1()->AEGP_UnlockMemHandle(unicode_pathPH);

        // Frees the memory handle.
        suites_->MemorySuite1()->AEGP_FreeMemHandle(unicode_pathPH);

        return utf8Result;  // Returns the project path as a UTF-8 string.
    }
    else {
        // Failed to lock the memory handle, so handles the error case.

        // Frees the memory handle.
        suites_->MemorySuite1()->AEGP_FreeMemHandle(unicode_pathPH);

        return "";  // Returns an empty string.
    }
}




// Implementation of the App class.
App::App(AEGP_SuiteHandler& suites) : suites_(suites) {  // Initializes member variables with constructor arguments.
    populateAttributes();  // Calls method to populate attribute values.
}

void App::populateAttributes() {
    version = getVersion();  // Gets the app version using the getVersion method and sets it as the version attribute.
    AEGP_ProjectH projH;
    suites_.ProjSuite6()->AEGP_GetProjectByIndex(0, &projH);  // Gets the project handle for the current project (assuming only one project).
    project = std::make_unique<Project>(&suites_, projH);  // Creates a new Project instance and sets it as the project attribute.
 }

std::string App::getVersion() const {  // Method to get the app version.
    return std::string("1.0.0");  // Returns a hardcoded version string.
}

const Project& App::getProject() const {
    return *project;  // Dereferences the unique_ptr to the Project instance and returns it as a reference.
}

// Implementation for starting an undo group.
void App::startUndoGroup(const std::string& undo_name) {
    // Call the AEGP_StartUndoGroup function from the After Effects SDK.
    // Convert the std::string to a C-style string using c_str().
    suites_.UtilitySuite5()->AEGP_StartUndoGroup(undo_name.c_str());
}

// Implementation for ending an undo group.
void App::endUndoGroup() {
    // Call the AEGP_EndUndoGroup function from the After Effects SDK.
    suites_.UtilitySuite5()->AEGP_EndUndoGroup();
}

void App::executeCommand(const int commandId) {
    AEGP_Command cmd = commandId;
    suites_.CommandSuite1()->AEGP_DoCommand(cmd);
}