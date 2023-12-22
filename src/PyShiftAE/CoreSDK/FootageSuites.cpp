#include "FootageSuites.h"

Result<AEGP_FootageH> createFootage(const std::string& path)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_FootageH footageH;
	AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();
	std::vector<A_UTF16Char> unicode_name = convertUTF8ToUTF16(path);
	if (unicode_name.empty()) {
		return Result<AEGP_FootageH>(A_Err_STRUCT); // Handle conversion error
	}
	A_UTF16Char* pathZ = reinterpret_cast<A_UTF16Char*>(unicode_name.data());


	if (pluginIDPtr != nullptr) {
		// Dereference the pointer to get the plugin ID
		AEGP_PluginID pluginID = *pluginIDPtr;
		// Replace ERR with PT_ETX macro to check and throw error if not A_Err_NONE

		ERR(suites.FootageSuite5()->AEGP_NewFootage(pluginID, pathZ, NULL, NULL, AEGP_InterpretationStyle_DIALOG_OK, NULL, &footageH));
	}
	if (err != A_Err_NONE) {
		Result<AEGP_FootageH> errorResult;
		errorResult.error = err;
		return errorResult; // Return an error result if AEGP_NewFootage fails
	}

	// If we reach this point, it means everything went well
	Result<AEGP_FootageH> successResult;
	successResult.value = footageH;
	successResult.error = A_Err_NONE;
	return successResult;
}

Result<void> disposeFootage(Result<AEGP_FootageH> footageH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;

	ERR(suites.FootageSuite5()->AEGP_DisposeFootage(footageH.value));

	Result<void> result;
	result.error = err;

	return result;
}


Result<AEGP_ItemH> addFootageToProject(Result<AEGP_FootageH> footageH, Result<AEGP_ItemH> parentFolderH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_ItemH parent_folderH = parentFolderH.value;
	AEGP_ItemH new_footageH = NULL;
	AEGP_FootageH footage = footageH.value;
	ERR(suites.FootageSuite5()->AEGP_AddFootageToProject(footage, parent_folderH, &new_footageH));

	Result<AEGP_ItemH> result;
	result.value = new_footageH;
	result.error = err;

	return result;

}

Result<AEGP_ColorVal> GetItemSolidColor(Result<AEGP_ItemH> itemH, bool proxyB) {
	A_Err err = A_Err_NONE;
	AEGP_ColorVal color;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
if (item == NULL) {
		return Result<AEGP_ColorVal>(A_Err_STRUCT);
	}
	err = suites.FootageSuite5()->AEGP_GetSolidFootageColor(item, proxyB, &color);

	Result<AEGP_ColorVal> result(color, err);
	return result;
}

Result<void> SetSolidColor(Result<AEGP_ItemH> itemH, bool proxyB, float red, float green, float blue, float alpha) {
	AEGP_ColorVal AEcolor;
	AEcolor.alphaF = alpha;
	AEcolor.blueF = blue;
	AEcolor.greenF = green;
	AEcolor.redF = red;
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (item == NULL) {
		return Result<void>(A_Err_STRUCT);
	}

	err = suites.FootageSuite5()->AEGP_SetSolidFootageColor(item, proxyB, &AEcolor);

	Result<void> result(err);
	return result;
}

Result<void> SetSolidDimensions(Result<AEGP_ItemH> itemH, bool proxyB, float width, float height) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (item == NULL) {
		return Result<void>(A_Err_STRUCT);
	}
	err = suites.FootageSuite5()->AEGP_SetSolidFootageDimensions(item, proxyB, width, height);

	Result<void> result(err);
	return result;
}

Result<AEGP_FootageH> GetMainFootageFromItem(Result<AEGP_ItemH> itemH) {
	A_Err err = A_Err_NONE;
	AEGP_FootageH footageH;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
if (item == NULL) {
		return Result<AEGP_FootageH>(A_Err_STRUCT);
	}

	err = suites.FootageSuite5()->AEGP_GetMainFootageFromItem(item, &footageH);

	Result<AEGP_FootageH> result(footageH, err);
	return result;
}

Result<AEGP_FootageH> GetProxyFootageFromItem(Result<AEGP_ItemH> itemH) {
	A_Err err = A_Err_NONE;
	AEGP_FootageH proxyFtgH;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (item == NULL) {
		return Result<AEGP_FootageH>(A_Err_STRUCT);
	}
	err = suites.FootageSuite5()->AEGP_GetProxyFootageFromItem(item, &proxyFtgH);

	Result<AEGP_FootageH> result(proxyFtgH, err);
	return result;
}

Result<std::pair<int, int>> GetFootageNumFiles(Result<AEGP_FootageH> footageH) {
	A_Err err = A_Err_NONE;
	A_long numFiles = 0, filesPerFrame = 0;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_FootageH footage = footageH.value;
if (footage == NULL) {
		return Result<std::pair<int, int>>(A_Err_STRUCT);
	}
	err = suites.FootageSuite5()->AEGP_GetFootageNumFiles(footage, &numFiles, &filesPerFrame);

	Result<std::pair<int, int>> result(std::make_pair(numFiles, filesPerFrame), err);
	return result;
}

Result<std::string> GetFootagePath(Result<AEGP_FootageH> footageH, int frameNum, int fileIndex) {
	A_Err err = A_Err_NONE;
	AEGP_MemHandle unicodePathH = NULL;
	A_UTF16Char* unicodePathP = NULL;
	std::string path;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_FootageH footage = footageH.value;
if (footage == NULL) {
		return Result<std::string>(A_Err_STRUCT);
	}
	PT_XTE_START{
		PT_ETX(suites.FootageSuite5()->AEGP_GetFootagePath(footage, frameNum, fileIndex, &unicodePathH));
		if (unicodePathH) {
			PT_ETX(suites.MemorySuite1()->AEGP_LockMemHandle(unicodePathH, (void**)&unicodePathP));
			path = convertUTF16ToUTF8(unicodePathP);
			// Unlock and dispose of the memory handle
			PT_ETX(suites.MemorySuite1()->AEGP_UnlockMemHandle(unicodePathH));
			PT_ETX(suites.MemorySuite1()->AEGP_FreeMemHandle(unicodePathH));
		}


	Result<std::string> result(path, err);
	return result;
	}
	PT_XTE_CATCH_RETURN_ERR;
}


Result<AEGP_FootageSignature> GetFootageSignature(Result<AEGP_FootageH> footageH) {
	A_Err err = A_Err_NONE;
	AEGP_FootageSignature sig;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_FootageH footage = footageH.value;
if (footage == NULL) {
		return Result<AEGP_FootageSignature>(A_Err_STRUCT);
	}
	err = suites.FootageSuite5()->AEGP_GetFootageSignature(footage, &sig);

	Result<AEGP_FootageSignature> result(sig, err);
	return result;
}

Result<void> SetItemProxyFootage(Result<AEGP_FootageH> footageH, Result<AEGP_ItemH> itemH) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	AEGP_FootageH footage = footageH.value;
if (item == NULL || footage == NULL) {
		return Result<void>(A_Err_STRUCT);
	}

	err = suites.FootageSuite5()->AEGP_SetItemProxyFootage(footage, item);

	Result<void> result(err);
	return result;
}

Result<AEGP_ItemH> ReplaceItemMainFootage(Result<AEGP_FootageH> footageH, Result<AEGP_ItemH> itemH) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	AEGP_FootageH footage = footageH.value;
	if (item == NULL || footage == NULL) {
		throw std::runtime_error("Error replacing main footage. Error code: " + std::to_string(A_Err_STRUCT));
	}

	err = suites.FootageSuite5()->AEGP_ReplaceItemMainFootage(footage, item);

	Result<AEGP_ItemH> result(item, err);
	return result;
}

Result<void> SetFootageInterpretation(Result<AEGP_ItemH> itemH, bool proxyB, AEGP_FootageInterp interpP) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	if (item == NULL) {
			return Result<void>(A_Err_STRUCT);
		}
	err = suites.FootageSuite5()->AEGP_SetFootageInterpretation(item, proxyB, &interpP);

	Result<void> result(err);
	return result;
}

Result<AEGP_FootageInterp> GetFootageInterpretation(Result<AEGP_ItemH> itemH, bool proxyB) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ItemH item = itemH.value;
	AEGP_FootageInterp interpP;
	if (item == NULL) {
return Result<AEGP_FootageInterp>(A_Err_STRUCT);
		}

	err = suites.FootageSuite5()->AEGP_GetFootageInterpretation(item, proxyB, &interpP);

	Result<AEGP_FootageInterp> result(interpP, err);
	return result;
}

Result<AEGP_FootageLayerKey> GetFootageLayerKey(Result<AEGP_FootageH> footageH) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_FootageLayerKey layerKeyP;
	AEGP_FootageH footage = footageH.value;
	if (footage == NULL) {
		return Result<AEGP_FootageLayerKey>(A_Err_STRUCT);
	}
	err = suites.FootageSuite5()->AEGP_GetFootageLayerKey(footage, &layerKeyP);

	Result<AEGP_FootageLayerKey> result(layerKeyP, err);
	return result;
}

Result<AEGP_FootageH> NewPlaceholderFootage(const std::string& name,
											float width, float height, float duration, float frameRate) {
	A_Err err = A_Err_NONE;
	AEGP_FootageH footageH;
	A_Time durationPT = ConvertFloatToATime(duration, frameRate);
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();
	std::vector<A_UTF16Char> unicode_name = convertUTF8ToUTF16(name);
	if (unicode_name.empty()) {
		return Result<AEGP_FootageH>(A_Err_STRUCT); // Handle conversion error
	}
	const A_char* nameZ = reinterpret_cast<A_char*>(unicode_name.data());

	if (pluginIDPtr != nullptr) {
		// Dereference the pointer to get the plugin ID
		AEGP_PluginID pluginID = *pluginIDPtr;
		// Replace ERR with PT_ETX macro to check and throw error if not A_Err_NONE

		ERR(suites.FootageSuite5()->AEGP_NewPlaceholderFootage(pluginID, nameZ, width, height, &durationPT, &footageH));
	}
	if (err != A_Err_NONE) {
		Result<AEGP_FootageH> errorResult;
		errorResult.error = err;
		return errorResult; // Return an error result if AEGP_NewFootage fails
	}

	// If we reach this point, it means everything went well
	Result<AEGP_FootageH> successResult;
	successResult.value = footageH;
	successResult.error = A_Err_NONE;
	return successResult;
}

Result<AEGP_FootageH> NewPlaceholderFootageWithPath(const std::string& path, AEGP_Platform path_platform, AEIO_FileType file_type,
													float width, float height, float duration, float frameRate) {
	A_Err err = A_Err_NONE;
	AEGP_FootageH footageH;
	A_Time durationPT = ConvertFloatToATime(duration, frameRate);
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();
	std::vector<A_UTF16Char> unicode_path = convertUTF8ToUTF16(path);
	if (unicode_path.empty()) {
		return Result<AEGP_FootageH>(A_Err_STRUCT); // Handle conversion error
	}
	const A_UTF16Char* pathZ = reinterpret_cast<A_UTF16Char*>(unicode_path.data());

	if (pluginIDPtr != nullptr) {
		// Dereference the pointer to get the plugin ID
		AEGP_PluginID pluginID = *pluginIDPtr;
		// Replace ERR with PT_ETX macro to check and throw error if not A_Err_NONE

		ERR(suites.FootageSuite5()->AEGP_NewPlaceholderFootageWithPath(pluginID, pathZ, path_platform, file_type, width, height, &durationPT, &footageH));
	}
	if (err != A_Err_NONE) {
		Result<AEGP_FootageH> errorResult;
		errorResult.error = err;
		return errorResult; // Return an error result if AEGP_NewFootage fails
	}

	// If we reach this point, it means everything went well
	Result<AEGP_FootageH> successResult;
	successResult.value = footageH;
	successResult.error = A_Err_NONE;
	return successResult;
}

Result<AEGP_FootageH> NewSolidFootage(const std::string& name, float width, float height, float red, float green, float blue, float alpha) {
	A_Err err = A_Err_NONE;
	AEGP_FootageH footageH;
	const AEGP_ColorVal* colorP;
	AEGP_ColorVal color;
	color.alphaF = alpha;
	color.blueF = blue;
	color.greenF = green;
	color.redF = red;
	colorP = &color;

	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();
	std::vector<A_UTF16Char> unicode_name = convertUTF8ToUTF16(name);
	if (unicode_name.empty()) {
		return Result<AEGP_FootageH>(A_Err_STRUCT); // Handle conversion error
	}
	const A_char* nameZ = reinterpret_cast<A_char*>(unicode_name.data());

	if (pluginIDPtr != nullptr) {
		// Dereference the pointer to get the plugin ID
		AEGP_PluginID pluginID = *pluginIDPtr;
		// Replace ERR with PT_ETX macro to check and throw error if not A_Err_NONE

		ERR(suites.FootageSuite5()->AEGP_NewSolidFootage(nameZ, width, height, colorP, &footageH));
	}
	if (err != A_Err_NONE) {
		Result<AEGP_FootageH> errorResult;
		errorResult.error = err;
		return errorResult; // Return an error result if AEGP_NewFootage fails
	}

	// If we reach this point, it means everything went well
	Result<AEGP_FootageH> successResult;
	successResult.value = footageH;
	successResult.error = A_Err_NONE;
	return successResult;
}

Result<AEGP_SoundDataFormat> GetFootageSoundDataFormat(Result<AEGP_FootageH> footageH) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_FootageH footage = footageH.value;
	AEGP_SoundDataFormat formatP;

if (footage == NULL) {
		return Result<AEGP_SoundDataFormat>(A_Err_STRUCT);
	}

	err = suites.FootageSuite5()->AEGP_GetFootageSoundDataFormat(footage, &formatP);

	Result<AEGP_SoundDataFormat> result(formatP, err);
	return result;
}

Result<AEGP_FileSequenceImportOptions> GetFootageSequenceImportOptions(Result<AEGP_FootageH> footageH) {
	A_Err err = A_Err_NONE;
	AEGP_FileSequenceImportOptions options;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_FootageH footage = footageH.value;
if (footage == NULL) {
		return Result<AEGP_FileSequenceImportOptions>(A_Err_STRUCT);
	}
	err = suites.FootageSuite5()->AEGP_GetFootageSequenceImportOptions(footage, &options);

	Result<AEGP_FileSequenceImportOptions> result(options, err);
	return result;
}


