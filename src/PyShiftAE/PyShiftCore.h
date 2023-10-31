#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING  // Suppress deprecation warnings

#pragma once
//PyShiftCore.h
#include <string>
#include <stdexcept>
#include <memory>  // Include for std::unique_ptr
#include <codecvt>
#include "AEGP_SuiteHandler.h"


//Project class definition
class Project {
public:
    Project(AEGP_SuiteHandler* suites, AEGP_ProjectH& projH);  // Change to pointer

    std::string getName() const;
    std::string GetProjectPath() const;

    std::string name;
    std::string path;
private:
    void populateAttributes();

    AEGP_SuiteHandler* suites_;  // Change to pointer
    AEGP_ProjectH projH_;
};


class Item {
    // Declarations for Item
};

class Layer {
    // Declarations for Layer
};

class Comp {
    // Declarations for Comp
};

class App {
public:
    App(AEGP_SuiteHandler& suites); //Constructor -- Initialize this first.
    std::string getVersion() const; //Get version as string
    const Project& getProject() const; // Return by reference

    std::string version; //Public member variable for version
    std::unique_ptr<Project> project; // Use a smart pointer
private:
    void populateAttributes(); //Internal method to populate attributes
    AEGP_SuiteHandler& suites_; //Suite handler
};
