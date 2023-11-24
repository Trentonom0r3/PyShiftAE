#include "LayerSuites.h"



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

Result<void> changeLayerIndex(Result<AEGP_LayerH> layerH, int index)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layer = layerH.value;
	A_long layer_index = index;
	ERR(suites.LayerSuite9()->AEGP_ReorderLayer(layer, layer_index));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error changing layer index. Error code: " + std::to_string(err));
		return Result<void>();
	}

	Result<void> result;
	result.error = err;

	return result;
}

Result<bool> isAddLayerValid(Result<AEGP_ItemH> itemH, Result<AEGP_CompH> compH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Boolean valid = FALSE;
	AEGP_ItemH item = itemH.value;
	AEGP_CompH comp = compH.value;
	ERR(suites.LayerSuite9()->AEGP_IsAddLayerValid(item, comp, &valid));

	Result<bool> result;
	result.value = valid;
	result.error = err;

	return result;
}

Result<AEGP_LayerH> AddLayer(Result<AEGP_ItemH> itemH, Result<AEGP_CompH> compH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_ItemH item = itemH.value;
	AEGP_CompH comp = compH.value;
	AEGP_LayerH newItem;
	ERR(suites.LayerSuite9()->AEGP_AddLayer(item, comp, &newItem));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error adding layer. Error code: " + std::to_string(err));
		return Result<AEGP_LayerH>();
	}

	Result<AEGP_LayerH> result;
	result.value = newItem;
	result.error = err;

	return result;
}

Result<AEGP_ItemH> getLayerSourceItem(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_ItemH source_itemH = NULL;
	AEGP_LayerH layer = layerH.value;
	ERR(suites.LayerSuite9()->AEGP_GetLayerSourceItem(layer, &source_itemH));

	Result<AEGP_ItemH> result;
	result.value = source_itemH;
	result.error = err;

	return result;

}

