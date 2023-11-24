#pragma once
#include "Core.h"


Result<AEGP_RenderOptionsH> getRenderOptions(Result<AEGP_ItemH> itemH);

Result<AEGP_RenderOptionsH> setTime(Result<AEGP_RenderOptionsH> roH, float time);

Result<AEGP_RenderOptionsH> getWorldType(Result<AEGP_RenderOptionsH> roH);


Result<void> disposeRenderOptions(Result <AEGP_RenderOptionsH> roH);