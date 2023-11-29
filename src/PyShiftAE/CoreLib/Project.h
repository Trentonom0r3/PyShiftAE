#pragma once
#include "../CoreSDK/TaskUtils.h"
#include "../CoreSDK/Utils.h"
#include "ItemManager.h"

class ProjectCollection {
public:
    explicit ProjectCollection(const Result<AEGP_ProjectH>& projHandle) : projHandle_(projHandle) {
        items_ = ProjectCollection::getItems();
    }

    std::vector<std::shared_ptr<Item>> getItems();

    std::size_t size() const {
        return items_.size();
    }

    Item& operator[](std::size_t index) {
        if (index >= items_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return *items_[index];
    }

    const Item& operator[](std::size_t index) const {
        if (index >= items_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return *items_[index];
    }
    // Iterator-related methods
    std::vector<std::shared_ptr<Item>>::iterator begin() { return items_.begin(); }
    std::vector<std::shared_ptr<Item>>::iterator end() { return items_.end(); }
    std::vector<std::shared_ptr<Item>> append(std::shared_ptr<Item> item) {
        auto proj = this->projHandle_;
        auto& message = enqueueSyncTask(getProjectRootFolder);
        message->wait();
        Result<AEGP_ItemH> itemhandle = message->getResult();

        // Check if the current folder item is valid
        if (itemhandle.value == NULL) {
			throw std::runtime_error("No active item");
		}
        Result<AEGP_ItemH> itemhandle2 = item->getItemHandle();
        auto& message2 = enqueueSyncTask(SetItemParentFolder, itemhandle2, itemhandle);
        message2->wait();
        Result<void> result = message2->getResult();
        if (result.error != A_Err_NONE) {
			throw std::runtime_error("Error setting item parent folder");
		}
        items_.push_back(item);

		return items_;
	}
    std::vector<std::shared_ptr<Item>> remove(std::shared_ptr<Item> item) {
        if (item == nullptr) {
            throw std::runtime_error("Item is null");
        }
        Item item2 = *item;
        item2.deleteItem();
	}

private:
    Result<AEGP_ProjectH> projHandle_;
    std::vector<std::shared_ptr<Item>> items_;
};

//Project class definition
class Project {
public:
    Project() = default;  // Default constructor
    explicit Project(const Result<AEGP_ItemH>& itemHandle)
        : activeItem(std::make_shared<Item>(itemHandle)) {}

    std::shared_ptr<Item> ActiveItem();
    std::string getName();
    std::string getPath();
    void saveAs(std::string path);
    std::shared_ptr<ProjectCollection> ChildItems();
private:
    std::shared_ptr<Item> activeItem;
    Result<AEGP_ProjectH> projH_;
};
