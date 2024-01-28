
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


static A_Err
S_CreatePanelHook(
	AEGP_GlobalRefcon		plugin_refconP,
	AEGP_CreatePanelRefcon	refconP,
	AEGP_PlatformViewRef	container, //HWN
	AEGP_PanelH				panelH,
	AEGP_PanelFunctions1* outFunctionTable,
	AEGP_PanelRefcon* outRefcon)
{
	try {
		Manifest* manifest = reinterpret_cast<Manifest*>(refconP);
		std::string name = manifest->name;
		std::string version = manifest->version;
		std::string sessionID = name + version;
		// Construct the path to the Python executable in the venv
		std::filesystem::path entryPathObj(manifest->entryPath);
		std::filesystem::path venvPath = entryPathObj.parent_path() / "venv" / "Scripts" / "python.exe";
		// Form the command to run the Python script
		std::string venvPathS = venvPath.string();
		//create command which is the path to the python executable, followed by the path to the script, wrapped in quotes
		std::string command = "\"" + venvPathS + "\" \"" + manifest->entryPath + "\"";

		std::cout << "Executing command: " << command << std::endl;
		SessionManager::GetInstance().executeCommandAsync(command, sessionID);

		int timeout = 100;
		while (timeout > 0) {
			if (SessionManager::GetInstance().isHWNDStored(sessionID)) {
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			timeout--;
			if (timeout == 0) {
				std::cout << "Timeout reached" << std::endl;
				App app;
				app.reportInfo("Timed out finding HWND");
				return A_Err_PARAMETER;
			}
		}

		PanelatorUI_Plat* myPanel = new PanelatorUI_Plat(sessionID, sP, panelH, container, outFunctionTable);
		*outRefcon = reinterpret_cast<AEGP_PanelRefcon>(myPanel);
		return A_Err_NONE;
	}
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
				ERR(suites.CommandSuite1()->AEGP_GetUniqueCommand(&cmdID));
				manifest.command = cmdID;
				manifests.push_back(manifest);
				ERR(suites.CommandSuite1()->AEGP_InsertMenuCommand(manifest.command,
					manifest.name.c_str(),
					AEGP_Menu_WINDOW,
					AEGP_MENU_INSERT_SORTED));

				SuiteHelper<AEGP_PanelSuite1> i_ps(sP);
				std::string name = manifest.name;
				const A_u_char* matchName = reinterpret_cast<const A_u_char*>(name.c_str());
				Manifest* manifestP = new Manifest();
				*manifestP = manifest;
				AEGP_CreatePanelRefcon refcon = reinterpret_cast<AEGP_CreatePanelRefcon>(manifestP);
				i_ps->AEGP_RegisterCreatePanelHook(PyShiftAE, matchName,
					S_CreatePanelHook, refcon, FALSE);

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
		//py::object pyShiftCore = py::module_::import("PyShiftCore");

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
	SessionManager::GetInstance().cleanAll();
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
	SuiteHelper<AEGP_PanelSuite1>	i_ps(sP);
	A_Boolean	out_thumb_is_shownB = FALSE, out_panel_is_frontmostB = FALSE;

	Manifests *manifests = reinterpret_cast<Manifests*>(plugin_refconPV);
	for (int i = 0; i < manifests->manifests.size(); i++) {
		if (manifests->manifests[i].useJS == FALSE)
		{
			ERR(suites.CommandSuite1()->AEGP_EnableCommand(manifests->manifests[i].command));
			i_ps->AEGP_IsShown(reinterpret_cast<const A_u_char*>(manifests->manifests[i].name.c_str()), &out_thumb_is_shownB, &out_panel_is_frontmostB);
			suites.CommandSuite1()->AEGP_CheckMarkMenuCommand(manifests->manifests[i].command, out_thumb_is_shownB && out_panel_is_frontmostB);
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
				std::string name = manifest.name;

			    //show the panel that matches the manifest name
				i_ps->AEGP_ToggleVisibility(reinterpret_cast<const A_u_char*>(name.c_str()));

			}
			*handledPB = TRUE;
			return err;
		}
		*handledPB = FALSE;
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
	//get the env for PYTHONHOM
	startPythonThread(shouldExitPythonThread, scriptQueueMutex, scriptAddedCond, scriptExecutionQueue);
	pythonThreadRunning = true;

	// Initialize the suite managers
	SuiteManager::GetInstance().InitializeSuiteHandler(sP);
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	SuiteManager::GetInstance().InitializePanelSuiteHandler(sP);

	SuiteManager::GetInstance().SetPluginID(&PyShiftAE); // Set the plugin ID
	
	ERR(suites.CommandSuite1()->AEGP_GetUniqueCommand(&PyShift));

	ERR(suites.CommandSuite1()->AEGP_InsertMenuCommand(PyShift,
		"Run Script (.py)",
		AEGP_Menu_FILE,
		AEGP_MENU_INSERT_SORTED));

	ERR(suites.RegisterSuite5()->AEGP_RegisterCommandHook(PyShiftAE,
		AEGP_HP_BeforeAE,
		AEGP_Command_ALL,
		CommandHook,
		0));

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
		