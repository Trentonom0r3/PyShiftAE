#pragma once
#include "Core.h"

/*

Marker Streams
AEGP_MarkerSuite allows for direct manipulation of marker data.

AEGP_MarkerSuite2
Function

Purpose

AEGP_NewMarker

Creates a new marker.

AEGP_NewMarker(
  AEGP_MarkerValP  *markerPP);
AEGP_DisposeMarker

Disposes of a marker.

AEGP_DisposeMarker(
  AEGP_MarkerValP  markerP);
AEGP_DuplicateMarker

Duplicates a marker (didn’t see that one coming, eh?).

AEGP_DuplicateMarker(
  AEGP_MarkerValP  markerP,
  AEGP_MarkerValP  *new_markerP);
AEGP_SetMarkerFlag

Sets a marker flag’s value.

AEGP_SetMarkerFlag(
  AEGP_MarkerValP      markerP,
  AEGP_MarkerFlagType  flagType,
  A_Boolean            valueB);
Currently, AEGP_MarkerFlagType is one of the following:

AEGP_MarkerFlag_NONE

AEGP_MarkerFlag_NAVIGATION

AEGP_GetMarkerFlag

Gets the value (see above) of a given AEGP_MarkerFlagType.

AEGP_GetMarkerFlag(
  AEGP_ConstMarkerValP  markerP,
  AEGP_MarkerFlagType   flagType
  A_Boolean             *valueBP);
AEGP_GetMarkerString

Retrieves the UTF-16, NULL-terminated string located in the specified marker field. Must be disposed of by caller using AEGP_FreeMemHandle.

AEGP_GetMarkerString(
  AEGP_PluginID          id,
  AEGP_ConstMarkerValP   markerP,
  AEGP_MarkerStringType  strType,
  AEGP_MemHandle         *unicodePH);
AEGP_SetMarkerString

Sets the specified field of a marker to the provided text.

AEGP_SetMarkerString(
  AEGP_MarkerValP        markerP,
  AEGP_MarkerStringType  strType,
  const A_u_short        *unicodeP,
  A_long                 lengthL);
AEGP_CountCuePointParams

Returns the number of cue point parameters.

AEGP_CountCuePointParams(
  AEGP_ConstMarkerValP  markerP,
  A_long                *paramsLP);
AEGP_GetIndCuePointParam

Returns the cue point param at the specified index (which must be between 0 and (cue point params -1). Returned handles are UTF-16, NULL-terminated strings, and must be disposed of by caller using AEGP_FreeMemHandle.

AEGP_GetIndCuePointParam(
  AEGP_PluginID         id,
  AEGP_ConstMarkerValP  markerP,
  A_long                param_indexL,
  AEGP_MemHandle        *unicodeKeyPH,
  AEGP_MemHandle        *uni_ValuePH);
AEGP_SetIndCuePointParam

Set the value of an indexed cue point parameter to the specified value. key_lengthL is “number of unicode characters”, and value_lenL is the length of the provided value. unicode_KeyP and unicode_ValueP point to UTF-16 data.

AEGP_SetIndCuePointParam(
  AEGP_MarkerValP  markerP,
  A_long           param_idxL,
  const A_u_short  *unicode_KeyP,
  A_long           key_lengthL,
  const A_u_short  *unicode_ValueP,
  A_long           value_lengthL);
AEGP_InsertCuePointParam

Inserts a cue point parameter. This call is following by AEGP_SetIndCuePointParam to actually set the data.

AEGP_InsertCuePointParam(
  AEGP_MarkerValP  markerP,
  A_long           param_idxL);
AEGP_DeleteInd CuePointParam

Deletes the cue point param at the specified index.

AEGP_DeleteIndCuePointParam(
  AEGP_MarkerValP  markerP,
  A_long           param_idxL);
AEGP_SetMarkerDuration

AEGP_SetMarkerDuration(
  AEGP_MarkerValP  markerP,
  const A_Time     *durationPT);
AEGP_GetMarkerDuration

AEGP_GetMarkerDuration(
  AEGP_ConstMarkerValP  markerP,
  A_Time                *durationPT);
  */

Result<AEGP_MarkerValP> NewMarker()
{
	A_Err 	err = A_Err_NONE,
		err2 = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_MarkerValP markerP = nullptr;

	ERR(suites.MarkerSuite2()->AEGP_NewMarker(&markerP));

	Result<AEGP_MarkerValP> result;
	result.error = err;
	result.value = markerP;

	return result;
}

Result<void> DisposeMarker(Result<AEGP_MarkerValP> markerP)
{
	A_Err 	err = A_Err_NONE,
		err2 = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();

	ERR(suites.MarkerSuite2()->AEGP_DisposeMarker(markerP.value));

	Result<void> result;
	result.error = err;

	return result;

}

Result<AEGP_MarkerValP> DuplicateMarker(Result<AEGP_MarkerValP> markerP)
{
	A_Err 	err = A_Err_NONE,
		err2 = A_Err_NONE;
	AEGP_SuiteHandler& suites = SuiteManager::GetInstance().GetSuiteHandler();
	AEGP_MarkerValP new_markerP = nullptr;

	ERR(suites.MarkerSuite2()->AEGP_DuplicateMarker(markerP.value, &new_markerP));

	Result<AEGP_MarkerValP> result;
	result.error = err;
	result.value = new_markerP;

	return result;

}

