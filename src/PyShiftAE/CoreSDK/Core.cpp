#include "Core.h"
/*
 * File: Core.cpp
 * Description: This file contains the core logic wrappers for the After Effects SDK. It is designed to be simple and heavily abstracted for reusability.
 *
 * Guidelines for Contributors:
 * 1. Simplicity: Functions should be straightforward, performing a single task.
 * 2. Suite Access: Always access suites via `SuiteManager::GetInstance().GetSuiteHandler()`.
 * 3. Plugin ID: Retrieve the plugin ID using `SuiteManager::GetInstance().GetPluginID()`. Check for null pointers.
 * 4. Error Handling: Use the `Result<T>` class for returning both values and error codes.
 *	 `Result<void>` for functions that do not return a value.
 *	'Result<T>' is a template class that contains a value of type T and an error code of type A_Err.
 * 5. Abstraction: Maintain a high level of abstraction. Complex functionalities should be delegated to 'PyCore.cpp'.
 * 6. Reusability: Write reusable code. Many functions will be used in various combinations in 'PyCore.cpp'.
 * 7. Do not modify the standard set unless absolutely necessary.
 * 
 * Error Handling:
 * 1. Use the `Result<T>` class for returning both values and error codes.
 * `Result<void>` for functions that do not return a value.
 * 2. `Result<T>` is a template class that contains a value of type T and an error code of type A_Err.
 * 3. The `Result<T>` class is defined in 'Core.h'.
 * 4. The `Result<T>` class is used to return both values and error codes from functions.
 * 5. The `Result<T>` class is used to return error codes from functions that do not return a value.
 * 
 * See Existing code and header file for examples.
 *
 */
std::string convertUTF16ToUTF8(const A_UTF16Char* utf16String) {
	if (utf16String == nullptr) {
		return "";
	}

	int utf8Length = WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<const wchar_t*>(utf16String), -1, nullptr, 0, nullptr, nullptr);
	if (utf8Length <= 0) {
		return "";
	}

	std::vector<char> utf8String(utf8Length);
	WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<const wchar_t*>(utf16String), -1, utf8String.data(), utf8Length, nullptr, nullptr);
	return std::string(utf8String.begin(), utf8String.end() - 1);
}

std::vector<A_UTF16Char> convertUTF8ToUTF16(const std::string& utf8String) {
	int utf16Length = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, nullptr, 0);
	if (utf16Length <= 0) {
		return std::vector<A_UTF16Char>();
	}

	std::vector<wchar_t> utf16Buffer(utf16Length);
	MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, utf16Buffer.data(), utf16Length);

	std::vector<A_UTF16Char> utf16String(utf16Buffer.begin(), utf16Buffer.end());
	return utf16String;
}


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


Result<void> ExecuteCommand(int commandId)
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
		PT_ETX(suites.MemorySuite1()->AEGP_LockMemHandle(unicode_nameMH, (void**)&unicode_nameP));
		name = convertUTF16ToUTF8(unicode_nameP);
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
	std::vector<A_UTF16Char> unicode_name = convertUTF8ToUTF16(name);
	if (unicode_name.empty()) {
		return Result<void>(A_Err_STRUCT); // Handle conversion error
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

Result<AEGP_RenderOptionsH> getRenderOptions(Result<AEGP_ItemH> itemH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();

	if (pluginIDPtr != nullptr) {
		// Dereference the pointer to get the plugin ID
		AEGP_PluginID pluginID = *pluginIDPtr;
	A_Err err = A_Err_NONE;
	AEGP_RenderOptionsH roH = NULL;
	AEGP_ItemH item = itemH.value;
	ERR(suites.RenderOptionsSuite1()->AEGP_NewFromItem(pluginID, item, &roH));

	Result<AEGP_RenderOptionsH> result;
	if (err != A_Err_NONE) {
		result.value = nullptr;
		throw std::runtime_error("Error getting render options. Error code: " + std::to_string(err));
	}
	else {
		result.value = roH;
	}
	result.error = err;

	return result;
}
else {
	throw std::runtime_error("Plugin ID is null");
}
	return Result<AEGP_RenderOptionsH>();
}

Result<AEGP_RenderOptionsH> setTime(Result<AEGP_RenderOptionsH> roH, float time)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Time timeT;
	timeT.value = static_cast<A_long>(time * 1000000);  // Convert seconds to microseconds.
	timeT.scale = 1000000;  // Set the scale factor to 1,000,000.
	ERR(suites.RenderOptionsSuite1()->AEGP_SetTime(roH.value, timeT));  // Set render time.

	Result<AEGP_RenderOptionsH> result;
	result.value = roH.value;
	result.error = err;

	return result;
}

Result<AEGP_RenderOptionsH> getWorldType(Result<AEGP_RenderOptionsH> roH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_WorldType type = AEGP_WorldType_NONE;
	ERR(suites.RenderOptionsSuite1()->AEGP_GetWorldType(roH.value, &type));

	Result<AEGP_RenderOptionsH> result;
	result.value = roH.value;
	result.error = err;

	return result;
}

Result<AEGP_FrameReceiptH> renderAndCheckoutFrame(Result<AEGP_RenderOptionsH> roH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_FrameReceiptH receiptH = NULL;
	ERR(suites.RenderSuite2()->AEGP_RenderAndCheckoutFrame(roH.value, NULL, NULL, &receiptH));

	Result<AEGP_FrameReceiptH> result;
	result.value = receiptH;
	result.error = err;

	return result;
}

Result<AEGP_WorldH> getReceiptWorld(Result<AEGP_FrameReceiptH> receiptH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_WorldH frameH = NULL;
	ERR(suites.RenderSuite2()->AEGP_GetReceiptWorld(receiptH.value, &frameH));

	Result<AEGP_WorldH> result;
	result.value = frameH;
	result.error = err;

	return result;
}

Result<void> checkinFrame(Result<AEGP_FrameReceiptH> receiptH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	ERR(suites.RenderSuite2()->AEGP_CheckinFrame(receiptH.value));

	Result<void> result;
	result.error = err;

	return result;
}

Result<PF_Pixel8*> getBaseAddr8(Result<AEGP_WorldH> frameH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	PF_Pixel8* baseAddr = nullptr;
	ERR(suites.WorldSuite3()->AEGP_GetBaseAddr8(frameH.value, &baseAddr));

	Result<PF_Pixel8*> result;
	result.value = baseAddr;
	result.error = err;

	return result;
}

Result<size> getSize(Result<AEGP_WorldH> frameH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	int width, height;
	ERR(suites.WorldSuite3()->AEGP_GetSize(frameH.value, &width, &height));

	Result<size> result;
	result.value.width = width;
	result.value.height = height;
	result.error = err;

	return result;
}

Result<void> disposeRenderOptions(Result <AEGP_RenderOptionsH> roH)
{
	//check in the render options
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	ERR(suites.RenderOptionsSuite1()->AEGP_Dispose(roH.value));

	Result<void> result;

	result.error = err;

	return result;
}


Result<int> getNumLayers(Result<AEGP_CompH> compH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_long num_layers = 0;
	AEGP_CompH comp = compH.value;
	ERR(suites.LayerSuite9()->AEGP_GetCompNumLayers(comp, &num_layers));

	Result<int> result;
	result.value = num_layers;
	result.error = err;

	return result;
}

Result<AEGP_CompH> getCompFromItem(Result<AEGP_ItemH> itemH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_CompH compH = NULL;
	AEGP_ItemH item = itemH.value;
	ERR(suites.CompSuite4()->AEGP_GetCompFromItem(item, &compH));

	Result<AEGP_CompH> result;
	result.value = compH;
	result.error = err;

	return result;
}

Result<int> getLayerIndex(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_long layer_index = 0;
	AEGP_LayerH layer = layerH.value;
	ERR(suites.LayerSuite9()->AEGP_GetLayerIndex(layer, &layer_index));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting layer index. Error code: " + std::to_string(err));
	}
	Result<int> result;
	result.value = layer_index;
	result.error = err;

	return result;
}


Result<std::string> getLayerName(Result<AEGP_LayerH> layerH) {
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_MemHandle utf_layer_nameMH = NULL;
	A_UTF16Char* utf_layer_nameP = NULL;
	std::string layerName;
	A_Err err = A_Err_NONE;

	PT_XTE_START{
		if (!layerH.value) {
			throw A_Err_STRUCT;
		}
		AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();
		if (pluginIDPtr) {
			AEGP_PluginID pluginID = *pluginIDPtr;
			PT_ETX(suites.LayerSuite9()->AEGP_GetLayerName(pluginID, layerH.value, &utf_layer_nameMH, nullptr));
		}
		if (utf_layer_nameMH) {
			PT_ETX(suites.MemorySuite1()->AEGP_LockMemHandle(utf_layer_nameMH, (void**)&utf_layer_nameP));
			layerName = convertUTF16ToUTF8(utf_layer_nameP);
			PT_ETX(suites.MemorySuite1()->AEGP_UnlockMemHandle(utf_layer_nameMH));
			PT_ETX(suites.MemorySuite1()->AEGP_FreeMemHandle(utf_layer_nameMH));
		}

	Result<std::string> result;
	result.value = layerName;
	result.error = err;
	return result;
	}
	PT_XTE_CATCH_RETURN_ERR;
}


Result<std::string> getLayerSourceName(Result<AEGP_LayerH> layerH) {
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_MemHandle utf_source_nameMH = NULL;
	A_UTF16Char* utf_source_nameP = NULL;
	std::string sourceName;
	A_Err err = A_Err_NONE;

	PT_XTE_START{
		if (!layerH.value) {
			throw A_Err_STRUCT;
		}
		AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();
		if (pluginIDPtr) {
			AEGP_PluginID pluginID = *pluginIDPtr;
			PT_ETX(suites.LayerSuite9()->AEGP_GetLayerName(pluginID, layerH.value, nullptr, &utf_source_nameMH));
		}
		if (utf_source_nameMH) {
			PT_ETX(suites.MemorySuite1()->AEGP_LockMemHandle(utf_source_nameMH, (void**)&utf_source_nameP));
			sourceName = convertUTF16ToUTF8(utf_source_nameP);
			PT_ETX(suites.MemorySuite1()->AEGP_UnlockMemHandle(utf_source_nameMH));
			PT_ETX(suites.MemorySuite1()->AEGP_FreeMemHandle(utf_source_nameMH));
		}

	Result<std::string> result;
	result.value = sourceName;
	result.error = err;
	return result;
	}
	PT_XTE_CATCH_RETURN_ERR;
}


Result<void> setLayerName(Result<AEGP_LayerH> layerH, const std::string& name) {
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_LayerH item = layerH.value;
	// Calculate the length of the resulting UTF-16 string
	std::vector<A_UTF16Char> unicode_name = convertUTF8ToUTF16(name);
	if (unicode_name.empty()) {
		return Result<void>(A_Err_STRUCT); // Handle conversion error
	}
	A_Err err = A_Err_NONE;
	// Call the AE SDK function to set the item name
	err = suites.LayerSuite9()->AEGP_SetLayerName(item, reinterpret_cast<const A_UTF16Char*>(unicode_name.data()));
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

Result<AEGP_LayerH> getLayerFromComp(Result<AEGP_CompH> compH, int index)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layerH = NULL;
	AEGP_CompH comp = compH.value;
	ERR(suites.LayerSuite9()->AEGP_GetCompLayerByIndex(comp, index, &layerH));

	Result<AEGP_LayerH> result;
	result.value = layerH;
	result.error = err;

	return result;
}
