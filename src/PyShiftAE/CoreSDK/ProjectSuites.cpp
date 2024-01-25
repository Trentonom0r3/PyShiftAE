#include "ProjectSuites.h"



Result<AEGP_ProjectH> getProject() {
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_ProjectH projH = nullptr; // Initialize to nullptr in case AEGP_GetProjectByIndex fails
	err = suites.ProjSuite6()->AEGP_GetProjectByIndex(0, &projH);

	Result<AEGP_ProjectH> result;
	if (err != A_Err_NONE) {
		result.value = nullptr;
		throw std::runtime_error("Error getting project. Error code: " + std::to_string(err));
	}
	else {
		result.value = projH;
	}
	result.error = err;   // Set the error

	return result;
}

/*
AEGP_DoCommand(
  AEGP_Command  command);*/


Result<void> ExecuteCommand(int commandId)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_Command command = commandId;
	ERR(suites.CommandSuite1()->AEGP_DoCommand(command));

	if (err != A_Err_NONE) {
		throw std::runtime_error("Error executing command. Error code: " + std::to_string(err));
	}
	Result<void> result;

	result.error = err;   // Set the error

	return result;
}

Result<std::string> GetProjectName(Result<AEGP_ProjectH> projH) {
	A_Err err = A_Err_NONE;
	char nameZ[AEGP_MAX_PROJ_NAME_SIZE + 1];
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ProjectH proj = projH.value;
	if (proj == nullptr) {
		Result<std::string> result;
		result.value = "";
		result.error = err;
		return result;
	}
	err = suites.ProjSuite6()->AEGP_GetProjectName(proj, nameZ);

	Result<std::string> result;
	if (err != A_Err_NONE) {
		throw std::runtime_error("Error getting project name. Error code: " + std::to_string(err));
	}
	else {
		result.value = std::string(nameZ);
	}
	result.error = err;
	return result;
}

Result<std::string> GetProjectPath(Result<AEGP_ProjectH> projH) {
	A_Err err = A_Err_NONE;
	AEGP_MemHandle unicode_pathMH = nullptr;
	A_UTF16Char* unicode_pathP = nullptr;
	std::string path;
	AEGP_ProjectH proj = projH.value;
	if (proj == nullptr) {
		Result<std::string> result;
		result.value = "";
		result.error = err;
		return result;
	}
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ProjSuite6()->AEGP_GetProjectPath(proj, &unicode_pathMH);
	if (err == A_Err_NONE && unicode_pathMH) {
		suites.MemorySuite1()->AEGP_LockMemHandle(unicode_pathMH, (void**)&unicode_pathP);
		path = convertUTF16ToUTF8(unicode_pathP);  // Assuming convertUTF16ToUTF8 is already implemented
		suites.MemorySuite1()->AEGP_UnlockMemHandle(unicode_pathMH);
		suites.MemorySuite1()->AEGP_FreeMemHandle(unicode_pathMH);
	}

	Result<std::string> result(path, err);
	return result;
}

Result<void> SaveProjectToPath(Result<AEGP_ProjectH> projH, const std::string& path) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ProjectH proj = projH.value;
if (proj == nullptr) {
		Result<void> result;
		result.error = err;
		return result;
	}
	err = suites.ProjSuite6()->AEGP_SaveProjectToPath(proj, reinterpret_cast<const A_UTF16Char*>(path.c_str()));

	Result<void> result(err);
	return result;
}

Result<AEGP_TimeDisplay3> GetProjectTimeDisplay(Result<AEGP_ProjectH> projH) {
	A_Err err = A_Err_NONE;
	AEGP_TimeDisplay3 time_display;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ProjectH proj = projH.value;
	if (proj == nullptr) {
Result<AEGP_TimeDisplay3> result;
		result.value = time_display;
		result.error = err;
		return result;
	}
	err = suites.ProjSuite6()->AEGP_GetProjectTimeDisplay(proj, &time_display);

	Result<AEGP_TimeDisplay3> result(time_display, err);
	return result;
}

Result<void> SetProjectTimeDisplay2(Result<AEGP_ProjectH> projH,
	const std::string& displayType,
	int timebase,
	bool nonDropFrame,
	int framesPerFoot,
	int startingFrame,
	bool autoTimecodeBase) {
	AEGP_TimeDisplay2 timeDisplay;
	// Set display type
	if (displayType == "TIMECODE") timeDisplay.time_display_type = AEGP_TimeDisplayType_TIMECODE;
	else if (displayType == "FRAMES") timeDisplay.time_display_type = AEGP_TimeDisplayType_FRAMES;
	else if (displayType == "FEET_AND_FRAMES") timeDisplay.time_display_type = AEGP_TimeDisplayType_FEET_AND_FRAMES;
	else throw std::runtime_error("Invalid display type: " + displayType);

	// Set other properties
	timeDisplay.timebaseC = static_cast<A_char>(timebase);
	timeDisplay.non_drop_30B = static_cast<A_Boolean>(nonDropFrame);
	timeDisplay.frames_per_footC = static_cast<A_char>(framesPerFoot);
	timeDisplay.starting_frameL = static_cast<A_long>(startingFrame);
	timeDisplay.auto_timecode_baseB = static_cast<A_Boolean>(autoTimecodeBase);

	// Now call the SDK function to set the time display
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ProjectH proj = projH.value;
	if (proj == nullptr) {
		Result<void> result;
		result.error = err;
		return result;
	}
	err = suites.ProjSuite6()->AEGP_SetProjectTimeDisplay(proj, reinterpret_cast<AEGP_TimeDisplay3*>(&timeDisplay));

	Result<void> result(err);
	return result;
}


Result<bool> IsProjectDirty(Result<AEGP_ProjectH> projH) {
	A_Err err = A_Err_NONE;
	A_Boolean is_dirty = FALSE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ProjectH proj = projH.value;
	if (proj == nullptr) {
Result<bool> result;
		result.value = false;
		result.error = err;
		return result;
	}
	err = suites.ProjSuite6()->AEGP_ProjectIsDirty(proj, &is_dirty);

	Result<bool> result(static_cast<bool>(is_dirty), err);
	return result;
}

Result<void> SaveProjectAs(Result<AEGP_ProjectH> projH, const std::string& path) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ProjectH proj = projH.value;
	if (proj == nullptr) {
		Result<void> result;
		result.error = err;
		return result;
	}
	err = suites.ProjSuite6()->AEGP_SaveProjectAs(proj, reinterpret_cast<const A_UTF16Char*>(path.c_str()));

	Result<void> result(err);
	return result;
}

Result<AEGP_ProjectH> NewProject() {
	A_Err err = A_Err_NONE;
	AEGP_ProjectH new_projectH = nullptr;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ProjSuite6()->AEGP_NewProject(&new_projectH);

	Result<AEGP_ProjectH> result(new_projectH, err);
	return result;
}

Result<AEGP_ProjectH> OpenProjectFromPath(const std::string& path) {
	A_Err err = A_Err_NONE;
	AEGP_ProjectH projectH = nullptr;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	err = suites.ProjSuite6()->AEGP_OpenProjectFromPath(reinterpret_cast<const A_UTF16Char*>(path.c_str()), &projectH);

	Result<AEGP_ProjectH> result(projectH, err);
	return result;
}

Result<std::string> GetProjectBitDepth(Result<AEGP_ProjectH> projH) {
	A_Err err = A_Err_NONE;
	AEGP_ProjBitDepth bit_depth;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ProjectH proj = projH.value;
	if (proj == nullptr) {
		Result<std::string> result;
		result.value = "";
		result.error = err;
		return result;
	}
	err = suites.ProjSuite6()->AEGP_GetProjectBitDepth(proj, &bit_depth);

	std::string bitDepthStr;
	if (bit_depth == AEGP_ProjBitDepth_8) bitDepthStr = "8";
	else if (bit_depth == AEGP_ProjBitDepth_16) bitDepthStr = "16";
	else if (bit_depth == AEGP_ProjBitDepth_32) bitDepthStr = "32";
	else bitDepthStr = "Unknown";

	Result<std::string> result(bitDepthStr, err);
	return result;
}


Result<void> SetProjectBitDepth(Result<AEGP_ProjectH> projH, std::string bit_depth) {
	A_Err err = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_ProjBitDepth bitDepth = ConvertToProjBitDepth(bit_depth);
	AEGP_ProjectH proj = projH.value;
if (proj == nullptr) {
		Result<void> result;
		result.error = err;
		return result;
	}
	err = suites.ProjSuite6()->AEGP_SetProjectBitDepth(proj, bitDepth);

	Result<void> result(err);
	return result;
}

Result<AEGP_ItemH> getProjectRootFolder()
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_ItemH root_folderH;
	AEGP_ProjectH projH;
	ERR(suites.ProjSuite6()->AEGP_GetProjectByIndex(0, &projH));
	ERR(suites.ProjSuite6()->AEGP_GetProjectRootFolder(projH, &root_folderH));

	Result<AEGP_ItemH> result;
	result.value = root_folderH;
	result.error = err;

	return result;
}

