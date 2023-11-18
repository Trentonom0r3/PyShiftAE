#include "Core.h"
/*
* In Each Function, Access Suites using;
* AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
* In order to ensure that the suites are safely accessed. 
* 
* If you need to access the plugin ID, use;
* AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();
*	if (pluginIDPtr != nullptr) {
*		// Dereference the pointer to get the plugin ID
*		AEGP_PluginID pluginID = *pluginIDPtr;
*	Use pluginID in the function call.
*
* When writing functions, use the Result<T> class to return a value and an error code.
* If the function returns void, use Result<void>.
* 
* If the function takes an argument, use Result<t> name, args 
* to pass in arguments, to maintain clarity.
*/
Result<void>ReportInfo(std::string info) {
	A_Err err = A_Err_NONE; // This is the error code that AE will return if something goes wrong

	const A_char* info_stringZ = info.c_str(); // Convert the string to a const A_char* (A_char is a typedef for char16_t)
	AEGP_SuiteHandler&  suites = SuiteManager::GetInstance().GetSuiteHandler();
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

Result<A_UTF16Char> AppVersion()
{
	AEGP_MemHandle unicode_pathPH = NULL;  // Handle for memory allocation.
	AEGP_MemSize resultSize = 0;  // Variable to hold the size of the allocated memory.
	A_UTF16Char* resultStr = NULL;  // Pointer to hold the project path string.

	A_Err err = A_Err_NONE;

	AEGP_RQM_SessionId sessid = NULL;
	sessid = 01;
	//worry about this one later
	return Result<A_UTF16Char>();
}

Result<AEGP_ProjectH> getProject() {
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_ProjectH projH = nullptr; // Initialize to nullptr in case AEGP_GetProjectByIndex fails
	err = suites.ProjSuite6()->AEGP_GetProjectByIndex(0, &projH);

	Result<AEGP_ProjectH> result;
	if (err != A_Err_NONE) {
		result.value = nullptr;
		throw std::runtime_error("Error getting project. Error code: " + std::to_string(err));
	}
	else {
		result.value = projH;
	}
	result.error = err;   // Set the error

	return result;
}

/*
AEGP_DoCommand(
  AEGP_Command  command);*/


Result<void> executeCommand(int commandId)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_Command command = commandId;
	ERR(suites.CommandSuite1()->AEGP_DoCommand(command));

	if (err != A_Err_NONE) {
		throw std::runtime_error("Error executing command. Error code: " + std::to_string(err));
	}
	Result<void> result;

	result.error = err;   // Set the error

return result;
}

Result<AEGP_ItemH> getActiveItem()
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_ItemH itemH = nullptr; // Initialize to nullptr in case AEGP_GetActiveItem fails
	ERR(suites.ItemSuite9()->AEGP_GetActiveItem(&itemH));

	Result<AEGP_ItemH> result;
	if (err != A_Err_NONE) {
		result.value = nullptr;
		throw std::runtime_error("Error getting active item. Error code: " + std::to_string(err));
	}
	else {
		result.value = itemH;
	}
	result.error = err;   // Set the error

	return result;
}

Result<AEGP_ItemType> getItemType(Result<AEGP_ItemH> item)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_ItemType item_type = AEGP_ItemType_NONE; // Initialize to AEGP_ItemType_NONE in case AEGP_GetItemType fails
	AEGP_ItemH itemH = item.value;
	ERR(suites.ItemSuite9()->AEGP_GetItemType(itemH, &item_type));

	Result<AEGP_ItemType> result;
	result.value = item_type; // Set the value to the item type
	result.error = err;       // Set the error

	return result;
}

Result<std::string>getItemName(Result<AEGP_ItemH> itemH) {
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_MemHandle unicode_nameMH = NULL;
	A_UTF16Char* unicode_nameP = NULL;
	std::string name;
	A_Err err = A_Err_NONE;
	AEGP_ItemH item = itemH.value;

	PT_XTE_START // start of try block
	{
		if (!item) {
			throw A_Err_STRUCT; // throw an error if item is null
		}
	AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();

	if (pluginIDPtr != nullptr) {
		// Dereference the pointer to get the plugin ID
		AEGP_PluginID pluginID = *pluginIDPtr;
		// Replace ERR with PT_ETX macro to check and throw error if not A_Err_NONE
		PT_ETX(suites.ItemSuite9()->AEGP_GetItemName(pluginID, item, &unicode_nameMH));
	}
	if (unicode_nameMH) {
		// Lock the memory handle to get the string pointer
		PT_ETX(suites.MemorySuite1()->AEGP_LockMemHandle(unicode_nameMH, (void**)&unicode_nameP));

		// Calculate the length of the resulting UTF-8 string
		int utf8Length = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)unicode_nameP, -1, NULL, 0, NULL, NULL);
		if (utf8Length > 0) {
			std::vector<char> utf8String(utf8Length);
			// Convert UTF-16 to UTF-8
			WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)unicode_nameP, -1, &utf8String[0], utf8Length, NULL, NULL);
			// Assign to name, excluding the null terminator
			name.assign(utf8String.begin(), utf8String.end() - 1);
		}
		std::cout << name << std::endl;
		// Unlock and dispose of the memory handle
		PT_ETX(suites.MemorySuite1()->AEGP_UnlockMemHandle(unicode_nameMH));
		PT_ETX(suites.MemorySuite1()->AEGP_FreeMemHandle(unicode_nameMH));
	}
	std::cout << name << std::endl;
	Result<std::string> result;
	result.value = name;
	result.error = err;
	return result; // return the result with the name and any error that occurred
	}
	PT_XTE_CATCH_RETURN_ERR; // catch block that handles any errors thrown within the PT_XTE_START block

}

Result<void>setItemName(Result<AEGP_ItemH> itemH, const std::string& name) {
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	// Calculate the length of the resulting UTF-16 string
	int utf16Length = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, NULL, 0);
	if (utf16Length <= 0) {
		Result<void> errorResult;
		errorResult.error = static_cast<A_Err>(GetLastError());
		return errorResult; // Return an error result if conversion fails
	}

	// Create a buffer to hold the UTF-16 string
	std::vector<wchar_t> unicode_name(utf16Length);

	// Convert UTF-8 to UTF-16
	int conversionResult = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, &unicode_name[0], utf16Length);
	if (conversionResult == 0) {
		Result<void> errorResult;
		errorResult.error = static_cast<A_Err>(GetLastError());
		return errorResult; // Return an error result if conversion fails
	}

	// Ensure that the last character is a null terminator, as expected for UTF-16 strings
	if (unicode_name[utf16Length - 1] != L'\0') {
		Result<void> errorResult;
		errorResult.error = A_Err_STRUCT; // Use an appropriate error code
		return errorResult; // Return an error if the string is not properly null-terminated
	}

	if (item == nullptr) {
		Result<void> errorResult;
		errorResult.error = A_Err_STRUCT; // Use an appropriate error code
		return errorResult; // Return an error if the item is null
	}
	A_Err err = A_Err_NONE;
	// Call the AE SDK function to set the item name
	err = suites.ItemSuite9()->AEGP_SetItemName(item, reinterpret_cast<const A_UTF16Char*>(unicode_name.data()));
	if (err != A_Err_NONE) {
		Result<void> errorResult;
		errorResult.error = err;
		return errorResult; // Return an error result if AEGP_SetItemName fails
	}

	// If we reach this point, it means everything went well
	Result<void> successResult;
	successResult.error = A_Err_NONE;
	return successResult;
}


Result<A_long> getUniqueItemID(Result<AEGP_ItemH> itemH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_long value = 0;
	A_long* item_id = &value;
	AEGP_ItemH item = itemH.value;
	ERR(suites.ItemSuite9()->AEGP_GetItemID(item, item_id));

	Result<A_long> result;
	if (err != A_Err_NONE) {
		result.value = 0;
		throw std::runtime_error("Error getting unique item ID. Error code: " + std::to_string(err));
	}
	else {
		result.value = *item_id;
	}
	result.error = err;

	return result;

}

