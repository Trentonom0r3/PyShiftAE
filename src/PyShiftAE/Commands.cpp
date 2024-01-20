#include "Commands.h"

class CommandFactory {
public:
    CommandFactory() {
        // Register commands
        commands["reportInfo"] = [] { return std::make_unique<ReportInfoCommand>(); };
        commands["getProject"] = [] { return std::make_unique<GetProjectCommand>(); };
        // Register other commands similarly
    }

    std::unique_ptr<CommandBase> createCommand(const std::string& commandName) {
        auto it = commands.find(commandName);
        if (it != commands.end()) {
            return it->second(); // Call the factory function
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, std::function<std::unique_ptr<CommandBase>()>> commands;
};