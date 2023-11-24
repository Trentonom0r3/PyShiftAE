#pragma once
#include "Core.h"

#include "LayerSuites.h"



Result<int> getNumLayers(Result<AEGP_CompH> compH);

Result<int> getLayerIndex(Result<AEGP_LayerH> layerH);

Result<std::string> getLayerName(Result<AEGP_LayerH> layerH);

Result<std::string> getLayerSourceName(Result<AEGP_LayerH> layerH);

Result<void> setLayerName(Result<AEGP_LayerH> layerH, const std::string& name);

Result<AEGP_LayerH> getLayerFromComp(Result<AEGP_CompH> compH, int index);

Result<void> changeLayerIndex(Result<AEGP_LayerH> layerH, int index);

Result<bool> isAddLayerValid(Result<AEGP_ItemH> itemH, Result<AEGP_CompH> compH);

Result<AEGP_LayerH> AddLayer(Result<AEGP_ItemH> itemH, Result<AEGP_CompH> compH);

Result<AEGP_ItemH> getLayerSourceItem(Result<AEGP_LayerH> layerH);
