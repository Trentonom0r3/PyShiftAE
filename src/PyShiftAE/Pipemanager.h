#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <variant>
#include "CoreLib/Json.h"
#include <iostream>
#include <sstream>
#include <map>
// Define a type that can hold multiple types (string, bool, double)
using JsonValue = std::variant<std::string, bool, double, std::nullptr_t>;

class NamedPipeManager {
public:
    NamedPipeManager();
    void AddPipe(const std::string& pipeName, const std::string& entryPath);
    void Run();

private:
    struct PipeInstance {
        HANDLE hPipe;
        std::string pipeName;
        std::string entryPath;
        OVERLAPPED overlap;
        char buffer[1024];
        DWORD bytesRead;
        bool pendingIO;
    };

    std::vector<PipeInstance> pipes;
    HANDLE hEvents[MAXIMUM_WAIT_OBJECTS];

    void CreatePipeInstance(const std::string& pipeName, const std::string& entryPath);
    void ProcessIO(PipeInstance& pipeInstance, DWORD index);
    void ResetPipeInstance(PipeInstance& pipeInstance);
    void ProcessClientRequest(PipeInstance& pipeInstance);
    void SendResponse(PipeInstance& pipeInstance, const std::string& response);
    std::tuple<std::string, std::string, std::map<std::string, JsonValue>> processRequest(const struct json_value_s* root);
};
