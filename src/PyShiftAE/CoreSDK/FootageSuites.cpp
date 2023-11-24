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

Result<AEGP_ColorVal> GetItemSolidColor(AEGP_ItemH itemH, bool proxyB) {
	A_Err err = A_Err_NONE;
	AEGP_ColorVal color;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.FootageSuite5()->AEGP_GetSolidFootageColor(itemH, proxyB, &color);

	Result<AEGP_ColorVal> result(color, err);
	return result;
}

Result<void> SetSolidColor(AEGP_ItemH itemH, bool proxyB, float red, float green, float blue, float alpha) {
	AEGP_ColorVal AEcolor;
	AEcolor.alphaF = alpha;
	AEcolor.blueF = blue;
	AEcolor.greenF = green;
	AEcolor.redF = red;
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.FootageSuite5()->AEGP_SetSolidFootageColor(itemH, proxyB, &AEcolor);

	Result<void> result(err);
	return result;
}

Result<void> SetSolidDimensions(AEGP_ItemH itemH, bool proxyB, int width, int height) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.FootageSuite5()->AEGP_SetSolidFootageDimensions(itemH, proxyB, width, height);

	Result<void> result(err);
	return result;
}

