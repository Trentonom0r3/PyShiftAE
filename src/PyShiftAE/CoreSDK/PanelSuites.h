#pragma once
#include "Core.h"


/*
enum {AEGP_FlyoutMenuCmdID_NONE = 0};
typedef A_long AEGP_FlyoutMenuCmdID;


enum {
	AEGP_FlyoutMenuMarkType_NORMAL,
	AEGP_FlyoutMenuMarkType_CHECKED,
	AEGP_FlyoutMenuMarkType_RADIO_BULLET,
	AEGP_FlyoutMenuMarkType_SEPARATOR
};
typedef A_long AEGP_FlyoutMenuMarkType;

typedef A_long AEGP_FlyoutMenuIndent;


typedef struct
{
	AEGP_FlyoutMenuIndent	indent;
	AEGP_FlyoutMenuMarkType	type;
	A_Boolean				enabledB;
	AEGP_FlyoutMenuCmdID	cmdID;		// limited to MAX(A_long) - 201;
	const A_u_char*			utf8NameZ;
}AEGP_FlyoutMenuItem;

*/

struct MenuMarks {
	float normal = AEGP_FlyoutMenuMarkType_NORMAL;
	float checked = AEGP_FlyoutMenuMarkType_CHECKED;
	float radio_bullet = AEGP_FlyoutMenuMarkType_RADIO_BULLET;
	float separator = AEGP_FlyoutMenuMarkType_SEPARATOR;
};


struct MenuItemsH {
	float indent;
	float type;
	float enabledB;
	float cmdID;
	std::string utf8NameZ;
};

struct MenuH {
	std::vector<MenuItemsH> menuItems;
};

struct PanelFunctions1H {
	std::vector<A_LPoint> snapSizes;
	std::vector<AEGP_FlyoutMenuItem> flyoutMenuItems;
	std::vector<AEGP_FlyoutMenuCmdID> flyoutMenuCmdIDs;
};

Result<void> setPanelTitle(Result<AEGP_PanelH> panelH, std::string title);

Result<void> togglePanelVisibility(std::string matchName);

Result<void> isPanelShown(std::string matchName, A_Boolean* out_tab_is_shownB, A_Boolean* out_panel_is_frontmostB);
