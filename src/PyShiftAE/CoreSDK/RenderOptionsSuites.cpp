#include "RenderOptionsSuites.h"


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
