#pragma once
#include "../CoreSDK/TaskUtils.h"
#include "../CoreSDK/Utils.h"
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>  // for the embedded interpreter
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
//forward declarations
class Item;
class Layer;
class LayerCollection;
class CompItem;
class FootageItem;
class FolderItem;

namespace py = pybind11;

// Slider class equivalent
class Slider {
public:
    Slider(const std::string& name, float minS, float maxS, float default_)
        : name(name), minimum(minS), maximum(maxS), _default(default_) {}

    std::string name;
    float minimum, maximum, _default;
};

// CustomEffect class equivalent
class CustomEffect {
public:
    CustomEffect() {}

    std::string name;
    std::vector<Slider> sliders;
    py::function callback_function;
};

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
    void addEffect(std::shared_ptr<CustomEffect> effect);
    std::shared_ptr<Item> getSource();
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
    float getDuration();
    float getCurrentTime();
    void setCurrentTime(float time);
    void deleteItem();
    Result<AEGP_ItemH> getItemHandle();
    bool isSelected();
    void setSelected(bool select);
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
    std::string getPath();
};
class LayerCollection {
public:
    explicit LayerCollection(const Result<AEGP_CompH>& compHandle, std::vector<std::shared_ptr<Layer>> layers)
        : compHandle_(compHandle), layers_(std::move(layers)) {}
    auto begin() const { return layers_.cbegin(); }
    auto end() const { return layers_.cend(); }
    std::size_t size() const {
        return layers_.size();
    }

    std::shared_ptr<Layer>& operator[](std::size_t index) {
        if (index >= layers_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return layers_[index];
    }

    const std::shared_ptr<Layer>& operator[](std::size_t index) const {
        if (index >= layers_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return layers_[index];
    }

    // Iterator-related methods
    auto begin() { return layers_.begin(); }
    auto end() { return layers_.end(); }

    std::shared_ptr<Layer> addLayerToCollection(Item itemHandle, int index = -1);
    std::shared_ptr<Layer> addSolidToCollection(Item itemHandle, int index = -1);
    void removeLayerFromCollection(Layer layerHandle);
    void RemoveLayerByIndex(int index);

    std::vector<std::shared_ptr<Layer>> getAllLayers() {
        return layers_;
    }

    std::string getCompName();

protected:
    Result<AEGP_CompH> compHandle_;
    std::vector<std::shared_ptr<Layer>> layers_;
};


//TODO: Figure out how to turn "item" into a factory class, but still access things the same way.
class CompItem : public Item {
public:
    explicit CompItem(const Result<AEGP_ItemH>& itemHandle) : Item(itemHandle) {}
    virtual ~CompItem() = default;

    std::shared_ptr<LayerCollection> CompItem::getLayers();
    int NumLayers();
    float getCurrentTime();
    void setCurrentTime(float time);
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
    std::shared_ptr<LayerCollection> CompItem::getSelectedLayers();
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

    std::shared_ptr<Item>& operator[](std::size_t index) {
        if (index >= items_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return items_[index];
    }

    const std::shared_ptr<Item>& operator[](std::size_t index) const {
        if (index >= items_.size()) {
            throw std::out_of_range("Index out of range");
        }
        return items_[index];
    }

    // Iterator-related methods
    auto begin() { return items_.begin(); }
    auto end() { return items_.end(); }
    auto begin() const { return items_.cbegin(); }
    auto end() const { return items_.cend(); }
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

    std::vector<float> GetSolidColor(); //TODO
    void SetSolidColor(float red, float green, float blue, float alpha);  //TODO
    void SetSolidDimensions(float width, float height);  //TODO
};

class AdjustmentLayer : public Layer {
public:
	explicit AdjustmentLayer(const Result<AEGP_LayerH>& layerHandle) : Layer(layerHandle) {}
	virtual ~AdjustmentLayer() = default;

	static std::shared_ptr<AdjustmentLayer> createNew(std::shared_ptr<CompItem> compH, std::string name = "Adjustment Layer") {
        CompItem comp = *compH;
        Result<AEGP_ItemH> itemHandle = comp.getItemHandle();
        if (itemHandle.value == NULL) {
			throw std::runtime_error("No active item");
		}
        auto& compHOut = enqueueSyncTask(getCompFromItem, itemHandle);
        compHOut->wait();

        Result<AEGP_CompH> compH2 = compHOut->getResult();
		if (compH2.error != A_Err_NONE) {
			throw std::runtime_error("Failed to get comp from item");
		}
        if (compH2.value == NULL) {
            throw std::runtime_error("Failed to get comp from item");
        }

		auto& message = enqueueSyncTask(CreateSolidInComp, name, comp.getWidth(), comp.getHeight(), 0, 0, 0, 0, compH2, comp.getDuration());
		message->wait();

		Result<AEGP_LayerH> result = message->getResult();
		AEGP_LayerH layerH = result.value;
        if (layerH == NULL) {
			throw std::runtime_error("Failed to create new layer");
		}
		if (result.error != A_Err_NONE) {
			throw std::runtime_error("Failed to create new layer");
		}

        LayerFlag flag = LayerFlag::ADJUSTMENT_LAYER;
        auto& message2 = enqueueSyncTask(SetLayerFlag, result, flag, true);
        message2->wait();

		Result<void> result2 = message2->getResult();
		if (result2.error != A_Err_NONE) {
			throw std::runtime_error("Failed to set layer flag");
		}

        AdjustmentLayer layer(result);
        return std::make_shared<AdjustmentLayer>(layer);
	}
};