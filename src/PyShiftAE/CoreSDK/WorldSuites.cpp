#include "WorldSuites.h"



Result<PF_Pixel8*> getBaseAddr8(Result<AEGP_WorldH> frameH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	PF_Pixel8* baseAddr = nullptr;
	ERR(suites.WorldSuite3()->AEGP_GetBaseAddr8(frameH.value, &baseAddr));

	Result<PF_Pixel8*> result;
	result.value = baseAddr;
	result.error = err;

	return result;
}

Result<size> getSize(Result<AEGP_WorldH> frameH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	int width, height;
	ERR(suites.WorldSuite3()->AEGP_GetSize(frameH.value, &width, &height));

	Result<size> result;
	result.value.width = width;
	result.value.height = height;
	result.error = err;

	return result;
}
