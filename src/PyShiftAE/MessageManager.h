#pragma once
#include "PyUtils/SessionManager.h"
#include "CoreLib/PyCore.h"
#include "Commands.h"


typedef std::variant<std::monostate, std::shared_ptr<Project>, std::shared_ptr<Item>, std::shared_ptr<ItemCollection>,
        std::shared_ptr<ProjectCollection>, std::shared_ptr<Layer>, std::shared_ptr<LayerCollection>, std::shared_ptr<CompItem>, std::shared_ptr<FolderItem>,
std::shared_ptr<SolidItem>, std::shared_ptr<FootageItem>> SessionObject;
typedef std::map<std::string, SessionObject> SessionMap;

class SessionManager {
public:
    static SessionManager& GetInstance() {
        static SessionManager instance;
        return instance;
    }

    SessionManager() {
        std::thread messageQueueThread(&SessionManager::handleMessageQueue, this);
        messageQueueThread.detach();
    }



    SessionManager(const SessionManager&) = delete;
    void operator=(const SessionManager&) = delete;

    const SessionObject& getSessionObject(const std::string& sessionID) {
        auto it = sessions.find(sessionID);
        if (it == sessions.end()) {
            throw std::runtime_error("Session ID not found: " + sessionID);
        }

        return it->second;
    }



    void addToSessions(const std::string& sessionID, const SessionObject& sessionObject) {
		sessions[sessionID] = sessionObject;
	}

protected:
    SessionMap sessions;
    void handleMessageQueue() {
        auto& mqm = MessageQueueManager::getInstance();
        CommandFactory commandFactory;

        while (true) {
            Command receivedCmd = mqm.receiveCommand();
            if (receivedCmd.name == "exit") {
                mqm.clearQueues();
                break;
            }

            auto cmd = commandFactory.createCommand(receivedCmd.name, receivedCmd);
            if (cmd) {
                cmd->execute();
            }
            else {
                std::cout << "Command not found: " << receivedCmd.name << std::endl;
            }
        }
    }


};
