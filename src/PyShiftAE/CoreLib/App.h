#pragma once
#include "../CoreSDK/TaskUtils.h"
#include "../CoreSDK/Core.h"
#include "Project.h"

class App {
public:
    App(); //Constructor -- Initialize this first.
    Project getProject();

    Project project;

    void beginUndoGroup(std::string undo_name = "Default Undo Group Name");
    void endUndoGroup();
    void executeCommand(int commandId);
    void reportInfo(std::string info);

};