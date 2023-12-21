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

Result<AEGP_LayerH> ActiveLayer()
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layerH;
	ERR(suites.LayerSuite9()->AEGP_GetActiveLayer(&layerH));

	Result<AEGP_LayerH> result;
	result.value = layerH;
	result.error = err;

	return result;
}

Result<AEGP_LayerH> getLayerFromComp(Result<AEGP_CompH> compH, int index)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layerH;
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
	AEGP_ItemH source_itemH;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerSourceItem(layer, &source_itemH));

	Result<AEGP_ItemH> result;
	result.value = source_itemH;
	result.error = err;

	return result;

}

Result<int> GetLayerSourceItemID(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_long source_id = 0;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerSourceItemID(layer, &source_id));

	Result<int> result;
	result.value = source_id;
	result.error = err;

	return result;
}

Result<AEGP_CompH> GetLayerParentComp(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_CompH compH;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerParentComp(layer, &compH));

	Result<AEGP_CompH> result;
	result.value = compH;
	result.error = err;

	return result;
}

Result<std::string> GetLayerQuality(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerQuality quality;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerQuality(layer, &quality));

	std::string qualitystr;
	if (quality == AEGP_LayerQual_WIREFRAME) {
		qualitystr = "WIREFRAME";
	}
	else if (quality == AEGP_LayerQual_DRAFT) {
		qualitystr = "DRAFT";
	}
	else if (quality == AEGP_LayerQual_BEST) {
		qualitystr = "BEST";
	}
	else {
		qualitystr = "NONE";
	}

	Result<std::string> result;
	result.value = qualitystr;
	result.error = err;

	return result;
}

Result<void> SetLayerQuality(Result<AEGP_LayerH> layerH, int qualityint)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerQuality quality;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	quality = static_cast<AEGP_LayerQuality>(qualityint);

	ERR(suites.LayerSuite9()->AEGP_SetLayerQuality(layer, quality));

	Result<void> result;
	result.error = err;

	return result;
}

Result<AEGP_LayerFlags> GetLayerFlags(Result<AEGP_LayerH> layerH) //DEAL WITH THIS LATER
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerFlags flags;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerFlags(layer, &flags)); //bitwise combination need to make func to split.

	Result<AEGP_LayerFlags> result;
	result.value = flags;
	result.error = err;

	return result;
}

Result<void> SetLayerFlag(Result<AEGP_LayerH> layerH, LayerFlag flag, A_Boolean value)  //DEAL WITH THIS LATER
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	if (layer == NULL) {
		throw std::runtime_error("Layer is null");
	}
	ERR(suites.LayerSuite9()->AEGP_SetLayerFlag(layer, flag, value));

	Result<void> result;
	result.error = err;

	return result;
}

Result<bool> IsLayerVideoReallyOn(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Boolean on = FALSE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_IsLayerVideoReallyOn(layer, &on));

	Result<bool> result;
	result.value = on;
	result.error = err;

	return result;
}

Result<bool> IsLayerAudioReallyOn(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Boolean on = FALSE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_IsLayerAudioReallyOn(layer, &on));

	Result<bool> result;
	result.value = on;
	result.error = err;

	return result;
}

Result<float> GetLayerCurrentTime(Result<AEGP_LayerH> layerH, AEGP_LTimeMode time_mode)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Time time;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerCurrentTime(layer, time_mode, &time));
	AEGP_CompH compH;
	ERR(suites.LayerSuite9()->AEGP_GetLayerParentComp(layer, &compH));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting layer current time. Error code: " + std::to_string(err));
	}
	A_FpLong timeH;
	ERR(suites.CompSuite11()->AEGP_GetCompFramerate(compH, &timeH));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting layer current time. Error code: " + std::to_string(err));
	}
	float frameRate = ConvertATimeToFloat(time, timeH);
	Result<float> result;
	result.value = frameRate;
	result.error = err;

	return result;
}

Result<float> GetLayerInPoint(Result<AEGP_LayerH> layerH, AEGP_LTimeMode time_mode)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Time time;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerInPoint(layer, time_mode, &time));

	AEGP_CompH compH;
	ERR(suites.LayerSuite9()->AEGP_GetLayerParentComp(layer, &compH));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting layer current time. Error code: " + std::to_string(err));
	}
	A_FpLong timeH;
	ERR(suites.CompSuite11()->AEGP_GetCompFramerate(compH, &timeH));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting layer current time. Error code: " + std::to_string(err));
	}
	float inPoint = ConvertATimeToFloat(time, timeH);
	Result<float> result;
	result.value = inPoint;
	result.error = err;

	return result;
}

Result<float> GetLayerDuration(Result<AEGP_LayerH> layerH, AEGP_LTimeMode time_mode)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Time time;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerDuration(layer, time_mode, &time));
	AEGP_CompH compH;
	ERR(suites.LayerSuite9()->AEGP_GetLayerParentComp(layer, &compH));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting layer current time. Error code: " + std::to_string(err));
	}
	A_FpLong timeH;
	ERR(suites.CompSuite11()->AEGP_GetCompFramerate(compH, &timeH));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting layer current time. Error code: " + std::to_string(err));
	}
	float duration = ConvertATimeToFloat(time, timeH);

	Result<float> result;
	result.value = duration;
	result.error = err;


	return result;
}

Result<void> SetLayerInPointAndDuration(Result<AEGP_LayerH> layerH, AEGP_LTimeMode time_mode, float in_point, float duration, float frameRate)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}

	A_Time in_point_time = ConvertFloatToATime(in_point, frameRate);
	A_Time duration_time = ConvertFloatToATime(duration, frameRate);

	ERR(suites.LayerSuite9()->AEGP_SetLayerInPointAndDuration(layer, time_mode, &in_point_time, &duration_time));

	Result<void> result;
	result.error = err;

	return result;
}

Result<float> GetLayerOffset(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Time offset;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerOffset(layer, &offset));

	AEGP_CompH compH;
	ERR(suites.LayerSuite9()->AEGP_GetLayerParentComp(layer, &compH));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting layer current time. Error code: " + std::to_string(err));
	}
	A_FpLong timeH;
	ERR(suites.CompSuite11()->AEGP_GetCompFramerate(compH, &timeH));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting layer current time. Error code: " + std::to_string(err));
	}
	float offsetH = ConvertATimeToFloat(offset, timeH);

	Result<float> result;
	result.value = offsetH;
	result.error = err;

	return result;
}

Result<void> SetLayerOffset(Result<AEGP_LayerH> layerH, float offsetH,float frameRate)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	A_Time offset = ConvertFloatToATime(offsetH, frameRate);
	ERR(suites.LayerSuite9()->AEGP_SetLayerOffset(layer, &offset));

	Result<void> result;
	result.error = err;

	return result;
}

Result<float> GetLayerStretch(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Ratio stretch;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerStretch(layer, &stretch));

	Result<float> result;
	result.value = stretch.num / stretch.den;
	result.error = err;


	return result;
}

Result<void> SetLayerStretch(Result<AEGP_LayerH> layerH, const A_Ratio& stretch) //deal with this later
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_SetLayerStretch(layer, &stretch));

	Result<void> result;
	result.error = err;

	return result;
}

Result<AEGP_LayerTransferMode> GetLayerTransferMode(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerTransferMode mode;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerTransferMode(layer, &mode));

	Result<AEGP_LayerTransferMode> result;
	result.value = mode;
	result.error = err;

	return result;
}

Result<void> SetLayerTransferMode(Result<AEGP_LayerH> layerH, const AEGP_LayerTransferMode mode)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_SetLayerTransferMode(layer, &mode));

	Result<void> result;
	result.error = err;

	return result;
}

Result<A_FloatRect> GetLayerMaskedBounds(Result<AEGP_LayerH> layerH, float intime, AEGP_LTimeMode comp_time, float frameRate)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_FloatRect bounds;
	A_Time time = ConvertFloatToATime(intime, frameRate);
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerMaskedBounds(layer, comp_time, &time, &bounds));

	Result<A_FloatRect> result;
	result.value = bounds;
	result.error = err;

	return result;
}

Result<AEGP_ObjectType> GetLayerObjectType(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_ObjectType type;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerObjectType(layer, &type));

	Result<AEGP_ObjectType> result;
	result.value = type;
	result.error = err;

	return result;
}

Result<A_Boolean> IsLayer3D(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Boolean is_3D = FALSE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_IsLayer3D(layer, &is_3D));

	Result<A_Boolean> result;
	result.value = is_3D;
	result.error = err;

	return result;
}

Result<A_Boolean> IsLayer2D(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Boolean is_2D = FALSE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_IsLayer2D(layer, &is_2D));

	Result<A_Boolean> result;
	result.value = is_2D;
	result.error = err;

	return result;
}

Result<A_Boolean> IsVideoActive(Result<AEGP_LayerH> layerH, AEGP_LTimeMode time_mode, float comp_timeH, float frameRate)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Boolean is_active = FALSE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	A_Time comp_time = ConvertFloatToATime(comp_timeH, frameRate);
	ERR(suites.LayerSuite9()->AEGP_IsVideoActive(layer, time_mode, &comp_time, &is_active));

	Result<A_Boolean> result;
	result.value = is_active;
	result.error = err;

	return result;
}

Result<bool> IsLayerUsedAsTrackMatte(Result<AEGP_LayerH> layerH, bool fill_must_be_active)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Boolean is_track_matte = FALSE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_IsLayerUsedAsTrackMatte(layer, fill_must_be_active, &is_track_matte));

	Result<bool> result;
	result.value = is_track_matte;
	result.error = err;

	return result;
}

Result<bool> DoesLayerHaveTrackMatte(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Boolean has_track_matte = FALSE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_DoesLayerHaveTrackMatte(layer, &has_track_matte));

	Result<bool> result;
	result.value = has_track_matte;
	result.error = err;

	return result;
}

Result<float> ConvertCompToLayerTime(Result<AEGP_LayerH> layerH, float comp_time)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Time layer_time;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	A_Time comp_timeA = ConvertFloatToATime(comp_time, 1);
	ERR(suites.LayerSuite9()->AEGP_ConvertCompToLayerTime(layer, &comp_timeA, &layer_time));

	AEGP_CompH compH;
	ERR(suites.LayerSuite9()->AEGP_GetLayerParentComp(layer, &compH));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting layer current time. Error code: " + std::to_string(err));
	}
	A_FpLong timeH;
	ERR(suites.CompSuite11()->AEGP_GetCompFramerate(compH, &timeH));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting layer current time. Error code: " + std::to_string(err));
	}
	float layerTime = ConvertATimeToFloat(layer_time, timeH);

	Result<float> result;
	result.value = layerTime;
	result.error = err;

	return result;
}

Result<float> ConvertLayerToCompTime(Result<AEGP_LayerH> layerH, float layer_time, float frameRate)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_Time comp_time;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	A_Time layer_timeA = ConvertFloatToATime(layer_time, frameRate);

	ERR(suites.LayerSuite9()->AEGP_ConvertLayerToCompTime(layer, &layer_timeA, &comp_time));

	AEGP_CompH compH;
	ERR(suites.LayerSuite9()->AEGP_GetLayerParentComp(layer, &compH));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting layer current time. Error code: " + std::to_string(err));
	}
	A_FpLong timeH;
	ERR(suites.CompSuite11()->AEGP_GetCompFramerate(compH, &timeH));
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting layer current time. Error code: " + std::to_string(err));
	}
	float compTime = ConvertATimeToFloat(comp_time, timeH);

	Result<float> result;
	result.value = compTime;
	result.error = err;

	return result;
}

Result<float> GetLayerDancingRandValue(Result<AEGP_LayerH> layerH, float comp_time, float frameRate)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_long rand_value = 0;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	A_Time comp_timeA = ConvertFloatToATime(comp_time, frameRate);
	ERR(suites.LayerSuite9()->AEGP_GetLayerDancingRandValue(layer, &comp_timeA, &rand_value));

	Result<float> result;
	result.value = rand_value;
	result.error = err;

	return result;
}

Result<int> GetLayerID(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerIDVal id;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerID(layer, &id));

	Result<int> result;
	result.value = id;
	result.error = err;

	return result;
}

Result<AEGP_LayerH> GetLayerFromLayerID(Result<AEGP_CompH> parent_comp, int id)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layer;
	AEGP_CompH comp = parent_comp.value;
	if (!comp) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerFromLayerID(comp, id, &layer));

	Result<AEGP_LayerH> result;
	result.value = layer;
	result.error = err;

	return result;
}

Result<AEGP_LabelID> GetLayerLabel(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LabelID label;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerLabel(layer, &label));

	Result<AEGP_LabelID> result;
	result.value = label;
	result.error = err;

	return result;
}

Result<void> SetLayerLabel(Result<AEGP_LayerH> layerH, AEGP_LabelID label)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_SetLayerLabel(layer, label));

	Result<void> result;
	result.error = err;

	return result;
}

Result<AEGP_LayerSamplingQuality> GetLayerSamplingQuality(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerSamplingQuality quality;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerSamplingQuality(layer, &quality));

	Result<AEGP_LayerSamplingQuality> result;
	result.value = quality;
	result.error = err;

	return result;
}


Result<void> SetLayerSamplingQuality(Result<AEGP_LayerH> layerH, AEGP_LayerSamplingQuality quality)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_SetLayerSamplingQuality(layer, quality));

	Result<void> result;
	result.error = err;

	return result;
}

Result<AEGP_LayerH> GetTrackMatteLayer(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH track_matte_layer;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetTrackMatteLayer(layer, &track_matte_layer));

	Result<AEGP_LayerH> result;
	result.value = track_matte_layer;
	result.error = err;

	return result;
}

Result<void> SetTrackMatte(Result<AEGP_LayerH> layerH, Result<AEGP_LayerH> track_matte_layer, AEGP_TrackMatte track_matte_type)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	AEGP_LayerH track_matte_layerH = track_matte_layer.value;
	if (!track_matte_layerH) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_SetTrackMatte(layer, track_matte_layerH, track_matte_type));

	Result<void> result;
	result.error = err;

	return result;
}

Result<void> RemoveTrackMatte(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_RemoveTrackMatte(layer));

	Result<void> result;
	result.error = err;

	return result;
}

Result<void> DeleteLayer(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_DeleteLayer(layer));

	Result<void> result;
	result.error = err;

	return result;
}

Result<AEGP_LayerH> DuplicateLayer(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH dupe_layer;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_DuplicateLayer(layer, &dupe_layer));

	Result<AEGP_LayerH> result;
	result.value = dupe_layer;
	result.error = err;

	return result;
}

Result<AEGP_LayerH> GetLayerParent(Result<AEGP_LayerH> layerH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH parent_layer;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_GetLayerParent(layer, &parent_layer));

	Result<AEGP_LayerH> result;
	result.value = parent_layer;
	result.error = err;

	return result;
}

Result<void> SetLayerParent(Result<AEGP_LayerH> layerH, AEGP_LayerH parent_layer)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH layer = layerH.value;
	if (!layer) {
		throw A_Err_STRUCT;
	}
	ERR(suites.LayerSuite9()->AEGP_SetLayerParent(layer, parent_layer));

	Result<void> result;
	result.error = err;

	return result;
}



