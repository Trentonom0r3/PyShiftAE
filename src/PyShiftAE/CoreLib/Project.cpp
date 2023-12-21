#include "Project.h"


std::shared_ptr<Item> Project::ActiveItem() {
	auto& message = enqueueSyncTask(getActiveItem);
	message->wait();

	Result<AEGP_ItemH> result = message->getResult();
	AEGP_ItemH itemH = result.value;

	if (itemH == nullptr) {
		throw std::runtime_error("No active item");
	}

	auto& message2 = enqueueSyncTask(getItemType, result);
	message2->wait();

	Result<AEGP_ItemType> result2 = message2->getResult();

	AEGP_ItemType itemType = result2.value;
	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error getting item type");
	}
	std::shared_ptr<FolderItem> folderItem;
	std::shared_ptr<FootageItem> footageItem;
	std::shared_ptr<CompItem> compItem;
	std::shared_ptr<Item> item;
	switch (itemType) {
	case AEGP_ItemType_FOLDER:
		folderItem = std::make_shared<FolderItem>(result);
		return folderItem;
	case AEGP_ItemType_FOOTAGE:
		footageItem = std::make_shared<FootageItem>(result);
		return footageItem;
	case AEGP_ItemType_COMP:
		compItem = std::make_shared<CompItem>(result);
		return compItem;
	case AEGP_ItemType_NONE:
		item = std::make_shared<Item>(result);
		return item;
	}
	return nullptr;

}

std::shared_ptr<Layer> Project::GetActiveLayer()
{
	auto& message = enqueueSyncTask(ActiveLayer);
	message->wait();

	Result<AEGP_LayerH> result = message->getResult();
	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting active layer");
	}	
	std::shared_ptr<Layer> layer = std::make_shared<Layer>(result);
	return layer;
}

std::string Project::getName()
{
	auto proj = this->projH_;
	auto& message = enqueueSyncTask(GetProjectName, proj);
	message->wait();

	Result<std::string> result = message->getResult();

	std::string name = result.value;
	return name;

}

std::string Project::getPath()
{
	auto proj = this->projH_;
	auto& message = enqueueSyncTask(GetProjectPath, proj);
	message->wait();

	Result<std::string> result = message->getResult();

	std::string path = result.value;
	return path;
}

void Project::saveAs(std::string path)
{
	auto proj = this->projH_;
	auto& message = enqueueSyncTask(SaveProjectAs, proj, path);
	message->wait();

	Result<void> result = message->getResult();
}

std::shared_ptr<ProjectCollection> Project::ChildItems()
{
	auto& message = enqueueSyncTask(getProject);
	message->wait();

	Result<AEGP_ProjectH> projH = message->getResult();

	if (projH.error != A_Err_NONE) {
		throw std::runtime_error("Error getting project");
	}
	if (projH.value == NULL) {
		throw std::runtime_error("No active project");
	}

	ProjectCollection collection(projH);

	std::shared_ptr<ProjectCollection> collectionPtr = std::make_shared<ProjectCollection>(collection);
	return collectionPtr;

}

std::shared_ptr<ProjectCollection> Project::SelectedItems()
{
	auto& message = enqueueSyncTask(getProject);
	message->wait();

	Result<AEGP_ProjectH> projH = message->getResult();

	if (projH.error != A_Err_NONE) {
		throw std::runtime_error("Error getting project");
	}
	if (projH.value == NULL) {
		throw std::runtime_error("No active project");
	}

	ProjectCollection collection(projH);
	std::vector<std::shared_ptr<Item>> items = collection.getItems();
	std::vector<std::shared_ptr<Item>> selectedItems;
	for (auto item : items) {
		Result<AEGP_ItemH> itemH = item.get()->getItemHandle();
		auto& message = enqueueSyncTask(IsItemSelected, itemH);
		message->wait();

		Result<bool> result = message->getResult();
		if (result.value) {
			selectedItems.push_back(item);
		}
	}
	
	ProjectCollection selectedCollection(selectedItems);
	std::shared_ptr<ProjectCollection> collectionPtr = std::make_shared<ProjectCollection>(selectedCollection);
	return collectionPtr;
}

std::vector<std::shared_ptr<Item>> ProjectCollection::getItems()
{
	std::vector<std::shared_ptr<Item>> items;
	auto& itemResult = enqueueSyncTask(getProjectRootFolder);
	itemResult->wait();
	Result<AEGP_ItemH> itemhandle = itemResult->getResult();

	// Check if the current folder item is valid
	if (itemhandle.value == NULL) {
		throw std::runtime_error("No active item");
	}

	// Get the project handle
	auto& projectMessage = enqueueSyncTask(getProject);
	projectMessage->wait();
	Result<AEGP_ProjectH> projectResult = projectMessage->getResult();
	if (projectResult.error != A_Err_NONE) {
		throw std::runtime_error("Error getting project");
	}

	// Start with the first project item
	auto& itemMessage = enqueueSyncTask(GetFirstProjItem, projectResult);
	itemMessage->wait();
	Result<AEGP_ItemH> currentItemResult = itemMessage->getResult();
	if (currentItemResult.error != A_Err_NONE) {
		throw std::runtime_error("Error getting first project item");
	}

	while (currentItemResult.value != NULL) {
		// Check if the current item is a child of the folder
		auto& message = enqueueSyncTask(GetItemParentFolder, currentItemResult);
		message->wait();

		Result<AEGP_ItemH> parentItemH = message->getResult();

		if (parentItemH.value == itemhandle.value) {
			// Get the type of the current item
			auto& typeMessage = enqueueSyncTask(getItemType, currentItemResult);
			typeMessage->wait();
			Result<AEGP_ItemType> itemTypeResult = typeMessage->getResult();
			if (itemTypeResult.error != A_Err_NONE) {
				throw std::runtime_error("Error getting item type");
			}

			// Declare shared_ptr variables outside the switch statement
			std::shared_ptr<FolderItem> folderItem;
			std::shared_ptr<FootageItem> footageItem;
			std::shared_ptr<CompItem> compItem;
			std::shared_ptr<Item> item;

			// Add the current item to the list based on its type
			switch (itemTypeResult.value) {
			case AEGP_ItemType_FOLDER:
				folderItem = std::make_shared<FolderItem>(currentItemResult);
				items.push_back(folderItem);
				break;
			case AEGP_ItemType_FOOTAGE:
				footageItem = std::make_shared<FootageItem>(currentItemResult);
				items.push_back(footageItem);
				break;
			case AEGP_ItemType_COMP:
				compItem = std::make_shared<CompItem>(currentItemResult);
				items.push_back(compItem);
				break;
			case AEGP_ItemType_NONE:
				item = std::make_shared<Item>(currentItemResult);
				items.push_back(item);
				break;
			}

		}

		// Get the next project item
		auto& nextItemMessage = enqueueSyncTask(GetNextProjItem, projectResult, currentItemResult);
		nextItemMessage->wait();
		currentItemResult = nextItemMessage->getResult();
		if (currentItemResult.error != A_Err_NONE) {
			throw std::runtime_error("Error getting next project item");
		}
	}

	return items;
}
