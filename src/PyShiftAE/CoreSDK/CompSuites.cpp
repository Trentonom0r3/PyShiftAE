#include "CompSuites.h"

Result<void> Addcomp(std::string name, int width,
					int height, float frameRate, int durationSecs,
					float pixelAspectRatio, Result<AEGP_ItemH> parentFolder) {

	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler(); //initialization checks error. Mutexed, thread safe.

	A_Err err = A_Err_NONE;

	AEGP_CompH new_compH;
	const A_char* nameZ = const_cast<A_char*>(name.c_str());

	// Create pixel aspect ratio
	A_Ratio pixel_aspect_ratio = { static_cast<A_long>(pixelAspectRatio * 100), 100 };

	// Create duration
	A_Time duration = ConvertFloatToATime(durationSecs, frameRate); // Assuming a function to convert float to A_Time

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

Result<AEGP_ItemH> GetItemFromComp(Result<AEGP_CompH> compH) {
	A_Err err = A_Err_NONE;
	AEGP_ItemH itemH;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<AEGP_ItemH>(itemH, err);
	}
	err = suites.CompSuite11()->AEGP_GetItemFromComp(comp, &itemH);

	Result<AEGP_ItemH> result(itemH, err);
	return result;
}

Result<AEGP_DownsampleFactor> GetCompDownsampleFactor(Result<AEGP_CompH> compH) {
	A_Err err = A_Err_NONE;
	AEGP_DownsampleFactor dsf;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<AEGP_DownsampleFactor>(dsf, err);
	}
	err = suites.CompSuite11()->AEGP_GetCompDownsampleFactor(comp, &dsf);

	Result<AEGP_DownsampleFactor> result(dsf, err);
	return result;
}

Result<void> SetCompDownsampleFactor(Result<AEGP_CompH> compH, int x, int y) {
	A_Err err = A_Err_NONE;
	AEGP_DownsampleFactor dsf;
	dsf.xS = x;
	dsf.yS = y;
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<void>(err);
	}
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.CompSuite11()->AEGP_SetCompDownsampleFactor(comp, &dsf);

	Result<void> result(err);
	return result;
}

Result<AEGP_ColorVal> GetCompBGColor(Result<AEGP_CompH> compH) {
	A_Err err = A_Err_NONE;
	AEGP_ColorVal bgColor;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<AEGP_ColorVal>(bgColor, err);
	}

	err = suites.CompSuite11()->AEGP_GetCompBGColor(comp, &bgColor);

	Result<AEGP_ColorVal> result(bgColor, err);
	return result;
}

Result<void> SetCompBGColor(Result<AEGP_CompH> compH, float red, float green, float blue, float alpha) {
	A_Err err = A_Err_NONE;
	AEGP_ColorVal bgColor;
	bgColor.alphaF = alpha;
	bgColor.blueF = blue;
	bgColor.greenF = green;
	bgColor.redF = red;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<void>(err);
	}
	err = suites.CompSuite11()->AEGP_SetCompBGColor(comp, &bgColor);

	Result<void> result(err);
	return result;
}

Result<AEGP_CompFlags> GetCompFlags(Result<AEGP_CompH> compH) {
	A_Err err = A_Err_NONE;
	AEGP_CompFlags compFlags;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
if (comp == NULL) {
		return Result<AEGP_CompFlags>(compFlags, err);
	}
	err = suites.CompSuite11()->AEGP_GetCompFlags(comp, &compFlags);

	Result<AEGP_CompFlags> result(compFlags, err);
	return result;
}

Result<bool> GetShowLayerNameOrSourceName(Result<AEGP_CompH> compH) {
	A_Err err = A_Err_NONE;
	A_Boolean layerNamesShown = FALSE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
if (comp == NULL) {
		return Result<bool>(static_cast<bool>(layerNamesShown), err);
	}
	err = suites.CompSuite11()->AEGP_GetShowLayerNameOrSourceName(comp, &layerNamesShown);

	Result<bool> result(static_cast<bool>(layerNamesShown), err);
	return result;
}

Result<void> SetShowLayerNameOrSourceName(Result<AEGP_CompH> compH, bool showLayerNames) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<void>(err);
	}
	err = suites.CompSuite11()->AEGP_SetShowLayerNameOrSourceName(comp, static_cast<A_Boolean>(showLayerNames));

	Result<void> result(err);
	return result;
}

Result<bool> GetShowBlendModes(Result<AEGP_CompH> compH) {
	A_Err err = A_Err_NONE;
	A_Boolean blendModesShown = FALSE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<bool>(false, err);
	}
	err = suites.CompSuite11()->AEGP_GetShowBlendModes(comp, &blendModesShown);

	Result<bool> result(static_cast<bool>(blendModesShown), err);
	return result;
}

Result<void> SetShowBlendModes(Result<AEGP_CompH> compH, bool showBlendModes) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<void>(err);
	}
	err = suites.CompSuite11()->AEGP_SetShowBlendModes(comp, static_cast<A_Boolean>(showBlendModes));

	Result<void> result(err);
	return result;
}

Result<float> GetCompFramerate(Result<AEGP_CompH> compH) {
	A_Err err = A_Err_NONE;
	A_FpLong fps = 0.0;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<float>(static_cast<float>(fps), err);
	}
	err = suites.CompSuite11()->AEGP_GetCompFramerate(comp, &fps);

	Result<float> result(static_cast<float>(fps), err);
	return result;
}

Result<void> SetCompFramerate(Result<AEGP_CompH> compH, float fps) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<void>(err);
	}
	A_FpLong fpsFpLong = static_cast<A_FpLong>(fps);
	err = suites.CompSuite11()->AEGP_SetCompFrameRate(comp, &fpsFpLong);

	Result<void> result(err);
	return result;
}

Result<std::pair<A_Ratio, A_Ratio>> GetCompShutterAnglePhase(Result<AEGP_CompH> compH) {
	A_Err err = A_Err_NONE;
	A_Ratio angle, phase;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		angle.den = 0;
		angle.num = 0;
		phase.den = 0;
		phase.num = 0;
		return Result<std::pair<A_Ratio, A_Ratio>>(std::make_pair(angle, phase), err);
	}
	err = suites.CompSuite11()->AEGP_GetCompShutterAnglePhase(comp, &angle, &phase);

	Result<std::pair<A_Ratio, A_Ratio>> result(std::make_pair(angle, phase), err);
	return result;
}

Result<void> SetCompSuggestedMotionBlurSamples(Result<AEGP_CompH> compH, int samples) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<void>(err);
	}
	err = suites.CompSuite11()->AEGP_SetCompSuggestedMotionBlurSamples(comp, samples);

	Result<void> result(err);
	return result;
}

Result<float> GetCompWorkAreaStart(Result<AEGP_CompH> compH) {
	A_Err err = A_Err_NONE;
	A_Time start;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
if (comp == NULL) {
		return Result<float>(0.0, err);
	}
	err = suites.CompSuite11()->AEGP_GetCompWorkAreaStart(comp, &start);

	float startTimeInSeconds = ConvertATimeToFloat(start);  // Use the previously defined conversion function
	Result<float> result(startTimeInSeconds, err);
	return result;
}

Result<float> GetCompWorkAreaDuration(Result<AEGP_CompH> compH) {
	A_Err err = A_Err_NONE;
	A_Time duration;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
if (comp == NULL) {
		return Result<float>(0.0, err);
	}

	err = suites.CompSuite11()->AEGP_GetCompWorkAreaDuration(comp, &duration);

	float durationInSeconds = ConvertATimeToFloat(duration);  // Use the previously defined conversion function
	Result<float> result(durationInSeconds, err);
	return result;
}

Result<void> SetCompWorkAreaStartAndDuration(Result<AEGP_CompH> compH, float start, float dur) {
	
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	A_FpLong frameRate;
	ERR(suites.CompSuite11()->AEGP_GetCompFramerate(compH.value, &frameRate));
	A_Time startTime = ConvertFloatToATime(start, frameRate);  // Assuming a function to convert float to A_Time
	A_Time duration = ConvertFloatToATime(dur, frameRate);  // Assuming a function to convert float to A_Time

	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<void>(err);
	}
	err = suites.CompSuite11()->AEGP_SetCompWorkAreaStartAndDuration(comp, &startTime, &duration);

	Result<void> result(err);
	return result;
}

Result<AEGP_LayerH> CreateSolidInComp(const std::string& name, int width, int height, float red, float green, float blue, float alpha, Result<AEGP_CompH> parentCompH, float dur) {
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_LayerH newSolidLayerH;
	AEGP_CompH parentComp = parentCompH.value;
	AEGP_ColorVal color;
	if (parentComp == NULL) {
		return Result<AEGP_LayerH>(newSolidLayerH, err);
	}
	color.alphaF = alpha;
	color.blueF = blue;
	color.greenF = green;
	color.redF = red;
    A_FpLong frameRate;
	std::vector<A_UTF16Char> unicode_name = convertUTF8ToUTF16(name);
	if (unicode_name.empty()) {
		return Result<AEGP_LayerH>(newSolidLayerH, err);
	}

	ERR(suites.CompSuite11()->AEGP_GetCompFramerate(parentComp, &frameRate));
	A_Time duration = ConvertFloatToATime(dur, frameRate);  // Assuming a function to convert float to A_Time
	ERR(suites.CompSuite11()->AEGP_CreateSolidInComp(reinterpret_cast<const A_UTF16Char*>(unicode_name.data()), width, height, &color, parentComp, &duration, &newSolidLayerH));

	Result<AEGP_LayerH> result(newSolidLayerH, err);
	return result;
}

Result<AEGP_LayerH> CreateCameraInComp(const std::string& name, float x, float y, Result<AEGP_CompH> parentCompH) {
	A_Err err = A_Err_NONE;
	AEGP_LayerH newCameraLayerH;
	A_FloatPoint centerPoint = { x, y };
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH parentComp = parentCompH.value;
if (parentComp == NULL) {
		return Result<AEGP_LayerH>(newCameraLayerH, err);
	}
	err = suites.CompSuite11()->AEGP_CreateCameraInComp(reinterpret_cast<const A_UTF16Char*>(name.c_str()), centerPoint, parentComp, &newCameraLayerH);

	Result<AEGP_LayerH> result(newCameraLayerH, err);
	return result;
}

Result<AEGP_LayerH> CreateLightInComp(const std::string& name, float x, float y, Result<AEGP_CompH> parentCompH) {
	A_Err err = A_Err_NONE;
	AEGP_LayerH newLightLayerH;
	A_FloatPoint centerPoint = { x, y };
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH parentComp = parentCompH.value;
	if (parentComp == NULL) {
		return Result<AEGP_LayerH>(newLightLayerH, err);
	}
	err = suites.CompSuite11()->AEGP_CreateLightInComp(reinterpret_cast<const A_UTF16Char*>(name.c_str()), centerPoint, parentComp, &newLightLayerH);

	Result<AEGP_LayerH> result(newLightLayerH, err);
	return result;
}

Result<AEGP_Collection2H> GetNewCollectionFromCompSelection(Result<AEGP_CompH> compH) {
	A_Err err = A_Err_NONE;
	AEGP_Collection2H collectionH;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();

	if (pluginIDPtr != nullptr) {
		// Dereference the pointer to get the plugin ID
		AEGP_PluginID pluginID = *pluginIDPtr;

		AEGP_CompH comp = compH.value;
		if (comp == NULL) {
			return Result<AEGP_Collection2H>(collectionH, err);
		}
		err = suites.CompSuite11()->AEGP_GetNewCollectionFromCompSelection(pluginID, comp, &collectionH);

		Result<AEGP_Collection2H> result(collectionH, err);
		return result;
	}
}

Result<void> SetSelection(Result<AEGP_CompH> compH, AEGP_Collection2H collectionH) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<void>(err);
	}
	err = suites.CompSuite11()->AEGP_SetSelection(comp, collectionH);

	Result<void> result(err);
	return result;
}

Result<float> GetCompDisplayStartTime(Result<AEGP_CompH> compH) {
	A_Err err = A_Err_NONE;
	A_Time startTime;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
if (comp == NULL) {
		return Result<float>(0.0, err);
	}
	err = suites.CompSuite11()->AEGP_GetCompDisplayStartTime(comp, &startTime);

	float startTimeInSeconds = ConvertATimeToFloat(startTime);  // Use the previously defined conversion function
	Result<float> result(startTimeInSeconds, err);
	return result;
}

Result<void> SetCompDisplayStartTime(Result<AEGP_CompH> compH, float startTimeInSeconds) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_FpLong frameRate;
ERR(suites.CompSuite11()->AEGP_GetCompFramerate(compH.value, &frameRate));

	A_Time startTime = ConvertFloatToATime(startTimeInSeconds, frameRate);  // Assuming a function to convert float to A_Time

	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<void>(err);
	}
	err = suites.CompSuite11()->AEGP_SetCompDisplayStartTime(comp, &startTime);

	Result<void> result(err);
	return result;
}

Result<void> SetCompDuration(Result<AEGP_CompH> compH, float durationInSeconds) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_FpLong frameRate;
	ERR(suites.CompSuite11()->AEGP_GetCompFramerate(compH.value, &frameRate));
	A_Time duration = ConvertFloatToATime(durationInSeconds, frameRate);  // Assuming a function to convert float to A_Time
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<void>(err);
	}
	err = suites.CompSuite11()->AEGP_SetCompDuration(comp, &duration);

	Result<void> result(err);
	return result;
}

Result<AEGP_LayerH> CreateNullInComp(const std::string& name, Result<AEGP_CompH> parentCompH, float dur) {
	A_Err err = A_Err_NONE;
	AEGP_LayerH newNullLayerH;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_FpLong frameRate;
	ERR(suites.CompSuite11()->AEGP_GetCompFramerate(parentCompH.value, &frameRate));
	A_Time duration = ConvertFloatToATime(dur, frameRate);  // Assuming a function to convert float to A_Time

	AEGP_CompH parentComp = parentCompH.value;
if (parentComp == NULL) {
		return Result<AEGP_LayerH>(newNullLayerH, err);
	}

	err = suites.CompSuite11()->AEGP_CreateNullInComp(reinterpret_cast<const A_UTF16Char*>(name.c_str()), parentComp, &duration, &newNullLayerH);

	Result<AEGP_LayerH> result(newNullLayerH, err);
	return result;
}

Result<void> SetCompPixelAspectRatio(Result<AEGP_CompH> compH, float pixelAspect) {
	A_Err err = A_Err_NONE;
	A_Ratio par = { static_cast<A_long>(pixelAspect * 1000), 1000 };
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<void>(err);
	}
	err = suites.CompSuite11()->AEGP_SetCompPixelAspectRatio(comp, &par);

	Result<void> result(err);
	return result;
}

Result<AEGP_LayerH> CreateTextLayerInComp(Result<AEGP_CompH> parentCompH, bool selectNewLayer) {
	A_Err err = A_Err_NONE;
	AEGP_LayerH newTextLayerH = nullptr;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH parentComp = parentCompH.value;
if (parentComp == NULL) {
		return Result<AEGP_LayerH>(newTextLayerH, err);
	}
	err = suites.CompSuite11()->AEGP_CreateTextLayerInComp(parentComp, static_cast<A_Boolean>(selectNewLayer), &newTextLayerH);

	Result<AEGP_LayerH> result(newTextLayerH, err);
	return result;
}

Result<AEGP_LayerH> CreateBoxTextLayerInComp(Result<AEGP_CompH> parentCompH, bool selectNewLayer, float x, float y) {
	A_Err err = A_Err_NONE;
	AEGP_LayerH newBoxTextLayerH;
	A_FloatPoint boxDimensions = { x, y };
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH parentComp = parentCompH.value;
	if (parentComp == NULL) {
		return Result<AEGP_LayerH>(newBoxTextLayerH, err);
	}
	err = suites.CompSuite11()->AEGP_CreateBoxTextLayerInComp(parentComp, static_cast<A_Boolean>(selectNewLayer), boxDimensions, &newBoxTextLayerH);

	Result<AEGP_LayerH> result(newBoxTextLayerH, err);
	return result;
}

Result<void> SetCompDimensions(Result<AEGP_CompH> compH, int width, int height) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<void>(err);
	}
	err = suites.CompSuite11()->AEGP_SetCompDimensions(comp, width, height);

	Result<void> result(err);
	return result;
}

Result<AEGP_CompH> DuplicateComp(Result<AEGP_CompH> compH) {
	A_Err err = A_Err_NONE;
	AEGP_CompH newCompH;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
if (comp == NULL) {
		return Result<AEGP_CompH>(newCompH, err);
	}
	err = suites.CompSuite11()->AEGP_DuplicateComp(comp, &newCompH);

	Result<AEGP_CompH> result(newCompH, err);
	return result;
}

Result<float> GetCompFrameDuration(Result<AEGP_CompH> compH) {
	A_Err err = A_Err_NONE;
	A_Time frameDuration;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
if (comp == NULL) {
		return Result<float>(0.0, err);
	}
	err = suites.CompSuite11()->AEGP_GetCompFrameDuration(comp, &frameDuration);

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

Result<AEGP_LayerH> CreateVectorLayerInComp(Result<AEGP_CompH> parentCompH) {
	A_Err err = A_Err_NONE;
	AEGP_LayerH newVectorLayerH;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH parentComp = parentCompH.value;
if (parentComp == NULL) {
		return Result<AEGP_LayerH>(newVectorLayerH, err);
	}
	err = suites.CompSuite11()->AEGP_CreateVectorLayerInComp(parentComp, &newVectorLayerH);

	Result<AEGP_LayerH> result(newVectorLayerH, err);
	return result;
}

Result<AEGP_StreamRefH> GetNewCompMarkerStream(Result<AEGP_CompH> parentCompH) {
	A_Err err = A_Err_NONE;
	AEGP_StreamRefH markerStreamH;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_PluginID* pluginIDPtr = SuiteManager::GetInstance().GetPluginID();

	if (pluginIDPtr != nullptr) {
		// Dereference the pointer to get the plugin ID
		AEGP_PluginID pluginID = *pluginIDPtr;
		AEGP_CompH parentComp = parentCompH.value;
		if (parentComp == NULL) {
			return Result<AEGP_StreamRefH>(markerStreamH, err);
		}

		err = suites.CompSuite11()->AEGP_GetNewCompMarkerStream(pluginID, parentComp, &markerStreamH);

		Result<AEGP_StreamRefH> result(markerStreamH, err);
		return result;
	}
}

Result<bool> GetCompDisplayDropFrame(Result<AEGP_CompH> compH) {
	A_Err err = A_Err_NONE;
	A_Boolean dropFrame = FALSE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
	return Result<bool>(static_cast<bool>(dropFrame), err);
	}
	err = suites.CompSuite11()->AEGP_GetCompDisplayDropFrame(comp, &dropFrame);

	Result<bool> result(static_cast<bool>(dropFrame), err);
	return result;
}

Result<void> SetCompDisplayDropFrame(Result<AEGP_CompH> compH, bool dropFrame) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<void>(err);
	}
	err = suites.CompSuite11()->AEGP_SetCompDisplayDropFrame(comp, static_cast<A_Boolean>(dropFrame));

	Result<void> result(err);
	return result;
}

Result<void> ReorderCompSelection(Result<AEGP_CompH> compH, int index) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_CompH comp = compH.value;
	if (comp == NULL) {
		return Result<void>(err);
	}
	err = suites.CompSuite11()->AEGP_ReorderCompSelection(comp, index);

	Result<void> result(err);
	return result;
}

