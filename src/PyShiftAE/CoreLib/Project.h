#pragma once
#include "../CoreSDK/TaskUtils.h"
#include "../CoreSDK/Core.h"
#include "ItemManager.h"

//Project class definition
class Project {
public:
    Project() = default;  // Default constructor
    explicit Project(const Result<AEGP_ItemH>& itemHandle)
        : activeItem(std::make_shared<Item>(itemHandle)) {}

    std::shared_ptr<Item> ActiveItem();


    std::string name;
    std::string path;

private:
    std::shared_ptr<Item> activeItem;
    Result<AEGP_ProjectH> projH_;
};