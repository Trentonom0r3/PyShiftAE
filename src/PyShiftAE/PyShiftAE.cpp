
/*	
	PyShiftAE.cpp

	Main Implementation file for PyShiftAE


	version		notes							engineer				date
	
	1.0			First implementation			Trentonom0r3 (Spigon) 	10/31/2023
	
*/
#include "PyShiftAE.h"
#include <filesystem>
#include "CoreLib/Json.h"
#include "MessageManager.h"

static AEGP_PluginID		PyShiftAE			=	6969L;
static AEGP_Command			PyShift				=	6769L;
static A_long				S_idle_count		=	0L;
static SPBasicSuite			*sP					=	NULL;
static AEGP_Command			console			    =   7991L;


PanelatorUI_Plat* PanelatorUI_Plat::instance = nullptr;

std::queue<ScriptTask> scriptExecutionQueue;
std::mutex scriptQueueMutex; // To ensure thread safety
std::atomic<bool> pythonThreadRunning = true;
std::atomic<bool> shouldExitPythonThread = false;

std::mutex resultsMutex;
std::thread pythonThread;
std::condition_variable scriptAddedCond;

bool newScriptAdded = false;


/*
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
*/

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

static A_Err
UpdateMenuHook(
	AEGP_GlobalRefcon		plugin_refconPV,	/* >> */
	AEGP_UpdateMenuRefcon	refconPV,			/* >> */
	AEGP_WindowType			active_window)		/* >> */
{
	A_Err 				err = A_Err_NONE,
		err2 = A_Err_NONE;


	AEGP_SuiteHandler	suites(sP);

	Manifests *manifests = reinterpret_cast<Manifests*>(plugin_refconPV);
	for (int i = 0; i < manifests->manifests.size(); i++) {
		if (manifests->manifests[i].useJS == FALSE)
		{
			ERR(suites.CommandSuite1()->AEGP_EnableCommand(manifests->manifests[i].command));
			//entry in this case will be the GUI script that shall be executed
		}
	}
	ERR(suites.CommandSuite1()->AEGP_EnableCommand(PyShift));
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
	else {
 		Manifests* manifests = reinterpret_cast<Manifests*>(plugin_refconPV);
		for (int i = 0; i < manifests->manifests.size(); i++) {
			if (command == manifests->manifests[i].command) {
				//run the manifest
				//get the manifest from the vector
				Manifest manifest = manifests->manifests[i];
				//venvpath is in same location as manifest, but in a folder called venv. manifest.entryPath is 'entry.py', need to remove that
				std::string venvPath = manifest.entryPath.substr(0, manifest.entryPath.size() - 8) + "venv";
				std::string guiScriptPath = manifest.entryPath;
				ScriptTask task;
				task.scriptPath = guiScriptPath;
				task.resultType = ScriptTask::GUIType;
				{
					std::lock_guard<std::mutex> lock(scriptQueueMutex);
					scriptExecutionQueue.push(std::move(task));
					scriptAddedCond.notify_one();

				}
				*handledPB = TRUE;
			}
			*handledPB = FALSE;
		}

	}

	return err;
}

std::vector<Manifest> getManifests() {
	std::vector<Manifest> manifests = std::vector<Manifest>();
	A_Err 					err = A_Err_NONE,
							err2 = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
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
				A_long cmdID;
				StringToLong(manifest.name.c_str(), &cmdID);
				manifest.command = cmdID;
				manifests.push_back(manifest);
			}
		}
		return manifests;
	}
	// cast manifest list to global refcon, if manifest.use_js == True, do below, if false, we'll create the commmand and register it for use.
	// Then, when the command is called from the menu, we'll match command with manifest.command, and then execute manifest.entryPath
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		std::string error = e.what();
	}
	catch (std::filesystem::filesystem_error& e) {
		std::cout << e.what() << std::endl;
		std::string error = e.what();
	}
	catch (...) {
		std::cout << "Unknown error occurred" << std::endl;
	}
}

void startPythonThread(std::atomic<bool>& shouldExitPythonThread, std::mutex& scriptQueueMutex, std::condition_variable& scriptAddedCond, std::queue<ScriptTask>& scriptExecutionQueue) {
	std::thread pythonThread([&]() {
		py::scoped_interpreter guard{}; // start the interpreter and keep it alive
		py::object pyShiftCore = py::module_::import("PyShiftCore");

		while (!shouldExitPythonThread.load()) {
			ScriptTask task;

			{
				std::unique_lock<std::mutex> lock(scriptQueueMutex);
				scriptAddedCond.wait(lock, [&] { return !scriptExecutionQueue.empty() || shouldExitPythonThread.load(); });

				if (!scriptExecutionQueue.empty()) {
					task = std::move(scriptExecutionQueue.front());
					scriptExecutionQueue.pop();
				}
			}

			if (!task.scriptPath.empty()) {
				switch (task.resultType) {
				case ScriptTask::ManifestType:
					task.manifestPromise.set_value(executeManifestFromFile(task.scriptPath));
					break;
				case ScriptTask::NoResult:
					executeFromFile(task.scriptPath);
					break;
				case ScriptTask::GUIType:
					executeFileInNewProcess(task.scriptPath);
					break;
					// other cases...
				}
			}
		}

		// Finalize the Python interpreter if necessary
		if (shouldExitPythonThread) {
			//finalize();
		}
		});

	pythonThread.detach();
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
	//get the env for PYTHONHOM
	startPythonThread(shouldExitPythonThread, scriptQueueMutex, scriptAddedCond, scriptExecutionQueue);
	pythonThreadRunning = true;

	// Initialize the suite managers
	SuiteManager::GetInstance().InitializeSuiteHandler(sP);
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	SuiteManager::GetInstance().InitializePanelSuiteHandler(sP);

	SuiteManager::GetInstance().SetPluginID(&PyShiftAE); // Set the plugin ID
	
	ERR(suites.CommandSuite1()->AEGP_GetUniqueCommand(&PyShift));
	//ERR(suites.CommandSuite1()->AEGP_GetUniqueCommand(&console));

	//ERR(suites.CommandSuite1()->AEGP_InsertMenuCommand(console,
				//"Python Console",
				//AEGP_Menu_WINDOW,
				//AEGP_MENU_INSERT_SORTED));

	ERR(suites.CommandSuite1()->AEGP_InsertMenuCommand(PyShift,
		"Run Script (.py)",
		AEGP_Menu_FILE,
		AEGP_MENU_INSERT_SORTED));

	ERR(suites.RegisterSuite5()->AEGP_RegisterCommandHook(PyShiftAE,
		AEGP_HP_BeforeAE,
		AEGP_Command_ALL,
		CommandHook,
		0));

	//SuiteHelper<AEGP_PanelSuite1> i_ps(sP);
	//i_ps->AEGP_RegisterCreatePanelHook(PyShiftAE, reinterpret_cast<A_u_char*>("PyConsole"),
		//S_CreatePanelHook, NULL, TRUE);

	ERR(suites.RegisterSuite5()->AEGP_RegisterDeathHook(PyShiftAE, DeathHook, NULL));

	ERR(suites.RegisterSuite5()->AEGP_RegisterUpdateMenuHook(PyShiftAE, UpdateMenuHook, NULL));

	ERR(suites.RegisterSuite5()->AEGP_RegisterIdleHook(PyShiftAE, IdleHook, NULL));

	if (err) { // not !err, err!
		ERR2(suites.UtilitySuite3()->AEGP_ReportInfo(PyShiftAE, "PyShiftAE : Could not register command hook."));
	}

	std::vector<Manifest> manifests = getManifests();
	Manifests *manifestsP = new Manifests();
	manifestsP->manifests = manifests;
	*global_refconV = reinterpret_cast<AEGP_GlobalRefcon>(manifestsP);
	SessionManager::GetInstance();

	return err;
}
		