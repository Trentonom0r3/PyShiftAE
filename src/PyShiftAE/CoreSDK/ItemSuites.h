#pragma once
#include "Core.h"

#include "ItemSuites.h"


Result<AEGP_ItemH> getActiveItem();

Result<AEGP_ItemType> getItemType(Result<AEGP_ItemH> item);

Result<std::string>getItemName(Result<AEGP_ItemH> itemH);

Result<void>setItemName(Result<AEGP_ItemH> itemH, const std::string& name);

Result<A_long> getUniqueItemID(Result<AEGP_ItemH> itemH);

Result<void> createFolderItem(const std::string& name, Result<AEGP_ItemH> parentFolderH);

Result<AEGP_ItemH> GetFirstProjItem(AEGP_ProjectH projectH);

Result<AEGP_ItemH> GetNextProjItem(AEGP_ProjectH projectH, AEGP_ItemH currentItem);

Result<bool> IsItemSelected(AEGP_ItemH itemH);

Result<void> SelectItem(AEGP_ItemH itemH, bool select, bool deselectOthers);

Result<AEGP_ItemFlags> GetItemFlags(AEGP_ItemH itemH);

Result<void> SetItemUseProxy(AEGP_ItemH itemH, bool useProxy);

Result<AEGP_ItemH> GetItemParentFolder(AEGP_ItemH itemH);

Result<void> SetItemParentFolder(AEGP_ItemH itemH, AEGP_ItemH parentFolderH);

Result<float> GetItemDuration(AEGP_ItemH itemH);

Result<float> GetItemCurrentTime(AEGP_ItemH itemH);

Result<std::pair<long, long>> GetItemDimensions(AEGP_ItemH itemH);

Result<float> GetItemPixelAspectRatio(AEGP_ItemH itemH);

Result<void> DeleteItem(AEGP_ItemH itemH);

Result<void> SetItemCurrentTime(AEGP_ItemH itemH, float time);

Result<std::string> GetItemComment(AEGP_ItemH itemH);

Result<void> SetItemComment(AEGP_ItemH itemH, const std::string& comment);

Result<AEGP_LabelID> GetItemLabel(AEGP_ItemH itemH);

Result<void> SetItemLabel(AEGP_ItemH itemH, AEGP_LabelID label);

Result<AEGP_ItemViewP> GetItemMRUView(AEGP_ItemH itemH);
