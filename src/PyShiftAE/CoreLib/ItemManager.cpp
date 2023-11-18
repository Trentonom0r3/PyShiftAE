#include "ItemManager.h"

std::string Item::getName() {
	auto item = this->itemHandle_;
	auto& message = enqueueSyncTask(getItemName, item);
	message->wait();

	Result<std::string> result = message->getResult();
	return result.value;
}

void Item::setName(std::string name) {
	auto item = this->itemHandle_;
	auto& message = enqueueSyncTask(setItemName, item, name);
	message->wait();

	Result<void> result = message->getResult();
}