#include "App.h"

App::App() {
	Project project;
}

Project App::getProject() {
	return this->project;
}

void App::beginUndoGroup(std::string undoName) {
	auto& message = enqueueSyncTask(StartUndoGroup, undoName);
	message->wait();

	Result<void> result = message->getResult();
}

void App::endUndoGroup() {
	auto& message = enqueueSyncTask(EndUndoGroup);
	message->wait();

	Result<void> result = message->getResult();
}

void App::executeCommand(int commandId)
{
	auto& message = enqueueSyncTask(ExecuteCommand, commandId);
	message->wait();

	Result<void> result = message->getResult();
}

void App::reportInfo(std::string info) {
	auto& message = enqueueSyncTask(ReportInfo, info);
	message->wait();

	Result<void> result = message->getResult();
}