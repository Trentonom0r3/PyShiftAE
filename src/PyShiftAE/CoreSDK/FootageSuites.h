#pragma once
#include "Core.h"

#include "FootageSuites.h"

Result<AEGP_FootageH> createFootage(const std::string& path);

Result<void> disposeFootage(Result<AEGP_FootageH> footageH);

Result<AEGP_ItemH> addFootageToProject(Result<AEGP_FootageH> footageH, Result<AEGP_ItemH> parentFolderH);

Result<AEGP_ColorVal> GetItemSolidColor(AEGP_ItemH itemH, bool proxyB);

Result<void> SetSolidColor(AEGP_ItemH itemH, bool proxyB, float red, float green, float blue, float alpha);

Result<void> SetSolidDimensions(AEGP_ItemH itemH, bool proxyB, int width, int height);