
/*	
	PyShiftAE.cpp

	Main Implementation file for PyShiftAE


	version		notes							engineer				date
	
	1.0			First implementation			Trentonom0r3 (Spigon) 	10/31/2023
	
*/
#include "PyShiftAE.h"
#include <filesystem>
#include "CoreLib/Json.h"

static AEGP_PluginID		PyShiftAE			=	6969L;
static AEGP_Command			PyShift				=	6769L;
static A_long				S_idle_count		=	0L;
static SPBasicSuite			*sP					=	NULL;
static AEGP_Command			console			    =   7991L;
PanelatorUI_Plat* PanelatorUI_Plat::instance = nullptr;
std::mutex PanelatorUI_Plat::instanceMutex;
std::queue<ScriptTask> scriptExecutionQueue;
std::mutex scriptQueueMutex; // To ensure thread safety
std::atomic<bool> pythonThreadRunning = true;
std::atomic<bool> shouldExitPythonThread = false;
std::mutex resultsMutex;
std::unordered_map<std::string, py::object> scriptResults;
std::thread pythonThread;
std::condition_variable scriptAddedCond;
bool newScriptAdded = false;


static A_Err
S_CreatePanelHook(
	AEGP_GlobalRefcon		plugin_refconP,
	AEGP_CreatePanelRefcon	refconP,
	AEGP_PlatformViewRef	container, //HWN
	AEGP_PanelH				panelH,
	AEGP_PanelFunctions1* outFunctionTable,
	AEGP_PanelRefcon* outRefcon)
{
	PanelatorUI_Plat::InitializeInstance(sP, panelH, container, outFunctionTable);
	PanelatorUI_Plat* myPanel = PanelatorUI_Plat::GetInstance();
	*outRefcon = reinterpret_cast<AEGP_PanelRefcon>(myPanel);
	return A_Err_NONE;
}

static A_Err
DeathHook(
	AEGP_GlobalRefcon	plugin_refconP,
	AEGP_DeathRefcon	refconP)
{
	A_Err	err			= A_Err_NONE;
	AEGP_SuiteHandler	suites(sP);

	A_char report[AEGP_MAX_ABOUT_STRING_SIZE] = {'\0'};
	
	if (pythonThread.joinable()) {
		// Signal the Python thread to stop
		pythonThreadRunning = false;

		shouldExitPythonThread = true; // Signal the thread to exit
		pythonThread.join(); // Wait for the thread to finish
	}

	// Finalize the Python interpreter (if not done in the Python thread)
	//finalize();
	return err;
}

std::tuple<std::string, std::string, std::map<std::string, JsonValue>> processRequest(const struct json_value_s* root) {
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

				// Debug print
				std::cout << "Key: " << argKey << ", Type: " << argValue->type << std::endl;

				auto* stringValue = (struct json_string_s*)argValue->payload;
				switch (argValue->type) {
				case json_type_string:
					argsMap[argKey] = std::string(stringValue->string);	
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

void managePipes(const std::vector<PipeInfo>& pipesInfo) {
	std::vector<std::unique_ptr<PipeInstance>> pipeInstances;

	for (const auto& info : pipesInfo) {
		auto instance = std::make_unique<PipeInstance>(info.name, info.entryPath);
		std::string fullPipeName = "\\\\.\\pipe\\" + info.name;

		// Setup each pipe instance
		instance->hPipe = CreateNamedPipe(
			fullPipeName.c_str(),
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
			PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			1024,
			1024,
			0,
			NULL);

		if (instance->hPipe == INVALID_HANDLE_VALUE) {
			std::cerr << "Failed to create named pipe: " << fullPipeName << std::endl;
			continue;
		}

		BOOL connected = ConnectNamedPipe(instance->hPipe, &instance->overlapped);
		DWORD lastError = GetLastError();
		if (connected) {
			std::cerr << "ConnectNamedPipe should wait for a client on: " << fullPipeName << std::endl;
			continue;
		}
		else if (lastError == ERROR_IO_PENDING) {
			instance->isPending = true;
		}
		else if (lastError == ERROR_PIPE_CONNECTED) {
			SetEvent(instance->overlapped.hEvent);
		}
		else {
			std::cerr << "ConnectNamedPipe failed with error " << lastError << " on: " << fullPipeName << std::endl;
			continue;
		}

		pipeInstances.push_back(std::move(instance));
	}

	while (true) {
		// Manage all pipe instances
		std::vector<HANDLE> events;
		for (const auto& inst : pipeInstances) {
			events.push_back(inst->overlapped.hEvent);
		}

		DWORD wait = WaitForMultipleObjects(events.size(), events.data(), FALSE, INFINITE);
		int idx = wait - WAIT_OBJECT_0;
		if (idx >= 0 && idx < pipeInstances.size()) {
			auto& completedInstance = pipeInstances[idx];
			DWORD transferred;
			GetOverlappedResult(completedInstance->hPipe, &completedInstance->overlapped, &transferred, FALSE);
			completedInstance->isPending = false;

			BOOL success = ReadFile(completedInstance->hPipe, completedInstance->buffer, sizeof(completedInstance->buffer) - 1, &completedInstance->bytesRead, NULL);
			if (success && completedInstance->bytesRead > 0) {
				completedInstance->buffer[completedInstance->bytesRead] = '\0'; // Null-terminate the string

				// Parse the JSON message
				struct json_value_s* root = json_parse(completedInstance->buffer, completedInstance->bytesRead);
				if (root) {
					std::tuple<std::string, std::string, std::map<std::string, JsonValue>> request = processRequest(root);
					free(root);  // Don't forget to free the root
					std::string endP = std::get<0>(request);
					std::string func = std::get<1>(request);
					std::map<std::string, JsonValue> args = std::get<2>(request);
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
						data.name = completedInstance->pipeName;
						data.funcName = func;
						data.args = list;
						ScriptTask task;
						task.jsData = data;
						task.pathFunc = std::make_tuple(completedInstance->entryPath, completedInstance->pipeName);
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
						WriteFile(completedInstance->hPipe, finalResult.c_str(), finalResult.size(), &bytesWritten, NULL);
					}
					else if (endP == "NoResponse") {
						JSData data;
						data.name = completedInstance->pipeName;
						data.funcName = func;
						data.args = list;
						ScriptTask task;
						task.jsData = data;
						task.resultType = ScriptTask::Generic;
						task.scriptPath = "PATH";
						task.pathFunc = std::make_tuple(completedInstance->entryPath, completedInstance->pipeName);
						std::future<std::string> result = task.resultPromise.get_future();
						{
							std::lock_guard<std::mutex> lock(scriptQueueMutex);
							scriptExecutionQueue.push(std::move(task));
							scriptAddedCond.notify_one();
						}
						std::string resultString = result.get();
						DWORD bytesWritten;
						std::string finalResult = resultString + "\n";
						WriteFile(completedInstance->hPipe, finalResult.c_str(), finalResult.size(), &bytesWritten, NULL);
					}
				}
				else {
					std::cerr << "Failed to parse JSON" << std::endl;
				}
			}
			// After processing the message and sending response
			DisconnectNamedPipe(completedInstance->hPipe);

			// Reconnect the pipe for a new client connection
			BOOL connected = ConnectNamedPipe(completedInstance->hPipe, &completedInstance->overlapped);
			DWORD lastError = GetLastError();
			if (connected) {
				std::cerr << "ConnectNamedPipe should wait for a client on: " << completedInstance->pipeName << std::endl;
			}
			else if (lastError == ERROR_IO_PENDING) {
				completedInstance->isPending = true;
			}
			else if (lastError == ERROR_PIPE_CONNECTED) {
				SetEvent(completedInstance->overlapped.hEvent);
			}
			else {
				std::cerr << "ConnectNamedPipe failed with error " << lastError << " on: " << completedInstance->pipeName << std::endl;
			}
		}
		else {
			// Handle read error or no data case
		}
	}
	// Remove completed or failed pipe instances from the vector
	pipeInstances.erase(
		std::remove_if(pipeInstances.begin(), pipeInstances.end(),
			[](const std::unique_ptr<PipeInstance>& inst) { return !inst->isPending; }),
		pipeInstances.end());
}

void createNamedPipes(const std::vector<PipeInfo>& pipesInfo) {
	std::thread pipeManagerThread(managePipes, pipesInfo);
	pipeManagerThread.detach(); // Or join, depending on your application's needs
}


static A_Err
UpdateMenuHook(
	AEGP_GlobalRefcon		plugin_refconPV,	/* >> */
	AEGP_UpdateMenuRefcon	refconPV,			/* >> */
	AEGP_WindowType			active_window)		/* >> */
{
	A_Err 				err = A_Err_NONE,
		err2 = A_Err_NONE;


	AEGP_SuiteHandler	suites(sP);

	ERR(suites.CommandSuite1()->AEGP_EnableCommand(PyShift));
	ERR(suites.CommandSuite1()->AEGP_EnableCommand(console));
	
	return err;
}


static A_Err IdleHook(AEGP_GlobalRefcon plugin_refconP, AEGP_IdleRefcon refconP, A_long* max_sleepPL) {
	A_Err err = A_Err_NONE;
	
	auto message = MessageQueue::getInstance().dequeue();

	if (message != nullptr) {
			message->execute();
			return err;
	}
	
	return err;
}

static A_Err
CommandHook(
	AEGP_GlobalRefcon    plugin_refconPV,        /* >> */
	AEGP_CommandRefcon    refconPV,                /* >> */
	AEGP_Command        command,                /* >> */
	AEGP_HookPriority    hook_priority,            /* >> */
	A_Boolean            already_handledB,        /* >> */
	A_Boolean* handledPB)                /* << */
{
	A_Err            err = A_Err_NONE,
		err2 = A_Err_NONE;

	AEGP_SuiteHandler    suites(sP);
	SuiteHelper<AEGP_PanelSuite1>	i_ps(sP);

	if (command == PyShift) {
		std::cout << "PyShift Command Received" << std::endl;

		// Get the main window handle of After Effects
		HWND ae_hwnd;
		suites.UtilitySuite6()->AEGP_GetMainHWND(&ae_hwnd);

		// Set up an OPENFILENAME structure to configure the file picker dialog
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = ae_hwnd;
		ofn.lpstrFilter = "Python Scripts (*.py)\0*.py\0All Files (*.*)\0*.*\0";
		ofn.lpstrFile = new CHAR[MAX_PATH];
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = "py";

		// Show the file picker dialog
		if (GetOpenFileName(&ofn)) {
			std::string scriptPath = ofn.lpstrFile;
			ScriptTask task;
			task.scriptPath = scriptPath;
			task.resultType = ScriptTask::NoResult;
			{
				std::lock_guard<std::mutex> lock(scriptQueueMutex);
				scriptExecutionQueue.push(std::move(task));
				scriptAddedCond.notify_one();

			}
		}

		else {
			// User cancelled the dialog or an error occurred
			DWORD error = CommDlgExtendedError();
			if (error) {
				std::cerr << "File dialog error: " << error << std::endl;
			}
			else {
				std::cerr << "No script file selected" << std::endl;
			}
		}

		delete[] ofn.lpstrFile;

		*handledPB = TRUE;  // Mark the command as handled
	}
	else if (command == console) {
		i_ps->AEGP_ToggleVisibility(reinterpret_cast<const A_u_char*>("PyConsole"));
		A_Boolean isVisible;
		A_Boolean isRegistered;
		i_ps->AEGP_IsShown(reinterpret_cast<const A_u_char*>("PyConsole"), &isVisible, &isRegistered);
		if (isVisible) {
			ERR(suites.CommandSuite1()->AEGP_CheckMarkMenuCommand(console, TRUE));
		}
		else {
			ERR(suites.CommandSuite1()->AEGP_CheckMarkMenuCommand(console, FALSE));
		}
		*handledPB = TRUE;
	}

	return err;
}



A_Err
EntryPointFunc(
	struct SPBasicSuite* pica_basicP,		/* >> */
	A_long				 	major_versionL,		/* >> */
	A_long					minor_versionL,		/* >> */
	AEGP_PluginID			aegp_plugin_id,		/* >> */
	AEGP_GlobalRefcon* global_refconV)	/* << */
{	

	PyShiftAE = aegp_plugin_id;

	A_Err 					err = A_Err_NONE,
							err2 = A_Err_NONE;

	sP = pica_basicP;

	std::thread pythonThread([]() {
		initialize_python_module(); // Initialize the Python interpreter

		while (!shouldExitPythonThread.load()) {
			ScriptTask task;

			{
				std::unique_lock<std::mutex> lock(scriptQueueMutex);
				scriptAddedCond.wait(lock, [] { return !scriptExecutionQueue.empty() || shouldExitPythonThread.load(); });

				if (!scriptExecutionQueue.empty()) {
					task = std::move(scriptExecutionQueue.front());
					scriptExecutionQueue.pop();
				}
			}

			if (!task.scriptPath.empty()) {
				py::gil_scoped_acquire acquire; // Acquire GIL
				// Inside the Python thread function
				// Inside the Python thread function
				if (task.resultType == ScriptTask::Generic) {
					//get the data from pathFunc tuple
					std::string result = executeJSFunction(std::get<0>(task.pathFunc), std::get<1>(task.pathFunc), task.jsData);
					task.resultPromise.set_value(result);
				}
				else if (task.resultType == ScriptTask::ManifestType) {
					Manifest manifest = executeManifestFromFile(task.scriptPath);
					task.manifestPromise.set_value(manifest);
				}
				else if (task.resultType == ScriptTask::NoResult) {
				executeFromFile(task.scriptPath);

				}
				else if (task.resultType == ScriptTask::FlyOutType) {
					executeFlyoutFunction(*task.flyoutItem);
				}
				else if (task.resultType == ScriptTask::PanelType) {
					Panel panel = executePanelLoad(task.scriptPath);
					task.panelPromise.set_value(panel);
				}

			}

		}

		// Finalize the Python interpreter
		if (shouldExitPythonThread) {
			finalize();
		}
		});

	pythonThread.detach();
	pythonThreadRunning = true;

	//initialize interpreter on the messagequeue thread
	// In EntryPointFunc
	SuiteManager::GetInstance().InitializeSuiteHandler(sP);
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	SuiteManager::GetInstance().InitializePanelSuiteHandler(sP);

	SuiteManager::GetInstance().SetPluginID(&PyShiftAE);
	
	ERR(suites.CommandSuite1()->AEGP_GetUniqueCommand(&PyShift));
	ERR(suites.CommandSuite1()->AEGP_GetUniqueCommand(&console));

	ERR(suites.CommandSuite1()->AEGP_InsertMenuCommand(console,
				"Python Console",
				AEGP_Menu_WINDOW,
				AEGP_MENU_INSERT_SORTED));

	ERR(suites.CommandSuite1()->AEGP_InsertMenuCommand(PyShift,
		"Run Script (.py)",
		AEGP_Menu_FILE,
		AEGP_MENU_INSERT_SORTED));

	ERR(suites.RegisterSuite5()->AEGP_RegisterCommandHook(PyShiftAE,
		AEGP_HP_BeforeAE,
		AEGP_Command_ALL,
		CommandHook,
		0));

	SuiteHelper<AEGP_PanelSuite1> i_ps(sP);
	i_ps->AEGP_RegisterCreatePanelHook(PyShiftAE, reinterpret_cast<A_u_char*>("PyConsole"),
		S_CreatePanelHook, NULL, TRUE);

	ERR(suites.RegisterSuite5()->AEGP_RegisterDeathHook(PyShiftAE, DeathHook, NULL));

	ERR(suites.RegisterSuite5()->AEGP_RegisterUpdateMenuHook(PyShiftAE, UpdateMenuHook, NULL));

	ERR(suites.RegisterSuite5()->AEGP_RegisterIdleHook(PyShiftAE, IdleHook, NULL));
	//if pythonthread is fully initialized, then we can start the python thread, and then start the idle hook.
	//check if python instance is active using pybind11/python
	if (err) { // not !err, err!
		ERR2(suites.UtilitySuite3()->AEGP_ReportInfo(PyShiftAE, "PyShiftAE : Could not register command hook."));
	}

	std::vector<Manifest> manifests = std::vector<Manifest>();
	//C:\Program Files (x86)\Common Files\Adobe\CEP\extensions
	//pluginPath will give us C:\Program Files\Adobe\Common\Plug-ins\7.0\MediaCore
	//need to tae the drive letter and add \Program Files (x86)\Common Files\Adobe\CEP\extensions
	//then search through everyfolder and subfolder for a manifest.py file
	Result<std::string> paths = getPluginPaths();
	if (paths.error != A_Err_NONE) {
		ERR2(suites.UtilitySuite3()->AEGP_ReportInfo(PyShiftAE, "PyShiftAE : Could not get plugin paths."));
	}
	std::string pluginPath = paths.value;
	pluginPath = pluginPath.substr(0, 3);
	pluginPath += "Program Files (x86)\\Common Files\\Adobe\\CEP\\extensions";

	std::string path = pluginPath;
	try {
		for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
			// Find folders following the format "com.psc.[EXTENSION_NAME]"
			std::string folderName = entry.path().filename().string();
			if (folderName.rfind("com.psc.", 0) == 0) {  // More efficient way to check prefix
				//check within for a manifest.py file
				std::string manifestPath = entry.path().string() + "\\manifest.py";
				// Creating a Manifest task
				ScriptTask manifestTask;
				manifestTask.scriptPath = manifestPath;
				manifestTask.resultType = ScriptTask::ManifestType;
				std::future<Manifest> manifestFuture = manifestTask.manifestPromise.get_future();
				Manifest manifest;
				{
					std::lock_guard<std::mutex> lock(scriptQueueMutex);
					scriptExecutionQueue.push(std::move(manifestTask));
					scriptAddedCond.notify_one();
				}
				manifest = manifestFuture.get();
				//turn manifest name into A_long
				std::string name = manifest.name;
				A_long nameLong = 0;
				StringToLong(name.c_str(), &nameLong);
				manifest.command = nameLong;
			
				manifests.push_back(manifest);
			}
		}
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		std::string error = e.what();
	}
	catch (std::filesystem::filesystem_error& e) {
		std::cout << e.what() << std::endl;
		std::string error = e.what();
	}
	try {
		if (manifests.size() > 0) {
			std::vector<PipeInfo> pipeInfos = std::vector<PipeInfo>();
			for (int i = 0; i < manifests.size(); i++) {
				//import the manifest as a new python module, under the manifests name
				std::string name = manifests[i].name;
				std::string entry = manifests[i].entryPath;
				//remove "entry.py" from the end of the entry path
				entry = entry.substr(0, entry.size() - 9);
				PipeInfo pipeInfo;
				pipeInfo.name = name;
				pipeInfo.entryPath = entry;
				if (name != "Plugin Name") {
				pipeInfos.push_back(pipeInfo);
			}
				else if (name == "Plugin Name") {
					//remove the manifest from the vector
					manifests.erase(manifests.begin() + i);
				}
			}
			createNamedPipes(pipeInfos);
		}
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		std::string error = e.what();
	}
	catch (std::filesystem::filesystem_error& e) {
		std::cout << e.what() << std::endl;
		std::string error = e.what();
	}
	return err;
}
			  