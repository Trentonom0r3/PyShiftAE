#pragma once
#include "../CoreSDK/TaskUtils.h"
#include "../CoreSDK/Core.h"


class Layer {
public:
    explicit Layer(const Result<AEGP_LayerH>& layerHandle) : layerHandle_(layerHandle) {}

    std::string GetLayerName();
    std::string GetSourceName();
    void SetLayerName(std::string name);
    int index();
    void changeIndex(int index);    

protected:
    Result<AEGP_LayerH> layerHandle_;
    int index_;
    std::string name_;
    std::string sourceName_;
};

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

   std::vector<Layer> getLayers();
   int NumLayers();
   // float frameRate;
   void addLayer(std::string name, std::string path = NULL, int index = -1);

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

    void addFolder(std::string name);
};
