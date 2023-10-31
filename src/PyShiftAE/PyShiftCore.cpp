#include "PyShiftCore.h"

// PyShiftCore.Cpp
/*
This is the core Implementation File. May Refactor into individual component files as 
the library expands, but keeping everything here for now while I figure it all out.

*/

// Project class implementation
Project::Project(AEGP_SuiteHandler* suites, AEGP_ProjectH& projH)
    : suites_(suites), projH_(projH) {  // Change to pointer
    populateAttributes();
}

    void Project::populateAttributes() {
        name = getName();  // Populate the public name attribute
        path = GetProjectPath();  // Populate the public path attribute
    }

    std::string Project::getName() const { // Uses Project Suite to get project name
        A_char nameZ[AEGP_MAX_PROJ_NAME_SIZE]; // A_char is a typedef for char
        A_Err err = suites_->ProjSuite6()->AEGP_GetProjectName(projH_, nameZ); // Get project name
        if (err) { 
            throw std::runtime_error("Error obtaining project name"); // Throw error if error
        }
        return std::string(nameZ); // Return project name as string
    }

    std::string Project::GetProjectPath() const { // Uses Project Suite to get project path
        AEGP_MemHandle unicode_pathPH = NULL; // Have to Use Memory Suites for this
        AEGP_MemSize resultSize = 0;
        A_UTF16Char* resultStr = NULL; // Initialize to NULL

        A_Err err = suites_->ProjSuite6()->AEGP_GetProjectPath(projH_, &unicode_pathPH); // Get project path

        if (err) {
            return "";
        }

        // Get size of the path in memory
        suites_->MemorySuite1()->AEGP_GetMemHandleSize(unicode_pathPH, &resultSize);

        // Lock the memory handle so it's safe to dereference
        if (suites_->MemorySuite1()->AEGP_LockMemHandle(unicode_pathPH, (void**)&resultStr) == A_Err_NONE) {
            // Conversion from UTF-16 to std::string (UTF-8)
            std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
            std::string utf8Result = convert.to_bytes((char16_t*)resultStr, (char16_t*)(resultStr + resultSize / sizeof(A_UTF16Char)));

            // Unlock the memory handle
            suites_->MemorySuite1()->AEGP_UnlockMemHandle(unicode_pathPH);

            // Free the memory handle
            suites_->MemorySuite1()->AEGP_FreeMemHandle(unicode_pathPH);

            return utf8Result;
        }
        else {
            // Failed to lock memory handle. Handle error case.

            // Free the memory handle
            suites_->MemorySuite1()->AEGP_FreeMemHandle(unicode_pathPH);

            return "";
        }
    }


// App class implementation
App::App(AEGP_SuiteHandler& suites) : suites_(suites) {
    populateAttributes();
}

    void App::populateAttributes() {
        version = getVersion();
        AEGP_ProjectH projH;
        suites_.ProjSuite6()->AEGP_GetProjectByIndex(0, &projH); // AE Only has one project
        project = std::make_unique<Project>(&suites_, projH);  // Pass the address of suites_
    }

    std::string App::getVersion() const { // Implement using minor, major from entry point
        return std::string("1.0.0");
    }

    const Project& App::getProject() const {
        return *project; // Dereference the unique_ptr to return a reference
    }
