#pragma once
#include "Core.h"

#include "ProjectSuites.h"

Result<AEGP_ProjectH> getProject();

Result<void> ExecuteCommand(int commandId);

Result<std::string> GetProjectName(AEGP_ProjectH projH);

Result<std::string> GetProjectPath(AEGP_ProjectH projH);

Result<void> SaveProjectToPath(AEGP_ProjectH projH, const std::string& path);

Result<AEGP_TimeDisplay3> GetProjectTimeDisplay(AEGP_ProjectH projH);

Result<void> SetProjectTimeDisplay2(AEGP_ProjectH projH,
	const std::string& displayType = "TIMECODE",
	int timebase = 30,
	bool nonDropFrame = false,
	int framesPerFoot = 16,
	int startingFrame = 0,
	bool autoTimecodeBase = false);


Result<bool> IsProjectDirty(AEGP_ProjectH projH);

Result<void> SaveProjectAs(AEGP_ProjectH projH, const std::string& path);

Result<AEGP_ProjectH> NewProject();

Result<AEGP_ProjectH> OpenProjectFromPath(const std::string& path);

Result<std::string> GetProjectBitDepth(AEGP_ProjectH projH);

Result<void> SetProjectBitDepth(AEGP_ProjectH projH, std::string bit_depth);

Result<AEGP_ItemH> getProjectRootFolder();
