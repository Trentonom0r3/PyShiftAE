#pragma once
#include "Core.h"

#include "LayerSuites.h"


Result<int> getNumLayers(Result<AEGP_CompH> compH);

Result<int> getLayerIndex(Result<AEGP_LayerH> layerH);

Result<std::string> getLayerName(Result<AEGP_LayerH> layerH);

Result<std::string> getLayerSourceName(Result<AEGP_LayerH> layerH);

Result<void> setLayerName(Result<AEGP_LayerH> layerH, const std::string& name);

Result<AEGP_LayerH> ActiveLayer();

Result<AEGP_LayerH> getLayerFromComp(Result<AEGP_CompH> compH, int index);

Result<void> changeLayerIndex(Result<AEGP_LayerH> layerH, int index);

Result<bool> isAddLayerValid(Result<AEGP_ItemH> itemH, Result<AEGP_CompH> compH);

Result<AEGP_LayerH> AddLayer(Result<AEGP_ItemH> itemH, Result<AEGP_CompH> compH);

Result<AEGP_ItemH> getLayerSourceItem(Result<AEGP_LayerH> layerH);

Result<int> GetLayerSourceItemID(Result<AEGP_LayerH> layerH);

Result<AEGP_CompH> GetLayerParentComp(Result<AEGP_LayerH> layerH);

Result<std::string> GetLayerQuality(Result<AEGP_LayerH> layerH);

Result<void> SetLayerQuality(Result<AEGP_LayerH> layerH, int qualityint);

//gets flags for layer, any flags that are true are returned
Result<AEGP_LayerFlags> GetLayerFlags(Result<AEGP_LayerH> layerH); //DEAL WITH THIS LATER

// sets flag false or true given user input
Result<void> SetLayerFlag(Result<AEGP_LayerH> layerH, LayerFlag flag, A_Boolean value);  //DEAL WITH THIS LATER

Result<bool> IsLayerVideoReallyOn(Result<AEGP_LayerH> layerH);

Result<bool> IsLayerAudioReallyOn(Result<AEGP_LayerH> layerH);

Result<float> GetLayerCurrentTime(Result<AEGP_LayerH> layerH, AEGP_LTimeMode time_mode);

Result<float> GetLayerInPoint(Result<AEGP_LayerH> layerH, AEGP_LTimeMode time_mode);

Result<float> GetLayerDuration(Result<AEGP_LayerH> layerH, AEGP_LTimeMode time_mode);

Result<void> SetLayerInPointAndDuration(Result<AEGP_LayerH> layerH, AEGP_LTimeMode time_mode, float in_point, float duration, float frameRate);

Result<float> GetLayerOffset(Result<AEGP_LayerH> layerH);

Result<void> SetLayerOffset(Result<AEGP_LayerH> layerH, float offsetH, float frameRate);

Result<float> GetLayerStretch(Result<AEGP_LayerH> layerH);

Result<void> SetLayerStretch(Result<AEGP_LayerH> layerH, const A_Ratio& stretch); //deal with this later

Result<AEGP_LayerTransferMode> GetLayerTransferMode(Result<AEGP_LayerH> layerH);

Result<void> SetLayerTransferMode(Result<AEGP_LayerH> layerH, const AEGP_LayerTransferMode mode);

Result<A_FloatRect> GetLayerMaskedBounds(Result<AEGP_LayerH> layerH, float intime, AEGP_LTimeMode comp_time, float frameRate);

Result<AEGP_ObjectType> GetLayerObjectType(Result<AEGP_LayerH> layerH);

Result<A_Boolean> IsLayer3D(Result<AEGP_LayerH> layerH);

Result<A_Boolean> IsLayer2D(Result<AEGP_LayerH> layerH);

Result<A_Boolean> IsVideoActive(Result<AEGP_LayerH> layerH, AEGP_LTimeMode time_mode, float comp_timeH, float frameRate);

Result<bool> IsLayerUsedAsTrackMatte(Result<AEGP_LayerH> layerH, bool fill_must_be_active);

Result<bool> DoesLayerHaveTrackMatte(Result<AEGP_LayerH> layerH);

Result<float> ConvertCompToLayerTime(Result<AEGP_LayerH> layerH, float comp_time);

Result<float> ConvertLayerToCompTime(Result<AEGP_LayerH> layerH, float layer_time, float frameRate);

Result<float> GetLayerDancingRandValue(Result<AEGP_LayerH> layerH, float comp_time, float frameRate);

Result<int> GetLayerID(Result<AEGP_LayerH> layerH);

Result<AEGP_LayerH> GetLayerFromLayerID(Result<AEGP_CompH> parent_comp, int id);

Result<AEGP_LabelID> GetLayerLabel(Result<AEGP_LayerH> layerH);

Result<void> SetLayerLabel(Result<AEGP_LayerH> layerH, AEGP_LabelID label);

Result<AEGP_LayerSamplingQuality> GetLayerSamplingQuality(Result<AEGP_LayerH> layerH);

Result<void> SetLayerSamplingQuality(Result<AEGP_LayerH> layerH, AEGP_LayerSamplingQuality quality);

Result<AEGP_LayerH> GetTrackMatteLayer(Result<AEGP_LayerH> layerH);

Result<void> SetTrackMatte(Result<AEGP_LayerH> layerH, Result<AEGP_LayerH> track_matte_layer, AEGP_TrackMatte track_matte_type);

Result<void> RemoveTrackMatte(Result<AEGP_LayerH> layerH);

Result<void> DeleteLayer(Result<AEGP_LayerH> layerH);

Result<AEGP_LayerH> DuplicateLayer(Result<AEGP_LayerH> layerH);

Result<AEGP_LayerH> GetLayerParent(Result<AEGP_LayerH> layerH);

Result<void> SetLayerParent(Result<AEGP_LayerH> layerH, AEGP_LayerH parent_layer);
