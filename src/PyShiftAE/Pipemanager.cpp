#include "Pipemanager.h"

NamedPipeManager::NamedPipeManager() {
    for (auto& hEvent : hEvents) {
        hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);  // Manual reset event
    }
}

void NamedPipeManager::AddPipe(const std::string& pipeName, const std::string& entryPath) {
    CreatePipeInstance(pipeName, entryPath);
}

void NamedPipeManager::Run() {
    while (true) {
        DWORD wait = WaitForMultipleObjects(pipes.size(), hEvents, FALSE, INFINITE);
        DWORD idx = wait - WAIT_OBJECT_0;

        if (idx < 0 || idx >= pipes.size()) {
            std::cerr << "WaitForMultipleObjects returned unexpected index." << std::endl;
            continue;
        }

        ProcessIO(pipes[idx], idx);
    }
}

void NamedPipeManager::CreatePipeInstance(const std::string& pipeName, const std::string& entryPath) {
    std::string fullPipeName = "\\\\.\\pipe\\" + pipeName;
    PipeInstance pipeInstance;
    pipeInstance.hPipe = CreateNamedPipe(
        fullPipeName.c_str(),
        PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        1024,
        1024,
        0,
        NULL);

    // Error checking for pipeInstance.hPipe...

    pipeInstance.pipeName = pipeName;
    pipeInstance.entryPath = entryPath;
    pipeInstance.pendingIO = false;

    pipes.push_back(pipeInstance);
}

void NamedPipeManager::ProcessIO(PipeInstance& pipeInstance, DWORD idx) {
    if (!pipeInstance.pendingIO) {
        // Start an overlapped connection or read operation.
        // ...
    }
    else {
        // An overlapped operation has just completed; process the result.
        // ...
    }
}

void NamedPipeManager::ResetPipeInstance(PipeInstance& pipeInstance) {
    // Disconnect and reset the named pipe instance for reuse.
    // ...
}

void NamedPipeManager::ProcessClientRequest(PipeInstance& pipeInstance) {
    pipeInstance.buffer[pipeInstance.bytesRead] = '\0';

    // Parse the JSON message
    struct json_value_s* root = json_parse(pipeInstance.buffer, pipeInstance.bytesRead);
    if (root) {
        auto request = processRequest(root);
        free(root);

        std::string endpoint = std::get<0>(request);
        std::string functionName = std::get<1>(request);
        auto args = std::get<2>(request);
        std::vector<std::variant<std::string, bool, double, std::nullptr_t>> list;
        JsonValue value;
        //convert the args to a vector
        for (auto& arg : args) {
            //std::cout << arg.first << ": " << arg.second << std::endl;
            if (std::holds_alternative<std::string>(arg.second)) {
                value = std::get<std::string>(arg.second);
            }
            else if (std::holds_alternative<bool>(arg.second)) {
                value = std::get<bool>(arg.second);
            }
            else if (std::holds_alternative<double>(arg.second)) {
                value = std::get<double>(arg.second);
            }
            else if (std::holds_alternative<std::nullptr_t>(arg.second)) {
                value = std::get<std::nullptr_t>(arg.second);
            }
            list.push_back(value);
        }
        if (endP == "Response") {
            JSData data;
            data.name = pipeName;
            data.funcName = func;
            data.args = list;
            ScriptTask task;
            task.jsData = data;
            task.pathFunc = std::make_tuple(entryPath, pipeName);
            task.resultType = ScriptTask::Generic;
            task.scriptPath = "PATH";
            std::future<std::string> result = task.resultPromise.get_future();
            {
                std::lock_guard<std::mutex> lock(scriptQueueMutex);
                scriptExecutionQueue.push(std::move(task));
                scriptAddedCond.notify_one();
            }
            std::string resultString = result.get();
            //write the result back to the client
            DWORD bytesWritten;
            std::string finalResult = resultString + "\n";
            WriteFile(hPipe, finalResult.c_str(), finalResult.size(), &bytesWritten, NULL);
        }
        else if (endP == "NoResponse") {
            JSData data;
            data.name = pipeName;
            data.funcName = func;
            data.args = list;
            ScriptTask task;
            task.jsData = data;
            task.resultType = ScriptTask::Generic;
            task.scriptPath = "PATH";
            task.pathFunc = std::make_tuple(entryPath, pipeName);
            std::future<std::string> result = task.resultPromise.get_future();
            {
                std::lock_guard<std::mutex> lock(scriptQueueMutex);
                scriptExecutionQueue.push(std::move(task));
                scriptAddedCond.notify_one();
            }
            std::string resultString = result.get();
            DWORD bytesWritten;
            std::string finalResult = resultString + "\n";
            WriteFile(hPipe, finalResult.c_str(), finalResult.size(), &bytesWritten, NULL);
        }
    }
    else {
        std::cerr << "Failed to parse JSON" << std::endl;
    
    }
    // Depending on the request, prepare a response and call SendResponse
    std::string response = "Your response here";
    SendResponse(pipeInstance, response);
}

void NamedPipeManager::SendResponse(PipeInstance& pipeInstance, const std::string& response) {
    DWORD bytesWritten;
    std::string finalResult = response + "\n";
    WriteFile(pipeInstance.hPipe, finalResult.c_str(), finalResult.size(), &bytesWritten, NULL);
}

std::tuple<std::string, std::string, std::map<std::string, JsonValue>> NamedPipeManager::processRequest(const struct json_value_s* root) {
    auto* rootObject = (struct json_object_s*)root->payload;

    std::string endpoint;
    std::string functionName;
    std::map<std::string, JsonValue> argsMap;

    for (auto element = rootObject->start; element; element = element->next) {
        std::string key = element->name->string;
        auto* value = (struct json_value_s*)element->value;

        if (key == "endpoint") {
            endpoint = ((struct json_string_s*)value->payload)->string;
        }
        else if (key == "functionName") {
            functionName = ((struct json_string_s*)value->payload)->string;
        }
        else if (key == "args" && value->type == json_type_object) {
            auto* argsObject = (struct json_object_s*)value->payload;
            for (auto arg = argsObject->start; arg; arg = arg->next) {
                std::string argKey = arg->name->string;
                auto* argValue = (struct json_value_s*)arg->value;

                switch (argValue->type) {
                case json_type_string:
                    argsMap[argKey] = ((struct json_string_s*)argValue->payload)->string;
                    break;
                case json_type_number:
                    argsMap[argKey] = std::stod(((struct json_number_s*)argValue->payload)->number);
                    break;
                case json_type_true:
                    argsMap[argKey] = true;
                    break;
                case json_type_false:
                    argsMap[argKey] = false;
                    break;
                case json_type_null:
                    argsMap[argKey] = nullptr;
                    break;
                }
            }
        }
    }
    std::tuple<std::string, std::string, std::map<std::string, JsonValue>> result = std::make_tuple(endpoint, functionName, argsMap);
    return result;
}
