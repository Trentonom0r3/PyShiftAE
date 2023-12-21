#include "ItemManager.h"

void Item::deleteItem()
{
	auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
	}
	auto& message = enqueueSyncTask(DeleteItem, item);
	message->wait();

	Result<void> result = message->getResult();
	return;
}

Result<AEGP_ItemH> Item::getItemHandle()
{
return this->itemHandle_;
}

bool Item::isSelected()
{
	auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
		return false;
	}
	auto& message = enqueueSyncTask(IsItemSelected, item);
	message->wait();

	Result<bool> result = message->getResult();
	bool isSelected = result.value;
	return isSelected;
}

void Item::setSelected(bool select)
{
	bool deselectOthers = false;
	auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
	}
	auto& message = enqueueSyncTask(SelectItem, item, select, deselectOthers);
	message->wait();

	Result<void> result = message->getResult();
	return;
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
	std::string name = result.value;
	return name;
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

	float width = result.value.width;
	return width;
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

	float height = result.value.height;
	return height;
}

float Item::getCurrentTime()
{
	auto item = this->itemHandle_;
	if (item.value == NULL) {
			throw std::runtime_error("No active item");
			return 0.0f;
		}
	auto& message = enqueueSyncTask(GetItemCurrentTime, item);
	message->wait();

	Result<float> result = message->getResult();

	if (result.error != A_Err_NONE) {
			throw std::runtime_error("Error getting item current time");
			return 0.0f;
		}

	float time = result.value;
	return time;
}

float Item::getDuration()
{
	auto item = this->itemHandle_;
	if (item.value == NULL) {
			throw std::runtime_error("No active item");
			return 0.0f;
		}
	auto& message = enqueueSyncTask(GetItemDuration, item);
	message->wait();

	Result<float> result = message->getResult();

	if (result.error != A_Err_NONE) {
			throw std::runtime_error("Error getting item duration");
			return 0.0f;
		}

	float time = result.value;
	return time;
}

void Item::setCurrentTime(float time)
{
	auto item = this->itemHandle_;
	if (item.value == NULL) {
			throw std::runtime_error("No active item");
		}
	auto& message = enqueueSyncTask(SetItemCurrentTime, item, time);
	message->wait();

	Result<void> result = message->getResult();

	if (result.error != A_Err_NONE) {
			throw std::runtime_error("Error setting item current time");
		}
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

	float time = result2.value;
	return time;
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

	float time = result2.value;
	return time;
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

	std::shared_ptr<Layer> layer = std::make_shared<Layer>(result2);
	return layer;
}

std::shared_ptr<LayerCollection> CompItem::getSelectedLayers()
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

	auto& message2 = enqueueSyncTask(GetNewCollectionFromCompSelection, result);
	message2->wait();

	Result<std::vector<Result<AEGP_LayerH>>> result2 = message2->getResult();

	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error getting selected layers");
	}

	std::vector<std::shared_ptr<Layer>> layers;	

	for (int i = 0; i < result2.value.size(); i++) {
		Layer layer = Layer(result2.value[i]);
		std::shared_ptr<Layer> layerPtr = std::make_shared<Layer>(layer);
		layers.push_back(layerPtr);
	}

	std::shared_ptr<LayerCollection> layerCollection = std::make_shared<LayerCollection>(result, layers);
	return layerCollection;
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

	std::vector<std::shared_ptr<Layer>> layers;

	for (int i = 0; i < numLayers; i++) { 
		auto index = i;
		auto& message3 = enqueueSyncTask(getLayerFromComp, result, index);
		message3->wait();

		Result<AEGP_LayerH> result3 = message3->getResult();

		if (result3.error != A_Err_NONE) {
			throw std::runtime_error("Error getting layer from comp");
		}
		std::shared_ptr<Layer> layer = std::make_shared<Layer>(result3);
		layers.push_back(layer);
	}

	std::shared_ptr<LayerCollection> layerCollection = std::make_shared<LayerCollection>(result, layers);
	return layerCollection;

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

	int numLayers = result2.value;
	return numLayers;
}

float CompItem::getCurrentTime()
{
	auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
		return 0.0f;
	}

	auto& compH = enqueueSyncTask(getCompFromItem, item);
	compH->wait();

	Result<AEGP_CompH> resultZ = compH->getResult();

	auto& frameRate = enqueueSyncTask(GetCompFramerate, resultZ);
	frameRate->wait();

	Result<float> result = frameRate->getResult();

	auto& message = enqueueSyncTask(GetCompItemCurrentTime, item, result.value);
	message->wait();

	Result<float> resultH = message->getResult();

	if (resultH.error != A_Err_NONE) {
		throw std::runtime_error("Error getting current time");
		return 0.0f;
	}

	float time = resultH.value;
	return time;
}

void CompItem::setCurrentTime(float time)
{
	auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
	}

	auto& compH = enqueueSyncTask(getCompFromItem, item);
	compH->wait();

	Result<AEGP_CompH> resultZ = compH->getResult();

	auto& frameRate = enqueueSyncTask(GetCompFramerate, resultZ);
	frameRate->wait();

	Result<float> result = frameRate->getResult();

	auto& message = enqueueSyncTask(SetCompItemCurrentTime, item, time, result.value);
	message->wait();

	Result<void> resultH = message->getResult();

	if (resultH.error != A_Err_NONE) {
		throw std::runtime_error("Error setting current time");
		return;
	}
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

	Result<std::string> result = message->getResult();
	std::string	name = result.value;
	if (name == "") {
		auto& message2 = enqueueSyncTask(getLayerSourceName, layer);
		message2->wait();

		Result<std::string> result2 = message2->getResult();
		std::string	name2 = result2.value;
		return name2;
	}
	return name;
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
	std::string	name = result.value;
	return name;
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
	int index = result.value;
	return index;
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

	std::shared_ptr<FootageItem> footageItem = std::make_shared<FootageItem>(result2);	
	return footageItem;
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
	float time = result.value;
	return time;
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
	float time = result.value;
	return time;
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
	float time = result.value;
	return time;
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
	float time = result.value;
	return time;
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
	float time = result.value;
	return time;
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
	float time = result.value;
	return time;
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
	std::string quality = result.value;
	return quality;
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
	float offset = result.value;
	return offset;
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

void Layer::addEffect(std::shared_ptr<CustomEffect>  effect)
{
	if (effect == NULL) {
		throw std::invalid_argument("Invalid effect");
		return;
	}
	void* effectPtr = reinterpret_cast<void*>(effect.get());
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
	}

	auto& message = enqueueSyncTask(GetNumInstalledEffects);
	message->wait();

	Result<int> result = message->getResult();

	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting number of installed effects");
		return;
	}

	int numEffects = result.value;
	Result<AEGP_InstalledEffectKey> effectKey;
	effectKey.value = 0;
	effectKey.error = A_Err_NONE;
	for (int i = 0; i < numEffects; i++) {
		auto index = i;
		auto& getEffect = enqueueSyncTask(GetNextInstalledEffect, effectKey);
		getEffect->wait();

		Result<AEGP_InstalledEffectKey> nextKey = getEffect->getResult();

		if (nextKey.error != A_Err_NONE) {
			throw std::runtime_error("Error getting next installed effect");
			return;
		}

		effectKey = nextKey;

		auto& getEffectName = enqueueSyncTask(GetEffectMatchName, effectKey);
		getEffectName->wait();

		Result<std::string> effectName = getEffectName->getResult();

		if (effectName.error != A_Err_NONE) {
			throw std::runtime_error("Error getting effect name");
		}

		if (effectName.value == "ADBE Skeleton") {
			auto& addEffect = enqueueSyncTask(ApplyEffect, layer, effectKey);
			addEffect->wait();

			Result<AEGP_EffectRefH> effectRef = addEffect->getResult();

			if (effectRef.error != A_Err_NONE) {
				throw std::runtime_error("Error adding effect");
			}

			auto& sendEffect = enqueueSyncTask(EffectCallGeneric, effectRef, effectPtr);
			sendEffect->wait();

			Result<void> result = sendEffect->getResult();

			if (result.error != A_Err_NONE) {
				throw std::runtime_error("Error sending effect");
			}
		}
	}


}

std::shared_ptr<Item> Layer::getSource()
{
	//Result<AEGP_ItemH> getLayerSourceItem(Result<AEGP_LayerH> layerH);
	auto layer = this->layerHandle_;
	if (layer.value == NULL) {
		throw std::runtime_error("No active layer");
	}
	auto& message = enqueueSyncTask(getLayerSourceItem, layer);
	message->wait();

	Result<AEGP_ItemH> result = message->getResult();

	//get item type
	auto& message2 = enqueueSyncTask(getItemType, result);
	message2->wait();

	Result<AEGP_ItemType> result2 = message2->getResult();

	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error getting item type");
	}

	if (result2.value == AEGP_ItemType_FOOTAGE) {
		std::shared_ptr<FootageItem> footageItem = std::make_shared<FootageItem>(result);
		return footageItem;
	}
	else if (result2.value == AEGP_ItemType_COMP) {
		std::shared_ptr<CompItem> compItem = std::make_shared<CompItem>(result);
		return compItem;
	}
	else if (result2.value == AEGP_ItemType_FOLDER) {
		std::shared_ptr<FolderItem> folderItem = std::make_shared<FolderItem>(result);
		return folderItem;
	}
	else {
		std::shared_ptr<Item> item = std::make_shared<Item>(result);
		return item;
	}
}


std::shared_ptr<ItemCollection> FolderItem::ChildItems()
{
	auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
	}

	Result<AEGP_ItemH> itemH = item;
	ItemCollection collection(itemH);

	std::shared_ptr<ItemCollection> itemCollection = std::make_shared<ItemCollection>(collection);
	return itemCollection;

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

	layer->deleteLayer();

}


std::string LayerCollection::getCompName() 
{
	auto comp = this->compHandle_;
	if (comp.value == NULL) {
		throw std::runtime_error("No active comp");
		return std::string{};
	}
	auto& compmessage = enqueueSyncTask(GetItemFromComp, comp);
	compmessage->wait();

	Result<AEGP_ItemH> item = compmessage->getResult();

	if (item.error != A_Err_NONE) {
	throw std::runtime_error("Error getting item from comp");
	return std::string{};
	}

	auto& message = enqueueSyncTask(getItemName, item);
	message->wait();

	Result<std::string> result = message->getResult();

	std::string info = "Layers for Composition: " + result.value;

	return info;

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

	std::shared_ptr<Layer> layer = std::make_shared<Layer>(newLayer);
	return layer;
}

std::vector<std::shared_ptr<Item>> ItemCollection::getItems()
{
	std::vector<std::shared_ptr<Item>> items;
	auto item = this->itemHandle_;

	Result<AEGP_ItemH> itemhandle = item;

	// Check if the current folder item is valid
	if (itemhandle.value == NULL) {
		throw std::runtime_error("No active item");
	}
	
	auto& projectMessage = enqueueSyncTask(getProject);
	projectMessage->wait();

	Result<AEGP_ProjectH> projectResult = projectMessage->getResult();

	Result<AEGP_ItemH> currentItemResult = itemhandle;

	while (currentItemResult.value != NULL) {
		// Check if the current item is a child of the folder
		auto& message = enqueueSyncTask(GetItemParentFolder, currentItemResult);
		message->wait();

		Result<AEGP_ItemH> parentItemH = message->getResult();

		if (parentItemH.value == itemhandle.value) {
			// Get the type of the current item
			auto& typeMessage = enqueueSyncTask(getItemType, currentItemResult);
			typeMessage->wait();
			Result<AEGP_ItemType> itemTypeResult = typeMessage->getResult();
			if (itemTypeResult.error != A_Err_NONE) {
				throw std::runtime_error("Error getting item type");
			}

			// Add the current item to the list based on its type
			switch (itemTypeResult.value) {
			case AEGP_ItemType_FOLDER:
				items.push_back(std::make_shared<FolderItem>(currentItemResult));
				break;
			case AEGP_ItemType_FOOTAGE:
				items.push_back(std::make_shared<FootageItem>(currentItemResult));
				break;
			case AEGP_ItemType_COMP:
				items.push_back(std::make_shared<CompItem>(currentItemResult));
				break;
				// Handle other types as needed
			}
		}

		// Get the next project item
		auto& nextItemMessage = enqueueSyncTask(GetNextProjItem, projectResult, currentItemResult);
		nextItemMessage->wait();
		currentItemResult = nextItemMessage->getResult();
		if (currentItemResult.error != A_Err_NONE) {
			throw std::runtime_error("Error getting next project item");
		}
	}

	return items;
}

std::string FootageItem::getPath()
{
	auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
		return std::string{};
	}

	auto& curTime = enqueueSyncTask(GetItemCurrentTime, item);
	curTime->wait();

	Result<float> time = curTime->getResult();

	if (time.error != A_Err_NONE) {
		throw std::runtime_error("Error getting current time");
		return std::string{};
	}

	auto& message = enqueueSyncTask(GetMainFootageFromItem, item);
	message->wait();

	Result<AEGP_FootageH> result = message->getResult();

	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting main footage from item");
		return std::string{};
	}

	auto& message2 = enqueueSyncTask(GetFootagePath, result, time.value, AEGP_FOOTAGE_MAIN_FILE_INDEX);
	message2->wait();

	Result<std::string> result2 = message2->getResult();

	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error getting footage path");
		return std::string{};
	}

	std::string result2String = result2.value;
	return result2String;


}
