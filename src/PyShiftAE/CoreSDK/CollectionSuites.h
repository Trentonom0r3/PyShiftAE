#pragma once
#include "Core.h"

/*

Managing Selections
This suite manages selection states, mirroring the functionality supplied by vectors in the C++ Standard Template Library.

Many types of items may be simultaneously selected in After Effects; AEGP_CollectionItems are unions of layer, mask, effect, stream, mask vertex, and keyframe items.

First acquire the current collection, then iterate across its members to ensure that whatever your AEGP does is applicable to each.

We’ve added AEGP_Collection2H and AEGP_CollectionItemV2 so that selected dynamic streams can be handled with the AEGP_CollectionSuite.

AEGP_CollectionSuite2
Function

Purpose

AEGP_NewCollection

Creates and returns a new, empty collection. To obtain the current composition’s selection as a collection, use AEGP_GetNewCollectionFromCompSelection.

AEGP_NewCollection(
  AEGP_PluginID      plugin_id,
  AEGP_Collection2H  *collectionPH);
AEGP_DisposeCollection

Disposes of a collection.

AEGP_DisposeCollection(
  AEGP_Collection2H  collectionH);
AEGP_GetCollectionNumItems

Returns the number of items contained in the given collection.

AEGP_GetCollectionNumItems(
  AEGP_Collection2H  collectionH,
  A_u_long           *num_itemsPL);
AEGP_GetCollectionItemByIndex

Retrieves (creates and populates) the index’d collection item.

AEGP_GetCollectionItemByIndex(
  AEGP_Collection2H      collectionH,
  A_u_long               indexL,
  AEGP_CollectionItemV2  *itemP);
AEGP_CollectionPushBack

Adds an item to the given collection.

AEGP_CollectionPushBack(
  AEGP_Collection2H            collectionH,
  const AEGP_CollectionItemV2  *itemP);
AEGP_CollectionErase

Removes an index’d item (or items) from a given collection. NOTE: this range is exclusive, like STL iterators. To erase the first item, you would pass 0 and 1, respectively.

AEGP_CollectionErase(
  AEGP_Collection2H  collectionH,
  A_u_long           index_firstL,
  A_u_long           index_lastL);
Ownership Of Collection Items
When AEGP_StreamRefHs are inserted into a collection, they are adopted by the collection; do not free them.

AEGP_EffectRefHs, on the other hand, are not adopted, and must be freed by the calling AEGP.

*/

Result<AEGP_Collection2H> NewCollection();

Result<void> DisposeCollection(AEGP_Collection2H collectionH);

Result<int> GetCollectionNumItems(AEGP_Collection2H collectionH);

Result<AEGP_CollectionItemV2> GetCollectionItemByIndex( AEGP_Collection2H collectionH, int indexL);

Result<void> CollectionPushBack( AEGP_Collection2H collectionH, const AEGP_CollectionItemV2* itemP);

Result<void> CollectionErase( AEGP_Collection2H collectionH, int index_firstL, int index_lastL);

