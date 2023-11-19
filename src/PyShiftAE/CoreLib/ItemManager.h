#pragma once
#include "../CoreSDK/TaskUtils.h"
#include "../CoreSDK/Core.h"


//PyCore.h"
class Item {
public:
    explicit Item(const Result<AEGP_ItemH>& itemHandle) : itemHandle_(itemHandle) {}
    virtual ~Item() = default;

    std::string getName();
    void setName(std::string name);
    ImageData frameAtTime(float time);

    std::string name;
protected:
    Result<AEGP_ItemH> itemHandle_;
};

//TODO: Figure out how to turn "item" into a factory class, but still access things the same way.
class CompItem : public Item {
public:
    explicit CompItem(const Result<AEGP_ItemH>& itemHandle) : Item(itemHandle) {}
    virtual ~CompItem() = default;

    //ImageData frameAtTime(float time);
    //void replaceFrameAtTime(ImageData& new_img, float time);

   // int numLayers;
   // float frameRate;

};


class FootageItem : public Item {
public:
    // Constructors and Destructors
    explicit FootageItem(const Result<AEGP_ItemH>& itemHandle) : Item(itemHandle) {}
    virtual ~FootageItem() = default;

};

class FolderItem : public Item {
public:
    // Constructors and Destructors
    explicit FolderItem(const Result<AEGP_ItemH>& itemHandle) : Item(itemHandle) {}
    virtual ~FolderItem() = default;

};
