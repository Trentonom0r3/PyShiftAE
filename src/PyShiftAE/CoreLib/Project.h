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
    void addFolder(std::string name);
    void Project::addComp(std::string name, int width,
                         int height, float frameRate, int duration, float aspectRatio);
    Result<AEGP_ItemH> addFootage(std::string path);

    std::string name;
    std::string path;

private:
    std::shared_ptr<Item> activeItem;
    Result<AEGP_ProjectH> projH_;
};