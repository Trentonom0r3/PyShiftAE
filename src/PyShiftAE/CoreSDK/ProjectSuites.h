#pragma once
#include "Core.h"

#include "ProjectSuites.h"

Result<AEGP_ProjectH> getProject();

Result<void> ExecuteCommand(int commandId);

Result<std::string> GetProjectName(Result<AEGP_ProjectH> projH);

Result<std::string> GetProjectPath(Result<AEGP_ProjectH> projH);

Result<void> SaveProjectToPath(Result<AEGP_ProjectH> projH, const std::string& path);

Result<AEGP_TimeDisplay3> GetProjectTimeDisplay(Result<AEGP_ProjectH> projH);

Result<void> SetProjectTimeDisplay2(Result<AEGP_ProjectH> projH,
	const std::string& displayType = "TIMECODE",
	int timebase = 30,
	bool nonDropFrame = false,
	int framesPerFoot = 16,
	int startingFrame = 0,
	bool autoTimecodeBase = false);


Result<bool> IsProjectDirty(Result<AEGP_ProjectH> projH);

Result<void> SaveProjectAs(Result<AEGP_ProjectH> projH, const std::string& path);

Result<AEGP_ProjectH> NewProject();

Result<AEGP_ProjectH> OpenProjectFromPath(const std::string& path);

Result<std::string> GetProjectBitDepth(Result<AEGP_ProjectH> projH);

Result<void> SetProjectBitDepth(Result<AEGP_ProjectH> projH, std::string bit_depth);

Result<AEGP_ItemH> getProjectRootFolder();
