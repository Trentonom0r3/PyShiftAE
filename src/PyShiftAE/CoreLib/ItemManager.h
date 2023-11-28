#pragma once
#include "../CoreSDK/TaskUtils.h"
#include "../CoreSDK/Utils.h"

//forward declarations
class Item;
class Layer;
class LayerCollection;
class CompItem;
class FootageItem;
class FolderItem;

class Layer {
public:
    explicit Layer(const Result<AEGP_LayerH>& layerHandle) : layerHandle_(layerHandle) {}

    std::string GetLayerName();
    std::string GetSourceName();
    void SetLayerName(std::string name);
    int index();
    void changeIndex(int index);    
    std::shared_ptr<FootageItem> duplicate();
    float layerTime(); //gets the current time in layer time 
    float layerCompTime(); //gets the current time in comp time
    float inPoint(); //gets the in point in layer time
    float compInPoint(); //gets the in point in comp time
    float duration(); //gets the duration in layer time
    float compDuration(); //gets the duration in comp time
    std::string getQuality();
    void setQuality(int quality);
    void deleteLayer(); //deletes the layer
    float getOffset();
    void setOffset(float offset);
    void setFlag(LayerFlag flag, bool value);
    bool getFlag(LayerFlag flag);
    Result<AEGP_LayerH> getLayerHandle();
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
    Result<AEGP_ItemH> getItemHandle();
protected:
    Result<AEGP_ItemH> itemHandle_;
};


class FootageItem : public Item {
public:
    // Constructors and Destructors
    explicit FootageItem(const Result<AEGP_ItemH>& itemHandle) : Item(itemHandle) {}
    virtual ~FootageItem() = default;

    static std::shared_ptr<FootageItem> createNew(std::string name, std::string path = NULL, int index = -1) {

        auto& createfootage = enqueueSyncTask(createFootage, path);
        createfootage->wait();

        Result<AEGP_FootageH> createFootage = createfootage->getResult();

        auto& getroot = enqueueSyncTask(getProjectRootFolder);
        getroot->wait();

        Result<AEGP_ItemH> Root = getroot->getResult();

        auto& addtoproj = enqueueSyncTask(addFootageToProject, createFootage, Root);
        addtoproj->wait();

        Result<AEGP_ItemH> footageItem = addtoproj->getResult();

        if (footageItem.error != A_Err_NONE) {
			throw std::runtime_error("Failed to create new footage item");
            return NULL;
		}

        return std::make_shared<FootageItem>(footageItem);
    }

};

class LayerCollection {
public:
    explicit LayerCollection(const Result<AEGP_CompH>& compHandle, std::vector<Layer> layers) : compHandle_(compHandle), layers_(layers) {}

    std::shared_ptr<Layer> addLayerToCollection(Item itemHandle, int index = -1);
    std::shared_ptr<Layer> addSolidToCollection(Item itemHandle, int index = -1);
    void removeLayerFromCollection(Layer layerHandle);
    void RemoveLayerByIndex(int index);
    std::vector<Layer> getAllLayers();
protected:
    Result<AEGP_CompH> compHandle_;
    std::vector<Layer> layers_;
};

//TODO: Figure out how to turn "item" into a factory class, but still access things the same way.
class CompItem : public Item {
public:
    explicit CompItem(const Result<AEGP_ItemH>& itemHandle) : Item(itemHandle) {}
    virtual ~CompItem() = default;

    std::shared_ptr<LayerCollection> CompItem::getLayers();
    int NumLayers();
   // float frameRate;
    void addLayer(std::string name, std::string path = NULL, int index = -1);
    float getFrameRate();
    void setFrameRate(float frameRate);
    float getDuration();
    void setDuration(float duration);
    void setWidth(float width);
    std::shared_ptr<CompItem> duplicate();
    void setHeight(float height);
    std::shared_ptr<Layer> CompItem::newSolid(std::string name, float width, float height,float red, float green, float blue, float alpha,
    float duration);

    // Static factory method to create a new CompItem
    static std::shared_ptr<CompItem> CreateNew(std::string name, float width, float height, float frameRate, float duration, float aspectRatio) {
        Result<AEGP_ItemH> itemHandle;
        itemHandle.value = NULL;
        itemHandle.error = A_Err_NONE;

        // Enqueue a task to create a new composition in After Effects
        auto& message = enqueueSyncTask(Addcomp, name, width, height, frameRate, duration, aspectRatio, itemHandle);
        message->wait(); // Wait for the task to complete

        // Obtain the result and create a new CompItem instance
        Result<AEGP_CompH> result = message->getResult();
        AEGP_CompH compH = result.value;
        if (result.error != A_Err_NONE) {
            throw std::runtime_error("Failed to create new composition");
        }

        auto& message2 = enqueueSyncTask(GetItemFromComp, result);
        message2->wait();

        Result<AEGP_ItemH> result2 = message2->getResult();
        AEGP_ItemH itemH = result2.value;
        if (result2.error != A_Err_NONE) {
            throw std::runtime_error("Failed to get item from comp");
        }

        return std::make_shared<CompItem>(result2);
    }
};

class FolderItem : public Item {
public:
    // Constructors and Destructors
    explicit FolderItem(const Result<AEGP_ItemH>& itemHandle) : Item(itemHandle) {}
    virtual ~FolderItem() = default;

    void addFolder(std::string name);
};
