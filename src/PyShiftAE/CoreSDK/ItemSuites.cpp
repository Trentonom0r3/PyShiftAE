#include "ItemSuites.h"


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


Result<void> createFolderItem(const std::string& name, Result<AEGP_ItemH> parentFolderH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_ItemH parent_folderH = parentFolderH.value;
	AEGP_ItemH new_folderH = NULL;
	std::vector<A_UTF16Char> unicode_name = convertUTF8ToUTF16(name);
	if (unicode_name.empty()) {
		return Result<void>(A_Err_STRUCT); // Handle conversion error
	}
	A_UTF16Char* nameZ = reinterpret_cast<A_UTF16Char*>(unicode_name.data());
	AEGP_ProjectH projH = NULL;
	AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();

	if (pluginIDPtr != nullptr) {
		// Dereference the pointer to get the plugin ID
		AEGP_PluginID pluginID = *pluginIDPtr;
		// Replace ERR with PT_ETX macro to check and throw error if not A_Err_NONE
		ERR(suites.ItemSuite9()->AEGP_CreateNewFolder(nameZ, parent_folderH, &new_folderH));
	}
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

Result<AEGP_ItemH> GetFirstProjItem(AEGP_ProjectH projectH) {
	A_Err err = A_Err_NONE;
	AEGP_ItemH itemH = nullptr;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_GetFirstProjItem(projectH, &itemH);

	Result<AEGP_ItemH> result(itemH, err);
	return result;
}

Result<AEGP_ItemH> GetNextProjItem(AEGP_ProjectH projectH, AEGP_ItemH currentItem) {
	A_Err err = A_Err_NONE;
	AEGP_ItemH nextItemH = nullptr;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_GetNextProjItem(projectH, currentItem, &nextItemH);

	Result<AEGP_ItemH> result(nextItemH, err);
	return result;
}

Result<bool> IsItemSelected(AEGP_ItemH itemH) {
	A_Err err = A_Err_NONE;
	A_Boolean isSelected = FALSE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_IsItemSelected(itemH, &isSelected);

	Result<bool> result(static_cast<bool>(isSelected), err);
	return result;
}

Result<void> SelectItem(AEGP_ItemH itemH, bool select, bool deselectOthers) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_SelectItem(itemH, static_cast<A_Boolean>(select), static_cast<A_Boolean>(deselectOthers));

	Result<void> result(err);
	return result;
}

Result<AEGP_ItemFlags> GetItemFlags(AEGP_ItemH itemH) {
	A_Err err = A_Err_NONE;
	AEGP_ItemFlags itemFlags;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_GetItemFlags(itemH, &itemFlags);

	Result<AEGP_ItemFlags> result(itemFlags, err);
	return result;
}

Result<void> SetItemUseProxy(AEGP_ItemH itemH, bool useProxy) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_SetItemUseProxy(itemH, static_cast<A_Boolean>(useProxy));

	Result<void> result(err);
	return result;
}

Result<AEGP_ItemH> GetItemParentFolder(AEGP_ItemH itemH) {
	A_Err err = A_Err_NONE;
	AEGP_ItemH parentItemH = nullptr;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_GetItemParentFolder(itemH, &parentItemH);

	Result<AEGP_ItemH> result(parentItemH, err);
	return result;
}

Result<void> SetItemParentFolder(AEGP_ItemH itemH, AEGP_ItemH parentFolderH) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_SetItemParentFolder(itemH, parentFolderH);

	Result<void> result(err);
	return result;
}

Result<float> GetItemDuration(AEGP_ItemH itemH) {
	A_Err err = A_Err_NONE;
	A_Time duration;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_GetItemDuration(itemH, &duration);

	float durationInSeconds = ConvertATimeToFloat(duration);
	Result<float> result(durationInSeconds, err);
	return result;
}

Result<float> GetItemCurrentTime(AEGP_ItemH itemH) {
	A_Err err = A_Err_NONE;
	A_Time currTime;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_GetItemCurrentTime(itemH, &currTime);

	float currTimeInSeconds = ConvertATimeToFloat(currTime);
	Result<float> result(currTimeInSeconds, err);  // Assuming the time is in a format that can be directly cast to float
	return result;
}

Result<std::pair<long, long>> GetItemDimensions(AEGP_ItemH itemH) {
	A_Err err = A_Err_NONE;
	A_long width = 0, height = 0;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_GetItemDimensions(itemH, &width, &height);

	Result<std::pair<long, long>> result(std::make_pair(width, height), err);
	return result;
}

Result<float> GetItemPixelAspectRatio(AEGP_ItemH itemH) {
	A_Err err = A_Err_NONE;
	A_Ratio ratio;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_GetItemPixelAspectRatio(itemH, &ratio);

	Result<float> result(static_cast<float>(ratio.num) / ratio.den, err); // Assuming ratio is a fraction
	return result;
}

Result<void> DeleteItem(AEGP_ItemH itemH) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_DeleteItem(itemH);

	Result<void> result(err);
	return result;
}


Result<void> SetItemCurrentTime(AEGP_ItemH itemH, float time) {
	A_Err err = A_Err_NONE;
	A_Time newTime = ConvertFloatToATime(time);
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_SetItemCurrentTime(itemH, &newTime);

	Result<void> result(err);
	return result;
}

Result<std::string> GetItemComment(AEGP_ItemH itemH) {
	A_Err err = A_Err_NONE;
	AEGP_MemHandle unicodeNameMH;
	A_UTF16Char* unicodeNameP;
	std::string comment;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_GetItemComment(itemH, &unicodeNameMH);
	if (err == A_Err_NONE && unicodeNameMH) {
		suites.MemorySuite1()->AEGP_LockMemHandle(unicodeNameMH, (void**)&unicodeNameP);
		comment = convertUTF16ToUTF8(unicodeNameP); // Assuming convertUTF16ToUTF8 is implemented
		suites.MemorySuite1()->AEGP_UnlockMemHandle(unicodeNameMH);
		suites.MemorySuite1()->AEGP_FreeMemHandle(unicodeNameMH);
	}

	Result<std::string> result(comment, err);
	return result;
}

Result<void> SetItemComment(AEGP_ItemH itemH, const std::string& comment) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_SetItemComment(itemH, reinterpret_cast<const A_UTF16Char*>(comment.c_str()));

	Result<void> result(err);
	return result;
}

Result<AEGP_LabelID> GetItemLabel(AEGP_ItemH itemH) {
	A_Err err = A_Err_NONE;
	AEGP_LabelID label;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_GetItemLabel(itemH, &label);

	Result<AEGP_LabelID> result(label, err);
	return result;
}

Result<void> SetItemLabel(AEGP_ItemH itemH, AEGP_LabelID label) { //INVESTIGATE FURTHER
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_SetItemLabel(itemH, label);

	Result<void> result(err);
	return result;
}

Result<AEGP_ItemViewP> GetItemMRUView(AEGP_ItemH itemH) {
	A_Err err = A_Err_NONE;
	AEGP_ItemViewP mruView;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_GetItemMRUView(itemH, &mruView);

	Result<AEGP_ItemViewP> result(mruView, err);
	return result;
}



