#include "RenderSuites.h"


Result<AEGP_FrameReceiptH> renderAndCheckoutFrame(Result<AEGP_RenderOptionsH> roH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_FrameReceiptH receiptH = NULL;
	ERR(suites.RenderSuite2()->AEGP_RenderAndCheckoutFrame(roH.value, NULL, NULL, &receiptH));

	Result<AEGP_FrameReceiptH> result;
	result.value = receiptH;
	result.error = err;

	return result;
}

Result<AEGP_WorldH> getReceiptWorld(Result<AEGP_FrameReceiptH> receiptH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	AEGP_WorldH frameH = NULL;
	ERR(suites.RenderSuite2()->AEGP_GetReceiptWorld(receiptH.value, &frameH));

	Result<AEGP_WorldH> result;
	result.value = frameH;
	result.error = err;

	return result;
}

Result<void> checkinFrame(Result<AEGP_FrameReceiptH> receiptH)
{
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	A_Err err = A_Err_NONE;
	ERR(suites.RenderSuite2()->AEGP_CheckinFrame(receiptH.value));

	Result<void> result;
	result.error = err;

	return result;
}
