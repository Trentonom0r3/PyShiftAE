#pragma once
#include "../CoreSDK/TaskUtils.h"
#include "../CoreSDK/Utils.h"
#include "../CoreLib/PyCore.h"

enum FlyoutMenuMarkType {
	NORMAL = AEGP_FlyoutMenuMarkType_NORMAL,
	SEPARATOR = AEGP_FlyoutMenuMarkType_SEPARATOR,
	RADIO = AEGP_FlyoutMenuMarkType_RADIO_BULLET,
	CHECKED = AEGP_FlyoutMenuMarkType_CHECKED
};

class FlyoutMenuItem {
public:
FlyoutMenuItem() = default;

FlyoutMenuItem(int indent, FlyoutMenuMarkType type, bool enabled, std::string name, py::function callback, int cmd) :
		indent(indent),
		type(type),
		enabled(enabled),
		name(name),
		callback(callback),
		cmd_id(cmd){};

	int indent;
	FlyoutMenuMarkType type;
	bool enabled;
	int cmd_id;
	std::string name;
	py::function callback;
};

class FlyoutMenu {
public:
	FlyoutMenu() = default;

    FlyoutMenu(std::vector<FlyoutMenuItem> items) :
		items(items) {};

	std::vector<FlyoutMenuItem> items;
};

class Panel {
public:
    Panel(FlyoutMenu flyoutMenu) :
        flyoutMenu(flyoutMenu),
		load(py::function())
		{};
    //constructor for when Manifest and Flyout are empty, so we can set them later
	Panel() = default;


    FlyoutMenu flyoutMenu;
	py::function load;

};

//usage example:
//Panel panel = Panel(Manifest(), FlyoutMenu(std::vector<FlyoutMenuItem>()));

//usage example:
//Manifest manifest = Manifest();
//manifest.name = "My Plugin";
//manifest.version = "1.0.0";
//manifest.author = "My Name";
//manifest.description = "A Python-based plugin for After Effects.";
//manifest.size = std::make_tuple(400, 400);
//manifest.resizable = true;

//FlyoutMenu MakeMenu(std::vector<FlyoutMenuItem> items) {
//    return FlyoutMenu(items);
//}
//
//FlyoutMenuItem MakeMenuItem(int indent, std::string type, bool enabled, std::string name, std::function<void()> callback) {
//    return FlyoutMenuItem(indent, type, enabled, name, callback);
//}
//
//FlyoutMenu menu = MakeMenu({
//    MakeMenuItem(0, "NORMAL", true, "Item 1", []() { std::cout << "Item 1 clicked" << std::endl; }),
//    MakeMenuItem(0, "NORMAL", true, "Item 2", []() { std::cout << "Item 2 clicked" << std::endl; }),
//    MakeMenuItem(0, "NORMAL", true, "Item 3", []() { std::cout << "Item 3 clicked" << std::endl; }),
//    MakeMenuItem(0, "NORMAL", true, "Item 4", []() { std::cout << "Item 4 clicked" << std::endl; }),
// });
//
//Panel panel = Panel(manifest, menu);
//FlyoutMenu menu = panel.flyoutMenu;
//std::vector<FlyoutMenuItem> items = menu.items;

/*
flyout menu's are a simple declarative structure

AEGP_FlyoutMenuItem	myMenu[] = {
	{1, AEGP_FlyoutMenuMarkType_NORMAL,		FALSE,	AEGP_FlyoutMenuCmdID_NONE, "Hi!"  },
	{1, AEGP_FlyoutMenuMarkType_SEPARATOR,	TRUE,	AEGP_FlyoutMenuCmdID_NONE, NULL  },
	{1, AEGP_FlyoutMenuMarkType_NORMAL,		TRUE,	AEGP_FlyoutMenuCmdID_NONE, "Set BG Color"  },
		{2, AEGP_FlyoutMenuMarkType_NORMAL,		TRUE,	PT_MenuCmd_RED,				"Red"  },
		{2, AEGP_FlyoutMenuMarkType_NORMAL,		TRUE,	PT_MenuCmd_GREEN,			"Green"  },
		{2, AEGP_FlyoutMenuMarkType_NORMAL,		TRUE,	PT_MenuCmd_BLUE,			"Blue"  },
	{1, AEGP_FlyoutMenuMarkType_NORMAL,		TRUE,	PT_MenuCmd_STANDARD,		"Normal Fill Color"  },
	{1, AEGP_FlyoutMenuMarkType_NORMAL,		TRUE,	AEGP_FlyoutMenuCmdID_NONE,	"Set Title"  },
		{2, AEGP_FlyoutMenuMarkType_NORMAL,		TRUE,	PT_MenuCmd_TITLE_LONGER,	"Longer"  },
		{2, AEGP_FlyoutMenuMarkType_NORMAL,		TRUE,	PT_MenuCmd_TITLE_SHORTER,	"Shorter"  }
};


*/

//usage example:	
//std::vector<FlyoutMenuItem> items = panel.flyoutMenu.items;
//AEGP_FlyoutMenuItem* myMenu = new AEGP_FlyoutMenuItem[items.size()];
//for (int i = 0; i < items.size(); i++) {
//    FlyoutMenuItem item = items[i];
//    myMenu[i] = AEGP_FlyoutMenuItem();
//    myMenu[i].indent = item.indent;
//    myMenu[i].type = item.type;
//    myMenu[i].enabled = item.enabled;
//    myMenu[i].cmd_id = item.cmd_id;
//    myMenu[i].name = item.name.c_str();
//}

//usage example:
//void DoFlyoutCommand(AEGP_FlyoutMenuCmdID commandID)
//auto items = panel.flyoutMenu.items; //this->flyoutMenu.items;
//for (int i = 0; i < items.size(); i++) {
//    FlyoutMenuItem item = items[i];
//    if (item.cmd_id == commandID) {
//        item.callback();
//    }
	//}
//}
