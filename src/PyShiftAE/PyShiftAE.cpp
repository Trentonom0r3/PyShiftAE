
/*	
	PyShiftAE.cpp

	Main Implementation file for PyShiftAE


	version		notes							engineer				date
	
	1.0			First implementation			Trentonom0r3 (Spigon) 	10/31/2023
	
*/
#include "PyShiftAE.h"

static AEGP_PluginID		PyShiftAE			=	6969L;
static AEGP_Command			PyShift				=	6769L;
static A_long				S_idle_count		=	0L;
static SPBasicSuite			*sP					=	NULL;

std::queue<std::string> scriptExecutionQueue;
std::mutex scriptQueueMutex; // To ensure thread safety
std::thread pythonThread;
std::atomic<bool> pythonThreadRunning = true;
std::atomic<bool> shouldExitPythonThread = false;

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
	A_Err 				err 			=	A_Err_NONE,
						err2			=	A_Err_NONE;
		
	AEGP_ItemH			active_itemH	=	NULL;
	
	AEGP_ItemType		item_type		=	AEGP_ItemType_NONE;
	
	AEGP_SuiteHandler	suites(sP);

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

			{
				std::lock_guard<std::mutex> lock(scriptQueueMutex);
				scriptExecutionQueue.push(scriptPath);
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
	//finalize();
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
	std::thread pythonThread([]() {
		initialize_python_module(); // Initialize the Python interpreter

		while (!shouldExitPythonThread.load()) { // Check the flag in each iteration
			std::string scriptPath;

			{
				std::lock_guard<std::mutex> lock(scriptQueueMutex);
				if (!scriptExecutionQueue.empty()) {
					scriptPath = scriptExecutionQueue.front();
					scriptExecutionQueue.pop();
				}
			}

			if (!scriptPath.empty()) {
				std::string output = execute_python_script_from_file(scriptPath); // Execute the script
				if (output != "All good") {
					auto& message = enqueueSyncTask(ReportInfo, output);
					message->wait();
				}
			}
		}
		if (shouldExitPythonThread) {
			finalize(); // Finalize the Python interpreter
		}
		});
	pythonThread.detach(); // Detach the thread so it can run in the background
	PyShiftAE = aegp_plugin_id;

	A_Err 					err = A_Err_NONE,
							err2 = A_Err_NONE;

	sP = pica_basicP;
	//initialize interpreter on the messagequeue thread
	// In EntryPointFunc
	SuiteManager::GetInstance().InitializeSuiteHandler(sP);
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	SuiteManager::GetInstance().SetPluginID(&PyShiftAE);

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
	return err;
}
			  
			  
