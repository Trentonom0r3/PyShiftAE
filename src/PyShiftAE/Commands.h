#pragma once
#include "PyUtils/SessionManager.h"
#include "CoreLib/PyCore.h"
class SessionManager; // Forward declaration

class CommandBase {
public:
    CommandBase() = default;
    CommandBase(Command cmd) : cmd(cmd) {};
    virtual void execute() = 0;
    Command cmd;
};

class ReportInfoCommand : public CommandBase {
public:
    ReportInfoCommand() = default;

    ReportInfoCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};

class GetProjectCommand : public CommandBase {
public:
    GetProjectCommand() = default;

    GetProjectCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};

class BeginUndoGroupCommand : public CommandBase {
public:
	BeginUndoGroupCommand() = default;

	BeginUndoGroupCommand(Command cmd) : CommandBase(cmd) {

	};
	void execute() override;
};

class EndUndoGroupCommand : public CommandBase {
public:
	EndUndoGroupCommand() = default;

	EndUndoGroupCommand(Command cmd) : CommandBase(cmd) {

	};
	void execute() override;
};

class ExecutecommandCommand : public CommandBase {
public:
    ExecutecommandCommand() = default;

    ExecutecommandCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};

class GetpluginpathsCommand : public CommandBase {
public:
    GetpluginpathsCommand() = default;

    GetpluginpathsCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};

class IsitemselectedCommand : public CommandBase {
public:
    IsitemselectedCommand() = default;

    IsitemselectedCommand(Command cmd) : CommandBase(cmd) {
  
    };
    void execute() override;
};

class SelectitemCommand : public CommandBase {
public:
    SelectitemCommand() = default;

    SelectitemCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};

class GetitemnameCommand : public CommandBase {
public:
    GetitemnameCommand() = default;

    GetitemnameCommand(Command cmd) : CommandBase(cmd) {
 
    };
    void execute() override;
};

class SetitemnameCommand : public CommandBase {
public:
    SetitemnameCommand() = default;

    SetitemnameCommand(Command cmd) : CommandBase(cmd) {
    
    };
    void execute() override;
};

class GetitemwidthCommand : public CommandBase {
public:
    GetitemwidthCommand() = default;

    GetitemwidthCommand(Command cmd) : CommandBase(cmd) {
     
    };
    void execute() override;
};

class GetitemheightCommand : public CommandBase {
public:
    GetitemheightCommand() = default;

    GetitemheightCommand(Command cmd) : CommandBase(cmd) {
   
    };
    void execute() override;
};

class GetitemcurrenttimeCommand : public CommandBase {
public:
    GetitemcurrenttimeCommand() = default;

    GetitemcurrenttimeCommand(Command cmd) : CommandBase(cmd) {
      
    };
    void execute() override;
};

class GetitemdurationCommand : public CommandBase {
public:
    GetitemdurationCommand() = default;

    GetitemdurationCommand(Command cmd) : CommandBase(cmd) {
  
    };
    void execute() override;
};

class SetitemcurrenttimeCommand : public CommandBase {
public:
    SetitemcurrenttimeCommand() = default;

    SetitemcurrenttimeCommand(Command cmd) : CommandBase(cmd) {
    
    };
    void execute() override;
};

class GetcompframerateCommand : public CommandBase {
public:
    GetcompframerateCommand() = default;

    GetcompframerateCommand(Command cmd) : CommandBase(cmd) {
       
    };
    void execute() override;
};

class SetcompframerateCommand : public CommandBase {
public:
    SetcompframerateCommand() = default;

    SetcompframerateCommand(Command cmd) : CommandBase(cmd) {
    
    };
    void execute() override;
};

class GetcompworkareadurationCommand : public CommandBase {
public:
    GetcompworkareadurationCommand() = default;

    GetcompworkareadurationCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};

class SetcompdurationCommand : public CommandBase {
public:
    SetcompdurationCommand() = default;

    SetcompdurationCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};

class SetcompwidthCommand : public CommandBase {
public:
    SetcompwidthCommand() = default;

    SetcompwidthCommand(Command cmd) : CommandBase(cmd) {
   
    };
    void execute() override;
};

class SetcompheightCommand : public CommandBase {
public:
    SetcompheightCommand() = default;

    SetcompheightCommand(Command cmd) : CommandBase(cmd) {
     
    };
    void execute() override;
};

class CreatesolidincompCommand : public CommandBase {
public:
    CreatesolidincompCommand() = default;

    CreatesolidincompCommand(Command cmd) : CommandBase(cmd) {
    
    };
    void execute() override;
};

class GetselectedlayersCommand : public CommandBase {
public:
    GetselectedlayersCommand() = default;

    GetselectedlayersCommand(Command cmd) : CommandBase(cmd) {
     
    };
    void execute() override;
};

class GetlayersCommand : public CommandBase {
public:
    GetlayersCommand() = default;

    GetlayersCommand(Command cmd) : CommandBase(cmd) {
     
    };
    void execute() override;
};

class GetnumlayersCommand : public CommandBase {
public:
    GetnumlayersCommand() = default;

    GetnumlayersCommand(Command cmd) : CommandBase(cmd) {
      
    };
    void execute() override;
};

class GetcompitemcurrenttimeCommand : public CommandBase {
public:
    GetcompitemcurrenttimeCommand() = default;

    GetcompitemcurrenttimeCommand(Command cmd) : CommandBase(cmd) {
       
    };
    void execute() override;
};

class SetcompitemcurrenttimeCommand : public CommandBase {
public:
    SetcompitemcurrenttimeCommand() = default;

    SetcompitemcurrenttimeCommand(Command cmd) : CommandBase(cmd) {
     
    };
    void execute() override;
};

class AddlayertocompCommand : public CommandBase {
public:
    AddlayertocompCommand() = default;

    AddlayertocompCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};

class GetlayernameCommand : public CommandBase {
public:
    GetlayernameCommand() = default;

    GetlayernameCommand(Command cmd) : CommandBase(cmd) {
    
    };
    void execute() override;
};

class GetlayersourcenameCommand : public CommandBase {
public:
    GetlayersourcenameCommand() = default;

    GetlayersourcenameCommand(Command cmd) : CommandBase(cmd) {
     
    };
    void execute() override;
};

class SetlayernameCommand : public CommandBase {
public:
    SetlayernameCommand() = default;

    SetlayernameCommand(Command cmd) : CommandBase(cmd) {
 
    };
    void execute() override;
};

class GetlayerindexCommand : public CommandBase {
public:
    GetlayerindexCommand() = default;

    GetlayerindexCommand(Command cmd) : CommandBase(cmd) {
       
    };
    void execute() override;
};

class ChangelayerindexCommand : public CommandBase {
public:
    ChangelayerindexCommand() = default;

    ChangelayerindexCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};

class DuplicatelayerCommand : public CommandBase {
public:
    DuplicatelayerCommand() = default;

    DuplicatelayerCommand(Command cmd) : CommandBase(cmd) {
   
    };
    void execute() override;
};

class GetlayercurrenttimeCommand : public CommandBase {
public:
    GetlayercurrenttimeCommand() = default;

    GetlayercurrenttimeCommand(Command cmd) : CommandBase(cmd) {
    
    };
    void execute() override;
};

class GetlayerComptimeCommand : public CommandBase {
public:
    GetlayerComptimeCommand() = default;

    GetlayerComptimeCommand(Command cmd) : CommandBase(cmd) {
 
    };
    void execute() override;
};

class GetlayerinpointCommand : public CommandBase {
public:
    GetlayerinpointCommand() = default;

    GetlayerinpointCommand(Command cmd) : CommandBase(cmd) {
      
    };
    void execute() override;
};


class GetlayerdurationCommand : public CommandBase {
public:
    GetlayerdurationCommand() = default;

    GetlayerdurationCommand(Command cmd) : CommandBase(cmd) {
    
    };
    void execute() override;
};


class GetlayerqualityCommand : public CommandBase {
public:
    GetlayerqualityCommand() = default;

    GetlayerqualityCommand(Command cmd) : CommandBase(cmd) {
  
    };
    void execute() override;
};

class SetlayerqualityCommand : public CommandBase {
public:
    SetlayerqualityCommand() = default;

    SetlayerqualityCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};

class DeletelayerCommand : public CommandBase {
public:
    DeletelayerCommand() = default;

    DeletelayerCommand(Command cmd) : CommandBase(cmd) {
   
    };
    void execute() override;
};

class GetlayeroffsetCommand : public CommandBase {
public:
    GetlayeroffsetCommand() = default;

    GetlayeroffsetCommand(Command cmd) : CommandBase(cmd) {
 
    };
    void execute() override;
};

class SetlayeroffsetCommand : public CommandBase {
public:
    SetlayeroffsetCommand() = default;

    SetlayeroffsetCommand(Command cmd) : CommandBase(cmd) {
 
    };
    void execute() override;
};

class SetlayerflagCommand : public CommandBase {
public:
    SetlayerflagCommand() = default;

    SetlayerflagCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};

class GetlayerflagCommand : public CommandBase {
public:
    GetlayerflagCommand() = default;

    GetlayerflagCommand(Command cmd) : CommandBase(cmd) {
  
    };
    void execute() override;
};

class GetlayersourceCommand : public CommandBase {
public:
    GetlayersourceCommand() = default;

    GetlayersourceCommand(Command cmd) : CommandBase(cmd) {
 
    };
    void execute() override;
};

class GetchilditemsCommand : public CommandBase {
public:
    GetchilditemsCommand() = default;

    GetchilditemsCommand(Command cmd) : CommandBase(cmd) {
   
    };
    void execute() override;
};

class RemovelayerfromcollectionCommand : public CommandBase {
public:
    RemovelayerfromcollectionCommand() = default;

    RemovelayerfromcollectionCommand(Command cmd) : CommandBase(cmd) {
        
    };
    void execute() override;
};

class RemovelayerbyindexCommand : public CommandBase {
public:
    RemovelayerbyindexCommand() = default;

    RemovelayerbyindexCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};

class GetcompnameCommand : public CommandBase {
public:
    GetcompnameCommand() = default;

    GetcompnameCommand(Command cmd) : CommandBase(cmd) {
 
    };
    void execute() override;
};

class AddlayertocollectionCommand : public CommandBase {
public:
    AddlayertocollectionCommand() = default;

    AddlayertocollectionCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};

class GetitemsCommand : public CommandBase {
public:
    GetitemsCommand() = default;

    GetitemsCommand(Command cmd) : CommandBase(cmd) {
    
    };
    void execute() override;
};

class GetfootagepathCommand : public CommandBase {
public:
    GetfootagepathCommand() = default;

    GetfootagepathCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};

class ReplacefootageCommand : public CommandBase {
public:
    ReplacefootageCommand() = default;

    ReplacefootageCommand(Command cmd) : CommandBase(cmd) {
 
    };
    void execute() override;
};

class ActiveitemCommand : public CommandBase {
public:
    ActiveitemCommand() = default;

    ActiveitemCommand(Command cmd) : CommandBase(cmd) {
 
    };
    void execute() override;
};

class GetactivelayerCommand : public CommandBase {
public:
    GetactivelayerCommand() = default;

    GetactivelayerCommand(Command cmd) : CommandBase(cmd) {
 
    };
    void execute() override;
};

class GetprojectnameCommand : public CommandBase {
public:
    GetprojectnameCommand() = default;

    GetprojectnameCommand(Command cmd) : CommandBase(cmd) {
  
    };
    void execute() override;
};

class GetprojectpathCommand : public CommandBase {
public:
    GetprojectpathCommand() = default;

    GetprojectpathCommand(Command cmd) : CommandBase(cmd) {
 
    };
    void execute() override;
};

class SaveprojectasCommand : public CommandBase {
public:
    SaveprojectasCommand() = default;

    SaveprojectasCommand(Command cmd) : CommandBase(cmd) {
        execute();
    };
    void execute() override;
};


class GetselecteditemsCommand : public CommandBase {
public:
    GetselecteditemsCommand() = default;

    GetselecteditemsCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};


class AppenditemCommand : public CommandBase {
public:
    AppenditemCommand() = default;

    AppenditemCommand(Command cmd) : CommandBase(cmd) {

    };
    void execute() override;
};

class GetAlllayersCommand : public CommandBase {
public:
    GetAlllayersCommand() = default;

    GetAlllayersCommand(Command cmd) : CommandBase(cmd) {

	};
	void execute() override;

};

class CreateFootageItemCommand : public CommandBase {
public:
	CreateFootageItemCommand() = default;

	CreateFootageItemCommand(Command cmd) : CommandBase(cmd) {

	};
	void execute() override;
};

class CreateSolidItemCommand : public CommandBase {
public:
	CreateSolidItemCommand() = default;

	CreateSolidItemCommand(Command cmd) : CommandBase(cmd) {

	};
	void execute() override;
};

class getAllChildItemsCommand : public CommandBase {
public:
	getAllChildItemsCommand() = default;

	getAllChildItemsCommand(Command cmd) : CommandBase(cmd) {

	};
	void execute() override;
};

class GetfolderitemsCommand : public CommandBase {
public:
    GetfolderitemsCommand() = default;

    GetfolderitemsCommand(Command cmd) : CommandBase(cmd) {

	};
	void execute() override;
};

class CreateFolderItemCommand : public CommandBase {
public:
    CreateFolderItemCommand() = default;

    CreateFolderItemCommand(Command cmd) : CommandBase(cmd) {

	};
	void execute() override;

};

class AppendItemCommand : public CommandBase {
public:
    AppendItemCommand() = default;

    AppendItemCommand(Command cmd) : CommandBase(cmd) {

	};
	void execute() override;

};

class RemoveItemCommand : public CommandBase {
public:
	RemoveItemCommand() = default;

    RemoveItemCommand(Command cmd) : CommandBase(cmd) {

	};
	void execute() override;

};

class CreatePanelCommand : public CommandBase {
public: 
	CreatePanelCommand() = default;

    CreatePanelCommand(Command cmd) : CommandBase(cmd) {

	};
	void execute() override;

};

class CommandFactory {
public:
    CommandFactory() {
        // Register commands
        commands["reportInfo"] =  [](const Command& cmd) { return std::make_unique<ReportInfoCommand>(cmd); };
        commands["getProject"] =  [](const Command& cmd) { return std::make_unique<GetProjectCommand>(cmd); };
        commands["beginUndoGroup"] =  [](const Command& cmd) { return std::make_unique<BeginUndoGroupCommand>(cmd); };
    	commands["endUndoGroup"] =  [](const Command& cmd) { return std::make_unique<EndUndoGroupCommand>(cmd); };
        commands["executeCommand"] =  [](const Command& cmd) { return std::make_unique<ExecutecommandCommand>(cmd); };
        commands["getPluginPaths"] =  [](const Command& cmd) { return std::make_unique<GetpluginpathsCommand>(cmd); };
        commands["IsItemSelected"] =  [](const Command& cmd) { return std::make_unique<IsitemselectedCommand>(cmd); };
        commands["SelectItem"] =  [](const Command& cmd) { return std::make_unique<SelectitemCommand>(cmd); };
        commands["GetItemName"] =  [](const Command& cmd) { return std::make_unique<GetitemnameCommand>(cmd); };
        commands["SetItemName"] =  [](const Command& cmd) { return std::make_unique<SetitemnameCommand>(cmd); };
        commands["GetItemWidth"] =  [](const Command& cmd) { return std::make_unique<GetitemwidthCommand>(cmd); };
        commands["GetItemHeight"] =  [](const Command& cmd) { return std::make_unique<GetitemheightCommand>(cmd); };
        commands["GetItemCurrentTime"] =  [](const Command& cmd) { return std::make_unique<GetitemcurrenttimeCommand>(cmd); };
        commands["GetItemDuration"] =  [](const Command& cmd) { return std::make_unique<GetitemdurationCommand>(cmd); };
        commands["SetItemCurrentTime"] =  [](const Command& cmd) { return std::make_unique<SetitemcurrenttimeCommand>(cmd); };
        commands["GetCompFramerate"] =  [](const Command& cmd) { return std::make_unique<GetcompframerateCommand>(cmd); };
        commands["SetCompFramerate"] =  [](const Command& cmd) { return std::make_unique<SetcompframerateCommand>(cmd); };
        commands["GetCompWorkAreaDuration"] =  [](const Command& cmd) { return std::make_unique<GetcompworkareadurationCommand>(cmd); };
        commands["SetCompDuration"] =  [](const Command& cmd) { return std::make_unique<SetcompdurationCommand>(cmd); };
        commands["SetCompWidth"] =  [](const Command& cmd) { return std::make_unique<SetcompwidthCommand>(cmd); };
        commands["SetCompHeight"] =  [](const Command& cmd) { return std::make_unique<SetcompheightCommand>(cmd); };
        commands["GetSelectedLayers"] =  [](const Command& cmd) { return std::make_unique<GetselectedlayersCommand>(cmd); };
        commands["GetLayers"] =  [](const Command& cmd) { return std::make_unique<GetlayersCommand>(cmd); };
        commands["GetNumLayers"] =  [](const Command& cmd) { return std::make_unique<GetnumlayersCommand>(cmd); };
        commands["GetCompItemCurrentTime"] =  [](const Command& cmd) { return std::make_unique<GetcompitemcurrenttimeCommand>(cmd); };
        commands["SetCompItemCurrentTime"] =  [](const Command& cmd) { return std::make_unique<SetcompitemcurrenttimeCommand>(cmd); };
        commands["AddLayerToComp"] =  [](const Command& cmd) { return std::make_unique<AddlayertocompCommand>(cmd); };
        commands["GetLayerName"] =  [](const Command& cmd) { return std::make_unique<GetlayernameCommand>(cmd); };
        commands["GetLayerSourceName"] =  [](const Command& cmd) { return std::make_unique<GetlayersourcenameCommand>(cmd); };
        commands["SetLayerName"] =  [](const Command& cmd) { return std::make_unique<SetlayernameCommand>(cmd); };
        commands["GetLayerIndex"] =  [](const Command& cmd) { return std::make_unique<GetlayerindexCommand>(cmd); };
        commands["ChangeLayerIndex"] =  [](const Command& cmd) { return std::make_unique<ChangelayerindexCommand>(cmd); };
        commands["DuplicateLayer"] =  [](const Command& cmd) { return std::make_unique<DuplicatelayerCommand>(cmd); };
        commands["GetLayerCurrentTime"] =  [](const Command& cmd) { return std::make_unique<GetlayercurrenttimeCommand>(cmd); };
        commands["GetLayerCompTime"] =  [](const Command& cmd) { return std::make_unique<GetlayerComptimeCommand>(cmd); };
        commands["GetLayerInPoint"] =  [](const Command& cmd) { return std::make_unique<GetlayerinpointCommand>(cmd); };
        commands["GetLayerInPoint"] =  [](const Command& cmd) { return std::make_unique<GetlayerinpointCommand>(cmd); };
        commands["GetLayerDuration"] =  [](const Command& cmd) { return std::make_unique<GetlayerdurationCommand>(cmd); };
        commands["GetLayerDuration"] =  [](const Command& cmd) { return std::make_unique<GetlayerdurationCommand>(cmd); };
        commands["GetLayerQuality"] =  [](const Command& cmd) { return std::make_unique<GetlayerqualityCommand>(cmd); };
        commands["SetLayerQuality"] =  [](const Command& cmd) { return std::make_unique<SetlayerqualityCommand>(cmd); };
        commands["DeleteLayer"] =  [](const Command& cmd) { return std::make_unique<DeletelayerCommand>(cmd); };
        commands["GetLayerOffset"] =  [](const Command& cmd) { return std::make_unique<GetlayeroffsetCommand>(cmd); };
        commands["SetLayerOffset"] =  [](const Command& cmd) { return std::make_unique<SetlayeroffsetCommand>(cmd); };
        commands["SetLayerFlag"] =  [](const Command& cmd) { return std::make_unique<SetlayerflagCommand>(cmd); };
        commands["GetLayerFlag"] =  [](const Command& cmd) { return std::make_unique<GetlayerflagCommand>(cmd); };
        commands["GetLayerSource"] =  [](const Command& cmd) { return std::make_unique<GetlayersourceCommand>(cmd); };
        commands["getFolderChildren"] =  [](const Command& cmd) { return std::make_unique<GetfolderitemsCommand>(cmd); };
        commands["RemoveLayerFromCollection"] =  [](const Command& cmd) { return std::make_unique<RemovelayerfromcollectionCommand>(cmd); };
        commands["RemoveLayerByIndex"] =  [](const Command& cmd) { return std::make_unique<RemovelayerbyindexCommand>(cmd); };
        commands["GetCompName"] =  [](const Command& cmd) { return std::make_unique<GetcompnameCommand>(cmd); };
        commands["AddLayerToCollection"] =  [](const Command& cmd) { return std::make_unique<AddlayertocollectionCommand>(cmd); };

        commands["GetFootagePath"] =  [](const Command& cmd) { return std::make_unique<GetfootagepathCommand>(cmd); };
        commands["ReplaceFootage"] =  [](const Command& cmd) { return std::make_unique<ReplacefootageCommand>(cmd); };
        commands["ActiveItem"] =  [](const Command& cmd) { return std::make_unique<ActiveitemCommand>(cmd); };
        commands["GetActiveLayer"] =  [](const Command& cmd) { return std::make_unique<GetactivelayerCommand>(cmd); };
        commands["GetProjectName"] =  [](const Command& cmd) { return std::make_unique<GetprojectnameCommand>(cmd); };
        commands["GetProjectPath"] =  [](const Command& cmd) { return std::make_unique<GetprojectpathCommand>(cmd); };
        commands["SaveProjectAs"] =  [](const Command& cmd) { return std::make_unique<SaveprojectasCommand>(cmd); };
        commands["getChildItems"] =  [](const Command& cmd) { return std::make_unique<GetchilditemsCommand>(cmd); };
        commands["getSelectedItems"] =  [](const Command& cmd) { return std::make_unique<GetselecteditemsCommand>(cmd); };
        commands["getItems"] =  [](const Command& cmd) { return std::make_unique<GetitemsCommand>(cmd); };
        commands["appendItem"] =  [](const Command& cmd) { return std::make_unique<AppenditemCommand>(cmd); };
        commands["GetAllLayers"] =  [](const Command& cmd) { return std::make_unique<GetAlllayersCommand>(cmd); };
        commands["CreateFootageItem"] = [](const Command& cmd) { return std::make_unique<CreateFootageItemCommand>(cmd); };
        commands["CreateSolidItem"] = [](const Command& cmd) { return std::make_unique<CreateSolidItemCommand>(cmd); }; // [TODO]
        commands["getAllChildItems"] = [](const Command& cmd) { return std::make_unique<getAllChildItemsCommand>(cmd); };
        commands["CreateFolderItem"] = [](const Command& cmd) { return std::make_unique<CreateFolderItemCommand>(cmd); };
        commands["AppendItem"] = [](const Command& cmd) { return std::make_unique<AppendItemCommand>(cmd); };
        commands["RemoveItem"] = [](const Command& cmd) { return std::make_unique<RemoveItemCommand>(cmd); };
        commands["CreatePanel"] = [](const Command& cmd) { return std::make_unique<CreatePanelCommand>(cmd); };
    }

    std::unique_ptr<CommandBase> createCommand(const std::string& commandName, const Command& cmd) {
        auto it = commands.find(commandName);
        if (it != commands.end()) {
            return it->second(cmd); // Call the factory function with Command object
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, std::function<std::unique_ptr<CommandBase>(const Command&)>> commands;
};