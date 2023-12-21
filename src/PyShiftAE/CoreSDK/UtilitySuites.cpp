#include "UtilitySuites.h"


Result<void>ReportInfo(std::string info) {
	A_Err err = A_Err_NONE; // This is the error code that AE will return if something goes wrong

	const A_char* info_stringZ = info.c_str(); // Convert the string to a const A_char* (A_char is a typedef for char16_t)
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();

	if (pluginIDPtr != nullptr) {
		// Dereference the pointer to get the plugin ID
		AEGP_PluginID  pluginID = *pluginIDPtr;
		// Use the macro and capture any error that occurs
		ERR(suites.UtilitySuite6()->AEGP_ReportInfo(pluginID, info_stringZ)); // Report the info to AE
		if (err != A_Err_NONE) { // If there was an error
			// Throw a runtime error with the error code
			throw std::runtime_error("Error reporting info. Error code: " + std::to_string(err));
		}
		Result<void> result; // Create a result object
		result.error = err; // Set the error code

		return result; // Return the result
	}
	else {
		throw std::runtime_error("Plugin ID is null");
	}
}


Result<void> StartUndoGroup(std::string undo_name)
{
	A_Err 	err = A_Err_NONE,
		err2 = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	const A_char* undo_nameZ = undo_name.c_str();

	ERR(suites.UtilitySuite6()->AEGP_StartUndoGroup(undo_nameZ));

	Result<void> result;
	result.error = err;

	return result;

}

Result<void> EndUndoGroup()
{
	A_Err 	err = A_Err_NONE,
		err2 = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	ERR(suites.UtilitySuite6()->AEGP_EndUndoGroup());

	Result<void> result;
	result.error = err;

	return result;

}

Result<std::string> getPluginPaths()
{
	A_Err 	err = A_Err_NONE,
		err2 = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_GetPathTypes pathType = AEGP_GetPathTypes_USER_PLUGIN;
	AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();
	AEGP_MemHandle unicode_pathMH = nullptr;
	A_UTF16Char* unicode_pathP = nullptr;
	std::string path;

	if (pluginIDPtr != nullptr) {
		// Dereference the pointer to get the plugin ID
		AEGP_PluginID  pluginID = *pluginIDPtr;
		// Use the macro and capture any error that occurs
		ERR(suites.UtilitySuite6()->AEGP_GetPluginPaths(pluginID, pathType, &unicode_pathMH));
		if (err != A_Err_NONE) { // If there was an error
					// Throw a runtime error with the error code
					throw std::runtime_error("Error getting plugin paths. Error code: " + std::to_string(err));
				}
		if (unicode_pathMH) {
			suites.MemorySuite1()->AEGP_LockMemHandle(unicode_pathMH, (void**)&unicode_pathP);
			path = convertUTF16ToUTF8(unicode_pathP);  // Assuming convertUTF16ToUTF8 is already implemented
			suites.MemorySuite1()->AEGP_UnlockMemHandle(unicode_pathMH);
			suites.MemorySuite1()->AEGP_FreeMemHandle(unicode_pathMH);
		}
		Result<std::string> result(path, err);
		return result;
	}
	else {
		throw std::runtime_error("Plugin ID is null");
	}

}
