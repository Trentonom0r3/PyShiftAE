#include "Project.h"


std::shared_ptr<Item> Project::ActiveItem() {
	auto& message = enqueueSyncTask(getActiveItem);
	message->wait();

	Result<AEGP_ItemH> result = message->getResult();
	AEGP_ItemH itemH = result.value;

	auto& message2 = enqueueSyncTask(getItemType, result);
	message2->wait();

	Result<AEGP_ItemType> result2 = message2->getResult();

	AEGP_ItemType itemType = result2.value;

	switch (itemType) {
	case AEGP_ItemType_FOLDER:
		return std::make_shared<FolderItem>(result); // 'result' is of type 'Result<AEGP_ItemH>'
	case AEGP_ItemType_COMP:
		return std::make_shared<CompItem>(result);
	case AEGP_ItemType_FOOTAGE:
		return std::make_shared<FootageItem>(result);
	default:
		return std::make_shared<Item>(result);
	}

}