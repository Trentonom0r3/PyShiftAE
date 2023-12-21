#pragma once
#include "Core.h"

Result<AEGP_CompH> Addcomp(std::string name, float width, float height, float frameRate, float durationSecs,
									  float pixelAspectRatio, Result<AEGP_ItemH> parentFolder);

Result<AEGP_CompH> getCompFromItem(Result<AEGP_ItemH> itemH);

Result<AEGP_ItemH> GetItemFromComp(Result<AEGP_CompH> compH);

Result<AEGP_DownsampleFactor> GetCompDownsampleFactor(Result<AEGP_CompH> compH);

Result<void> SetCompDownsampleFactor(Result<AEGP_CompH> compH, int x, int y);

Result<AEGP_ColorVal> GetCompBGColor(Result<AEGP_CompH> compH);

Result<void> SetCompBGColor(Result<AEGP_CompH> compH, float red, float green, float blue, float alpha);

Result<AEGP_CompFlags> GetCompFlags(Result<AEGP_CompH> compH);

Result<bool> GetShowLayerNameOrSourceName(Result<AEGP_CompH> compH);

Result<void> SetShowLayerNameOrSourceName(Result<AEGP_CompH> compH, bool showLayerNames);

Result<bool> GetShowBlendModes(Result<AEGP_CompH> compH);

Result<void> SetShowBlendModes(Result<AEGP_CompH> compH, bool showBlendModes);

Result<float> GetCompFramerate(Result<AEGP_CompH> compH);

Result<void> SetCompFramerate(Result<AEGP_CompH> compH, float fps);

Result<std::pair<A_Ratio, A_Ratio>> GetCompShutterAnglePhase(Result<AEGP_CompH> compH);

Result<void> SetCompSuggestedMotionBlurSamples(Result<AEGP_CompH> compH, int samples);

Result<float> GetCompWorkAreaStart(Result<AEGP_CompH> compH);

Result<float> GetCompWorkAreaDuration(Result<AEGP_CompH> compH);

Result<void> SetCompWorkAreaStartAndDuration(Result<AEGP_CompH> compH, float start, float dur);

Result<AEGP_LayerH> CreateSolidInComp(const std::string& name, int width, int height, float red, float green, float blue, float alpha, Result<AEGP_CompH> parentCompH, float dur);

Result<AEGP_LayerH> CreateCameraInComp(const std::string& name, float x, float y, Result<AEGP_CompH> parentCompH);

Result<AEGP_LayerH> CreateLightInComp(const std::string& name, float x, float y, Result<AEGP_CompH> parentCompH);

Result<std::vector<Result<AEGP_LayerH>>> GetNewCollectionFromCompSelection(Result<AEGP_CompH> compH);

Result<void> SetSelection(Result<AEGP_CompH> compH, AEGP_Collection2H collectionH);

Result<float> GetCompDisplayStartTime(Result<AEGP_CompH> compH);

Result<void> SetCompDisplayStartTime(Result<AEGP_CompH> compH, float startTimeInSeconds);

Result<void> SetCompDuration(Result<AEGP_CompH> compH, float durationInSeconds);

Result<AEGP_LayerH> CreateNullInComp(const std::string& name, Result<AEGP_CompH> parentCompH, float dur);

Result<void> SetCompPixelAspectRatio(Result<AEGP_CompH> compH, float pixelAspect);

Result<AEGP_LayerH> CreateTextLayerInComp(Result<AEGP_CompH> parentCompH, bool selectNewLayer);

Result<AEGP_LayerH> CreateBoxTextLayerInComp(Result<AEGP_CompH> parentCompH, bool selectNewLayer, float x, float y);

Result<void> SetCompDimensions(Result<AEGP_CompH> compH, int width, int height);

Result<AEGP_CompH> DuplicateComp(Result<AEGP_CompH> compH);

Result<float> GetCompFrameDuration(Result<AEGP_CompH> compH);

Result<AEGP_CompH> GetMostRecentlyUsedComp();

Result<AEGP_LayerH> CreateVectorLayerInComp(Result<AEGP_CompH> parentCompH);

Result<AEGP_StreamRefH> GetNewCompMarkerStream(Result<AEGP_CompH> parentCompH);

Result<bool> GetCompDisplayDropFrame(Result<AEGP_CompH> compH);

Result<void> SetCompDisplayDropFrame(Result<AEGP_CompH> compH, bool dropFrame);

Result<void> ReorderCompSelection(Result<AEGP_CompH> compH, int index);


