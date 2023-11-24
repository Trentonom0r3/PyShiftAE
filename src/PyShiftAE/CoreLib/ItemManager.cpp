#include "ItemManager.h"

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

/*
* This is really only useful for single frame grabs. Should not be ran in a loop.
* TODO: Add methods for accessing source files, then manipulating those. 
* This will be done in other classes.
*
*/
ImageData CompItem::frameAtTime(float time) {
	//get the active item
	auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
		return ImageData{};
	}
	//get the render options for the active item
	auto& message = enqueueSyncTask(getRenderOptions, item);
	message->wait();

	//get the result of the render options
	Result<AEGP_RenderOptionsH> result = message->getResult();

	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting render options");
		return ImageData{};
	}
	//set the time of the render options
	auto& message2 = enqueueSyncTask(setTime, result, time);
	message2->wait();

	Result<AEGP_RenderOptionsH> result2 = message2->getResult();

	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error setting time");
		return ImageData{};
	}
	//render and checkout the frame
	auto& message3 = enqueueSyncTask(renderAndCheckoutFrame, result2);
	message3->wait();

	Result<AEGP_FrameReceiptH> result3 = message3->getResult();

	if (result3.error != A_Err_NONE) {
		throw std::runtime_error("Error rendering and checking out frame");
		return ImageData{};
	}
	//get the world from the frame receipt
	auto& message4 = enqueueSyncTask(getReceiptWorld, result3);
	message4->wait();

	Result<AEGP_WorldH> result4 = message4->getResult();

	if (result4.error != A_Err_NONE) {
		throw std::runtime_error("Error getting receipt world");
		return ImageData{};
	}
	//get the base address of the world
	auto& message5 = enqueueSyncTask(getBaseAddr8, result4);
	message5->wait();

	Result<PF_Pixel8*> result5 = message5->getResult();

	if (result5.error != A_Err_NONE) {
		throw std::runtime_error("Error getting base address");
		return ImageData{};
	}
	//get the size of the world
	auto& message6 = enqueueSyncTask(getSize, result4);
	message6->wait();

	Result<size> result6 = message6->getResult();

	if (result6.error != A_Err_NONE) {
		throw std::runtime_error("Error getting size");
		return ImageData{};
	}
	//check in the frame
	auto& message7 = enqueueSyncTask(checkinFrame, result3);
	message7->wait();

	Result<void> result7 = message7->getResult();

	if (result7.error != A_Err_NONE) {
		throw std::runtime_error("Error checking in frame");
		return ImageData{};
	}
	//dispose the render options
	auto& message8 = enqueueSyncTask(disposeRenderOptions, result2);
	message8->wait();

	Result<void> result8 = message8->getResult();

	if (result8.error != A_Err_NONE) {
		throw std::runtime_error("Error disposing render options");
		return ImageData{};
	}
	// Determine the total size of the image data in bytes.
	size_t dataSize = result6.value.width * result6.value.height * 4;  // Assumes 4 bytes per pixel.

	// Create a vector to hold the image data.
	std::vector<uint8_t> img(dataSize);

	// Manually copy the data from baseAddr to img.
	std::memcpy(img.data(), result5.value, dataSize);

	// Convert ARGB (BGRA in memory on little-endian machines) to RGBA.

	for (size_t i = 0; i < dataSize; i += 4) {
		// Swap the blue (B) and the alpha (A).
		std::swap(img[i], img[i + 3]);
		// Swap the green (G) and blue (B).
		std::swap(img[i + 1], img[i + 2]);
	}

	// Create an ImageData object to hold the image data and dimensions.

	std::shared_ptr<std::vector<uint8_t>> img_data_ptr = std::make_shared<std::vector<uint8_t>>(std::move(img));

	ImageData image_data{ img_data_ptr, result6.value.width, result6.value.height, 4 };

	return image_data;  // Return the populated ImageData object.
}


std::vector<Layer> CompItem::getLayers()
{
	auto item = this->itemHandle_;
	if (item.value == NULL) {
		throw std::runtime_error("No active item");
		return std::vector<Layer>{};
	}
	auto& message = enqueueSyncTask(getCompFromItem, item);
	message->wait();

	Result<AEGP_CompH> result = message->getResult();

	if (result.error != A_Err_NONE) {
		throw std::runtime_error("Error getting comp from item");
		return std::vector<Layer>{};
	}

	auto& message2 = enqueueSyncTask(getNumLayers, result);
	message2->wait();

	Result<int> result2 = message2->getResult();

	if (result2.error != A_Err_NONE) {
		throw std::runtime_error("Error getting number of layers");
		return std::vector<Layer>{};
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
			return std::vector<Layer>{};
		}

		layers.push_back(Layer(result3));
	}

	return layers;

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
