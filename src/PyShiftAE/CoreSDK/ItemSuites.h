#pragma once
#include "Core.h"

#include "ItemSuites.h"

Result<AEGP_ItemH> getActiveItem();

Result<AEGP_ItemType> getItemType(Result<AEGP_ItemH> item);

Result<std::string>getItemName(Result<AEGP_ItemH> itemH);

Result<void>setItemName(Result<AEGP_ItemH> itemH, const std::string& name);

Result<A_long> getUniqueItemID(Result<AEGP_ItemH> itemH);

Result<AEGP_ItemH> createFolderItem(const std::string& name, Result<AEGP_ItemH> parentFolderH);

Result<AEGP_ItemH> GetFirstProjItem(Result<AEGP_ProjectH> projectH);

Result<AEGP_ItemH> GetNextProjItem(Result<AEGP_ProjectH> projectH, Result<AEGP_ItemH> currentItemH);

Result<bool> IsItemSelected(Result<AEGP_ItemH> itemH);

Result<void> SelectItem(Result<AEGP_ItemH> itemH, bool select, bool deselectOthers);

Result<AEGP_ItemFlags> GetItemFlags(Result<AEGP_ItemH> itemH);

Result<float> GetCompItemCurrentTime(Result<AEGP_ItemH> itemH, float frameRate);
Result<void> SetCompItemCurrentTime(Result<AEGP_ItemH> itemH, float time, float frameRate);

Result<void> SetItemUseProxy(Result<AEGP_ItemH> itemH, bool useProxy);

Result<AEGP_ItemH> GetItemParentFolder(Result<AEGP_ItemH> itemH);

Result<void> SetItemParentFolder(Result<AEGP_ItemH> itemH, Result<AEGP_ItemH> parentFolderH);

Result<float> GetItemDuration(Result<AEGP_ItemH> itemH);

Result<float> GetItemCurrentTime(Result<AEGP_ItemH> itemH);

Result<size> GetItemDimensions(Result<AEGP_ItemH> itemH);

Result<float> GetItemPixelAspectRatio(Result<AEGP_ItemH>itemH);

Result<void> DeleteItem(Result<AEGP_ItemH> itemH);

Result<void> SetItemCurrentTime(Result<AEGP_ItemH> itemH, float time);

Result<std::string> GetItemComment(Result<AEGP_ItemH> itemH);

Result<void> SetItemComment(Result<AEGP_ItemH> itemH, const std::string& comment);

Result<AEGP_LabelID> GetItemLabel(Result<AEGP_ItemH> itemH);

Result<void> SetItemLabel(Result<AEGP_ItemH> itemH, AEGP_LabelID label);

Result<AEGP_ItemViewP> GetItemMRUView(Result<AEGP_ItemH> itemH);