#include "ItemManager.h"

Result<AEGP_ItemH> Item::getItemHandle()
{
return this->itemHandle_;
}

std::string Item::getName() {
	auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
		return std::string{};
	}
	auto& message = enqueueSyncTask(getItemName, item);
	message->wait();

	Result<std::string> result = message->getResult();
	return result.value;
}

void Item::setName(std::string name) {
	auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
	}
	auto& message = enqueueSyncTask(setItemName, item, name);
	message->wait();

	Result<void> result = message->getResult();
}

float Item::getWidth()
{
	auto item = this->itemHandle_;
	if (item.value == NULL) {
			throw std::runtime_error("No active item");
			return 0.0f;
		}
		auto& message = enqueueSyncTask(GetItemDimensions, item);
	message->wait();

	Result<size> result = message->getResult();

	if (result.error != A_Err_NONE) {
			throw std::runtime_error("Error getting item dimensions");
			return 0.0f;
		}

	return result.value.width;
}

float Item::getHeight()
{
	auto item = this->itemHandle_;
	if (item.value == NULL) {
			throw std::runtime_error("No active item");
			return 0.0f;
		}
		auto& message = enqueueSyncTask(GetItemDimensions, item);
	message->wait();

	Result<size> result = message->getResult();

	if (result.error != A_Err_NONE) {
			throw std::runtime_error("Error getting item dimensions");
			return 0.0f;
		}

	return result.value.height;
}

float CompItem::getFrameRate()
{
auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
		return 0;
	}
	auto& message = enqueueSyncTask(getCompFromItem, item);
	message->wait();

	Result<AEGP_CompH> result = message->getResult();

	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting comp from item");
		return 0;
	}

	auto& message2 = enqueueSyncTask(GetCompFramerate, result);
	message2->wait();

	Result<float> result2 = message2->getResult();

	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error getting frame rate");
		return 0;
	}

	return result2.value;
}

void CompItem::setFrameRate(float frameRate)
{
auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
	}
	auto& message = enqueueSyncTask(getCompFromItem, item);
	message->wait();

	Result<AEGP_CompH> result = message->getResult();

	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting comp from item");
		return;
	}

	auto& message2 = enqueueSyncTask(SetCompFramerate, result, frameRate);
	message2->wait();

	Result<void> result2 = message2->getResult();

	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error setting frame rate");
		return;
	}
}

float CompItem::getDuration()
{
auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
		return 0;
	}
	auto& message = enqueueSyncTask(getCompFromItem, item);
	message->wait();

	Result<AEGP_CompH> result = message->getResult();

	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting comp from item");
		return 0;
	}

	auto& message2 = enqueueSyncTask(GetCompWorkAreaDuration, result);
	message2->wait();

	Result<float> result2 = message2->getResult();

	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error getting duration");
		return 0;
	}

	return result2.value;
}

void CompItem::setDuration(float duration)
{
auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
	}
	auto& message = enqueueSyncTask(getCompFromItem, item);
	message->wait();

	Result<AEGP_CompH> result = message->getResult();

	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting comp from item");
		return;
	}

	auto& message2 = enqueueSyncTask(SetCompDuration, result, duration);
	message2->wait();

	Result<void> result2 = message2->getResult();

	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error setting duration");
		return;
	}
}

void CompItem::setWidth(float width)
{
auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
	}
	auto& message = enqueueSyncTask(getCompFromItem, item);
	message->wait();

	Result<AEGP_CompH> result = message->getResult();

	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting comp from item");
		return;
	}

	float height = this->getHeight();
	auto& message2 = enqueueSyncTask(SetCompDimensions, result, width, height);
	message2->wait();

	Result<void> result2 = message2->getResult();

	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error setting width");
		return;
	}
}

void CompItem::setHeight(float height)
{
auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
	}
	auto& message = enqueueSyncTask(getCompFromItem, item);
	message->wait();

	Result<AEGP_CompH> result = message->getResult();

	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting comp from item");
		return;
	}

	auto& message2 = enqueueSyncTask(SetCompDimensions, result, this->getWidth(), height);
	message2->wait();

	Result<void> result2 = message2->getResult();

	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error setting height");
		return;
	}
}

std::shared_ptr<Layer> CompItem::newSolid(std::string name, float width, float height,float red, float green, float blue, float alpha,
						float duration)
{
	auto item = this->itemHandle_;

	auto& message = enqueueSyncTask(getCompFromItem, item);
	message->wait();

	Result<AEGP_CompH> result = message->getResult();

	auto& message2 = enqueueSyncTask(CreateSolidInComp, name, width, height, red, green, blue, alpha, result, duration);
	message2->wait();

	Result<AEGP_LayerH> result2 = message2->getResult();

	return std::make_shared<Layer>(result2);
}

std::shared_ptr<LayerCollection> CompItem::getLayers()
{
	auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
	}
	auto& message = enqueueSyncTask(getCompFromItem, item);
	message->wait();

	Result<AEGP_CompH> result = message->getResult();

	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting comp from item");
	}

	auto& message2 = enqueueSyncTask(getNumLayers, result);
	message2->wait();

	Result<int> result2 = message2->getResult();

	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error getting number of layers");
	}

	int numLayers = result2.value;

	std::vector<Layer> layers;

	for (int i = 0; i < numLayers; i++) { 
		auto index = i;
		auto& message3 = enqueueSyncTask(getLayerFromComp, result, index);
		message3->wait();

		Result<AEGP_LayerH> result3 = message3->getResult();

		if (result3.error != A_Err_NONE) {
			throw std::runtime_error("Error getting layer from comp");
		}

		layers.push_back(Layer(result3));
	}

	return std::make_shared<LayerCollection>(result, layers);

}


int CompItem::NumLayers() {
	auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
		return 0;
	}
	auto& message = enqueueSyncTask(getCompFromItem, item);
	message->wait();

	Result<AEGP_CompH> result = message->getResult();

	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting comp from item");
		return 0;
	}

	auto& message2 = enqueueSyncTask(getNumLayers, result);
	message2->wait();

	Result<int> result2 = message2->getResult();

	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error getting number of layers");
		return 0;
	}

	return result2.value;
}

void CompItem::addLayer(std::string name, std::string path, int index)
{
	auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
	}

	auto& getComp = enqueueSyncTask(getCompFromItem, item);
	getComp->wait();

	Result<AEGP_CompH> compH = getComp->getResult();

	auto& createfootage = enqueueSyncTask(createFootage, path);
	createfootage->wait();

	Result<AEGP_FootageH> createFootage = createfootage->getResult();

	auto& getroot = enqueueSyncTask(getProjectRootFolder);
	getroot->wait();

	Result<AEGP_ItemH> Root = getroot->getResult();

	auto& addtoproj = enqueueSyncTask(addFootageToProject, createFootage, Root);
	addtoproj->wait();

	Result<AEGP_ItemH> footageItem = addtoproj->getResult();

	auto& isValid = enqueueSyncTask(isAddLayerValid, footageItem, compH);
	isValid->wait();

	Result<bool> isvalid = isValid->getResult();

	if (isvalid.value == false) {
		throw std::runtime_error("Error adding layer");
		return;
	}

	if (isvalid.error != A_Err_NONE) {
		throw std::runtime_error("Error adding layer");
		return;
	}

	auto& addedLayer = enqueueSyncTask(AddLayer, footageItem, compH);
	addedLayer->wait();

	Result<AEGP_LayerH> newLayer = addedLayer->getResult();

	if (newLayer.error != A_Err_NONE) {
		throw std::runtime_error("Error adding layer");
		return;
	}

	auto& changeName = enqueueSyncTask(setLayerName, newLayer, name);
	changeName->wait();

	Result<void> result = changeName->getResult();

	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error changing layer name");
		return;
	}

	if (int(index) != -1) {
		auto& message6 = enqueueSyncTask(changeLayerIndex, newLayer, index);
		message6->wait();

		Result<void> result6 = message6->getResult();

		if (result6.error != A_Err_NONE) {
			throw std::runtime_error("Error changing layer index");
			return;
		}
	}
}

std::string Layer::GetLayerName()
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
		return std::string{};
	}
	auto& message = enqueueSyncTask(getLayerName, layer);
	message->wait();

	//try to get the layer flags, just as a test
	auto& message2 = enqueueSyncTask(GetLayerFlags, layer);
	message2->wait();

	Result<AEGP_LayerFlags> result2 = message2->getResult();

	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error getting layer flags");
		return std::string{};
	}
	AEGP_LayerFlags flags = result2.value;

	Result<std::string> result = message->getResult();
	return result.value;
}

std::string Layer::GetSourceName()
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
		return std::string{};
	}
	auto& message = enqueueSyncTask(getLayerSourceName, layer);
	message->wait();

	Result<std::string> result = message->getResult();
	return result.value;
}

void Layer::SetLayerName(std::string name)
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
	}
	auto& message = enqueueSyncTask(setLayerName, layer, name);
	message->wait();

	Result<void> result = message->getResult();
	return;
}

int Layer::index()
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
		return 0;
	}
	auto& message = enqueueSyncTask(getLayerIndex, layer);
	message->wait();

	Result<int> result = message->getResult();
	return result.value;
}

//Swaps the index of the layer with the layer at the given index
void Layer::changeIndex(int index)
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
	}
	auto& message = enqueueSyncTask(changeLayerIndex, layer, index);
	message->wait();

	Result<void> result = message->getResult();
	return;
}

std::shared_ptr<FootageItem> Layer::duplicate()
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
	}
	auto& message = enqueueSyncTask(DuplicateLayer, layer);
	message->wait();

	Result<AEGP_LayerH> result = message->getResult();

	auto& message2 = enqueueSyncTask(getLayerSourceItem, result);
	message2->wait();

	Result<AEGP_ItemH> result2 = message2->getResult();

	return std::make_shared<FootageItem>(result2);
}


float Layer::layerTime()
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
		return 0.0f;
	}
	AEGP_LTimeMode time_mode = AEGP_LTimeMode_LayerTime;
	auto& message = enqueueSyncTask(GetLayerCurrentTime, layer, time_mode);
	message->wait();

	Result<float> result = message->getResult();
	return result.value;
}

float Layer::layerCompTime()
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
		return 0.0f;
	}
	AEGP_LTimeMode time_mode = AEGP_LTimeMode_CompTime;
	auto& message = enqueueSyncTask(GetLayerCurrentTime, layer, time_mode);
	message->wait();

	Result<float> result = message->getResult();
	return result.value;
}

float Layer::inPoint()
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
		return 0.0f;
	}
	AEGP_LTimeMode time_mode = AEGP_LTimeMode_LayerTime;
	auto& message = enqueueSyncTask(GetLayerInPoint, layer, time_mode);
	message->wait();

	Result<float> result = message->getResult();
	return result.value;
}

float Layer::compInPoint()
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
		return 0.0f;
	}
	AEGP_LTimeMode time_mode = AEGP_LTimeMode_CompTime;
	auto& message = enqueueSyncTask(GetLayerInPoint, layer, time_mode);
	message->wait();

	Result<float> result = message->getResult();
	return result.value;
}

float Layer::duration()
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
		return 0.0f;
	}
	AEGP_LTimeMode time_mode = AEGP_LTimeMode_LayerTime;
	auto& message = enqueueSyncTask(GetLayerDuration, layer, time_mode);
	message->wait();

	Result<float> result = message->getResult();
	return result.value;
}

float Layer::compDuration()
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
		return 0.0f;
	}
	AEGP_LTimeMode time_mode = AEGP_LTimeMode_CompTime;
	auto& message = enqueueSyncTask(GetLayerDuration, layer, time_mode);
	message->wait();

	Result<float> result = message->getResult();
	return result.value;
}

std::string Layer::getQuality()
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
		return std::string{};
	}
	auto& message = enqueueSyncTask(GetLayerQuality, layer);
	message->wait();

	Result<std::string> result = message->getResult();
	return result.value;
}

void Layer::setQuality(int quality)
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
	}
	if (quality != AEGP_LayerQual_NONE &&
		quality != AEGP_LayerQual_WIREFRAME &&
		quality != AEGP_LayerQual_DRAFT &&
		quality != AEGP_LayerQual_BEST) {
		throw std::invalid_argument("Invalid quality value");
	}
	auto& message = enqueueSyncTask(SetLayerQuality, layer, quality);
	message->wait();

	Result<void> result = message->getResult();
	return;
}

void Layer::deleteLayer()
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
	}
	auto& message = enqueueSyncTask(DeleteLayer, layer);
	message->wait();

	Result<void> result = message->getResult();
	return;
}

float Layer::getOffset()
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
		return 0;
	}
	auto& message = enqueueSyncTask(GetLayerOffset, layer);
	message->wait();

	Result<float> result = message->getResult();
	return result.value;
}

void Layer::setOffset(float offset)
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
	}
	// get parent comp
	auto& message = enqueueSyncTask(GetLayerParentComp, layer);
	message->wait();

	Result<AEGP_CompH> result = message->getResult();

	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting parent comp");
		return;
	}

	//get comp frame rate
	auto& message2 = enqueueSyncTask(GetCompFramerate, result);
	message2->wait();

	Result<float> result2 = message2->getResult();

	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error getting frame rate");
		return;
	}

	float frameRate = result2.value;
	auto& fmessage = enqueueSyncTask(SetLayerOffset, layer, offset, frameRate);
	fmessage->wait();

	Result<void> fresult = fmessage->getResult();
	return;
}

void Layer::setFlag(LayerFlag flag, bool value)
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
	}
	auto& message = enqueueSyncTask(SetLayerFlag, layer, flag, value);
	message->wait();

	Result<void> result = message->getResult();
	return;
}

bool Layer::getFlag(LayerFlag specificFlag)
{
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
		return false;
	}

	auto& message = enqueueSyncTask(GetLayerFlags, layer);
	message->wait();

	Result<AEGP_LayerFlags> result = message->getResult();

	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting layer flags");
		return false;
	}

	AEGP_LayerFlags combinedFlags = result.value;

	// Check if the specific flag is set in the combined flags
	return (combinedFlags & specificFlag) != 0;
}

Result<AEGP_LayerH> Layer::getLayerHandle()
{
return this->layerHandle_;
}


void FolderItem::addFolder(std::string name)
{
	auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
	}
	auto& message = enqueueSyncTask(createFolderItem, name, item);
	message->wait();

	Result<void> result = message->getResult();
	return;
}

void LayerCollection::removeLayerFromCollection(Layer layerHandle)
{
	auto& layer = layerHandle.getLayerHandle();
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
	}
	layerHandle.deleteLayer();
}

void LayerCollection::RemoveLayerByIndex(int index)
{
	auto& layers = this->layers_;
	if (index < 0 || index >= layers.size()) {
		throw std::invalid_argument("Invalid index");
	}
	auto& layer = layers[index];

	layer.deleteLayer();

}

std::vector<Layer> LayerCollection::getAllLayers() {
	return this->layers_;
}

std::shared_ptr<Layer> LayerCollection::addLayerToCollection(Item itemHandle, int index) {
	auto result = itemHandle.getItemHandle();
	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting item handle");
		return std::make_shared<Layer>(Result<AEGP_LayerH>{});
	}

	auto comp = this->compHandle_;
	Result<AEGP_CompH> compH = comp;
	if (compH.value == NULL) {
		throw std::runtime_error("No active comp");
		return std::make_shared<Layer>(Result<AEGP_LayerH>{});
	}


	if (compH.error != A_Err_NONE) {
		throw std::runtime_error("Error getting comp handle");
		return std::make_shared<Layer>(Result<AEGP_LayerH>{});
	}

	auto& isValid = enqueueSyncTask(isAddLayerValid, result, compH);
	isValid->wait();

	Result<bool> isvalid = isValid->getResult();

	if (isvalid.value == false) {
		throw std::runtime_error("Error adding layer");
		return std::make_shared<Layer>(Result<AEGP_LayerH>{});
	}

	if (isvalid.error != A_Err_NONE) {
		throw std::runtime_error("Error adding layer");
		return std::make_shared<Layer>(Result<AEGP_LayerH>{});
	}

	auto& addedLayer = enqueueSyncTask(AddLayer, result, compH);

	addedLayer->wait();

	Result<AEGP_LayerH> newLayer = addedLayer->getResult();

	if (newLayer.error != A_Err_NONE) {
		throw std::runtime_error("Error adding layer");
		return std::make_shared<Layer>(Result<AEGP_LayerH>{});
	}

	if (int(index) != -1) {
		auto& message6 = enqueueSyncTask(changeLayerIndex, newLayer, index);
		message6->wait();

		Result<void> result6 = message6->getResult();

		if (result6.error != A_Err_NONE) {
			throw std::runtime_error("Error changing layer index");
			return std::make_shared<Layer>(Result<AEGP_LayerH>{});
		}
	}

	return std::make_shared<Layer>(newLayer);
}
