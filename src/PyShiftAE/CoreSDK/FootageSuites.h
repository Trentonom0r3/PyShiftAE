#pragma once
#include "Core.h"

#include "FootageSuites.h"


Result<AEGP_FootageH> createFootage(const std::string& path);

Result<void> disposeFootage(Result<AEGP_FootageH> footageH);

Result<AEGP_ItemH> addFootageToProject(Result<AEGP_FootageH> footageH, Result<AEGP_ItemH> parentFolderH);

Result<AEGP_ColorVal> GetItemSolidColor(Result<AEGP_ItemH> itemH, bool proxyB);

Result<void> SetSolidColor(Result<AEGP_ItemH> itemH, bool proxyB, float red, float green, float blue, float alpha);

Result<void> SetSolidDimensions(Result<AEGP_ItemH> itemH, bool proxyB, float width, float height);

Result<AEGP_FootageH> GetMainFootageFromItem(Result<AEGP_ItemH> itemH);

Result<AEGP_FootageH> GetProxyFootageFromItem(Result<AEGP_ItemH> itemH);

Result<std::pair<int, int>> GetFootageNumFiles(Result<AEGP_FootageH> footageH);

Result<std::string> GetFootagePath(Result<AEGP_FootageH> footageH, int frameNum, int fileIndex);

Result<AEGP_FootageSignature> GetFootageSignature(Result<AEGP_FootageH> footageH);

Result<void> SetItemProxyFootage(Result<AEGP_FootageH> footageH, Result<AEGP_ItemH> itemH);

Result<AEGP_ItemH> ReplaceItemMainFootage(Result<AEGP_FootageH> footageH, Result<AEGP_ItemH> itemH);

Result<void> SetFootageInterpretation(Result<AEGP_ItemH> itemH, bool proxyB, AEGP_FootageInterp interpP);

Result<AEGP_FootageInterp> GetFootageInterpretation(Result<AEGP_ItemH> itemH, bool proxyB);

Result<AEGP_FootageLayerKey> GetFootageLayerKey(Result<AEGP_FootageH> footageH);

Result<AEGP_FootageH> NewPlaceholderFootage(const std::string& name,
	float width, float height, float duration, float frameRate);

Result<AEGP_FootageH> NewPlaceholderFootageWithPath(const std::string& path, AEGP_Platform path_platform, AEIO_FileType file_type,
	float width, float height, float duration, float frameRate);

Result<AEGP_FootageH> NewSolidFootage(const std::string& name, float width, float height, float red, float green, float blue, float alpha);

Result<AEGP_SoundDataFormat> GetFootageSoundDataFormat(Result<AEGP_FootageH> footageH);

Result<AEGP_FileSequenceImportOptions> GetFootageSequenceImportOptions(Result<AEGP_FootageH> footageH);


