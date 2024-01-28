#pragma once
#include "../CoreSDK/TaskUtils.h"
#include "../CoreSDK/Utils.h"
#include "Project.h"

class Panel {
public:
	Panel(int hwnd, std::string sessionID) : _hwnd(hwnd), sessionID(sessionID) {}

	int getHwnd() const { return _hwnd; }
private:
	int _hwnd;
	std::string sessionID;
};


class App {
public:
	App() {
		project = Project();
	}
    Project getProject();

    Project project;

    void beginUndoGroup(std::string undo_name = "Default Undo Group Name");
    void endUndoGroup();
    void executeCommand(int commandId);
    void reportInfo(std::string info);
    std::string pluginPaths();

};

template <>
const A_char* SuiteTraits<PFAppSuite4>::i_name = kPFAppSuite;
template <>
const int32_t SuiteTraits<PFAppSuite4>::i_version = kPFAppSuiteVersion4;

class PanelatorUI
{
public:
	explicit PanelatorUI(SPBasicSuite* spbP, AEGP_PanelH panelH,
		AEGP_PlatformViewRef platformWindowRef,
		AEGP_PanelFunctions1* outFunctionTable);

protected:
	void operator=(const PanelatorUI&);
	PanelatorUI(const PanelatorUI&); // private, unimplemented

	AEGP_PlatformViewRef i_refH;
	AEGP_PanelH			i_panelH;

	A_u_char red;
	A_u_char green;
	A_u_char blue;
	A_Boolean		i_use_bg;
	A_long			i_numClicks;
	virtual void 	GetSnapSizes(A_LPoint* snapSizes, A_long* numSizesP);
	virtual void	PopulateFlyout(AEGP_FlyoutMenuItem* itemsP, A_long* in_out_numItemsP);
	virtual void	DoFlyoutCommand(AEGP_FlyoutMenuCmdID commandID);


	virtual void InvalidateAll() = 0;

	SuiteHelper<PFAppSuite4>	i_appSuite;
	SuiteHelper<AEGP_PanelSuite1>	i_panelSuite;

private:
	static A_Err	S_GetSnapSizes(AEGP_PanelRefcon refcon, A_LPoint* snapSizes, A_long* numSizesP);
	static A_Err	S_PopulateFlyout(AEGP_PanelRefcon refcon, AEGP_FlyoutMenuItem* itemsP, A_long* in_out_numItemsP);
	static A_Err	S_DoFlyoutCommand(AEGP_PanelRefcon refcon, AEGP_FlyoutMenuCmdID commandID);

};


// Class Declaration
class PanelatorUI_Plat : public PanelatorUI {
public:
    explicit PanelatorUI_Plat(std::string sessionID, SPBasicSuite* spbP, 
                              AEGP_PanelH panelH, AEGP_PlatformViewRef platformWindowRef, 
                              AEGP_PanelFunctions1* outFunctionTable);
    ~PanelatorUI_Plat();

protected:
    virtual void InvalidateAll();

private:
    HWND pyGUI; // HWND for the Python GUI
    std::string sessionID;
    // Prevent copying and assignment
    PanelatorUI_Plat(const PanelatorUI_Plat&) = delete;
    PanelatorUI_Plat& operator=(const PanelatorUI_Plat&) = delete;

    static PanelatorUI_Plat* instance;
    typedef LRESULT(CALLBACK* WindowProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    
    WindowProc i_prevWindowProc;
    HWND hWnd;
    HWND pyHWND;
    static LRESULT CALLBACK StaticOSWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT OSWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

