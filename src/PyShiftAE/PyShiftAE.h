#pragma once

#include "AEConfig.h"
#ifdef AE_OS_WIN
	#include <windows.h>
#endif

//SDK HEADERS
#include "entry.h"
#include "AE_GeneralPlug.h"
#include "AE_Macros.h"
#include "AEGP_SuiteHandler.h"
#include "AE_Macros.h"
#include "CoreSDK/SuiteManager.h"
#include "CoreSDK/MessageQueue.h"
#include "PyUtils/PyLink.h"

//CUSTOM HEADERS

#define AEGP_MAX_STREAM_DIM 4

// This entry point is exported through the PiPL (.r file)
extern "C" DllExport AEGP_PluginInitFuncPrototype EntryPointFunc;
