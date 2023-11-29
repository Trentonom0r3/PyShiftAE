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

std::string Project::getName()
{
	auto proj = this->projH_;
	auto& message = enqueueSyncTask(GetProjectName, proj);
	message->wait();

	Result<std::string> result = message->getResult();

	return result.value;

}

std::string Project::getPath()
{
	auto proj = this->projH_;
	auto& message = enqueueSyncTask(GetProjectPath, proj);
	message->wait();

	Result<std::string> result = message->getResult();

	return result.value;
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

	return std::make_shared<ProjectCollection>(collection);

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

			// Add the current item to the list based on its type
			switch (itemTypeResult.value) {
			case AEGP_ItemType_FOLDER:
				items.push_back(std::make_shared<FolderItem>(currentItemResult));
				break;
			case AEGP_ItemType_FOOTAGE:
				items.push_back(std::make_shared<FootageItem>(currentItemResult));
				break;
			case AEGP_ItemType_COMP:
				items.push_back(std::make_shared<CompItem>(currentItemResult));
				break;
				// Handle other types as needed
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
