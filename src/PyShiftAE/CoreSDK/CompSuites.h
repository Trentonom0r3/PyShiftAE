#pragma once
#include "Core.h"

Result<void> Addcomp(std::string name, int width,int height, float frameRate, int durationSecs,
									  float pixelAspectRatio, Result<AEGP_ItemH> parentFolder);

Result<AEGP_CompH> getCompFromItem(Result<AEGP_ItemH> itemH);

Result<AEGP_ItemH> GetItemFromComp(AEGP_CompH compH);

Result<AEGP_DownsampleFactor> GetCompDownsampleFactor(AEGP_CompH compH);

Result<void> SetCompDownsampleFactor(AEGP_CompH compH, int x, int y);

Result<AEGP_ColorVal> GetCompBGColor(AEGP_CompH compH);

Result<void> SetCompBGColor(AEGP_CompH compH, float red, float green, float blue, float alpha);

Result<AEGP_CompFlags> GetCompFlags(AEGP_CompH compH);

Result<bool> GetShowLayerNameOrSourceName(AEGP_CompH compH);

Result<void> SetShowLayerNameOrSourceName(AEGP_CompH compH, bool showLayerNames);

Result<bool> GetShowBlendModes(AEGP_CompH compH);

Result<void> SetShowBlendModes(AEGP_CompH compH, bool showBlendModes);

Result<float> GetCompFramerate(AEGP_CompH compH);

Result<void> SetCompFramerate(AEGP_CompH compH, float fps);

Result<std::pair<A_Ratio, A_Ratio>> GetCompShutterAnglePhase(AEGP_CompH compH);

Result<void> SetCompSuggestedMotionBlurSamples(AEGP_CompH compH, int samples);

Result<float> GetCompWorkAreaStart(AEGP_CompH compH);

Result<float> GetCompWorkAreaDuration(AEGP_CompH compH);

Result<void> SetCompWorkAreaStartAndDuration(AEGP_CompH compH, float start, float dur);

Result<AEGP_LayerH> CreateSolidInComp(const std::string& name, int width, int height, AEGP_ColorVal color, AEGP_CompH parentCompH, float dur);

Result<AEGP_LayerH> CreateCameraInComp(const std::string& name, float x, float y, AEGP_CompH parentCompH);

Result<AEGP_LayerH> CreateLightInComp(const std::string& name, float x, float y, AEGP_CompH parentCompH);

Result<AEGP_Collection2H> GetNewCollectionFromCompSelection(AEGP_PluginID pluginID, AEGP_CompH compH);

Result<void> SetSelection(AEGP_CompH compH, AEGP_Collection2H collectionH);

Result<float> GetCompDisplayStartTime(AEGP_CompH compH);

Result<void> SetCompDisplayStartTime(AEGP_CompH compH, float startTimeInSeconds);

Result<void> SetCompDuration(AEGP_CompH compH, float durationInSeconds);

Result<AEGP_LayerH> CreateNullInComp(const std::string& name, AEGP_CompH parentCompH, float dur);

Result<void> SetCompPixelAspectRatio(AEGP_CompH compH, float pixelAspect);

Result<AEGP_LayerH> CreateTextLayerInComp(AEGP_CompH parentCompH, bool selectNewLayer);

Result<AEGP_LayerH> CreateBoxTextLayerInComp(AEGP_CompH parentCompH, bool selectNewLayer, float x, float y);

Result<void> SetCompDimensions(AEGP_CompH compH, int width, int height);

Result<AEGP_CompH> DuplicateComp(AEGP_CompH compH);

Result<float> GetCompFrameDuration(AEGP_CompH compH);

Result<AEGP_CompH> GetMostRecentlyUsedComp();

Result<AEGP_LayerH> CreateVectorLayerInComp(AEGP_CompH parentCompH);

Result<AEGP_StreamRefH> GetNewCompMarkerStream(AEGP_PluginID pluginID, AEGP_CompH parentCompH);

Result<bool> GetCompDisplayDropFrame(AEGP_CompH compH);

Result<void> SetCompDisplayDropFrame(AEGP_CompH compH, bool dropFrame);

Result<void> ReorderCompSelection(AEGP_CompH compH, int index);


