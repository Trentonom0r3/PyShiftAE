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

void Project::addFolder(std::string name)
{
	auto& message = enqueueSyncTask(createFolderItem, name, NULL); //passing NULL as first argument creates folder in root directory
	message->wait();

	Result<void> result = message->getResult();
}


void Project::addComp(std::string name, int width, int height, float frameRate, int duration, float aspectRatio)
{
	Result<AEGP_ItemH> itemHandle;
	itemHandle.value = NULL;
	itemHandle.error = A_Err_NONE;

	auto& message = enqueueSyncTask(Addcomp, name, width, height, frameRate, duration, aspectRatio, itemHandle); //passing NULL as last argument creates comp in root directory
	message->wait();

	Result<void> result = message->getResult();
}

Result<AEGP_ItemH> Project::addFootage(std::string path)
{
	//sanitizePath(path);
	auto& message = enqueueSyncTask(createFootage,path);
	message->wait();

	Result<AEGP_FootageH> result = message->getResult();

	auto& message2 = enqueueSyncTask(getProjectRootFolder);
	message2->wait();

	Result<AEGP_ItemH> result2 = message2->getResult();

	auto& message3 = enqueueSyncTask(addFootageToProject, result, result2);
	message3->wait();

	Result<AEGP_ItemH> result3 = message3->getResult();

	return result3;
}
