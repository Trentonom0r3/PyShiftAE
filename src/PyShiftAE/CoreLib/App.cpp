#include "App.h"


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

void App::reportInfo(std::string info) { //should modify to accept string, int, float, etc.
	auto& message = enqueueSyncTask(ReportInfo, info);
	message->wait();

	Result<void> result = message->getResult();
}

std::string App::pluginPaths()
{
	auto& message = enqueueSyncTask(getPluginPaths);
	message->wait();

	Result<std::string> result = message->getResult();

	std::string resultString = result.value;
	return resultString;
}

PanelatorUI::PanelatorUI(SPBasicSuite* spbP, AEGP_PanelH panelH,
	AEGP_PlatformViewRef platformWindowRef,
	AEGP_PanelFunctions1* outFunctionTable)
	: i_refH(platformWindowRef), i_panelH(panelH),
	red(0), green(0), blue(0), i_use_bg(true),
	i_appSuite(spbP), i_panelSuite(spbP), i_numClicks(0)

{
	outFunctionTable->DoFlyoutCommand = S_DoFlyoutCommand;
	outFunctionTable->GetSnapSizes = S_GetSnapSizes;
	outFunctionTable->PopulateFlyout = S_PopulateFlyout;
}


void PanelatorUI::GetSnapSizes(A_LPoint* snapSizes, A_long* numSizesP)
{
	snapSizes[0].x = 100;
	snapSizes[0].y = 100;
	snapSizes[1].x = 200;
	snapSizes[1].y = 400;
	*numSizesP = 2;
}

void PanelatorUI::PopulateFlyout(AEGP_FlyoutMenuItem* itemsP, A_long* in_out_numItemsP) {
	// Dynamically allocate memory for myMenu based on the size of i_panel->flyoutMenu.items

}


void	PanelatorUI::DoFlyoutCommand(AEGP_FlyoutMenuCmdID commandID)
{
	InvalidateAll();
}

A_Err	PanelatorUI::S_GetSnapSizes(AEGP_PanelRefcon refcon, A_LPoint* snapSizes, A_long* numSizesP)
{
	PT_XTE_START{
		reinterpret_cast<PanelatorUI*>(refcon)->GetSnapSizes(snapSizes, numSizesP);
	} PT_XTE_CATCH_RETURN_ERR;
}

A_Err	PanelatorUI::S_PopulateFlyout(AEGP_PanelRefcon refcon, AEGP_FlyoutMenuItem* itemsP, A_long* in_out_numItemsP)
{
	PT_XTE_START{
		reinterpret_cast<PanelatorUI*>(refcon)->PopulateFlyout(itemsP, in_out_numItemsP);
	} PT_XTE_CATCH_RETURN_ERR;
}

A_Err	PanelatorUI::S_DoFlyoutCommand(AEGP_PanelRefcon refcon, AEGP_FlyoutMenuCmdID commandID)
{
	PT_XTE_START{
		reinterpret_cast<PanelatorUI*>(refcon)->DoFlyoutCommand(commandID);
	} PT_XTE_CATCH_RETURN_ERR;
}

PanelatorUI_Plat::PanelatorUI_Plat(SPBasicSuite* spbP, AEGP_PanelH panelH,
	AEGP_PlatformViewRef platformWindowRef,
	AEGP_PanelFunctions1* outFunctionTable)
	: PanelatorUI(spbP, panelH, platformWindowRef, outFunctionTable),
	i_prevWindowProc(nullptr),
	hWnd(static_cast<HWND>(platformWindowRef)) {

	i_prevWindowProc = reinterpret_cast<WindowProc>(GetWindowLongPtr(hWnd, GWLP_WNDPROC));
	SetWindowLongPtrA(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(StaticOSWindowWndProc));
	SetProp(hWnd, "PanelatorUI_PlatPtr", this);

	// Initialize the Python GUI
	pyGUI = CreateWindowEx(0, "EDIT", "",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
		0, 0, 100, 100, hWnd, nullptr, nullptr, nullptr);

	// Set up the font, colors, and initial text for the Python GUI
	InitializePyGUI();
}

void PanelatorUI_Plat::InitializePyGUI() {
	HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, "Courier New");
	SendMessage(pyGUI, WM_SETFONT, (WPARAM)hFont, TRUE);
	std::string initialInfo = "Python GUI Initialized: Welcome to PyShiftAE! \r\n"
		                      "All Python output will be displayed here. \r\n";
	SendMessage(pyGUI, WM_SETTEXT, 0, (LPARAM)initialInfo.c_str());
	HDC hdcEdit = GetDC(pyGUI);
	SendMessage(hWnd, WM_CTLCOLOREDIT, reinterpret_cast<WPARAM>(hdcEdit), reinterpret_cast<LPARAM>(pyGUI));
	ReleaseDC(pyGUI, hdcEdit);
}


LRESULT CALLBACK PanelatorUI_Plat::StaticOSWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PanelatorUI_Plat* platPtr = reinterpret_cast<PanelatorUI_Plat*>(GetProp(hWnd, "PanelatorUI_PlatPtr"));
	if (platPtr) {
		return platPtr->OSWindowWndProc(hWnd, message, wParam, lParam);
	}
	else {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

LRESULT PanelatorUI_Plat::OSWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PanelatorUI_Plat* platPtr = reinterpret_cast<PanelatorUI_Plat*>(GetProp(hWnd, "PanelatorUI_PlatPtr"));
	switch (message) {
	case WM_SIZE: {
		RECT rect;
		GetClientRect(hWnd, &rect);
		SetWindowPos(pyGUI, HWND_TOP, 0, 0, rect.right, rect.bottom, SWP_NOMOVE);
		return 0;
	}
	case WM_UPDATE_DEBUG_CONSOLE: {
		const char* debugMessage = reinterpret_cast<const char*>(lParam);
		std::string formattedMessage = debugMessage;

		// Get the current length of the text
		int length = GetWindowTextLength(platPtr->pyGUI);

		// Ensure the new message ends with a newline
		if (!formattedMessage.empty() && formattedMessage.back() != '\r\n') {
			formattedMessage += "\r\n";
		}

		// Check if the current content in pyGUI ends with a newline
		// If not, append a newline character
		SendMessage(platPtr->pyGUI, EM_SETSEL, (WPARAM)length, (LPARAM)length); // Move caret to the end
		SendMessage(platPtr->pyGUI, EM_REPLACESEL, 0, (LPARAM)"\r\n");

		// Now, move the caret to the new line, after the newly inserted newline
		length = GetWindowTextLength(platPtr->pyGUI); // Update the length after adding newline
		SendMessage(platPtr->pyGUI, EM_SETSEL, (WPARAM)length, (LPARAM)length); // Move caret to the new line

		// Append the formatted message to the edit control
		SendMessage(platPtr->pyGUI, EM_REPLACESEL, 0, (LPARAM)formattedMessage.c_str());

		// Auto-scroll to the bottom
		SendMessage(platPtr->pyGUI, WM_VSCROLL, SB_BOTTOM, 0);

		return 0; // Indicate message handled
	}

	case WM_CTLCOLOREDIT: {
		if ((HWND)lParam == pyGUI) {
			HDC hdcEdit = (HDC)wParam;
			SetTextColor(hdcEdit, RGB(255, 255, 255)); // White text
			SetBkColor(hdcEdit, RGB(0, 0, 0));         // Black background

			static HBRUSH hbrBkgnd = CreateSolidBrush(RGB(0, 0, 0));
			return (INT_PTR)hbrBkgnd;
		}
	}	// ... Handle other messages like WM_PAINT if necessary ...
	}

	if (i_prevWindowProc) {
		return CallWindowProc(i_prevWindowProc, hWnd, message, wParam, lParam);
	}
	else {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

void PanelatorUI_Plat::InvalidateAll() {
	InvalidateRect(hWnd, nullptr, FALSE);
}


static char debugMessageBuffer[1024];

Result<void> writeToDebugPanel(std::string message) {
	PanelatorUI_Plat* myPanel = PanelatorUI_Plat::GetInstance();
	HWND hWnd = myPanel ? myPanel->GetHWND() : nullptr;

	if (!hWnd) {
		throw std::runtime_error("Window handle is null");
	}

	const size_t bufferLength = sizeof(debugMessageBuffer);
	size_t messageLength = message.size();
	size_t processedLength = 0;

	while (processedLength < messageLength) {
		size_t chunkLength = (std::min)(messageLength - processedLength, bufferLength - 1);
		strncpy_s(debugMessageBuffer, message.substr(processedLength, chunkLength).c_str(), bufferLength);
		SendMessage(hWnd, WM_UPDATE_DEBUG_CONSOLE, 0, (LPARAM)debugMessageBuffer);
		processedLength += chunkLength;
	}

	return Result<void>();
}

