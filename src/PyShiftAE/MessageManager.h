#pragma once
#include "PyUtils/SessionManager.h"
#include "CoreLib/PyCore.h"
#include "Commands.h"
#include <tlhelp32.h>

typedef std::variant<std::monostate, std::shared_ptr<Panel>, std::shared_ptr<Project>, std::shared_ptr<Item>, std::shared_ptr<ItemCollection>,
        std::shared_ptr<ProjectCollection>, std::shared_ptr<Layer>, std::shared_ptr<LayerCollection>, std::shared_ptr<CompItem>, std::shared_ptr<FolderItem>,
std::shared_ptr<SolidItem>, std::shared_ptr<FootageItem>> SessionObject;
typedef std::map<std::string, SessionObject> SessionMap;

typedef std::string panelID;


class SessionManager {
    std::map<std::string, PROCESS_INFORMATION> processMap;
public:
    static SessionManager& GetInstance() {
        static SessionManager instance;
        return instance;
    }

    SessionManager() {
        sessions.clear();
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

    bool sessionExists(const std::string& sessionID) {
		auto it = sessions.find(sessionID);
        if (it == sessions.end()) {
			return false;
		}
		return true;
	}

    void addToSessions(const std::string& sessionID, const SessionObject& sessionObject) {
		sessions[sessionID] = sessionObject;
	}

    void removeFromSessions(const std::string& sessionID) {
		sessions.erase(sessionID);
	}

    void removePanel(std::string panelID) {
        auto it = std::find(panels.begin(), panels.end(), panelID);
        if (it != panels.end()) {
            panels.erase(it);
        }
    }

    HWND getHWND(std::string panelID) {
        auto it = hwndMap.find(panelID);
        if (it != hwndMap.end()) {
			return it->second;
		}
        return NULL;
    }

    void executeCommandAsync(const std::string& command, std::string panelID) {
        try {
            std::thread commandThread = std::thread([this, command, panelID]() {
                PROCESS_INFORMATION pi = executeCommand(command);
                processMap[panelID] = pi;

                const int timeoutMilliseconds = 30000; // 30 seconds timeout
                const int pollIntervalMilliseconds = 500; // Poll every 500 milliseconds
                int elapsedMilliseconds = 0;
                bool targetHwndFound = false;
                while (elapsedMilliseconds < timeoutMilliseconds && !targetHwndFound) {
                    std::vector<DWORD> childProcessIDs = GetChildProcessIDs(pi.dwProcessId);
                    for (DWORD childPID : childProcessIDs) {
                        std::vector<HWND> hwnds = FindAllWindowsByProcessId(childPID);

                        if (hwnds.size() >= 2) {
                            HWND targetHwnd = hwnds[0]; // Target the second HWND
                            hwndMap[panelID] = targetHwnd;
                            std::cout << "Target HWND found and stored for panelID: " << panelID << std::endl;
                            int intHWND = reinterpret_cast<int>(targetHwnd);
                            std::cout << "HWND: " << intHWND << std::endl;
                            targetHwndFound = true;
                            break; // Exit the inner loop once the target HWND is found
                        }
                    }

                    if (!targetHwndFound) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(pollIntervalMilliseconds));
                        elapsedMilliseconds += pollIntervalMilliseconds;
                    }
                }

                if (!targetHwndFound) {
                    std::cerr << "Timeout reached. Failed to find the target HWND for panelID: " << panelID << std::endl;
                }
                });
            commandThread.detach();
        }
        catch (const std::exception& e) {
            std::cerr << "Error in async execution: " << e.what() << std::endl;
        }
    }

    // Method to kill the process associated with a panelID
    void killProcess(std::string panelID) {
        removePanel(panelID);
        removeFromSessions(panelID);
        removefromHWNDMap(panelID);
        auto it = processMap.find(panelID);
        if (it != processMap.end()) {
            PROCESS_INFORMATION pi = it->second;
            if (TerminateProcess(pi.hProcess, 0)) {
                std::cout << "Process terminated: " << panelID << std::endl;
            }
            else {
                std::cerr << "Failed to terminate process: " << panelID << std::endl;
            }
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            processMap.erase(it);
        }
        else {
            std::cerr << "No process found for panelID: " << panelID << std::endl;
        }
    }

    void cleanAll() {
        try {
            std::cout << "Number of panels to clean: " << panels.size() << std::endl;
            std::string panelsize = std::to_string(panels.size());
            for (auto& panelID : panels) {
                std::cout << "Cleaning panel: " << panelID << std::endl;
                killProcess(panelID);
            }
            panels.clear();
            sessions.clear();
        }
        catch (const std::exception& e) {
            std::cerr << "Error in cleanAll: " << e.what() << std::endl;
        }
    }

    bool isHWNDStored(std::string panelID) {
		auto it = hwndMap.find(panelID);
        if (it != hwndMap.end()) {
			return true;
		}
		return false;
	}

    void removefromHWNDMap(std::string panelID) {
        for (auto it = hwndMap.begin(); it != hwndMap.end(); ++it) {
            if (it->first == panelID) {
				hwndMap.erase(it);
				break;
			}
		}
    }

protected:
    std::vector<panelID> panels;
    SessionMap sessions;
    std::map<std::string, HWND> hwndMap;

    std::vector<DWORD> GetChildProcessIDs(DWORD parentProcessID) {
        std::vector<DWORD> childPIDs;
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 pe32;
            pe32.dwSize = sizeof(PROCESSENTRY32);
            if (Process32First(hSnapshot, &pe32)) {
                do {
                    if (pe32.th32ParentProcessID == parentProcessID) {
                        childPIDs.push_back(pe32.th32ProcessID);
                    }
                } while (Process32Next(hSnapshot, &pe32));
            }
            CloseHandle(hSnapshot);
        }
        return childPIDs;
    }


    static BOOL CALLBACK EnumWindowsCallback2(HWND hwnd, LPARAM lParam) {
        DWORD processId = 0;
        GetWindowThreadProcessId(hwnd, &processId);

        auto pParams = reinterpret_cast<std::pair<DWORD, std::vector<HWND>*>*>(lParam);

        if (processId == pParams->first) {
            // Found a window matching the process ID
            // Add the HWND to the vector
            pParams->second->push_back(hwnd);
        }
        return TRUE; // Continue enumeration to find all windows
    }

    std::vector<HWND> FindAllWindowsByProcessId(DWORD processId) {
        std::vector<HWND> hwnds;
        std::pair<DWORD, std::vector<HWND>*> params = { processId, &hwnds };

        EnumWindows(SessionManager::EnumWindowsCallback2, reinterpret_cast<LPARAM>(&params));

        return hwnds; // Vector with all HWNDs found
    }


    PROCESS_INFORMATION executeCommand(const std::string& command) {
        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = TRUE;
        sa.lpSecurityDescriptor = NULL;

        HANDLE hRead, hWrite;
        if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
            throw std::runtime_error("CreatePipe failed");
        }

        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        si.hStdError = hWrite;
        si.hStdOutput = hWrite;
        si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;  // Hides the console window

        if (!CreateProcess(NULL, const_cast<char*>(command.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
            CloseHandle(hWrite);
            CloseHandle(hRead);
            throw std::runtime_error("CreateProcess failed");
        }

        // Close handles that are not needed anymore
        CloseHandle(hWrite);
        CloseHandle(hRead);

        // Do not close pi.hProcess and pi.hThread here
        // Return PROCESS_INFORMATION for later use
        return pi;
    }

    void handleMessageQueue() {
        auto& mqm = MessageQueueManager::getInstance();
        CommandFactory commandFactory;

        while (true) {
            Command receivedCmd;
            if (mqm.tryReceiveCommand(receivedCmd)) {
                // Process the command
                auto cmd = commandFactory.createCommand(receivedCmd.name, receivedCmd);
                if (cmd) {
                    cmd->execute();
                }
                else {
                    std::cout << "Command not found: " << receivedCmd.name << std::endl;
                }
            }
            else {
                // No command received, sleep for a bit
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }

};
