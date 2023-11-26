#pragma once
#include "../CoreSDK/TaskUtils.h"
#include "../CoreSDK/Utils.h"

class Layer {
public:
    explicit Layer(const Result<AEGP_LayerH>& layerHandle) : layerHandle_(layerHandle) {}

    std::string GetLayerName();
    std::string GetSourceName();
    void SetLayerName(std::string name);
    int index();
    void changeIndex(int index);    
    Layer duplicate();
    float layerTime(); //gets the current time in layer time 
    float layerCompTime(); //gets the current time in comp time
    float inPoint(); //gets the in point in layer time
    float compInPoint(); //gets the in point in comp time
    float duration(); //gets the duration in layer time
    float compDuration(); //gets the duration in comp time
    std::string getQuality();
    void setQuality(std::string quality);
    void deleteLayer(); //deletes the layer
    float getOffset();
    void setOffset(float offset);

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
    float getWidth();
    float getHeight();
    std::string name;
    float getCurrentTime();
    float getDuration();
    void deleteItem();
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
    float getFrameRate();
    void setFrameRate(float frameRate);
    float getDuration();
    void setDuration(float duration);
    void setWidth(float width);
    CompItem duplicate();
    void setHeight(float height);
    Layer CompItem::newSolid(std::string name, float width, float height,float red, float green, float blue, float alpha,
    float duration);
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
