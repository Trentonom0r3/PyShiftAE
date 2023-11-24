#include "CompSuites.h"

Result<void> Addcomp(std::string name, int width,
					int height, float frameRate, int durationSecs,
					float pixelAspectRatio, Result<AEGP_ItemH> parentFolder) {

	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;

	AEGP_CompH new_compH;
	const A_char* nameZ = const_cast<A_char*>(name.c_str());

	// Create pixel aspect ratio
	A_Ratio pixel_aspect_ratio = { static_cast<A_long>(pixelAspectRatio * 100), 100 };

	// Create duration
	A_Time duration;
	duration.value = durationSecs * 100; // Assuming durationSecs is in seconds and you want to convert to a time scale of 100
	duration.scale = 100;

	// Create frame rate
	A_Ratio frame_rate = { static_cast<A_long>(frameRate * 100), 100 };

	if (parentFolder.value == NULL) {
		ERR(suites.CompSuite4()->AEGP_CreateComp(NULL, nameZ, width, height, &pixel_aspect_ratio, &duration, &frame_rate, &new_compH));
	}
	else {
		ERR(suites.CompSuite4()->AEGP_CreateComp(parentFolder.value, nameZ, width, height, &pixel_aspect_ratio, &duration, &frame_rate, &new_compH));
	}

	if (err != A_Err_NONE) {
		Result<void> errorResult;
		errorResult.error = err;
		return errorResult; // Return an error result if AEGP_CreateComp fails
	}

	// If we reach this point, it means everything went well
	Result<void> successResult;
	successResult.error = A_Err_NONE;
	return successResult;
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

Result<AEGP_ItemH> GetItemFromComp(AEGP_CompH compH) {
	A_Err err = A_Err_NONE;
	AEGP_ItemH itemH = nullptr;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetItemFromComp(compH, &itemH);

	Result<AEGP_ItemH> result(itemH, err);
	return result;
}

Result<AEGP_DownsampleFactor> GetCompDownsampleFactor(AEGP_CompH compH) {
	A_Err err = A_Err_NONE;
	AEGP_DownsampleFactor dsf;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetCompDownsampleFactor(compH, &dsf);

	Result<AEGP_DownsampleFactor> result(dsf, err);
	return result;
}

Result<void> SetCompDownsampleFactor(AEGP_CompH compH, int x, int y) {
	A_Err err = A_Err_NONE;
	AEGP_DownsampleFactor dsf;
	dsf.xS = x;
	dsf.yS = y;

	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_SetCompDownsampleFactor(compH, &dsf);

	Result<void> result(err);
	return result;
}

Result<AEGP_ColorVal> GetCompBGColor(AEGP_CompH compH) {
	A_Err err = A_Err_NONE;
	AEGP_ColorVal bgColor;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetCompBGColor(compH, &bgColor);

	Result<AEGP_ColorVal> result(bgColor, err);
	return result;
}

Result<void> SetCompBGColor(AEGP_CompH compH, float red, float green, float blue, float alpha) {
	A_Err err = A_Err_NONE;
	AEGP_ColorVal bgColor;
	bgColor.alphaF = alpha;
	bgColor.blueF = blue;
	bgColor.greenF = green;
	bgColor.redF = red;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_SetCompBGColor(compH, &bgColor);

	Result<void> result(err);
	return result;
}

Result<AEGP_CompFlags> GetCompFlags(AEGP_CompH compH) {
	A_Err err = A_Err_NONE;
	AEGP_CompFlags compFlags;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetCompFlags(compH, &compFlags);

	Result<AEGP_CompFlags> result(compFlags, err);
	return result;
}

Result<bool> GetShowLayerNameOrSourceName(AEGP_CompH compH) {
	A_Err err = A_Err_NONE;
	A_Boolean layerNamesShown = FALSE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetShowLayerNameOrSourceName(compH, &layerNamesShown);

	Result<bool> result(static_cast<bool>(layerNamesShown), err);
	return result;
}

Result<void> SetShowLayerNameOrSourceName(AEGP_CompH compH, bool showLayerNames) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_SetShowLayerNameOrSourceName(compH, static_cast<A_Boolean>(showLayerNames));

	Result<void> result(err);
	return result;
}

Result<bool> GetShowBlendModes(AEGP_CompH compH) {
	A_Err err = A_Err_NONE;
	A_Boolean blendModesShown = FALSE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetShowBlendModes(compH, &blendModesShown);

	Result<bool> result(static_cast<bool>(blendModesShown), err);
	return result;
}

Result<void> SetShowBlendModes(AEGP_CompH compH, bool showBlendModes) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_SetShowBlendModes(compH, static_cast<A_Boolean>(showBlendModes));

	Result<void> result(err);
	return result;
}

Result<float> GetCompFramerate(AEGP_CompH compH) {
	A_Err err = A_Err_NONE;
	A_FpLong fps = 0.0;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetCompFramerate(compH, &fps);

	Result<float> result(static_cast<float>(fps), err);
	return result;
}

Result<void> SetCompFramerate(AEGP_CompH compH, float fps) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	A_FpLong fpsFpLong = static_cast<A_FpLong>(fps);
	err = suites.CompSuite11()->AEGP_SetCompFrameRate(compH, &fpsFpLong);

	Result<void> result(err);
	return result;
}

Result<std::pair<A_Ratio, A_Ratio>> GetCompShutterAnglePhase(AEGP_CompH compH) {
	A_Err err = A_Err_NONE;
	A_Ratio angle, phase;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetCompShutterAnglePhase(compH, &angle, &phase);

	Result<std::pair<A_Ratio, A_Ratio>> result(std::make_pair(angle, phase), err);
	return result;
}

Result<void> SetCompSuggestedMotionBlurSamples(AEGP_CompH compH, int samples) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_SetCompSuggestedMotionBlurSamples(compH, samples);

	Result<void> result(err);
	return result;
}

Result<float> GetCompWorkAreaStart(AEGP_CompH compH) {
	A_Err err = A_Err_NONE;
	A_Time start;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetCompWorkAreaStart(compH, &start);

	float startTimeInSeconds = ConvertATimeToFloat(start);  // Use the previously defined conversion function
	Result<float> result(startTimeInSeconds, err);
	return result;
}

Result<float> GetCompWorkAreaDuration(AEGP_CompH compH) {
	A_Err err = A_Err_NONE;
	A_Time duration;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetCompWorkAreaDuration(compH, &duration);

	float durationInSeconds = ConvertATimeToFloat(duration);  // Use the previously defined conversion function
	Result<float> result(durationInSeconds, err);
	return result;
}

Result<void> SetCompWorkAreaStartAndDuration(AEGP_CompH compH, float start, float dur) {
	A_Err err = A_Err_NONE;
	A_Time startTime = ConvertFloatToATime(start);  // Assuming a function to convert float to A_Time
	A_Time duration = ConvertFloatToATime(dur);  // Assuming a function to convert float to A_Time
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_SetCompWorkAreaStartAndDuration(compH, &startTime, &duration);

	Result<void> result(err);
	return result;
}

Result<AEGP_LayerH> CreateSolidInComp(const std::string& name, int width, int height, AEGP_ColorVal color, AEGP_CompH parentCompH, float dur) {
	A_Err err = A_Err_NONE;
	AEGP_LayerH newSolidLayerH;
	A_Time duration = ConvertFloatToATime(dur);  // Assuming a function to convert float to A_Time
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_CreateSolidInComp(reinterpret_cast<const A_UTF16Char*>(name.c_str()), width, height, &color, parentCompH, &duration, &newSolidLayerH);

	Result<AEGP_LayerH> result(newSolidLayerH, err);
	return result;
}

Result<AEGP_LayerH> CreateCameraInComp(const std::string& name, float x, float y, AEGP_CompH parentCompH) {
	A_Err err = A_Err_NONE;
	AEGP_LayerH newCameraLayerH;
	A_FloatPoint centerPoint = { x, y };
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_CreateCameraInComp(reinterpret_cast<const A_UTF16Char*>(name.c_str()), centerPoint, parentCompH, &newCameraLayerH);

	Result<AEGP_LayerH> result(newCameraLayerH, err);
	return result;
}

Result<AEGP_LayerH> CreateLightInComp(const std::string& name, float x, float y, AEGP_CompH parentCompH) {
	A_Err err = A_Err_NONE;
	AEGP_LayerH newLightLayerH;
	A_FloatPoint centerPoint = { x, y };
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_CreateLightInComp(reinterpret_cast<const A_UTF16Char*>(name.c_str()), centerPoint, parentCompH, &newLightLayerH);

	Result<AEGP_LayerH> result(newLightLayerH, err);
	return result;
}

Result<AEGP_Collection2H> GetNewCollectionFromCompSelection(AEGP_PluginID pluginID, AEGP_CompH compH) {
	A_Err err = A_Err_NONE;
	AEGP_Collection2H collectionH;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetNewCollectionFromCompSelection(pluginID, compH, &collectionH);

	Result<AEGP_Collection2H> result(collectionH, err);
	return result;
}

Result<void> SetSelection(AEGP_CompH compH, AEGP_Collection2H collectionH) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_SetSelection(compH, collectionH);

	Result<void> result(err);
	return result;
}

Result<float> GetCompDisplayStartTime(AEGP_CompH compH) {
	A_Err err = A_Err_NONE;
	A_Time startTime;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetCompDisplayStartTime(compH, &startTime);

	float startTimeInSeconds = ConvertATimeToFloat(startTime);  // Use the previously defined conversion function
	Result<float> result(startTimeInSeconds, err);
	return result;
}

Result<void> SetCompDisplayStartTime(AEGP_CompH compH, float startTimeInSeconds) {
	A_Err err = A_Err_NONE;
	A_Time startTime = ConvertFloatToATime(startTimeInSeconds);  // Assuming a function to convert float to A_Time
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_SetCompDisplayStartTime(compH, &startTime);

	Result<void> result(err);
	return result;
}

Result<void> SetCompDuration(AEGP_CompH compH, float durationInSeconds) {
	A_Err err = A_Err_NONE;
	A_Time duration = ConvertFloatToATime(durationInSeconds);  // Assuming a function to convert float to A_Time
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_SetCompDuration(compH, &duration);

	Result<void> result(err);
	return result;
}

Result<AEGP_LayerH> CreateNullInComp(const std::string& name, AEGP_CompH parentCompH, float dur) {
	A_Err err = A_Err_NONE;
	AEGP_LayerH newNullLayerH;
	A_Time duration = ConvertFloatToATime(dur);  // Assuming a function to convert float to A_Time
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_CreateNullInComp(reinterpret_cast<const A_UTF16Char*>(name.c_str()), parentCompH, &duration, &newNullLayerH);

	Result<AEGP_LayerH> result(newNullLayerH, err);
	return result;
}

Result<void> SetCompPixelAspectRatio(AEGP_CompH compH, float pixelAspect) {
	A_Err err = A_Err_NONE;
	A_Ratio par = { static_cast<A_long>(pixelAspect * 1000), 1000 };
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_SetCompPixelAspectRatio(compH, &par);

	Result<void> result(err);
	return result;
}

Result<AEGP_LayerH> CreateTextLayerInComp(AEGP_CompH parentCompH, bool selectNewLayer) {
	A_Err err = A_Err_NONE;
	AEGP_LayerH newTextLayerH = nullptr;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_CreateTextLayerInComp(parentCompH, static_cast<A_Boolean>(selectNewLayer), &newTextLayerH);

	Result<AEGP_LayerH> result(newTextLayerH, err);
	return result;
}

Result<AEGP_LayerH> CreateBoxTextLayerInComp(AEGP_CompH parentCompH, bool selectNewLayer, float x, float y) {
	A_Err err = A_Err_NONE;
	AEGP_LayerH newBoxTextLayerH;
	A_FloatPoint boxDimensions = { x, y };
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_CreateBoxTextLayerInComp(parentCompH, static_cast<A_Boolean>(selectNewLayer), boxDimensions, &newBoxTextLayerH);

	Result<AEGP_LayerH> result(newBoxTextLayerH, err);
	return result;
}

Result<void> SetCompDimensions(AEGP_CompH compH, int width, int height) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_SetCompDimensions(compH, width, height);

	Result<void> result(err);
	return result;
}

Result<AEGP_CompH> DuplicateComp(AEGP_CompH compH) {
	A_Err err = A_Err_NONE;
	AEGP_CompH newCompH;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_DuplicateComp(compH, &newCompH);

	Result<AEGP_CompH> result(newCompH, err);
	return result;
}

Result<float> GetCompFrameDuration(AEGP_CompH compH) {
	A_Err err = A_Err_NONE;
	A_Time frameDuration;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetCompFrameDuration(compH, &frameDuration);

	float frameDurationInSeconds = ConvertATimeToFloat(frameDuration);  // Use the previously defined conversion function
	Result<float> result(frameDurationInSeconds, err);
	return result;
}

Result<AEGP_CompH> GetMostRecentlyUsedComp() {
	A_Err err = A_Err_NONE;
	AEGP_CompH compH;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetMostRecentlyUsedComp(&compH);

	Result<AEGP_CompH> result(compH, err);
	return result;
}

Result<AEGP_LayerH> CreateVectorLayerInComp(AEGP_CompH parentCompH) {
	A_Err err = A_Err_NONE;
	AEGP_LayerH newVectorLayerH;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_CreateVectorLayerInComp(parentCompH, &newVectorLayerH);

	Result<AEGP_LayerH> result(newVectorLayerH, err);
	return result;
}

Result<AEGP_StreamRefH> GetNewCompMarkerStream(AEGP_PluginID pluginID, AEGP_CompH parentCompH) {
	A_Err err = A_Err_NONE;
	AEGP_StreamRefH markerStreamH;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetNewCompMarkerStream(pluginID, parentCompH, &markerStreamH);

	Result<AEGP_StreamRefH> result(markerStreamH, err);
	return result;
}

Result<bool> GetCompDisplayDropFrame(AEGP_CompH compH) {
	A_Err err = A_Err_NONE;
	A_Boolean dropFrame = FALSE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_GetCompDisplayDropFrame(compH, &dropFrame);

	Result<bool> result(static_cast<bool>(dropFrame), err);
	return result;
}

Result<void> SetCompDisplayDropFrame(AEGP_CompH compH, bool dropFrame) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_SetCompDisplayDropFrame(compH, static_cast<A_Boolean>(dropFrame));

	Result<void> result(err);
	return result;
}

Result<void> ReorderCompSelection(AEGP_CompH compH, int index) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_ReorderCompSelection(compH, index);

	Result<void> result(err);
	return result;
}

