#include "ItemSuites.h"


Result<AEGP_ItemH> getActiveItem()
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_ItemH itemH; // Initialize to nullptr in case AEGP_GetActiveItem fails
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
	if (!itemH) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	ERR(suites.ItemSuite9()->AEGP_GetItemType(itemH, &item_type));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting item type. Error code: " + std::to_string(err));
	}
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
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
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
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting item name. Error code: " + std::to_string(err));
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
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}

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


Result<AEGP_ItemH> createFolderItem(const std::string& name, Result<AEGP_ItemH> parentFolderH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_ItemH parent_folderH = parentFolderH.value;
	if (!parent_folderH) {
		throw A_Err_STRUCT; // throw an error if parent folder is null
	}
	AEGP_ItemH new_folderH;
	std::vector<A_UTF16Char> unicode_name = convertUTF8ToUTF16(name);
	if (unicode_name.empty()) {
		return Result<AEGP_ItemH>(parent_folderH, A_Err_STRUCT); // Handle conversion error
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
		Result<AEGP_ItemH> errorResult;
		errorResult.value = parent_folderH;
		errorResult.error = err;
		return errorResult; // Return an error result if AEGP_SetItemName fails
	}

	// If we reach this point, it means everything went well
	Result<AEGP_ItemH> successResult;
	successResult.value = new_folderH;
	successResult.error = A_Err_NONE;
	return successResult;
}

Result<AEGP_ItemH> GetFirstProjItem(Result<AEGP_ProjectH> projectH) {
	A_Err err = A_Err_NONE;
	AEGP_ItemH itemH;
	AEGP_ProjectH project = projectH.value;
	if (!project) {
		throw A_Err_STRUCT; // throw an error if project is null
	}
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_GetFirstProjItem(project, &itemH);

	Result<AEGP_ItemH> result(itemH, err);
	return result;
}

Result<AEGP_ItemH> GetNextProjItem(Result<AEGP_ProjectH> projectH, Result<AEGP_ItemH> currentItemH) {
	A_Err err = A_Err_NONE;
	AEGP_ItemH nextItemH;
	AEGP_ProjectH project = projectH.value;
	if (!project) {
	throw A_Err_STRUCT; // throw an error if project is null
	}
	AEGP_ItemH currentItem = currentItemH.value;
	if (!currentItem) {
	throw A_Err_STRUCT; // throw an error if item is null
	}

	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ItemSuite9()->AEGP_GetNextProjItem(project, currentItem, &nextItemH);

	Result<AEGP_ItemH> result(nextItemH, err);
	return result;
}

Result<bool> IsItemSelected(Result<AEGP_ItemH> itemH) {
	A_Err err = A_Err_NONE;
	A_Boolean isSelected = FALSE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}

	err = suites.ItemSuite9()->AEGP_IsItemSelected(item, &isSelected);

	Result<bool> result(static_cast<bool>(isSelected), err);
	return result;
}

Result<void> SelectItem(Result<AEGP_ItemH> itemH, bool select, bool deselectOthers) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	err = suites.ItemSuite9()->AEGP_SelectItem(item, static_cast<A_Boolean>(select), static_cast<A_Boolean>(deselectOthers));

	Result<void> result(err);
	return result;
}

Result<AEGP_ItemFlags> GetItemFlags(Result<AEGP_ItemH> itemH) {
	A_Err err = A_Err_NONE;
	AEGP_ItemFlags itemFlags;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	err = suites.ItemSuite9()->AEGP_GetItemFlags(item, &itemFlags);

	Result<AEGP_ItemFlags> result(itemFlags, err);
	return result;
}

Result<void> SetItemUseProxy(Result<AEGP_ItemH> itemH, bool useProxy) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	err = suites.ItemSuite9()->AEGP_SetItemUseProxy(item, static_cast<A_Boolean>(useProxy));

	Result<void> result(err);
	return result;
}

Result<AEGP_ItemH> GetItemParentFolder(Result<AEGP_ItemH> itemH) {
	A_Err err = A_Err_NONE;
	AEGP_ItemH parentItemH = nullptr;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	err = suites.ItemSuite9()->AEGP_GetItemParentFolder(item, &parentItemH);

	Result<AEGP_ItemH> result(parentItemH, err);
	return result;
}

Result<void> SetItemParentFolder(Result<AEGP_ItemH> itemH, Result<AEGP_ItemH> parentFolderH) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	AEGP_ItemH parentFolder = parentFolderH.value;
	if (!parentFolder) {
		throw A_Err_STRUCT; // throw an error if parent folder is null
	}

	err = suites.ItemSuite9()->AEGP_SetItemParentFolder(item, parentFolder);

	Result<void> result(err);
	return result;
}

Result<float> GetItemDuration(Result<AEGP_ItemH> itemH) {
	A_Err err = A_Err_NONE;
	A_Time duration;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	err = suites.ItemSuite9()->AEGP_GetItemDuration(item, &duration);
		float durationInSeconds = duration.value / duration.scale;
		Result<float> result(durationInSeconds, err);
		return result;
	
}

Result<float> GetItemCurrentTime(Result<AEGP_ItemH> itemH) {
	A_Err err = A_Err_NONE;
	A_Time currTime;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	err = suites.ItemSuite9()->AEGP_GetItemCurrentTime(item, &currTime);
		A_FpLong currTimeInSeconds = currTime.value / currTime.scale;
		float currTimeH = static_cast<float>(currTimeInSeconds);
		Result<float> result(currTimeH, err);
		return result;
}

Result<float> GetCompItemCurrentTime(Result<AEGP_ItemH> itemH, float frameRate) {
	A_Err err = A_Err_NONE;
	A_Time currTime;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	A_FpLong frameRateFp = static_cast<A_FpLong>(frameRate);
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	err = suites.ItemSuite9()->AEGP_GetItemCurrentTime(item, &currTime);
		float currTimeInSeconds = ConvertATimeToFloat(currTime, frameRateFp);
		Result<float> result(currTimeInSeconds, err);
		return result;

}

Result<size> GetItemDimensions(Result<AEGP_ItemH> itemH) {
	A_Err err = A_Err_NONE;
	A_long width = 0, height = 0;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	err = suites.ItemSuite9()->AEGP_GetItemDimensions(item, &width, &height);

	float widthInPixels = static_cast<float>(width);
	float heightInPixels = static_cast<float>(height);
	size dimensions;
	dimensions.width = widthInPixels;
	dimensions.height = heightInPixels;
	Result<size> result(dimensions, err);


	return result;

}

Result<float> GetItemPixelAspectRatio(Result<AEGP_ItemH>itemH) {
	A_Err err = A_Err_NONE;
	A_Ratio ratio;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	err = suites.ItemSuite9()->AEGP_GetItemPixelAspectRatio(item, &ratio);

	Result<float> result(static_cast<float>(ratio.num) / ratio.den, err); // Assuming ratio is a fraction
	return result;
}

Result<void> DeleteItem(Result<AEGP_ItemH> itemH) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	err = suites.ItemSuite9()->AEGP_DeleteItem(item);

	Result<void> result(err);
	return result;
}


Result<void> SetItemCurrentTime(Result<AEGP_ItemH> itemH, float time) {
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Time newTime;
	A_Err err = A_Err_NONE;
	AEGP_CompH compH;
	AEGP_ItemH item = itemH.value;
	A_Time currTime;
	ERR(suites.ItemSuite9()->AEGP_GetItemCurrentTime(item, &currTime));
	A_FpLong scle = currTime.scale;

	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}

	newTime.value = static_cast<A_long>(time * scle);
	newTime.scale = scle;
	ERR(suites.ItemSuite9()->AEGP_SetItemCurrentTime(item, &newTime));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error setting item current time. Error code: " + std::to_string(err));
	}

	Result<void> result(err);
	return result;
}

Result<void> SetCompItemCurrentTime(Result<AEGP_ItemH> itemH, float time, float frameRate) {
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Time newTime;
	A_Err err = A_Err_NONE;
	AEGP_CompH compH;
	AEGP_ItemH item = itemH.value;

	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}

	newTime.value = static_cast<A_long>(time);
	newTime.scale = frameRate;
	ERR(suites.ItemSuite9()->AEGP_SetItemCurrentTime(item, &newTime));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error setting item current time. Error code: " + std::to_string(err));
	}

	Result<void> result(err);
	return result;
}
Result<std::string> GetItemComment(Result<AEGP_ItemH> itemH) {
	A_Err err = A_Err_NONE;
	AEGP_MemHandle unicodeNameMH;
	A_UTF16Char* unicodeNameP;
	std::string comment;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	err = suites.ItemSuite9()->AEGP_GetItemComment(item, &unicodeNameMH);
	if (err == A_Err_NONE && unicodeNameMH) {
		suites.MemorySuite1()->AEGP_LockMemHandle(unicodeNameMH, (void**)&unicodeNameP);
		comment = convertUTF16ToUTF8(unicodeNameP); // Assuming convertUTF16ToUTF8 is implemented
		suites.MemorySuite1()->AEGP_UnlockMemHandle(unicodeNameMH);
		suites.MemorySuite1()->AEGP_FreeMemHandle(unicodeNameMH);
	}

	Result<std::string> result(comment, err);
	return result;
}

Result<void> SetItemComment(Result<AEGP_ItemH> itemH, const std::string& comment) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	err = suites.ItemSuite9()->AEGP_SetItemComment(item, reinterpret_cast<const A_UTF16Char*>(comment.c_str()));

	Result<void> result(err);
	return result;
}

Result<AEGP_LabelID> GetItemLabel(Result<AEGP_ItemH> itemH) {
	A_Err err = A_Err_NONE;
	AEGP_LabelID label;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	err = suites.ItemSuite9()->AEGP_GetItemLabel(item, &label);

	Result<AEGP_LabelID> result(label, err);
	return result;
}

Result<void> SetItemLabel(Result<AEGP_ItemH> itemH, AEGP_LabelID label) { //INVESTIGATE FURTHER
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	err = suites.ItemSuite9()->AEGP_SetItemLabel(item, label);

	Result<void> result(err);
	return result;
}

Result<AEGP_ItemViewP> GetItemMRUView(Result<AEGP_ItemH> itemH) {
	A_Err err = A_Err_NONE;
	AEGP_ItemViewP mruView;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (!item) {
		throw A_Err_STRUCT; // throw an error if item is null
	}
	err = suites.ItemSuite9()->AEGP_GetItemMRUView(item, &mruView);

	Result<AEGP_ItemViewP> result(mruView, err);
	return result;
}



