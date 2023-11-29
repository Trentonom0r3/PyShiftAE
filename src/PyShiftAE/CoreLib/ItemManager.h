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

enum qualityOptions {
    BEST = AEGP_LayerQual_BEST,
    DRAFT = AEGP_LayerQual_DRAFT,
    WIREFRAME = AEGP_LayerQual_WIREFRAME,
    NONE = AEGP_LayerQual_NONE
};


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
    std::size_t size() const {
        return layers_.size();
    }

    Layer& operator[](std::size_t index) {
        if (index >= layers_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return layers_[index];
    }

    const Layer& operator[](std::size_t index) const {
        if (index >= layers_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return layers_[index];
    }
    // Iterator-related methods
    std::vector<Layer>::iterator begin() { return layers_.begin(); }
    std::vector<Layer>::iterator end() { return layers_.end(); }
    std::shared_ptr<Layer> addLayerToCollection(Item itemHandle, int index = -1);
    std::shared_ptr<Layer> addSolidToCollection(Item itemHandle, int index = -1);
    void removeLayerFromCollection(Layer layerHandle);
    void RemoveLayerByIndex(int index);
    std::vector<Layer> getAllLayers();
    std::string getCompName();

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

class ItemCollection {
public:
    explicit ItemCollection(const Result<AEGP_ItemH>& itemHandle) : itemHandle_(itemHandle) {
        items_ = ItemCollection::getItems();
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
        Item item2 = *item;
        auto itemhandle = this->itemHandle_;

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

        return items_;
    }

private:
    Result<AEGP_ItemH> itemHandle_;
    std::vector<std::shared_ptr<Item>> items_;
};

class FolderItem : public Item {
public:
    // Constructors and Destructors
    explicit FolderItem(const Result<AEGP_ItemH>& itemHandle) : Item(itemHandle) {}
    virtual ~FolderItem() = default;

    static std::shared_ptr<FolderItem> createNew(std::string name) {

        auto& projectItem = enqueueSyncTask(getProjectRootFolder);
        projectItem->wait();
        Result<AEGP_ItemH> projItem = projectItem->getResult();

        auto& createfootage = enqueueSyncTask(createFolderItem, name, projItem);
		createfootage->wait();

        Result<AEGP_ItemH> createFolder = createfootage->getResult();

        if (createFolder.error != A_Err_NONE) {
			throw std::runtime_error("Failed to create new folder item");
			return NULL;
		}

		return std::make_shared<FolderItem>(createFolder);
	}

    std::shared_ptr<ItemCollection> ChildItems(); //std::shared_ptr<ItemCollection> ChildItems();
};
/* COpy ProjectCollection over, change to ItemCollection, pass in itemHandle to get childItems,*/
class SolidItem : public FootageItem {
public:
// Constructors and Destructors
	explicit SolidItem(const Result<AEGP_ItemH>& itemHandle) : FootageItem(itemHandle) {}
	virtual ~SolidItem() = default;

	static std::shared_ptr<SolidItem> createNew(std::string name, float width, float height, float red, float green, float blue, float alpha, float duration, int index = -1) {

		auto& createfootage = enqueueSyncTask(NewSolidFootage, name, width, height, red, green, blue, alpha);
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

		return std::make_shared<SolidItem>(footageItem);
	}
};

