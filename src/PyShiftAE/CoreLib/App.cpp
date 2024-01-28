#include "App.h"
#include "../MessageManager.h"


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


// Define a custom message for Python GUI resizing
#define WM_PYTHON_GUI_RESIZE WM_APP + 1

// OS Window Property Key
const char OSWndObjectProperty[] = "PanelatorUI_PlatPtr";

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

// Constructor
PanelatorUI_Plat::PanelatorUI_Plat(std::string sessionID, SPBasicSuite* spbP,
    AEGP_PanelH panelH, AEGP_PlatformViewRef platformWindowRef,
    AEGP_PanelFunctions1* outFunctionTable)
    : PanelatorUI(spbP, panelH, platformWindowRef, outFunctionTable),
    i_prevWindowProc(nullptr), hWnd(static_cast<HWND>(platformWindowRef)),
    sessionID(sessionID) {

    // Hook the main window
    i_prevWindowProc = (WindowProc)GetWindowLongPtr(platformWindowRef, GWLP_WNDPROC);
    SetWindowLongPtrA(platformWindowRef, GWLP_WNDPROC, (LONG_PTR)PanelatorUI_Plat::StaticOSWindowWndProc);
    ::SetProp(platformWindowRef, OSWndObjectProperty, (HANDLE)this);

    HWND hwnd = SessionManager::GetInstance().getHWND(sessionID);
    pyHWND = (HWND)hwnd;
    SetParent(pyHWND, hWnd);
}

// Static Window Procedure
LRESULT CALLBACK PanelatorUI_Plat::StaticOSWindowWndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam) {
    PanelatorUI_Plat* platPtr = reinterpret_cast<PanelatorUI_Plat*>(GetProp(hWnd, OSWndObjectProperty));
    if (platPtr) {
        return platPtr->OSWindowWndProc(hWnd, message, wParam, lParam);
    }
    else {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

// Instance-specific Window Procedure
LRESULT PanelatorUI_Plat::OSWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_SIZE: {
        PostMessage(hWnd, WM_PYTHON_GUI_RESIZE, wParam, lParam);
        break;
    }
    case WM_PYTHON_GUI_RESIZE: {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        MoveWindow(pyHWND, 0, 0, width, height, TRUE);
        break;
    }
    case WM_DESTROY: {
		InvalidateAll();
		break;
	}
    case WM_CLOSE: {
        InvalidateAll();
        break;
    }
    case WM_QUIT: {
        InvalidateAll();
        break;
    }
                 // Handle other messages
    }
    return CallWindowProc(i_prevWindowProc, hWnd, message, wParam, lParam);
}

// Invalidate and clean-up resources
void PanelatorUI_Plat::InvalidateAll() {
    InvalidateRect(pyHWND, nullptr, FALSE);
    InvalidateRect(hWnd, nullptr, FALSE);
    SessionManager::GetInstance().killProcess(sessionID);
    pyHWND = nullptr;
    hWnd = nullptr;
    i_prevWindowProc = nullptr;
    RemoveProp(i_refH, OSWndObjectProperty);
}

// Destructor
PanelatorUI_Plat::~PanelatorUI_Plat() {
    InvalidateAll();
}