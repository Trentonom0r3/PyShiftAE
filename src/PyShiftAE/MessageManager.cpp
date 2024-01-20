#include "MessageManager.h"

std::string createUUID() {
    GUID guid;
    HRESULT result = CoCreateGuid(&guid);  // Generate a new GUID/UUID

    if (FAILED(result)) {
        // Handle error if CoCreateGuid fails
        return "";
    }

    char buffer[39];
    snprintf(buffer, sizeof(buffer),
        "{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2],
        guid.Data4[3], guid.Data4[4], guid.Data4[5],
        guid.Data4[6], guid.Data4[7]);

    return std::string(buffer);
}

void ReportInfoCommand::execute() {
  	auto& mqm = MessageQueueManager::getInstance();
    try {
        std::string info = boost::get<std::string>(cmd.args[0]);
        App app;
        app.reportInfo(info);
        mqm.sendSuccessResponse(cmd.sessionID);
    }
    catch (const boost::bad_get& e) {
        Response resp;
        resp.sessionID = cmd.sessionID;
        std::string error = e.what();
        resp.args.push_back(error);
        mqm.sendResponse(resp);
    }
}

void GetProjectCommand::execute() {
  	auto& mqm = MessageQueueManager::getInstance();
    try {
		App app;
		Project project = app.getProject();
		std::string projectID = createUUID();
		SessionManager::GetInstance().addToSessions(projectID, std::make_shared<Project>(project));
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(projectID);
		mqm.sendResponse(resp);
	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void IsitemselectedCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& itemVariant = SessionManager::GetInstance().getSessionObject(cmd.sessionID);

		bool isSelected = std::visit([](const auto& arg) -> bool {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Item>>) {
				return arg->isSelected();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				return arg->isSelected();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FootageItem>>) {
				return arg->isSelected();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FolderItem>>) {
				return arg->isSelected();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<SolidItem>>) {
				return arg->isSelected();
			}
			// Handle other types or add a generic handler
			else {
				return false;
			}
			}, itemVariant);

		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(isSelected);
		mqm.sendResponse(resp);
	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void SelectitemCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		bool isSelected = boost::get<bool>(cmd.args[0]);
		const auto& itemVariant = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::visit([&isSelected](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Item>>) {
				arg->setSelected(isSelected);
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				arg->setSelected(isSelected);
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FootageItem>>) {
				arg->setSelected(isSelected);
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FolderItem>>) {
				arg->setSelected(isSelected);
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<SolidItem>>) {
				arg->setSelected(isSelected);
			}
			// Handle other types or add a generic handler
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, itemVariant);

		 mqm.sendSuccessResponse(cmd.sessionID);
	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void BeginUndoGroupCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		std::string undoName = boost::get<std::string>(cmd.args[0]);
		App app;
		app.beginUndoGroup(undoName);
		mqm.sendSuccessResponse(cmd.sessionID);
	}
    catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void EndUndoGroupCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		App app;
		app.endUndoGroup();
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
    catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void ExecutecommandCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		int commandID = boost::get<int>(cmd.args[0]);
		App app;
		app.executeCommand(commandID);
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
    catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetpluginpathsCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		App app;
		std::string pluginPaths = app.pluginPaths();
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(pluginPaths);
		mqm.sendResponse(resp);
	}
    catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetitemnameCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& itemVariant = SessionManager::GetInstance().getSessionObject(cmd.sessionID);

		std::string name = std::visit([](const auto& arg) -> std::string {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Item>>) {
				return arg->getName();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				return arg->getName();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FootageItem>>) {
				return arg->getName();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FolderItem>>) {
				return arg->getName();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<SolidItem>>) {
				return arg->getName();
			}
			// Handle other types or add a generic handler
			else {
				return "Unsupported type";
			}
			}, itemVariant);

		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(name);
		mqm.sendResponse(resp);
	}
	catch (const std::exception& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(e.what());
		mqm.sendResponse(resp);
	}
}



void SetitemnameCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		std::string name = boost::get<std::string>(cmd.args[0]);
		const auto& itemVariant = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::visit([&name](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Item>>) {
				arg->setName(name);
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				arg->setName(name);
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FootageItem>>) {
				arg->setName(name);
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FolderItem>>) {
				arg->setName(name);
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<SolidItem>>) {
				arg->setName(name);
			}
			// Handle other types or add a generic handler
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, itemVariant);
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
    catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetitemwidthCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		const auto& itemVariant = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		int width = std::visit([](const auto& arg) -> int {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Item>>) {
				return arg->getWidth();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				return arg->getWidth();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FootageItem>>) {
				return arg->getWidth();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FolderItem>>) {
				return arg->getWidth();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<SolidItem>>) {
				return arg->getWidth();
			}
			// Handle other types or add a generic handler
			else {
				return 0;
			}
			}, itemVariant);	

		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(width);
		mqm.sendResponse(resp);
	}
    catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}

}

void GetitemheightCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		const auto& itemVariant = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		int height = std::visit([](const auto& arg) -> int {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Item>>) {
				return arg->getHeight();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				return arg->getHeight();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FootageItem>>) {
				return arg->getHeight();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FolderItem>>) {
				return arg->getHeight();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<SolidItem>>) {
				return arg->getHeight();
			}
			// Handle other types or add a generic handler
			else {
				return 0;
			}
			}, itemVariant);

		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(height);
		mqm.sendResponse(resp);
	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);

	}
}

void GetitemcurrenttimeCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& itemVariant = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		int currentTime = std::visit([](const auto& arg) -> int {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Item>>) {
				return arg->getCurrentTime();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				return arg->getCurrentTime();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FootageItem>>) {
				return arg->getCurrentTime();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FolderItem>>) {
				return arg->getCurrentTime();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<SolidItem>>) {
				return arg->getCurrentTime();
			}
			// Handle other types or add a generic handler
			else {
				return 0;
			}
			}, itemVariant);

		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(currentTime);
		mqm.sendResponse(resp);
	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetitemdurationCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& itemVariant = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		int duration = std::visit([](const auto& arg) -> int {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Item>>) {
				return arg->getDuration();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				return arg->getDuration();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FootageItem>>) {
				return arg->getDuration();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FolderItem>>) {
				return arg->getDuration();
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<SolidItem>>) {
				return arg->getDuration();
			}
			// Handle other types or add a generic handler
			else {
				return 0;
			}
			}, itemVariant);

		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(duration);
		mqm.sendResponse(resp);
	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);

		mqm.sendResponse(resp);
	}
}

void SetitemcurrenttimeCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		int currentTime = boost::get<int>(cmd.args[0]);
		const auto& itemVariant = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::visit([&currentTime](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Item>>) {
				arg->setCurrentTime(currentTime);
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				arg->setCurrentTime(currentTime);
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FootageItem>>) {
				arg->setCurrentTime(currentTime);
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<FolderItem>>) {
				arg->setCurrentTime(currentTime);
			}
			else if constexpr (std::is_same_v<T, std::shared_ptr<SolidItem>>) {
				arg->setCurrentTime(currentTime);
			}
			// Handle other types or add a generic handler
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, itemVariant);

		 mqm.sendSuccessResponse(cmd.sessionID);
	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetcompframerateCommand::execute() {
  	auto& mqm = MessageQueueManager::getInstance();
    try {
		const auto& compItem = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		float frameRate = std::visit([](const auto& arg) -> float {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				return arg->getFrameRate();
			}
			else {
				return 0.0f;
			}
			}, compItem);
        Response resp;
        resp.sessionID = cmd.sessionID;
        resp.args.push_back(frameRate);
        mqm.sendResponse(resp);
    }
    catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void SetcompframerateCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		float frameRate = boost::get<float>(cmd.args[0]);
		 const auto& compItem = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		 std::visit([&frameRate](auto&& arg) {
			 using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				 arg->setFrameRate(frameRate);
			 }
			 else {
				 throw std::runtime_error("Unsupported type");
			 }
			 }, compItem);
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
    catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetcompworkareadurationCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		const auto& compItem = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		int duration = std::visit([](const auto& arg) -> int {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				return arg->getDuration();
			}
			else {
				return 0;
			}
			}, compItem);
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(duration);
		mqm.sendResponse(resp);
	}
    catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void SetcompdurationCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		int duration = boost::get<int>(cmd.args[0]);
		const auto& compItem = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::visit([&duration](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				arg->setDuration(duration);
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, compItem);
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
    catch (const boost::bad_get& e) {

		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void SetcompwidthCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		int width = boost::get<int>(cmd.args[0]);
		const auto& compItem = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::visit([&width](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				arg->setWidth(width);
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, compItem);
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
    catch (const boost::bad_get& e) {

		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void SetcompheightCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		int height = boost::get<int>(cmd.args[0]);
		const auto& compItem = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::visit([&height](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				arg->setHeight(height);
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, compItem);
		mqm.sendSuccessResponse(cmd.sessionID);
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
    catch (const boost::bad_get& e) {

		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}


void GetselectedlayersCommand::execute() {
  	auto& mqm = MessageQueueManager::getInstance();
    try {
		const auto& comp = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::shared_ptr<LayerCollection> layers = std::visit([](const auto& arg) -> std::shared_ptr<LayerCollection> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				return arg->getSelectedLayers();
			}
			else {
				return nullptr;
			}
			}, comp);
		std::string layersID = createUUID();
		SessionManager::GetInstance().addToSessions(layersID, layers);
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(layersID);
		mqm.sendResponse(resp);

    }
    catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetlayersCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& comp = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::shared_ptr<LayerCollection> layers = std::visit([](const auto& arg) -> std::shared_ptr<LayerCollection> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				return arg->getLayers();
			}
			else {
				return nullptr;
			}
			}, comp);
		std::string layersID = createUUID();
		SessionManager::GetInstance().addToSessions(layersID, layers);
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(layersID);
		mqm.sendResponse(resp);

	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}
void GetAlllayersCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		const auto& comp = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::shared_ptr<LayerCollection> layers = std::visit([](const auto& arg) -> std::shared_ptr<LayerCollection> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				return arg->getLayers();
			}
			else {
				return nullptr;
			}
			}, comp);
		std::string layersID = createUUID();
		SessionManager::GetInstance().addToSessions(layersID, layers);
		Response resp;
		resp.sessionID = cmd.sessionID;
		

		mqm.sendResponse(resp);

	}
    catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetnumlayersCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		const auto& comp = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		int numLayers = std::visit([](const auto& arg) -> int {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				return arg->NumLayers();
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, comp);
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(numLayers);
		mqm.sendResponse(resp);

	}
    catch (const boost::bad_get& e) {

		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetcompitemcurrenttimeCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		const auto& comp = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		int currentTime = std::visit([](const auto& arg) -> int {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				return arg->getCurrentTime();
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, comp);
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(currentTime);
		mqm.sendResponse(resp);

	}
    catch (const boost::bad_get& e) {

		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void SetcompitemcurrenttimeCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		int currentTime = boost::get<int>(cmd.args[0]);
		const auto& comp = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::visit([&currentTime](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				arg->setCurrentTime(currentTime);
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, comp);
		 mqm.sendSuccessResponse(cmd.sessionID);
		
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
    catch (const boost::bad_get& e) {

		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void AddlayertocompCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
    try {
		std::string footagePath = boost::get<std::string>(cmd.args[1]);
        std::string name = boost::get<std::string>(cmd.args[0]);
		const auto& comp = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::visit([&name, &footagePath](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				arg->addLayer(name, footagePath);
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, comp);

        mqm.sendSuccessResponse(cmd.sessionID);
    }
    catch (const boost::bad_get& e) {

		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetlayernameCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::string name = std::visit([](const auto& arg) -> std::string {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				return arg->GetLayerName();
			}
			else {
				return "";
			}
			}, layer);

		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(name);
		mqm.sendResponse(resp);

	}
	catch (const boost::bad_get& e) {

		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetlayersourcenameCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::string sourceName = std::visit([](const auto& arg) -> std::string {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				return arg->GetSourceName();
			}
			else {
				return "";
			}
			}, layer);

		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(sourceName);
		mqm.sendResponse(resp);

	}
	catch (const boost::bad_get& e) {

		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void SetlayernameCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::string name = boost::get<std::string>(cmd.args[0]);

		std::visit([&name](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				arg->SetLayerName(name);
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, layer);
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
	catch (const boost::bad_get& e) {

		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetlayerindexCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		int index = std::visit([](const auto& arg) -> int {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				return arg->index();
			}
			else {
				return 0;
			}
			}, layer);

		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(index);
		mqm.sendResponse(resp);

	}
	catch (const boost::bad_get& e) {

		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void ChangelayerindexCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		int index = boost::get<int>(cmd.args[0]);
		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);

		std::visit([&index](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				arg->changeIndex(index);
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, layer);
		 mqm.sendSuccessResponse(cmd.sessionID);

		 mqm.sendSuccessResponse(cmd.sessionID);
	}
	catch (const boost::bad_get& e) {

		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void DuplicatelayerCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::visit([](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				arg->duplicate();
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, layer);
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetlayercurrenttimeCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		int currentTime = std::visit([](const auto& arg) -> int {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				return arg->layerTime();
			}
			else {
				return 0;
			}
			}, layer);

		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(currentTime);
		mqm.sendResponse(resp);

	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetlayerComptimeCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		int compTime = std::visit([](const auto& arg) -> int {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				return arg->layerCompTime();
			}
			else {
				return 0;
			}
			}, layer);
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(compTime);
		mqm.sendResponse(resp);

	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetlayerinpointCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		int inPoint = std::visit([](const auto& arg) -> int {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				return arg->inPoint();
			}
			else {
				return 0;
			}
			}, layer);

		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(inPoint);
		mqm.sendResponse(resp);

	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}

}

void GetlayerdurationCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		int duration = std::visit([](const auto& arg) -> int {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				return arg->duration();
			}
			else {
				return 0;
			}
			}, layer);
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(duration);
		mqm.sendResponse(resp);

	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}

}

void GetlayerqualityCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::string quality = std::visit([](const auto& arg) -> std::string {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				return arg->getQuality();
			}
			else {
				return "";
			}
			}, layer);
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(quality);
		mqm.sendResponse(resp);

	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}

}

void SetlayerqualityCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		int quality = boost::get<int>(cmd.args[0]);

		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::visit([&quality](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				arg->setQuality(quality);
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, layer);
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
	catch (const boost::bad_get& e) {

		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}

}

void DeletelayerCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {

		 const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		 std::visit([](auto&& arg) {
			 using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				 arg->deleteLayer();
			 }
			 else {
				 throw std::runtime_error("Unsupported type");
			 }
			 }, layer);
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}

}

void GetlayeroffsetCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
        const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		int offset = std::visit([](const auto& arg) -> int {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				return arg->getOffset();
			}
			else {
				return 0;
			}
			}, layer);
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(offset);
		mqm.sendResponse(resp);

	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}

}

void SetlayeroffsetCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		int offset = boost::get<int>(cmd.args[0]);

		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::visit([&offset](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				arg->setOffset(offset);
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, layer);
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
	catch (const boost::bad_get& e) {

		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}

}

void SetlayerflagCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		int flag = boost::get<int>(cmd.args[0]);
		LayerFlag flagH = static_cast<LayerFlag>(flag);
		bool value = boost::get<bool>(cmd.args[1]);

		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::visit([&flagH, &value](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				arg->setFlag(flagH, value);
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, layer);
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
	catch (const boost::bad_get& e) {

		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}

}

void GetlayerflagCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		int flag = boost::get<int>(cmd.args[0]);
		LayerFlag flagH = static_cast<LayerFlag>(flag);
		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		bool value = std::visit([&flagH](auto&& arg) -> bool {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				return arg->getFlag(flagH);
			}
			else {
				return false;
			}
			}, layer);
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(value);
		mqm.sendResponse(resp);

	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}

}

void GetlayersourceCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& layer = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::shared_ptr<Item> source = std::visit([](const auto& arg) -> std::shared_ptr<Item> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				return arg->getSource();
			}
			else {
				return nullptr;
			}
			}, layer);

		std::string sourceID = createUUID();
		std::string type = source->getType();
		Response resp;
		resp.sessionID = cmd.sessionID;
		if (type == "Comp") {
			std::shared_ptr<CompItem> comp = std::dynamic_pointer_cast<CompItem>(source);
			SessionManager::GetInstance().addToSessions(sourceID, comp);
			resp.args.push_back(sourceID);
			resp.args.push_back(type);
			mqm.sendResponse(resp);
		}
		else if (type == "Folder") {
			std::shared_ptr<FolderItem> folder = std::dynamic_pointer_cast<FolderItem>(source);
			SessionManager::GetInstance().addToSessions(sourceID, folder);
			resp.args.push_back(sourceID);
			resp.args.push_back(type);
			mqm.sendResponse(resp);

		}
		else if (type == "Footage") {
			std::shared_ptr<FootageItem> footage = std::dynamic_pointer_cast<FootageItem>(source);
			SessionManager::GetInstance().addToSessions(sourceID, footage);
			resp.args.push_back(sourceID);
			resp.args.push_back(type);
			mqm.sendResponse(resp);
		}
	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}

}

void GetchilditemsCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& folder = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::shared_ptr<ItemCollection> items = std::visit([](const auto& arg) -> std::shared_ptr<ItemCollection> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<FolderItem>>) {
				return arg->ChildItems();
			}
			else {
				return nullptr;
			}
			}, folder);

		std::string itemsID = createUUID();
		SessionManager::GetInstance().addToSessions(itemsID, items);
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(itemsID);
		mqm.sendResponse(resp);

	}
	catch (const boost::bad_get& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;

		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}

}
void RemovelayerfromcollectionCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& layerCollectionVariant = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		const auto& layerVariant = SessionManager::GetInstance().getSessionObject(boost::get<std::string>(cmd.args[0]));

		// Ensure that the variant holds std::shared_ptr<LayerCollection>
		auto layerCollection = std::visit([](const auto& arg) -> std::shared_ptr<LayerCollection> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<LayerCollection>>) {
				return arg;
			}
			else {
				throw std::runtime_error("Invalid type for layer collection");
			}
			}, layerCollectionVariant);

		// Ensure that the variant holds std::shared_ptr<Layer>
		auto layer = std::visit([](const auto& arg) -> std::shared_ptr<Layer> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Layer>>) {
				return arg;
			}
			else {
				throw std::runtime_error("Invalid type for layer");
			}
			}, layerVariant);

		if (layerCollection && layer) {
			layerCollection->removeLayerFromCollection(*layer);
			mqm.sendSuccessResponse(cmd.sessionID);
		}
		else {
			throw std::runtime_error("Layer or LayerCollection is null");
		}
	}
	catch (const std::exception& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(e.what());
		mqm.sendResponse(resp);
	}
}


void RemovelayerbyindexCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		int index = boost::get<int>(cmd.args[0]);
		 const auto& layers = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		 std::visit([&index](auto&& arg) {
			 using T = std::decay_t<decltype(arg)>;
			 if constexpr (std::is_same_v<T, std::shared_ptr<LayerCollection>>) {
				 arg->RemoveLayerByIndex(index);
			 }
			 else {
				 throw std::runtime_error("Unsupported type");
			 }
			 }, layers);
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
	catch (const boost::bad_get& e) {
		Response resp;

		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetcompnameCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& comp = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::string name = std::visit([](const auto& arg) -> std::string {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<CompItem>>) {
				return arg->getName();
			}
			else {
				return "";
			}
			}, comp);

		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(name);
		mqm.sendResponse(resp);

	}
	catch (const boost::bad_get& e) {
		Response resp;

		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void AddlayertocollectionCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& layersVariant = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		const auto& layerVariant = SessionManager::GetInstance().getSessionObject(boost::get<std::string>(cmd.args[0]));

		std::shared_ptr<LayerCollection> layerCollection = std::visit([&](const auto& arg) -> std::shared_ptr<LayerCollection> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<LayerCollection>>) {
				return arg;
			}
			else {
				throw std::runtime_error("Invalid type for layer collection");
			}
			}, layersVariant);

		std::shared_ptr<FootageItem> footageItemH = std::visit([&](const auto& arg) -> std::shared_ptr<FootageItem> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<FootageItem>>) {
				return arg;
			}
			else {
				throw std::runtime_error("Invalid type for FootageItem");
			}
			}, layerVariant);

		if (layerCollection && footageItemH) {
			layerCollection->addLayerToCollection(*footageItemH);
			mqm.sendSuccessResponse(cmd.sessionID);
		}
		else {
			throw std::runtime_error("Layer or LayerCollection is null");
		}
	}
	catch (const std::exception& e) {
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(e.what());
		mqm.sendResponse(resp);
	}
}



void GetitemsCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {

		const auto& projects = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::vector<std::shared_ptr<Item>> items = std::visit([](const auto& arg) -> std::vector<std::shared_ptr<Item>> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<ProjectCollection>>) {
				return arg->getItems();
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, projects);
		Response resp;
		resp.sessionID = cmd.sessionID;
		for (auto item : items) {
			std::string itemID = createUUID();
			std::string type = item->getType();
			std::vector<std::string> sessionIDs;
			std::vector<std::string> types;
			if (type == "Comp") {
				//turn item into comp, then add to sessions
				std::shared_ptr<CompItem> comp = std::dynamic_pointer_cast<CompItem>(item);
				sessionIDs.push_back(itemID);
				types.push_back("Comp");
				SessionManager::GetInstance().addToSessions(itemID, comp);
			}
			else if (type == "Folder") {
				//turn item into folder, then add to sessions
				std::shared_ptr<FolderItem> folder = std::dynamic_pointer_cast<FolderItem>(item);
				sessionIDs.push_back(itemID);
				types.push_back("Folder");
				SessionManager::GetInstance().addToSessions(itemID, folder);
			}
			else if (type == "Footage") {
				//turn item into footage, then add to sessions
				std::shared_ptr<FootageItem> footage = std::dynamic_pointer_cast<FootageItem>(item);
				sessionIDs.push_back(itemID);
				types.push_back("Footage");
				SessionManager::GetInstance().addToSessions(itemID, footage);
			}
			resp.args.push_back(sessionIDs);
			resp.args.push_back(types);
		}
		mqm.sendResponse(resp);

		}
	catch (const boost::bad_get& e) {
		Response resp;

		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
	}

void GetselecteditemsCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& projects = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::vector<std::shared_ptr<Item>> items = std::visit([](const auto& arg) -> std::vector<std::shared_ptr<Item>> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<ProjectCollection>>) {
				return arg->getItems();
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, projects);

		for (auto item : items) {
			if (item->isSelected()) {
				std::string itemID = createUUID();
				std::string type = item->getType();
				std::vector<std::string> sessionIDs;
				std::vector<std::string> types;
				if (type == "Comp") {
					//turn item into comp, then add to sessions
					std::shared_ptr<CompItem> comp = std::dynamic_pointer_cast<CompItem>(item);
					sessionIDs.push_back(itemID);
					types.push_back("Comp");
					SessionManager::GetInstance().addToSessions(itemID, comp);
				}
				else if (type == "Folder") {
					//turn item into folder, then add to sessions
					std::shared_ptr<FolderItem> folder = std::dynamic_pointer_cast<FolderItem>(item);
					sessionIDs.push_back(itemID);
					types.push_back("Folder");
					SessionManager::GetInstance().addToSessions(itemID, folder);
				}
				else if (type == "Footage") {
					//turn item into footage, then add to sessions
					std::shared_ptr<FootageItem> footage = std::dynamic_pointer_cast<FootageItem>(item);
					sessionIDs.push_back(itemID);
					types.push_back("Footage");
					SessionManager::GetInstance().addToSessions(itemID, footage);
				}
				Response resp;
				resp.sessionID = cmd.sessionID;
				resp.args.push_back(sessionIDs);
				resp.args.push_back(types);
				mqm.sendResponse(resp);
			}
		}
	}
	catch (const boost::bad_get& e) {
		Response resp;

		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}

void GetfootagepathCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& footage = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::string path = std::visit([](const auto& arg) -> std::string {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<FootageItem>>) {
				return arg->getPath();
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, footage);

		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(path);
		mqm.sendResponse(resp);

	}
	catch (const boost::bad_get& e) {
		Response resp;

		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);
	}
}


void ReplacefootageCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& footage = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::string name = boost::get<std::string>(cmd.args[0]);
		std::string path = boost::get<std::string>(cmd.args[1]);

		std::visit([&name, &path](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<FootageItem>>) {
				arg->replaceWithNewSource(name, path);
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, footage);
		 mqm.sendSuccessResponse(cmd.sessionID);
	} 
	catch (const boost::bad_get& e) {
		Response resp;

		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);

	}
}

void ActiveitemCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		// Check if the session ID is valid and get the Project object
		const auto& project = SessionManager::GetInstance().getSessionObject(cmd.sessionID);

		// Get the active item


		std::shared_ptr<Item> item = std::visit([](const auto& arg) -> std::shared_ptr<Item> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Project>>) {
				return arg->ActiveItem();
			}
			else {
				return nullptr;
			}
			}, project);

		if (!item) {
			throw std::runtime_error("Active item is null");
		}

		std::string type = item->getType();
		std::string itemID = boost::get<std::string>(cmd.args.at(0));  // Use at() for bounds checking
		Response resp;
		resp.sessionID = cmd.sessionID;

		// Handling different types of items
		if (type == "Comp") {
			std::shared_ptr<CompItem> comp = std::dynamic_pointer_cast<CompItem>(item);
			SessionManager::GetInstance().addToSessions(itemID, comp);
			resp.args.push_back(itemID);
			resp.args.push_back(type);
		}
		else if (type == "Folder") {
			std::shared_ptr<FolderItem> folder = std::dynamic_pointer_cast<FolderItem>(item);
			SessionManager::GetInstance().addToSessions(itemID, folder);
			resp.args.push_back(itemID);
			resp.args.push_back(type);
		}
		else if (type == "Footage") {
			std::shared_ptr<FootageItem> footage = std::dynamic_pointer_cast<FootageItem>(item);
			SessionManager::GetInstance().addToSessions(itemID, footage);
			resp.args.push_back(itemID);
			resp.args.push_back(type);
		}
		else {
			throw std::runtime_error("Unknown item type");
		}

		mqm.sendResponse(resp);

	}
	catch (const std::runtime_error& e) {
		// Handle runtime errors, such as session ID validation or type mismatches
		Response errorResponse;
		errorResponse.sessionID = cmd.sessionID;
		errorResponse.args.push_back(std::string("Runtime error: ") + e.what());
		mqm.sendResponse(errorResponse);

	}
	catch (const boost::bad_get& e) {
		// Handle bad_get exception from boost::get
		Response errorResponse;
		errorResponse.sessionID = cmd.sessionID;
		errorResponse.args.push_back(std::string("Argument retrieval error: ") + e.what());
		mqm.sendResponse(errorResponse);

	}
	catch (const std::exception& e) {
		// Handle any other standard exceptions
		Response errorResponse;
		errorResponse.sessionID = cmd.sessionID;
		errorResponse.args.push_back(std::string("Exception: ") + e.what());
		mqm.sendResponse(errorResponse);

	}
	catch (...) {
		// Catch any other non-standard exceptions
		Response errorResponse;
		errorResponse.sessionID = cmd.sessionID;
		errorResponse.args.push_back("Unknown exception occurred");
		mqm.sendResponse(errorResponse);
	}
}


void GetactivelayerCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& project = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::shared_ptr<Layer> layer = std::visit([](const auto& arg) -> std::shared_ptr<Layer> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Project>>) {
				return arg->GetActiveLayer();
			}
			else {
				return nullptr;
			}
			}, project);
		std::string layerID = createUUID();
		SessionManager::GetInstance().addToSessions(layerID, layer);
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(layerID);
		mqm.sendResponse(resp);
	} 
	catch (const boost::bad_get& e) {
		Response resp;

		resp.sessionID = cmd.sessionID;

		std::string error = e.what();

		resp.args.push_back(error);
		mqm.sendResponse(resp);

	}
}

void GetprojectnameCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& project = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::string name = std::visit([](const auto& arg) -> std::string {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Project>>) {
				return arg->getName();
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, project);
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(name);
		mqm.sendResponse(resp);
	}
	catch (const boost::bad_get& e) {
		Response resp;

		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);

	}
}

void GetprojectpathCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& project = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::string path = std::visit([](const auto& arg) -> std::string {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Project>>) {
				return arg->getPath();
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, project);	
		Response resp;
		resp.sessionID = cmd.sessionID;
		resp.args.push_back(path);
		mqm.sendResponse(resp);
	}
	catch (const boost::bad_get& e) {
		Response resp;

		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);

	}
}

void SaveprojectasCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		std::string path = boost::get<std::string>(cmd.args[0]);
		const auto& project = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		std::visit([&path](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, std::shared_ptr<Project>>) {
				arg->saveAs(path);
			}
			else {
				throw std::runtime_error("Unsupported type");
			}
			}, project);
		 mqm.sendSuccessResponse(cmd.sessionID);
	}
	catch (const boost::bad_get& e) {
		Response resp;

		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);

	}
}

void AppenditemCommand::execute() {
	auto& mqm = MessageQueueManager::getInstance();
	try {
		const auto& projectC = SessionManager::GetInstance().getSessionObject(cmd.sessionID);
		const auto& itemVariant = SessionManager::GetInstance().getSessionObject(boost::get<std::string>(cmd.args[0]));

		std::shared_ptr<Item> item = std::visit([&](const auto& arg) -> std::shared_ptr<Item> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<Item>>) {
				return arg;
			}
			else {
				throw std::runtime_error("Invalid type for item");
			}
			}, itemVariant);

		std::shared_ptr<ProjectCollection> projects = std::visit([&](const auto& arg) -> std::shared_ptr<ProjectCollection> {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::shared_ptr<ProjectCollection>>) {
				return arg;
			}
			else {
				throw std::runtime_error("Invalid type for ProjectCollection");
			}
			}, projectC);

		// Ensure projects is valid before using it
		if (!projects) {
			throw std::runtime_error("ProjectCollection is null");
		}

		Response resp;
		resp.sessionID = cmd.sessionID;
		std::vector<std::string> sessionIDs;
		std::vector<std::string> types;
		for (auto item : projects->getItems()) {
			std::string itemID = createUUID();
			std::string type = item->getType();
			if (type == "Comp") {
				//turn item into comp, then add to sessions
				std::shared_ptr<CompItem> comp = std::dynamic_pointer_cast<CompItem>(item);
				sessionIDs.push_back(itemID);
				types.push_back("Comp");
				SessionManager::GetInstance().addToSessions(itemID, comp);
			}
			else if (type == "Folder") {
				//turn item into folder, then add to sessions
				std::shared_ptr<FolderItem> folder = std::dynamic_pointer_cast<FolderItem>(item);
				sessionIDs.push_back(itemID);
				types.push_back("Folder");
				SessionManager::GetInstance().addToSessions(itemID, folder);
			}
			else if (type == "Footage") {
				//turn item into footage, then add to sessions
				std::shared_ptr<FootageItem> footage = std::dynamic_pointer_cast<FootageItem>(item);
				sessionIDs.push_back(itemID);
				types.push_back("Footage");
				SessionManager::GetInstance().addToSessions(itemID, footage);
			}
		}
		resp.args.push_back(sessionIDs);
		resp.args.push_back(types);
		mqm.sendResponse(resp);

	}
	catch (const boost::bad_get& e) {
		Response resp;

		resp.sessionID = cmd.sessionID;
		std::string error = e.what();
		resp.args.push_back(error);
		mqm.sendResponse(resp);

	}
};
