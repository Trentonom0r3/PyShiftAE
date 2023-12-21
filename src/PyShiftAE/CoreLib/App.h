#pragma once
#include "../CoreSDK/TaskUtils.h"
#include "../CoreSDK/Utils.h"
#include "Project.h"

//forward declaration
class Effect;
class EffectCollection;
class EffectProperties;
class EffectFlags;

class App {
public:
    App(); //Constructor -- Initialize this first.
    Project getProject();

    Project project;

    void beginUndoGroup(std::string undo_name = "Default Undo Group Name");
    void endUndoGroup();
    void executeCommand(int commandId);
    void reportInfo(std::string info);
    std::string pluginPaths();

};

class Effect {
public:
    Effect();
    Effect(std::string effectName);

    std::string getDisplayName();
    std::string getCategory();
    std::string getMatchName();
    std::string getVersion();

    EffectFlags getFlags();
    void setFlags(EffectFlags flags);
protected:
    AEGP_EffectRefH effectRefH_;

};

class EffectCollection {
public:
    EffectCollection();
    EffectCollection(Result<AEGP_LayerH> layerH);

    std::size_t size() const {
		return effects_.size();
	}

    Effect& operator[](std::size_t index) {
        if (index >= effects_.size()) {
			throw std::out_of_range("Index out of range");
		}
		return *effects_[index];
	}

    const Effect& operator[](std::size_t index) const {
        if (index >= effects_.size()) {
			throw std::out_of_range("Index out of range");
		}
		return *effects_[index];
	}

	std::vector<std::shared_ptr<Effect>>::iterator begin() { return effects_.begin(); }
	std::vector<std::shared_ptr<Effect>>::iterator end() { return effects_.end(); }

    protected:
	Result<AEGP_LayerH> layerH_;
	std::vector<std::shared_ptr<Effect>> effects_;
};

class EffectProperties {
public: 
    EffectProperties(Result<AEGP_EffectRefH> effectRefH);

    //set up stream suite wrappers

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


class PanelatorUI_Plat : public PanelatorUI {
public:
	explicit PanelatorUI_Plat(SPBasicSuite* spbP, AEGP_PanelH panelH,
		AEGP_PlatformViewRef platformWindowRef, AEGP_PanelFunctions1* outFunctionTable);
	static PanelatorUI_Plat* GetInstance() {
		std::lock_guard<std::mutex> lock(instanceMutex);
		return instance;
	}

	// Method to initialize the instance
	static void InitializeInstance(SPBasicSuite* spbP, AEGP_PanelH panelH,
		AEGP_PlatformViewRef platformWindowRef, AEGP_PanelFunctions1* outFunctionTable) {
		std::lock_guard<std::mutex> lock(instanceMutex);
		if (!instance) {
			instance = new PanelatorUI_Plat(spbP, panelH, platformWindowRef, outFunctionTable);
		} 
		else if (instance) {
			delete instance;
			instance = new PanelatorUI_Plat(spbP, panelH, platformWindowRef, outFunctionTable);
		}
	}
	HWND GetHWND() const { return hWnd; }

protected:
	virtual void InvalidateAll();

private:
	HWND pyGUI; // HWND for the Python GUI
	void EmbedPythonGUI();
	void operator=(const PanelatorUI&);
	PanelatorUI_Plat(const PanelatorUI_Plat&); // private, unimplemented
	// Static instance pointer
	static PanelatorUI_Plat* instance;
	static std::mutex instanceMutex;
	typedef LRESULT(CALLBACK* WindowProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void InitializePyGUI();
	WindowProc i_prevWindowProc;
	HWND hWnd;

	static LRESULT CALLBACK StaticOSWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OSWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

// Static member definitions
#define WM_UPDATE_DEBUG_CONSOLE (WM_USER + 1)

Result<void> writeToDebugPanel(std::string message);

class PyOutputStream {
public:
	PyOutputStream() = default;

	void write(py::str message) {  // Use py::str for Python compatibility
		std::string std_message = message.cast<std::string>();  // Cast py::str to std::string
		auto& messageFuture = enqueueSyncTask(writeToDebugPanel, std_message);
		messageFuture->wait();
		Result<void> result = messageFuture->getResult();
		if (result.error != A_Err_NONE) {
			throw std::runtime_error("Error writing to panel");
		}
	}

	void flush() {
		// Implement flush if needed
	}
};
