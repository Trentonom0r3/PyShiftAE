<h2>Begin File: AEFX_ArbParseHelper.h</h2>

```cpp
// AEFX_ArbParseHelper.h
//

// This file has no header, and is designed to by #included as necessary. -jja 9/30/98


#ifndef _H_AEFX_ArbParseHelper
#define _H_AEFX_ArbParseHelper

#include <AE_Effect.h>

#define AEFX_Char_TAB			'\t'
#define AEFX_Char_EOL			'\r'
#define AEFX_Char_SPACE			' '

#ifndef AEFX_CELL_SIZE
#define		AEFX_CELL_SIZE			256
#endif

#ifdef __cplusplus
extern "C" {
#endif


enum {
	AEFX_ParseError_EXPECTING_MORE_DATA = 0x00FEBE00,
	AEFX_ParseError_APPEND_ERROR,
	AEFX_ParseError_EXPECTING_A_NUMBER,
	AEFX_ParseError_MATCH_ERROR
};
typedef A_long AEFX_ParseErrors;



#ifndef STR_EQUAL
	#define STR_EQUAL(A, B) (strcmp((A),(B)) == 0)
#endif


PF_Err			
AEFX_AppendText(	A_char					*srcAC,				/* >> */
					const A_u_long		dest_sizeL,			/* >> */
					A_char					*destAC,			/* <> */
					A_u_long			*current_indexPLu);	/* <> */


PF_Err
AEFX_ParseFpLong(	PF_InData			*in_data,				/* >> */
					PF_OutData			*out_data,				/* >> */
					const A_char			*startPC,				/* >> */
					A_u_long		*current_indexPL,		/* << */
					PF_FpLong			*dPF);					/* << */


PF_Err
AEFX_MatchCell(	PF_InData			*in_data,				/* >> */
				PF_OutData			*out_data,				/* >> */
				const A_char			*strPC,					/* >> */
				const A_char			*startPC,				/* >> */
				A_u_long		*current_indexPL,		/* << */
				PF_Boolean			*matchPB0);				/* << */

PF_Err
AEFX_ParseCell(	PF_InData			*in_data,				/* >> */
				PF_OutData			*out_data,				/* >> */
				const A_char			*startPC,				/* >> */
				A_u_long		*current_indexPL,		/* << */
				A_char				*bufAC);				/* << AEFX_CELL_SIZE */


#ifdef __cplusplus
} // extern "C"
#endif

#endif

```

---

<h2>Begin File: AEFX_ChannelDepthTpl.h</h2>

```cpp
#ifndef _H_AEFX_CHANNELDEPTHTPL
#define _H_AEFX_CHANNELDEPTHTPL

/** AEFX_ChannelDepthTpl.h

	(c) 2005 Adobe Systems Incorporated

**/

// Basic pixel traits structure. This structure is never used per se, merely overidden -- see below.
template <typename Pixel>
struct PixelTraits {
	typedef int PixType;
	typedef int DataType;
 	static DataType 
    LutFunc(DataType input, const DataType *map);
  	
	enum {max_value = 0 };
};


// 8 bit pixel types, constants, and functions
template <>
struct PixelTraits<PF_Pixel8>{
	typedef PF_Pixel8	PixType;
	typedef u_char		DataType;
	static DataType
	LutFunc(DataType input, const DataType *map){return map[input];}
   	enum {max_value = PF_MAX_CHAN8};
};

// 16 bit pixel types, constants, and functions
template <>
struct PixelTraits<PF_Pixel16>{
	typedef PF_Pixel16 PixType;
	typedef u_short		DataType;
	static u_short
	LutFunc(u_short input, const u_short *map); 
 	enum {max_value = PF_MAX_CHAN16};
};
 

inline u_short 
PixelTraits<PF_Pixel16>::LutFunc(u_short input,
							const u_short *map)
{
 	u_short index  = input >> (15 - PF_TABLE_BITS);
	uint32_t   fract  = input & ((1 << (15 - PF_TABLE_BITS)) - 1);
 	A_long       result = map [index];

	if (fract) {
		result += ((((A_long) map [index + 1] - result) * fract) +
				   (1 << (14 - PF_TABLE_BITS))) >> (15 - PF_TABLE_BITS);
	}
 	return (u_short) result;
	
}

#endif //_H_AEFX_CHANNELDEPTHTPL
```

---

<h2>Begin File: AEFX_SuiteHelper.h</h2>

```cpp
/**************************************************************************
*
* ADOBE CONFIDENTIAL
* ___________________
*
*  Copyright 2009 Adobe Systems Incorporated
*  All Rights Reserved.
*
* NOTICE:  All information contained herein is, and remains the property of
* Adobe Systems Incorporated  and its suppliers,  if any.  The intellectual 
* and technical concepts contained herein are proprietary to  Adobe Systems 
* Incorporated  and its suppliers  and may be  covered by U.S.  and Foreign 
* Patents,patents in process,and are protected by trade secret or copyright 
* law.  Dissemination of this  information or reproduction of this material
* is strictly  forbidden  unless prior written permission is  obtained from 
* Adobe Systems Incorporated.
**************************************************************************/

#ifndef AEFX_SUITE_HELPER_H
#define AEFX_SUITE_HELPER_H


/** AEFX_SuiteHelper.h

	Contains helper routines for acquiring/releasing suites.
	
	NOTE: 	If you're writing C++ plug-ins that support exceptions, use the AEGP_SuiteHandler class or AEFX_SuiteScoper.

**/

#include <AEFX_SuiteHandlerTemplate.h>
#include <AE_EffectCB.h>
#include <AE_EffectSuites.h>
#include <adobesdk/DrawbotSuite.h>



#ifdef __cplusplus
	extern "C" {
#endif

PF_Err AEFX_AcquireSuite(	PF_InData		*in_data,			/* >> */
							PF_OutData		*out_data,			/* >> */
							const char		*name,				/* >> */
							int32_t			version,			/* >> */
							const char		*error_stringPC0,	/* >> */
							void			**suite);			/* << */


PF_Err AEFX_ReleaseSuite(	PF_InData		*in_data,			/* >> */
							PF_OutData		*out_data,			/* >> */
							const char		*name,				/* >> */
							int32_t			version,			/* >> */
							const char		*error_stringPC0);	/* >> */


PF_Err AEFX_AcquireDrawbotSuites(	PF_InData			*in_data,			/* >> */
									PF_OutData			*out_data,			/* >> */
									DRAWBOT_Suites		*suiteP);			/* << */


PF_Err AEFX_ReleaseDrawbotSuites(	PF_InData			*in_data,			/* >> */
									PF_OutData			*out_data);			/* >> */


#ifdef __cplusplus
	}
#endif


#ifdef __cplusplus

	template<typename SuiteType>
	class AEFX_SuiteHelperT
	{
	public:
		AEFX_SuiteHelperT(	PF_InData		*in_data,			/* >> */
							PF_OutData		*out_data,			/* >> */
							const char		*name,				/* >> */
							int32_t			version) :			/* >> */
			mInDataP(in_data), mOutDataP(out_data), mSuiteName(name), mSuiteVersion(version), mSuiteP(NULL)
		{
			void	*suiteP(NULL);
			
			PF_Err err = AEFX_AcquireSuite(mInDataP, mOutDataP, mSuiteName, mSuiteVersion, NULL, &suiteP);
			
			if (err) {
				A_THROW(err);
			}
			
			mSuiteP = reinterpret_cast<SuiteType*>(suiteP);
		}

		~AEFX_SuiteHelperT()
		{
			(void)AEFX_ReleaseSuite(mInDataP, mOutDataP, mSuiteName, mSuiteVersion, NULL);
		}

		const SuiteType* operator->() const
		{
			return mSuiteP;
		}

		SuiteType* get() const
		{
			return mSuiteP;
		}

	private:

		PF_InData		*mInDataP;
		PF_OutData		*mOutDataP;
		const char		*mSuiteName;
		int32_t			mSuiteVersion;
		SuiteType		*mSuiteP;
	};



// clients of this class probably should just be using the regular template
// instead

class AEFX_DrawbotSuitesScoper
{
public:
	AEFX_DrawbotSuitesScoper(PF_InData *in_data, PF_OutData *out_data)
			:
			i_in_data(in_data),
			i_out_data(out_data)
	{
		PF_Err err = AEFX_AcquireDrawbotSuites(in_data, out_data, &i_suites);
		
		if (err) {
			A_THROW(err);
		}
	}
	
	inline	DRAWBOT_Suites*	Get()
	{
		return &i_suites;
	}
	
	~AEFX_DrawbotSuitesScoper()
	{
		AEFX_ReleaseDrawbotSuites(i_in_data, i_out_data);
	}
	
private:
	DRAWBOT_Suites		i_suites;
	PF_InData			*i_in_data;
	PF_OutData			*i_out_data;
};

#endif


#endif // AEFX_SUITE_HELPER_H

```

---

<h2>Begin File: AEGP_SuiteHandler.cpp</h2>

```cpp
/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

/** AEGP_SuiteHandler.cpp

	Implementation of AEGP_SuiteHandler non-inlines. See AEGP_SuiteHandler.h for usage.

	created 9/11/2000 jms
**/


#include <AEGP_SuiteHandler.h>
#include <AE_Macros.h>

AEGP_SuiteHandler::AEGP_SuiteHandler(const SPBasicSuite *pica_basicP) :
	i_pica_basicP(pica_basicP)
{
	AEFX_CLR_STRUCT(isuites);

	if (!i_pica_basicP) {						//can't construct w/out basic suite, everything else is demand loaded
		MissingSuiteError();
	}
}

AEGP_SuiteHandler::~AEGP_SuiteHandler()
{
	ReleaseAllSuites();
}

// Had to go to the header file to be inlined to please CW mach-o target
/*void *AEGP_SuiteHandler::pLoadSuite(A_char *nameZ, A_long versionL) const
{
	const void *suiteP;
	A_long err = i_pica_basicP->AcquireSuite(nameZ, versionL, &suiteP);

	if (err || !suiteP) {
		MissingSuiteError();
	}

	return (void*)suiteP;
}*/

// Free a particular suite. Ignore errors, because, what is there to be done if release fails?
void AEGP_SuiteHandler::ReleaseSuite(const A_char *nameZ, A_long versionL)
{
	i_pica_basicP->ReleaseSuite(nameZ, versionL);
}


```

---

<h2>Begin File: AEGP_SuiteHandler.h</h2>

```cpp
/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/



/** AEGP_SuiteHandler.h

	DEPRECATED:
		This way of doing things is out of date.  See AEFX_SuiteHandlerTemplate.h for the
		new way of doing things.

		-kjw 2/28/2014

	A very helpful class that manages demand loading and automatic, exception-safe freeing
	of AEGP suites.

	USAGE INSTRUCTIONS:

		The accompanying file, AEGP_SuiteHandler.cpp, is designed to be compiled right into
		the client application or plug-in.

		You'll get a link error.

		This is because AEGP_SuiteHandler.cpp lacks a definition for the MissingSuiteError()
		method. You must provide one to define the error handling behaviour of the class.
		This function may or may not display an error message etc. but it must end
		by throwing an exception. It cannot return.

		Other than that, usage is pretty trivial. Construct with a pointer to the PICA
		basic suite, and then call the public method to obtain lazily loaded pointers
		to other AEGP suites. Upon desctruction, all loaded suites are freed (so this class
		is really handy for writing exception-safe AEGP code.)

		NOTE!!!  If you need to upgrade a suite, DO NOT SIMPLY UPDATE THE VERSION NUMBER.
		You should:
			1.  Add a new member to the Suites structure for that suite.
			2.  Add the boiler plate macro to release the suite in ReleaseAllSuites (AEGP_SUITE_RELEASE_BOILERPLATE).
			3.  Add the boiler plate macro to define the suite. (AEGP_SUITE_ACCESS_BOILERPLATE)

		If you have any questions, ask me. -jja 5/7/2004

		If you'll be using ADM suites, #define I_NEED_ADM_SUPPORT before #including AEGP_SuiteHandler.h.

				-bbb 9/16/2004
**/

#ifndef _H_AEGP_SUITEHANDLER
#define _H_AEGP_SUITEHANDLER

#include <AE_GeneralPlug.h>
#include <AE_EffectSuites.h>
#include <AE_AdvEffectSuites.h>
#include <AE_EffectCBSuites.h>
#include <AE_EffectSuitesHelper.h>
#include <adobesdk/DrawbotSuite.h>
#include <SPSuites.h>

#ifdef	I_NEED_ADM_SUPPORT
#include <ADMBasic.h>
#include <ADMDialog.h>
#include <ADMDialogGroup.h>
#include <ADMItem.h>
#include <ADMList.h>
#include <ADMEntry.h>
#include <ADMNotifier.h>
#endif

// Suite registration and handling object
class AEGP_SuiteHandler {

private:
	// forbid copy construct
	AEGP_SuiteHandler(const AEGP_SuiteHandler&) {}
	AEGP_SuiteHandler& operator=(const AEGP_SuiteHandler&) { return *this; }

	// basic suite pointer
	const SPBasicSuite				*i_pica_basicP;

	// Suites we can register. These are mutable because they are demand loaded using a const object.

	struct Suites {
		AEGP_KeyframeSuite5			*keyframe_suite5P;
		AEGP_KeyframeSuite4			*keyframe_suite4P;
		AEGP_StreamSuite3			*stream_suite3P;
		AEGP_StreamSuite4			*stream_suite4P;
		AEGP_StreamSuite5			*stream_suite5P;
		AEGP_StreamSuite6			*stream_suite6P;
		AEGP_MarkerSuite1			*marker_suite1P;
		AEGP_MarkerSuite2			*marker_suite2P;
		AEGP_MarkerSuite3			*marker_suite3P;
		AEGP_CompSuite4				*comp_suite4P;
		AEGP_CompSuite5				*comp_suite5P;
		AEGP_CompSuite6				*comp_suite6P;
		AEGP_CompSuite7				*comp_suite7P;
		AEGP_CompSuite8				*comp_suite8P;
		AEGP_CompSuite9				*comp_suite9P;
		AEGP_CompSuite10			*comp_suite10P;
		AEGP_CompSuite11			*comp_suite11P;
		AEGP_LayerSuite3			*layer_suite3P;
		AEGP_LayerSuite4			*layer_suite4P;
		AEGP_StreamSuite2			*stream_suite2P;
		AEGP_DynamicStreamSuite2	*dynamic_stream_suite2P;
		AEGP_DynamicStreamSuite3	*dynamic_stream_suite3P;
		AEGP_DynamicStreamSuite4	*dynamic_stream_suite4P;
		AEGP_KeyframeSuite3			*keyframe_suite3P;
		AEGP_CanvasSuite5			*canvas_suite5P;
		AEGP_CanvasSuite6			*canvas_suite6P;
		AEGP_CanvasSuite7			*canvas_suite7P;
		AEGP_CanvasSuite8			*canvas_suite8P;
		AEGP_CameraSuite2			*camera_suite2P;
		AEGP_RegisterSuite5			*register_suite5P;
		AEGP_MemorySuite1			*memory_suite1P;
		AEGP_ItemViewSuite1			*item_view_suite1P;
		AEGP_ItemSuite9				*item_suite9P;
		AEGP_ItemSuite8				*item_suite8P;
		AEGP_ItemSuite7				*item_suite7P;
		AEGP_ItemSuite6				*item_suite6P;
		AEGP_ItemSuite5				*item_suite5P;
		AEGP_ItemSuite1				*item_suite1P;
		AEGP_LightSuite1			*light_suite1P;
		AEGP_LightSuite2			*light_suite2P;
		AEGP_EffectSuite1			*effect_suite1P;
		AEGP_EffectSuite2			*effect_suite2P;
		AEGP_EffectSuite3			*effect_suite3P;
		AEGP_EffectSuite4			*effect_suite4P;
		AEGP_MaskSuite4				*mask_suite4P;
		AEGP_MaskOutlineSuite1		*mask_outline_suite1P;
		AEGP_MaskOutlineSuite2		*mask_outline_suite2P;
		AEGP_MaskOutlineSuite3		*mask_outline_suite3P;
		AEGP_CommandSuite1			*command_suite1P;
		AEGP_UtilitySuite3			*utility_suite3P;
		AEGP_RenderSuite1			*render_suite1P;
		AEGP_RenderSuite2			*render_suite2P;
		AEGP_RenderSuite3			*render_suite3P;
		AEGP_RenderSuite4			*render_suite4P;
		AEGP_RenderSuite5			*render_suite5P;
		PF_ANSICallbacksSuite1		*ansi_callbacks_suite1P;
		PF_HandleSuite1				*handle_suite1P;
		PF_FillMatteSuite2			*fill_matte_suite2P;
		PF_WorldTransformSuite1		*world_transform_suite1P;
		AEGP_QueryXformSuite2		*query_xform_suite2P;
		AEGP_CompositeSuite2		*composite_suite2P;
		PF_WorldSuite1				*world_suite1P;
		AEGP_PFInterfaceSuite1		*pf_interface_suite1P;
		AEGP_MathSuite1			*math_suite1P;
		PF_AdvTimeSuite4			*adv_time_suite4P;
		PF_PathQuerySuite1			*path_query_suite1P;
		PF_PathDataSuite1			*path_data_suite1P;
		PF_ParamUtilsSuite3			*param_utils_suite3P;
		PFAppSuite4					*app_suite4P;
		PFAppSuite5					*app_suite5P;
		PFAppSuite6					*app_suite6P;
		PF_AdvAppSuite2				*adv_app_suite2P;
		AEGP_IOInSuite4				*io_in_suite4P;
		AEGP_IOOutSuite4			*io_out_suite4P;
		AEGP_PersistentDataSuite3	*persistent_data_suite3P;
		AEGP_PersistentDataSuite4	*persistent_data_suite4P;
		AEGP_RenderQueueSuite1		*render_queue_suite1P;
		AEGP_RQItemSuite2			*rq_item_suite2P;
		AEGP_OutputModuleSuite4		*output_module_suite4P;
		AEGP_FIMSuite3				*fim_suite3P;
		PF_Sampling8Suite1			*sampling_8_suite1P;
		PF_Sampling16Suite1			*sampling_16_suite1P;
		PF_Iterate8Suite1			*iterate_8_suite1P;
		PF_iterate16Suite1			*iterate_16_suite1P;
		PF_iterateFloatSuite1		*iterate_float_suite1P;
		PF_Iterate8Suite2			*iterate_8_suite2P;
		PF_iterate16Suite2			*iterate_16_suite2P;
		PF_iterateFloatSuite2		*iterate_float_suite2P;
		AEGP_CollectionSuite2		*collection_suite2P;
		AEGP_TextDocumentSuite1		*text_document_suite1P;
		AEGP_SoundDataSuite1		*sound_data_suite1P;
		AEGP_IterateSuite1			*aegp_iterate_suite1P;
		AEGP_IterateSuite2			*aegp_iterate_suite2P;
		AEGP_TextLayerSuite1		*text_layer_suite1P;
		AEGP_ArtisanUtilSuite1		*artisan_util_suite1P;
		AEGP_WorldSuite2			*aegp_world_suite_2P;
		AEGP_WorldSuite3			*aegp_world_suite_3P;
		AEGP_RenderOptionsSuite1	*render_options_suite_1P;
		AEGP_LayerRenderOptionsSuite1	*layer_render_options_suite_1P;
		AEGP_LayerRenderOptionsSuite2	*layer_render_options_suite_2P;
		AEGP_RenderAsyncManagerSuite1		*async_manager_suite_1P;
		AEGP_TrackerSuite1			*tracker_suite_1P;
		AEGP_TrackerUtilitySuite1	*tracker_utility_suite_1P;
		PF_HelperSuite2				*helper_suite_2P;
		AEGP_LayerSuite5			*layer_suite_5P;
		AEGP_LayerSuite6			*layer_suite_6P;
		AEGP_LayerSuite7			*layer_suite_7P;
		AEGP_LayerSuite8			*layer_suite_8P;
		AEGP_LayerSuite9			*layer_suite_9P;

#ifdef	I_NEED_ADM_SUPPORT
		ADMBasicSuite8				*adm_basic_suite_8P;
		ADMDialogSuite8				*adm_dialog_suite_8P;
		ADMDialogGroupSuite3		*adm_dialog_group_suite_3P;
		ADMItemSuite8				*adm_item_suite_8P;
		ADMListSuite3				*adm_list_suite_3P;
		ADMEntrySuite5				*adm_entry_suite_5P;
		ADMNotifierSuite2			*adm_notifier_suite_2P;
#endif
		AEGP_LayerSuite1			*layer_suite_1P;
		AEGP_MaskSuite1				*mask_suite_1P;
		AEGP_MaskSuite5				*mask_suite_5P;
		AEGP_MaskSuite6				*mask_suite_6P;
		AEGP_StreamSuite1			*stream_suite_1P;
		AEGP_CompSuite1				*comp_suite_1P;
		AEGP_CollectionSuite1		*collection_suite_1P;
		AEGP_KeyframeSuite1			*keyframe_suite_1P;
		PF_AdvAppSuite1				*adv_app_suite_1P;
		AEGP_UtilitySuite1			*utility_suite_1P;
		AEGP_RenderOptionsSuite2	*render_options_suite_2P;
		AEGP_ProjSuite5				*proj_suite_5P;
		AEGP_ProjSuite6				*proj_suite_6P;
		AEGP_FootageSuite5			*footage_suite_5P;
		AEGP_RQItemSuite3			*rq_item_suite_3P;
		AEGP_UtilitySuite4			*utility_suite_4P;
		AEGP_ColorSettingsSuite4	*color_settings_suite_4P;
		AEGP_ColorSettingsSuite3	*color_settings_suite_3P;
		AEGP_ColorSettingsSuite2	*color_settings_suite_2P;
		AEGP_ColorSettingsSuite1	*color_settings_suite_1P;
		PF_AdvItemSuite1			*adv_item_suite_1P;
		AEGP_RenderOptionsSuite3	*render_options_suite_3P;
		PF_ColorParamSuite1			*color_param_suite_1P;
		PF_SamplingFloatSuite1		*sampling_float_suite_1P;
		AEGP_UtilitySuite5			*utility_suite_5P;
		AEGP_UtilitySuite6			*utility_suite_6P;
		PF_EffectCustomUISuite1		*custom_ui_suite1P;
		PF_EffectCustomUISuite2		*custom_ui_suite2P;
		PF_EffectCustomUIOverlayThemeSuite1	*custom_ui_theme_suite1P;

		//Drawbot Suites
		DRAWBOT_DrawbotSuiteCurrent		*drawing_suite_currentP;
		DRAWBOT_SupplierSuiteCurrent	*drawbot_supplier_suite_currentP;
		DRAWBOT_SurfaceSuiteCurrent		*drawbot_surface_suite_currentP;
		DRAWBOT_PathSuiteCurrent		*drawbot_path_suite_currentP;

		SPSuitesSuite 					*suites_suite_2P;
	};

	mutable Suites isuites;

	// private methods
	// I had to make this inline by moving the definition from the .cpp file
	// CW mach-o target was freaking otherwise when seeing the call to this
	// function in inlined public suite accessors below

	void *LoadSuite(const A_char *nameZ, A_long versionL) const
	{
		const void *suiteP;
		A_long err = i_pica_basicP->AcquireSuite(nameZ, versionL, &suiteP);

		if (err || !suiteP) {
			MissingSuiteError();
		}

		return (void*)suiteP;
	}

	void ReleaseSuite(const A_char *nameZ, A_long versionL);
	void ReleaseAllSuites()
	{
		#define	AEGP_SUITE_RELEASE_BOILERPLATE(MEMBER_NAME, kSUITE_NAME, kVERSION_NAME)		\
			if (isuites.MEMBER_NAME) {														\
				ReleaseSuite(kSUITE_NAME, kVERSION_NAME);									\
			}

		AEGP_SUITE_RELEASE_BOILERPLATE(marker_suite1P, kAEGPMarkerSuite, kAEGPMarkerSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(marker_suite2P, kAEGPMarkerSuite, kAEGPMarkerSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(marker_suite3P, kAEGPMarkerSuite, kAEGPMarkerSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(layer_suite3P, kAEGPLayerSuite, kAEGPLayerSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(layer_suite4P, kAEGPLayerSuite, kAEGPLayerSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(stream_suite6P, kAEGPStreamSuite, kAEGPStreamSuiteVersion6);
		AEGP_SUITE_RELEASE_BOILERPLATE(stream_suite5P, kAEGPStreamSuite, kAEGPStreamSuiteVersion5);
		AEGP_SUITE_RELEASE_BOILERPLATE(stream_suite4P, kAEGPStreamSuite, kAEGPStreamSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(stream_suite3P, kAEGPStreamSuite, kAEGPStreamSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(stream_suite2P, kAEGPStreamSuite, kAEGPStreamSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(stream_suite_1P, kAEGPStreamSuite, kAEGPStreamSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(dynamic_stream_suite2P, kAEGPDynamicStreamSuite, kAEGPDynamicStreamSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(dynamic_stream_suite3P, kAEGPDynamicStreamSuite, kAEGPDynamicStreamSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(dynamic_stream_suite4P, kAEGPDynamicStreamSuite, kAEGPDynamicStreamSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(keyframe_suite5P, kAEGPKeyframeSuite, kAEGPKeyframeSuiteVersion5);
		AEGP_SUITE_RELEASE_BOILERPLATE(keyframe_suite4P, kAEGPKeyframeSuite, kAEGPKeyframeSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(keyframe_suite3P, kAEGPKeyframeSuite, kAEGPKeyframeSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(keyframe_suite_1P, kAEGPKeyframeSuite, kAEGPKeyframeSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(comp_suite4P, kAEGPCompSuite, kAEGPCompSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(comp_suite5P, kAEGPCompSuite, kAEGPCompSuiteVersion5);
		AEGP_SUITE_RELEASE_BOILERPLATE(comp_suite6P, kAEGPCompSuite, kAEGPCompSuiteVersion6);
		AEGP_SUITE_RELEASE_BOILERPLATE(comp_suite7P, kAEGPCompSuite, kAEGPCompSuiteVersion7);
		AEGP_SUITE_RELEASE_BOILERPLATE(comp_suite8P, kAEGPCompSuite, kAEGPCompSuiteVersion8);
		AEGP_SUITE_RELEASE_BOILERPLATE(comp_suite9P, kAEGPCompSuite, kAEGPCompSuiteVersion9);
		AEGP_SUITE_RELEASE_BOILERPLATE(comp_suite10P, kAEGPCompSuite, kAEGPCompSuiteVersion10);
		AEGP_SUITE_RELEASE_BOILERPLATE(comp_suite11P, kAEGPCompSuite, kAEGPCompSuiteVersion11);
		AEGP_SUITE_RELEASE_BOILERPLATE(canvas_suite5P, kAEGPCanvasSuite, kAEGPCanvasSuiteVersion5);
		AEGP_SUITE_RELEASE_BOILERPLATE(canvas_suite6P, kAEGPCanvasSuite, kAEGPCanvasSuiteVersion6);
		AEGP_SUITE_RELEASE_BOILERPLATE(canvas_suite7P, kAEGPCanvasSuite, kAEGPCanvasSuiteVersion7);
		AEGP_SUITE_RELEASE_BOILERPLATE(canvas_suite8P, kAEGPCanvasSuite, kAEGPCanvasSuiteVersion8);
		AEGP_SUITE_RELEASE_BOILERPLATE(camera_suite2P, kAEGPCameraSuite, kAEGPCameraSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(register_suite5P, kAEGPRegisterSuite, kAEGPRegisterSuiteVersion5);
		AEGP_SUITE_RELEASE_BOILERPLATE(item_view_suite1P, kAEGPItemViewSuite, kAEGPItemViewSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(item_suite8P, kAEGPItemSuite, kAEGPItemSuiteVersion8);
		AEGP_SUITE_RELEASE_BOILERPLATE(item_suite7P, kAEGPItemSuite, kAEGPItemSuiteVersion7);
		AEGP_SUITE_RELEASE_BOILERPLATE(item_suite6P, kAEGPItemSuite, kAEGPItemSuiteVersion6);
		AEGP_SUITE_RELEASE_BOILERPLATE(item_suite5P, kAEGPItemSuite, kAEGPItemSuiteVersion5);
		AEGP_SUITE_RELEASE_BOILERPLATE(item_suite1P, kAEGPItemSuite, kAEGPItemSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(pf_interface_suite1P, kAEGPPFInterfaceSuite, kAEGPPFInterfaceSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(math_suite1P, kAEGPMathSuite, kAEGPMathSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(adv_time_suite4P, kPFAdvTimeSuite, kPFAdvTimeSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(path_query_suite1P, kPFPathQuerySuite, kPFPathQuerySuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(memory_suite1P, kAEGPMemorySuite, kAEGPMemorySuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(path_data_suite1P, kPFPathDataSuite, kPFPathDataSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(param_utils_suite3P, kPFParamUtilsSuite, kPFParamUtilsSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(app_suite4P, kPFAppSuite, kPFAppSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(app_suite5P, kPFAppSuite, kPFAppSuiteVersion5);
		AEGP_SUITE_RELEASE_BOILERPLATE(app_suite6P, kPFAppSuite, kPFAppSuiteVersion6);
		AEGP_SUITE_RELEASE_BOILERPLATE(adv_app_suite2P, kPFAdvAppSuite, kPFAdvAppSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(light_suite1P, kAEGPLightSuite, kAEGPLightSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(light_suite2P, kAEGPLightSuite, kAEGPLightSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(effect_suite1P, kAEGPEffectSuite, kAEGPEffectSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(effect_suite2P, kAEGPEffectSuite, kAEGPEffectSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(effect_suite3P, kAEGPEffectSuite, kAEGPEffectSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(effect_suite4P, kAEGPEffectSuite, kAEGPEffectSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(mask_suite4P, kAEGPMaskSuite, kAEGPMaskSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(mask_outline_suite1P, kAEGPMaskOutlineSuite, kAEGPMaskOutlineSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(mask_outline_suite2P, kAEGPMaskOutlineSuite, kAEGPMaskOutlineSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(mask_outline_suite3P, kAEGPMaskOutlineSuite, kAEGPMaskOutlineSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(command_suite1P, kAEGPCommandSuite, kAEGPCommandSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(utility_suite3P, kAEGPUtilitySuite, kAEGPUtilitySuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(render_suite1P, kAEGPRenderSuite, kAEGPRenderSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(render_suite2P, kAEGPRenderSuite, kAEGPRenderSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(render_suite3P, kAEGPRenderSuite, kAEGPRenderSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(render_suite4P, kAEGPRenderSuite, kAEGPRenderSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(render_suite5P, kAEGPRenderSuite, kAEGPRenderSuiteVersion5);
		AEGP_SUITE_RELEASE_BOILERPLATE(ansi_callbacks_suite1P, kPFANSISuite, kPFANSISuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(handle_suite1P, kPFHandleSuite, kPFHandleSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(fill_matte_suite2P, kPFFillMatteSuite, kPFFillMatteSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(world_transform_suite1P, kPFWorldTransformSuite, kPFWorldTransformSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(query_xform_suite2P, kAEGPQueryXformSuite, kAEGPQueryXformSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(composite_suite2P, kAEGPCompositeSuite, kAEGPCompositeSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(world_suite1P, kPFWorldSuite, kPFWorldSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(io_in_suite4P, kAEGPIOInSuite, kAEGPIOInSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(io_out_suite4P, kAEGPIOOutSuite, kAEGPIOOutSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(render_queue_suite1P, kAEGPRenderQueueSuite, kAEGPRenderQueueSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(rq_item_suite2P, kAEGPRQItemSuite, kAEGPRQItemSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(output_module_suite4P, kAEGPOutputModuleSuite, kAEGPOutputModuleSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(fim_suite3P, kAEGPFIMSuite, kAEGPFIMSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(math_suite1P, kAEGPMathSuite, kAEGPMathSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(adv_time_suite4P, kPFAdvTimeSuite, kPFAdvTimeSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(sampling_8_suite1P, kPFSampling8Suite, kPFSampling8SuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(sampling_16_suite1P, kPFSampling16Suite, kPFSampling16SuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(iterate_8_suite1P, kPFIterate8Suite, kPFIterate8SuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(iterate_16_suite1P, kPFIterate16Suite, kPFIterate16SuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(iterate_float_suite1P, kPFIterateFloatSuite, kPFIterateFloatSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(iterate_8_suite2P, kPFIterate8Suite, kPFIterate8SuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(iterate_16_suite2P, kPFIterate16Suite, kPFIterate16SuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(iterate_float_suite2P, kPFIterateFloatSuite, kPFIterateFloatSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(collection_suite2P, kAEGPCollectionSuite, kAEGPCollectionSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(text_document_suite1P, kAEGPTextDocumentSuite, kAEGPTextDocumentSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(sound_data_suite1P, kAEGPSoundDataSuite, kAEGPSoundDataVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(text_layer_suite1P, kAEGPTextLayerSuite, kAEGPTextLayerSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(artisan_util_suite1P, kAEGPArtisanUtilSuite, kAEGPArtisanUtilSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(aegp_world_suite_2P, kAEGPWorldSuite, kAEGPWorldSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(aegp_world_suite_3P, kAEGPWorldSuite, kAEGPWorldSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(render_options_suite_1P, kAEGPRenderOptionsSuite, kAEGPRenderOptionsSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(tracker_suite_1P, kAEGPTrackerSuite, kAEGPTrackerSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(tracker_utility_suite_1P, kAEGPTrackerUtilitySuite, kAEGPTrackerUtilitySuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(helper_suite_2P, kPFHelperSuite2, kPFHelperSuite2Version2);
		AEGP_SUITE_RELEASE_BOILERPLATE(layer_suite_5P, kAEGPLayerSuite, kAEGPLayerSuiteVersion5);
		AEGP_SUITE_RELEASE_BOILERPLATE(layer_suite_6P, kAEGPLayerSuite, kAEGPLayerSuiteVersion6);
		AEGP_SUITE_RELEASE_BOILERPLATE(layer_suite_7P, kAEGPLayerSuite, kAEGPLayerSuiteVersion7);
		AEGP_SUITE_RELEASE_BOILERPLATE(layer_suite_8P, kAEGPLayerSuite, kAEGPLayerSuiteVersion8);
		AEGP_SUITE_RELEASE_BOILERPLATE(adv_item_suite_1P, kPFAdvItemSuite, kPFAdvItemSuiteVersion1);
#ifdef	I_NEED_ADM_SUPPORT
		AEGP_SUITE_RELEASE_BOILERPLATE(adm_basic_suite_8P, kADMBasicSuite, kADMBasicSuiteVersion8);
		AEGP_SUITE_RELEASE_BOILERPLATE(adm_dialog_suite_8P, kADMDialogSuite, kADMDialogSuiteVersion8);
		AEGP_SUITE_RELEASE_BOILERPLATE(adm_dialog_group_suite_3P, kADMDialogGroupSuite, kADMDialogGroupSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(adm_item_suite_8P, kADMItemSuite, kADMItemSuiteVersion8);
		AEGP_SUITE_RELEASE_BOILERPLATE(adm_list_suite_3P, kADMListSuite, kADMListSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(adm_entry_suite_5P, kADMEntrySuite, kADMEntrySuiteVersion5);
		AEGP_SUITE_RELEASE_BOILERPLATE(adm_notifier_suite_2P, kADMNotifierSuite, kADMNotifierSuiteVersion2);
#endif
		AEGP_SUITE_RELEASE_BOILERPLATE(layer_suite_1P, kAEGPLayerSuite, kAEGPLayerSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(mask_suite_1P, kAEGPMaskSuite, kAEGPMaskSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(mask_suite_5P, kAEGPMaskSuite, kAEGPMaskSuiteVersion5);
		AEGP_SUITE_RELEASE_BOILERPLATE(mask_suite_6P, kAEGPMaskSuite, kAEGPMaskSuiteVersion6);
		AEGP_SUITE_RELEASE_BOILERPLATE(comp_suite_1P, kAEGPCompSuite, kAEGPCompSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(collection_suite_1P, kAEGPCollectionSuite, kAEGPCollectionSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(adv_app_suite_1P, kPFAdvAppSuite, kPFAdvAppSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(utility_suite_1P, kAEGPUtilitySuite, kAEGPUtilitySuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(render_options_suite_2P, kAEGPRenderOptionsSuite, kAEGPRenderOptionsSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(layer_render_options_suite_1P, kAEGPLayerRenderOptionsSuite, kAEGPLayerRenderOptionsSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(layer_render_options_suite_2P, kAEGPLayerRenderOptionsSuite, kAEGPLayerRenderOptionsSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(async_manager_suite_1P, kAEGPRenderAsyncManagerSuite, kAEGPRenderAsyncManagerSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(proj_suite_5P, kAEGPProjSuite, kAEGPProjSuiteVersion5);
		AEGP_SUITE_RELEASE_BOILERPLATE(proj_suite_6P, kAEGPProjSuite, kAEGPProjSuiteVersion6);
		AEGP_SUITE_RELEASE_BOILERPLATE(footage_suite_5P, kAEGPFootageSuite, kAEGPFootageSuiteVersion5);
		AEGP_SUITE_RELEASE_BOILERPLATE(rq_item_suite_3P, kAEGPRQItemSuite, kAEGPRQItemSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(utility_suite_4P, kAEGPUtilitySuite, kAEGPUtilitySuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(persistent_data_suite4P, kAEGPPersistentDataSuite, kAEGPPersistentDataSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(persistent_data_suite3P, kAEGPPersistentDataSuite, kAEGPPersistentDataSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(color_settings_suite_4P, kAEGPColorSettingsSuite, kAEGPColorSettingsSuiteVersion4);
		AEGP_SUITE_RELEASE_BOILERPLATE(color_settings_suite_3P, kAEGPColorSettingsSuite, kAEGPColorSettingsSuiteVersion3);
		AEGP_SUITE_RELEASE_BOILERPLATE(color_settings_suite_2P, kAEGPColorSettingsSuite, kAEGPColorSettingsSuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(color_settings_suite_1P, kAEGPColorSettingsSuite, kAEGPColorSettingsSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(color_param_suite_1P, kPFColorParamSuite, kPFColorParamSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(sampling_float_suite_1P, kPFSamplingFloatSuite, kPFSamplingFloatSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(utility_suite_5P, kAEGPUtilitySuite, kAEGPUtilitySuiteVersion5);
		AEGP_SUITE_RELEASE_BOILERPLATE(utility_suite_6P, kAEGPUtilitySuite, kAEGPUtilitySuiteVersion6);
		AEGP_SUITE_RELEASE_BOILERPLATE(custom_ui_suite1P, kPFEffectCustomUISuite, kPFEffectCustomUISuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(custom_ui_suite2P, kPFEffectCustomUISuite, kPFEffectCustomUISuiteVersion2);
		AEGP_SUITE_RELEASE_BOILERPLATE(custom_ui_theme_suite1P, kPFEffectCustomUIOverlayThemeSuite, kPFEffectCustomUIOverlayThemeSuiteVersion1);
		AEGP_SUITE_RELEASE_BOILERPLATE(drawing_suite_currentP, kDRAWBOT_DrawSuite, kDRAWBOT_DrawSuite_VersionCurrent);
		AEGP_SUITE_RELEASE_BOILERPLATE(drawbot_supplier_suite_currentP, kDRAWBOT_SupplierSuite, kDRAWBOT_SupplierSuite_VersionCurrent);
		AEGP_SUITE_RELEASE_BOILERPLATE(drawbot_surface_suite_currentP, kDRAWBOT_SurfaceSuite, kDRAWBOT_SurfaceSuite_VersionCurrent);
		AEGP_SUITE_RELEASE_BOILERPLATE(drawbot_path_suite_currentP, kDRAWBOT_PathSuite, kDRAWBOT_PathSuite_VersionCurrent);
		AEGP_SUITE_RELEASE_BOILERPLATE(suites_suite_2P, kSPSuitesSuite, kSPSuitesSuiteVersion);
}

	// Here is the error handling function which must be defined.
	// It must exit by throwing an exception, it cannot return.
	void MissingSuiteError() const;

public:
	// To construct, pass pica_basicP
	AEGP_SuiteHandler(const SPBasicSuite *pica_basicP);
	~AEGP_SuiteHandler();

	const SPBasicSuite *Pica()	const	{ return i_pica_basicP; }

	#define	AEGP_SUITE_ACCESS_BOILERPLATE(SUITE_NAME, VERSION_NUMBER, SUITE_PREFIX, MEMBER_NAME, kSUITE_NAME, kVERSION_NAME)	\
		SUITE_PREFIX##SUITE_NAME##VERSION_NUMBER *SUITE_NAME##VERSION_NUMBER() const											\
	{																															\
		if (isuites.MEMBER_NAME == NULL) {																						\
			isuites.MEMBER_NAME = (SUITE_PREFIX##SUITE_NAME##VERSION_NUMBER*)													\
										LoadSuite(kSUITE_NAME, kVERSION_NAME);													\
		}																														\
		return isuites.MEMBER_NAME;																							\
	}

	AEGP_SUITE_ACCESS_BOILERPLATE(MarkerSuite, 1, AEGP_, marker_suite1P, kAEGPMarkerSuite, kAEGPMarkerSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(MarkerSuite, 2, AEGP_, marker_suite2P, kAEGPMarkerSuite, kAEGPMarkerSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(MarkerSuite, 3, AEGP_, marker_suite3P, kAEGPMarkerSuite, kAEGPMarkerSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(LayerSuite, 3, AEGP_, layer_suite3P, kAEGPLayerSuite, kAEGPLayerSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(LayerSuite, 4, AEGP_, layer_suite4P, kAEGPLayerSuite, kAEGPLayerSuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(StreamSuite, 6, AEGP_, stream_suite6P, kAEGPStreamSuite, kAEGPStreamSuiteVersion6);
	AEGP_SUITE_ACCESS_BOILERPLATE(StreamSuite, 5, AEGP_, stream_suite5P, kAEGPStreamSuite, kAEGPStreamSuiteVersion5);
	AEGP_SUITE_ACCESS_BOILERPLATE(StreamSuite, 4, AEGP_, stream_suite4P, kAEGPStreamSuite, kAEGPStreamSuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(StreamSuite, 3, AEGP_, stream_suite3P, kAEGPStreamSuite, kAEGPStreamSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(StreamSuite, 2, AEGP_, stream_suite2P, kAEGPStreamSuite, kAEGPStreamSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(StreamSuite, 1, AEGP_, stream_suite_1P, kAEGPStreamSuite, kAEGPStreamSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(DynamicStreamSuite, 2, AEGP_, dynamic_stream_suite2P, kAEGPDynamicStreamSuite, kAEGPDynamicStreamSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(DynamicStreamSuite, 3, AEGP_, dynamic_stream_suite3P, kAEGPDynamicStreamSuite, kAEGPDynamicStreamSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(DynamicStreamSuite, 4, AEGP_, dynamic_stream_suite4P, kAEGPDynamicStreamSuite, kAEGPDynamicStreamSuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(KeyframeSuite, 5, AEGP_, keyframe_suite5P, kAEGPKeyframeSuite, kAEGPKeyframeSuiteVersion5);
	AEGP_SUITE_ACCESS_BOILERPLATE(KeyframeSuite, 4, AEGP_, keyframe_suite4P, kAEGPKeyframeSuite, kAEGPKeyframeSuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(KeyframeSuite, 3, AEGP_, keyframe_suite3P, kAEGPKeyframeSuite, kAEGPKeyframeSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(KeyframeSuite, 1, AEGP_, keyframe_suite_1P, kAEGPKeyframeSuite, kAEGPKeyframeSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(CompSuite, 4, AEGP_, comp_suite4P, kAEGPCompSuite, kAEGPCompSuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(CompSuite, 5, AEGP_, comp_suite5P, kAEGPCompSuite, kAEGPCompSuiteVersion5);
	AEGP_SUITE_ACCESS_BOILERPLATE(CompSuite, 6, AEGP_, comp_suite6P, kAEGPCompSuite, kAEGPCompSuiteVersion6);
	AEGP_SUITE_ACCESS_BOILERPLATE(CompSuite, 7, AEGP_, comp_suite7P, kAEGPCompSuite, kAEGPCompSuiteVersion7);
	AEGP_SUITE_ACCESS_BOILERPLATE(CompSuite, 8, AEGP_, comp_suite8P, kAEGPCompSuite, kAEGPCompSuiteVersion8);
	AEGP_SUITE_ACCESS_BOILERPLATE(CompSuite, 9, AEGP_, comp_suite9P, kAEGPCompSuite, kAEGPCompSuiteVersion9);
	AEGP_SUITE_ACCESS_BOILERPLATE(CompSuite, 10, AEGP_, comp_suite10P, kAEGPCompSuite, kAEGPCompSuiteVersion10);
	AEGP_SUITE_ACCESS_BOILERPLATE(CompSuite, 11, AEGP_, comp_suite11P, kAEGPCompSuite, kAEGPCompSuiteVersion11);
	AEGP_SUITE_ACCESS_BOILERPLATE(CanvasSuite, 5, AEGP_, canvas_suite5P, kAEGPCanvasSuite, kAEGPCanvasSuiteVersion5);
	AEGP_SUITE_ACCESS_BOILERPLATE(CanvasSuite, 6, AEGP_, canvas_suite6P, kAEGPCanvasSuite, kAEGPCanvasSuiteVersion6);
	AEGP_SUITE_ACCESS_BOILERPLATE(CanvasSuite, 7, AEGP_, canvas_suite7P, kAEGPCanvasSuite, kAEGPCanvasSuiteVersion7);
	AEGP_SUITE_ACCESS_BOILERPLATE(CanvasSuite, 8, AEGP_, canvas_suite8P, kAEGPCanvasSuite, kAEGPCanvasSuiteVersion8);
	AEGP_SUITE_ACCESS_BOILERPLATE(CameraSuite, 2, AEGP_, camera_suite2P, kAEGPCameraSuite, kAEGPCameraSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(RegisterSuite, 5, AEGP_, register_suite5P, kAEGPRegisterSuite, kAEGPRegisterSuiteVersion5);
	AEGP_SUITE_ACCESS_BOILERPLATE(MemorySuite, 1, AEGP_, memory_suite1P, kAEGPMemorySuite, kAEGPMemorySuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(ItemViewSuite, 1, AEGP_, item_view_suite1P, kAEGPItemViewSuite, kAEGPItemViewSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(ItemSuite, 9, AEGP_, item_suite9P, kAEGPItemSuite, kAEGPItemSuiteVersion9);
	AEGP_SUITE_ACCESS_BOILERPLATE(ItemSuite, 8, AEGP_, item_suite8P, kAEGPItemSuite, kAEGPItemSuiteVersion8);
	AEGP_SUITE_ACCESS_BOILERPLATE(ItemSuite, 7, AEGP_, item_suite7P, kAEGPItemSuite, kAEGPItemSuiteVersion7);
	AEGP_SUITE_ACCESS_BOILERPLATE(ItemSuite, 6, AEGP_, item_suite6P, kAEGPItemSuite, kAEGPItemSuiteVersion6);
	AEGP_SUITE_ACCESS_BOILERPLATE(ItemSuite, 5, AEGP_, item_suite5P, kAEGPItemSuite, kAEGPItemSuiteVersion5);
	AEGP_SUITE_ACCESS_BOILERPLATE(ItemSuite, 1, AEGP_, item_suite1P, kAEGPItemSuite, kAEGPItemSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(PFInterfaceSuite, 1, AEGP_, pf_interface_suite1P, kAEGPPFInterfaceSuite, kAEGPPFInterfaceSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(MathSuite, 1, AEGP_, math_suite1P, kAEGPMathSuite, kAEGPMathSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(AdvTimeSuite, 4, PF_, adv_time_suite4P, kPFAdvTimeSuite, kPFAdvTimeSuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(PathQuerySuite, 1, PF_, path_query_suite1P, kPFPathQuerySuite, kPFPathQuerySuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(PathDataSuite, 1, PF_, path_data_suite1P, kPFPathDataSuite, kPFPathDataSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(ParamUtilsSuite, 3, PF_, param_utils_suite3P, kPFParamUtilsSuite, kPFParamUtilsSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(AppSuite, 4, PF, app_suite4P, kPFAppSuite, kPFAppSuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(AppSuite, 5, PF, app_suite5P, kPFAppSuite, kPFAppSuiteVersion5);
	AEGP_SUITE_ACCESS_BOILERPLATE(AppSuite, 6, PF, app_suite6P, kPFAppSuite, kPFAppSuiteVersion6);
	AEGP_SUITE_ACCESS_BOILERPLATE(AdvAppSuite, 2, PF_, adv_app_suite2P, kPFAdvAppSuite, kPFAdvAppSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(LightSuite, 1, AEGP_, light_suite1P, kAEGPLightSuite, kAEGPLightSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(LightSuite, 2, AEGP_, light_suite2P, kAEGPLightSuite, kAEGPLightSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(EffectSuite, 1, AEGP_, effect_suite1P, kAEGPEffectSuite, kAEGPEffectSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(EffectSuite, 2, AEGP_, effect_suite2P, kAEGPEffectSuite, kAEGPEffectSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(EffectSuite, 3, AEGP_, effect_suite3P, kAEGPEffectSuite, kAEGPEffectSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(EffectSuite, 4, AEGP_, effect_suite4P, kAEGPEffectSuite, kAEGPEffectSuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(MaskSuite, 4, AEGP_, mask_suite4P, kAEGPMaskSuite, kAEGPMaskSuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(MaskOutlineSuite, 1, AEGP_, mask_outline_suite1P, kAEGPMaskOutlineSuite, kAEGPMaskOutlineSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(MaskOutlineSuite, 2, AEGP_, mask_outline_suite2P, kAEGPMaskOutlineSuite, kAEGPMaskOutlineSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(MaskOutlineSuite, 3, AEGP_, mask_outline_suite3P, kAEGPMaskOutlineSuite, kAEGPMaskOutlineSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(CommandSuite, 1, AEGP_, command_suite1P, kAEGPCommandSuite, kAEGPCommandSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(UtilitySuite, 3, AEGP_, utility_suite3P, kAEGPUtilitySuite, kAEGPUtilitySuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(RenderSuite, 1, AEGP_, render_suite1P, kAEGPRenderSuite, kAEGPRenderSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(RenderSuite, 2, AEGP_, render_suite2P, kAEGPRenderSuite, kAEGPRenderSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(RenderSuite, 3, AEGP_, render_suite3P, kAEGPRenderSuite, kAEGPRenderSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(RenderSuite, 4, AEGP_, render_suite4P, kAEGPRenderSuite, kAEGPRenderSuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(RenderSuite, 5, AEGP_, render_suite5P, kAEGPRenderSuite, kAEGPRenderSuiteVersion5);
	AEGP_SUITE_ACCESS_BOILERPLATE(ANSICallbacksSuite, 1, PF_, ansi_callbacks_suite1P, kPFANSISuite, kPFANSISuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(HandleSuite, 1, PF_, handle_suite1P, kPFHandleSuite, kPFHandleSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(FillMatteSuite, 2, PF_, fill_matte_suite2P, kPFFillMatteSuite, kPFFillMatteSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(WorldTransformSuite, 1, PF_, world_transform_suite1P, kPFWorldTransformSuite, kPFWorldTransformSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(QueryXformSuite, 2, AEGP_, query_xform_suite2P, kAEGPQueryXformSuite, kAEGPQueryXformSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(CompositeSuite, 2, AEGP_, composite_suite2P, kAEGPCompositeSuite, kAEGPCompositeSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(WorldSuite, 1, PF_, world_suite1P, kPFWorldSuite, kPFWorldSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(IOInSuite, 4, AEGP_, io_in_suite4P, kAEGPIOInSuite, kAEGPIOInSuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(IOOutSuite, 4, AEGP_, io_out_suite4P, kAEGPIOOutSuite, kAEGPIOOutSuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(RenderQueueSuite, 1, AEGP_, render_queue_suite1P, kAEGPRenderQueueSuite, kAEGPRenderQueueSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(RQItemSuite, 2, AEGP_, rq_item_suite2P, kAEGPRQItemSuite, kAEGPRQItemSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(OutputModuleSuite, 4, AEGP_, output_module_suite4P, kAEGPOutputModuleSuite, kAEGPOutputModuleSuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(FIMSuite, 3, AEGP_, fim_suite3P, kAEGPFIMSuite, kAEGPFIMSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(Sampling8Suite, 1, PF_, sampling_8_suite1P, kPFSampling8Suite, kPFSampling8SuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(Sampling16Suite, 1, PF_, sampling_16_suite1P, kPFSampling16Suite, kPFSampling16SuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(Iterate8Suite, 1, PF_, iterate_8_suite1P, kPFIterate8Suite, kPFIterate8SuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(Iterate16Suite, 1, PF_, iterate_16_suite1P, kPFIterate16Suite, kPFIterate16SuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(IterateFloatSuite, 1, PF_, iterate_float_suite1P, kPFIterateFloatSuite, kPFIterateFloatSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(Iterate8Suite, 2, PF_, iterate_8_suite2P, kPFIterate8Suite, kPFIterate8SuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(Iterate16Suite, 2, PF_, iterate_16_suite2P, kPFIterate16Suite, kPFIterate16SuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(IterateFloatSuite, 2, PF_, iterate_float_suite2P, kPFIterateFloatSuite, kPFIterateFloatSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(CollectionSuite, 2, AEGP_, collection_suite2P, kAEGPCollectionSuite, kAEGPCollectionSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(TextDocumentSuite, 1, AEGP_, text_document_suite1P, kAEGPTextDocumentSuite, kAEGPTextDocumentSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(SoundDataSuite, 1, AEGP_, sound_data_suite1P, kAEGPSoundDataSuite, kAEGPSoundDataVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(IterateSuite, 1, AEGP_, aegp_iterate_suite1P, kAEGPIterateSuite, kAEGPIterateSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(IterateSuite, 2, AEGP_, aegp_iterate_suite2P, kAEGPIterateSuite, kAEGPIterateSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(TextLayerSuite, 1, AEGP_, text_layer_suite1P, kAEGPTextLayerSuite, kAEGPTextLayerSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(ArtisanUtilSuite, 1, AEGP_, artisan_util_suite1P, kAEGPArtisanUtilSuite, kAEGPArtisanUtilSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(WorldSuite, 2, AEGP_, aegp_world_suite_2P, kAEGPWorldSuite, kAEGPWorldSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(WorldSuite, 3, AEGP_, aegp_world_suite_3P, kAEGPWorldSuite, kAEGPWorldSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(RenderOptionsSuite, 1, AEGP_, render_options_suite_1P, kAEGPRenderOptionsSuite, kAEGPRenderOptionsSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(TrackerSuite, 1, AEGP_, tracker_suite_1P, kAEGPTrackerSuite, kAEGPTrackerSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(TrackerUtilitySuite, 1, AEGP_, tracker_utility_suite_1P, kAEGPTrackerUtilitySuite, kAEGPTrackerUtilitySuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(HelperSuite, 2, PF_, helper_suite_2P, kPFHelperSuite2, kPFHelperSuite2Version2);
	AEGP_SUITE_ACCESS_BOILERPLATE(LayerSuite, 5, AEGP_, layer_suite_5P, kAEGPLayerSuite, kAEGPLayerSuiteVersion5);
	AEGP_SUITE_ACCESS_BOILERPLATE(LayerSuite, 6, AEGP_, layer_suite_6P, kAEGPLayerSuite, kAEGPLayerSuiteVersion6);
	AEGP_SUITE_ACCESS_BOILERPLATE(LayerSuite, 7, AEGP_, layer_suite_7P, kAEGPLayerSuite, kAEGPLayerSuiteVersion7);
	AEGP_SUITE_ACCESS_BOILERPLATE(LayerSuite, 8, AEGP_, layer_suite_8P, kAEGPLayerSuite, kAEGPLayerSuiteVersion8);
	AEGP_SUITE_ACCESS_BOILERPLATE(LayerSuite, 9, AEGP_, layer_suite_9P, kAEGPLayerSuite, kAEGPLayerSuiteVersion9);
#ifdef	I_NEED_ADM_SUPPORT
	AEGP_SUITE_ACCESS_BOILERPLATE(BasicSuite, 8, ADM, adm_basic_suite_8P, kADMBasicSuite, kADMBasicSuiteVersion8);
	AEGP_SUITE_ACCESS_BOILERPLATE(DialogSuite, 8, ADM, adm_dialog_suite_8P, kADMDialogSuite, kADMDialogSuiteVersion8);
	AEGP_SUITE_ACCESS_BOILERPLATE(DialogGroupSuite, 3, ADM, adm_dialog_group_suite_3P, kADMDialogGroupSuite, kADMDialogGroupSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(ItemSuite, 8, ADM, adm_item_suite_8P, kADMItemSuite, kADMItemSuiteVersion8);
	AEGP_SUITE_ACCESS_BOILERPLATE(ListSuite, 3, ADM, adm_list_suite_3P, kADMListSuite, kADMListSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(EntrySuite, 5, ADM, adm_entry_suite_5P, kADMEntrySuite, kADMEntrySuiteVersion5);
	AEGP_SUITE_ACCESS_BOILERPLATE(NotifierSuite, 2, ADM, adm_notifier_suite_2P, kADMNotifierSuite, kADMNotifierSuiteVersion2);
#endif
	AEGP_SUITE_ACCESS_BOILERPLATE(LayerSuite, 1, AEGP_, layer_suite_1P, kAEGPLayerSuite, kAEGPLayerSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(AdvItemSuite, 1, PF_, adv_item_suite_1P, kPFAdvItemSuite, kPFAdvItemSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(MaskSuite, 1, AEGP_, mask_suite_1P, kAEGPMaskSuite, kAEGPMaskSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(MaskSuite, 5, AEGP_, mask_suite_5P, kAEGPMaskSuite, kAEGPMaskSuiteVersion5);
	AEGP_SUITE_ACCESS_BOILERPLATE(MaskSuite, 6, AEGP_, mask_suite_6P, kAEGPMaskSuite, kAEGPMaskSuiteVersion6);
	AEGP_SUITE_ACCESS_BOILERPLATE(CompSuite, 1, AEGP_, comp_suite_1P, kAEGPCompSuite, kAEGPCompSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(CollectionSuite, 1, AEGP_, collection_suite_1P, kAEGPCollectionSuite, kAEGPCollectionSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(AdvAppSuite, 1, PF_, adv_app_suite_1P, kPFAdvAppSuite, kPFAdvAppSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(UtilitySuite, 1, AEGP_, utility_suite_1P, kAEGPUtilitySuite, kAEGPUtilitySuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(RenderOptionsSuite, 2, AEGP_, render_options_suite_2P, kAEGPRenderOptionsSuite, kAEGPRenderOptionsSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(RenderOptionsSuite, 3, AEGP_, render_options_suite_3P, kAEGPRenderOptionsSuite, kAEGPRenderOptionsSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(LayerRenderOptionsSuite, 1, AEGP_, layer_render_options_suite_1P, kAEGPLayerRenderOptionsSuite, kAEGPLayerRenderOptionsSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(LayerRenderOptionsSuite, 2, AEGP_, layer_render_options_suite_2P, kAEGPLayerRenderOptionsSuite, kAEGPLayerRenderOptionsSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(RenderAsyncManagerSuite, 1, AEGP_, async_manager_suite_1P, kAEGPRenderAsyncManagerSuite, kAEGPRenderAsyncManagerSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(ProjSuite, 5, AEGP_, proj_suite_5P, kAEGPProjSuite, kAEGPProjSuiteVersion5);
	AEGP_SUITE_ACCESS_BOILERPLATE(ProjSuite, 6, AEGP_, proj_suite_6P, kAEGPProjSuite, kAEGPProjSuiteVersion6);
	AEGP_SUITE_ACCESS_BOILERPLATE(FootageSuite, 5, AEGP_, footage_suite_5P, kAEGPFootageSuite, kAEGPFootageSuiteVersion5);
	AEGP_SUITE_ACCESS_BOILERPLATE(RQItemSuite, 3, AEGP_, rq_item_suite_3P, kAEGPRQItemSuite, kAEGPRQItemSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(UtilitySuite, 4, AEGP_, utility_suite_4P, kAEGPUtilitySuite, kAEGPUtilitySuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(ColorSettingsSuite, 4, AEGP_, color_settings_suite_4P, kAEGPColorSettingsSuite, kAEGPColorSettingsSuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(ColorSettingsSuite, 3, AEGP_, color_settings_suite_3P, kAEGPColorSettingsSuite, kAEGPColorSettingsSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(ColorSettingsSuite, 2, AEGP_, color_settings_suite_2P, kAEGPColorSettingsSuite, kAEGPColorSettingsSuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(ColorSettingsSuite, 1, AEGP_, color_settings_suite_1P, kAEGPColorSettingsSuite, kAEGPColorSettingsSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(ColorParamSuite, 1, PF_, color_param_suite_1P, kPFColorParamSuite, kPFColorParamSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(PersistentDataSuite, 4, AEGP_, persistent_data_suite4P, kAEGPPersistentDataSuite, kAEGPPersistentDataSuiteVersion4);
	AEGP_SUITE_ACCESS_BOILERPLATE(PersistentDataSuite, 3, AEGP_, persistent_data_suite3P, kAEGPPersistentDataSuite, kAEGPPersistentDataSuiteVersion3);
	AEGP_SUITE_ACCESS_BOILERPLATE(SamplingFloatSuite, 1, PF_, sampling_float_suite_1P, kPFSamplingFloatSuite, kPFSamplingFloatSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(UtilitySuite, 5, AEGP_, utility_suite_5P, kAEGPUtilitySuite, kAEGPUtilitySuiteVersion5);
	AEGP_SUITE_ACCESS_BOILERPLATE(UtilitySuite, 6, AEGP_, utility_suite_6P, kAEGPUtilitySuite, kAEGPUtilitySuiteVersion6);
	AEGP_SUITE_ACCESS_BOILERPLATE(EffectCustomUISuite, 1, PF_, custom_ui_suite1P, kPFEffectCustomUISuite, kPFEffectCustomUISuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(EffectCustomUISuite, 2, PF_, custom_ui_suite2P, kPFEffectCustomUISuite, kPFEffectCustomUISuiteVersion2);
	AEGP_SUITE_ACCESS_BOILERPLATE(EffectCustomUIOverlayThemeSuite, 1, PF_, custom_ui_theme_suite1P, kPFEffectCustomUIOverlayThemeSuite, kPFEffectCustomUIOverlayThemeSuiteVersion1);
	AEGP_SUITE_ACCESS_BOILERPLATE(DrawbotSuite, Current, DRAWBOT_, drawing_suite_currentP, kDRAWBOT_DrawSuite, kDRAWBOT_DrawSuite_VersionCurrent);
	AEGP_SUITE_ACCESS_BOILERPLATE(SupplierSuite, Current, DRAWBOT_, drawbot_supplier_suite_currentP, kDRAWBOT_SupplierSuite, kDRAWBOT_SupplierSuite_VersionCurrent);
	AEGP_SUITE_ACCESS_BOILERPLATE(SurfaceSuite, Current, DRAWBOT_, drawbot_surface_suite_currentP, kDRAWBOT_SurfaceSuite, kDRAWBOT_SurfaceSuite_VersionCurrent);
	AEGP_SUITE_ACCESS_BOILERPLATE(PathSuite, Current, DRAWBOT_, drawbot_path_suite_currentP, kDRAWBOT_PathSuite, kDRAWBOT_PathSuite_VersionCurrent);

	AEGP_SUITE_ACCESS_BOILERPLATE(SuitesSuite, , SP, suites_suite_2P, kSPSuitesSuite, kSPSuitesSuiteVersion);
};

#endif

```

---

<h2>Begin File: AEGP_Utils.cpp</h2>

```cpp
#include "AEGP_Utils.h"


A_Err GetNewFirstLayerInFirstComp(
								  SPBasicSuite		*sP,
								  AEGP_LayerH			*first_layerPH)
{
	A_Err err = A_Err_NONE;
	
	AEGP_ItemH			itemH					= NULL;
	AEGP_ItemType		type					= AEGP_ItemType_NONE;
	AEGP_CompH			compH					= NULL;
	AEGP_ProjectH		projH					= NULL;
	A_long				num_projectsL			= 0,
	num_layersL				= 0;
	
	AEGP_SuiteHandler	suites(sP);
	
	ERR(suites.ProjSuite5()->AEGP_GetProjectByIndex(0, &projH));
	ERR(suites.ItemSuite8()->AEGP_GetFirstProjItem(projH, &itemH));
	ERR(suites.ItemSuite6()->AEGP_GetItemType(itemH, &type));
	
	while ((itemH != NULL) && (type != AEGP_ItemType_COMP)){
		ERR(suites.ItemSuite6()->AEGP_GetNextProjItem(projH, itemH, &itemH));
		ERR(suites.ItemSuite6()->AEGP_GetItemType(itemH, &type));
	}
	if (!err && (type == AEGP_ItemType_COMP)){
		err = suites.CompSuite4()->AEGP_GetCompFromItem(itemH, &compH);
	}
	if (!err && compH) {
		err = suites.LayerSuite5()->AEGP_GetCompNumLayers(compH, &num_layersL);
	}
	if (!err && num_layersL){
		err = suites.LayerSuite5()->AEGP_GetCompLayerByIndex(compH, 0, first_layerPH);
	}
	return err;
}
```

---

<h2>Begin File: AEGP_Utils.h</h2>

```cpp
#include "A.h"
#include "AE_GeneralPlug.h"
#include "AEGP_SuiteHandler.h"
#include "AE_Macros.h"

A_Err GetNewFirstLayerInFirstComp(
								  SPBasicSuite		*sP,
								  AEGP_LayerH			*first_layerPH);


```

---

<h2>Begin File: DuckSuite.h</h2>

```cpp
#include "A.h"
#include <SPTypes.h>

#ifdef AE_OS_WIN
	#include <windows.h>
#endif

#define kDuckSuite1				"AEGP Duck Suite"
#define kDuckSuiteVersion1		1

typedef struct DuckSuite1 {
	SPAPI A_Err	(*Quack)(A_u_short	timesSu);
} DuckSuite1;
```

---

<h2>Begin File: entry.h</h2>

```cpp
/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007-2023 Adobe Inc.                                  */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Inc. and its suppliers, if                    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Inc. and its                    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Inc.            */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

/*
	Entry.h
	
	Part of the Adobe After Effects SDK.
*/

#include "AE_PluginData.h"

#ifdef AE_OS_WIN
	#define DllExport   __declspec( dllexport )
#elif defined AE_OS_MAC
	#define DllExport   __attribute__ ((visibility ("default")))
#endif


#define AE_ENTRY_POINT			 "EffectMain"
#define AE_RESERVED_INFO		 8

#define PF_REGISTER_EFFECT(INPTR, CBPTR, NAME, MATCHNAME, CATEGORY,RESERVEDINFO)  \
       result = (*(CBPTR))((INPTR),\
				reinterpret_cast<const A_u_char*>(NAME),\
				reinterpret_cast<const A_u_char*>(MATCHNAME),\
				reinterpret_cast<const A_u_char*>(CATEGORY),\
				reinterpret_cast<const A_u_char*>(AE_ENTRY_POINT),\
				'eFKT',\
				PF_AE_PLUG_IN_VERSION,\
				PF_AE_PLUG_IN_SUBVERS,\
			    RESERVEDINFO);\
	  if(result == A_Err_NONE)\
	  {\
	     result = PF_Err_NONE;\
	  }

#define PF_REGISTER_EFFECT_EXT2(INPTR, CBPTR, NAME, MATCHNAME, CATEGORY,RESERVEDINFO, ENTRY_POINT, SUPPORT_URL)  \
       result = (*(CBPTR))((INPTR),\
				reinterpret_cast<const A_u_char*>(NAME),\
				reinterpret_cast<const A_u_char*>(MATCHNAME),\
				reinterpret_cast<const A_u_char*>(CATEGORY),\
				reinterpret_cast<const A_u_char*>(ENTRY_POINT),\
				'eFKT',\
				PF_AE_PLUG_IN_VERSION,\
				PF_AE_PLUG_IN_SUBVERS,\
			    RESERVEDINFO,\
				reinterpret_cast<const A_u_char*>(SUPPORT_URL));\
	  if(result == A_Err_NONE)\
	  {\
	     result = PF_Err_NONE;\
	  }

```

---

<h2>Begin File: MissingSuiteError.cpp</h2>

```cpp
/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007-2023 Adobe Inc.                                  */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Inc. and its suppliers, if                    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Inc. and its                    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Inc.            */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

#include "AEGP_SuiteHandler.h"

void AEGP_SuiteHandler::MissingSuiteError() const
{
	//	Yes, we've read Scott Meyers, and know throwing
	//	a stack-based object can cause problems. Since
	//	the err is just a long, and since we aren't de-
	//	referencing it in any way, risk is mimimal.

	//	As always, we expect those of you who use
	//	exception-based code to do a little less rudi-
	//	mentary job of it than we are here. 
	
	//	Also, excuse the Madagascar-inspired monkey 
	//	joke; couldn't resist. 
	//								-bbb 10/10/05
	
	PF_Err poop = PF_Err_BAD_CALLBACK_PARAM;

	throw poop;
}


```

---

<h2>Begin File: Param_Utils.h</h2>

```cpp
#ifndef H_PARAM_UTILS
#define H_PARAM_UTILS

//	do not include DVA headers here
#include <AE_Effect.h>
#include <AE_EffectCB.h>
#include <string.h>

// requires the explicit use of 'def' for the struct name

/* not quite strncpy because this always null terminates - unless SZ <= 0 */
#define PF_STRNNCPY(DST, SRC, SZ) \
	::strncpy((DST), (SRC), (SZ)); \
	if ((SZ) > 0U) (DST)[(SZ)-1] = 0

#define        PF_ParamDef_IS_PUI_FLAG_SET(_defP, _puiFlag)        \
  (((_defP)->ui_flags & _puiFlag) != 0)
   
#define        PF_ParamDef_IS_PARAM_FLAG_SET(_defP, _paramFlag)    \
   (((_defP)->flags & _paramFlag) != 0)


#define PF_ADD_COLOR(NAME, RED, GREEN, BLUE, ID)\
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_COLOR; \
		PF_STRNNCPY(def.name, (NAME), sizeof(def.name) ); \
		def.u.cd.value.red = (RED); \
		def.u.cd.value.green = (GREEN); \
		def.u.cd.value.blue = (BLUE); \
		def.u.cd.value.alpha = 255; \
		def.u.cd.dephault = def.u.cd.value; \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_ADD_ARBITRARY2(NAME, WIDTH, HEIGHT, PARAM_FLAGS, PUI_FLAGS, DFLT, ID, REFCON)\
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_ARBITRARY_DATA; \
		def.flags = (PARAM_FLAGS); \
		PF_STRNNCPY(def.name, (NAME), sizeof(def.name) ); \
		def.ui_width = (WIDTH);\
	    def.ui_height = (HEIGHT);\
		def.ui_flags = (PUI_FLAGS); \
		def.u.arb_d.value = NULL;\
		def.u.arb_d.pad = 0;\
		def.u.arb_d.dephault = (DFLT); \
		def.uu.id = def.u.arb_d.id = (ID); \
		def.u.arb_d.refconPV = REFCON; \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_ADD_ARBITRARY(NAME, WIDTH, HEIGHT, PUI_FLAGS, DFLT, ID, REFCON)\
	PF_ADD_ARBITRARY2(NAME, WIDTH, HEIGHT, PF_ParamFlag_NONE, PUI_FLAGS, DFLT, ID, REFCON)

#define PF_ADD_SLIDER(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, DFLT, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_SLIDER; \
		PF_STRNNCPY(def.name, (NAME), sizeof(def.name) ); \
		def.u.sd.value_str[0] = '\0'; \
		def.u.sd.value_desc[0] = '\0'; \
		def.u.sd.valid_min = (VALID_MIN); \
		def.u.sd.slider_min = (SLIDER_MIN); \
		def.u.sd.valid_max = (VALID_MAX); \
		def.u.sd.slider_max = (SLIDER_MAX); \
		def.u.sd.value = def.u.sd.dephault = (DFLT); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_ADD_FIXED(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, DFLT, PREC, DISP, FLAGS, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_FIX_SLIDER; \
		PF_STRNNCPY(def.name, (NAME), sizeof(def.name) ); \
		def.u.fd.value_str[0]	= '\0'; \
		def.u.fd.value_desc[0]	= '\0'; \
		def.u.fd.valid_min = (PF_Fixed)((VALID_MIN) * 65536.0); \
		def.u.fd.slider_min = (PF_Fixed)((SLIDER_MIN) * 65536.0); \
		def.u.fd.valid_max = (PF_Fixed)((VALID_MAX) * 65536.0); \
		def.u.fd.slider_max = (PF_Fixed)((SLIDER_MAX) * 65536.0); \
		def.u.fd.value = def.u.fd.dephault = (PF_Fixed)((DFLT) * 65536.0); \
		def.u.fd.precision		= (A_short)(PREC); \
		def.u.fd.display_flags  |= (A_short)(DISP); \
		def.flags				|= (FLAGS); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

// why does fs_flags get or-ed in? and why is CURVE_TOLERANCE param ignored? and .flags is never set. oy.
#define PF_ADD_FLOAT_SLIDER(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, CURVE_TOLERANCE, DFLT, PREC, DISP, WANT_PHASE, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_FLOAT_SLIDER; \
		PF_STRNNCPY(def.name, (NAME), sizeof(def.name) ); \
		def.u.fs_d.valid_min		= (VALID_MIN); \
		def.u.fs_d.slider_min		= (SLIDER_MIN); \
		def.u.fs_d.valid_max		= (VALID_MAX); \
		def.u.fs_d.slider_max		= (SLIDER_MAX); \
		def.u.fs_d.value			= (DFLT); \
		def.u.fs_d.dephault			= (PF_FpShort)(def.u.fs_d.value); \
		def.u.fs_d.precision		= (PREC); \
		def.u.fs_d.display_flags	= (DISP); \
		def.u.fs_d.fs_flags			|= (WANT_PHASE) ? PF_FSliderFlag_WANT_PHASE : 0; \
		def.u.fs_d.curve_tolerance	= AEFX_AUDIO_DEFAULT_CURVE_TOLERANCE;\
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

// safer newer version
#define PF_ADD_FLOAT_SLIDERX(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, DFLT, PREC, DISP, FLAGS, ID)	\
	do {																										\
		AEFX_CLR_STRUCT(def);																					\
		def.flags = (FLAGS);																					\
		PF_ADD_FLOAT_SLIDER(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, 0, DFLT, PREC, DISP, 0, ID);	\
	} while (0)

// copied from Pr version of Param_Utils.h. It is used in some of Pr versions of AE effects
#define PF_ADD_FLOAT_EXPONENTIAL_SLIDER(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, CURVE_TOLERANCE, DFLT, PREC, DISP, WANT_PHASE, EXPONENT, ID) \
do {\
PF_Err	priv_err = PF_Err_NONE; \
def.param_type = PF_Param_FLOAT_SLIDER; \
PF_STRNNCPY(def.name, (NAME), sizeof(def.name) ); \
def.u.fs_d.valid_min		= (VALID_MIN); \
def.u.fs_d.slider_min		= (SLIDER_MIN); \
def.u.fs_d.valid_max		= (VALID_MAX); \
def.u.fs_d.slider_max		= (SLIDER_MAX); \
def.u.fs_d.value			= (DFLT); \
def.u.fs_d.dephault			= (DFLT); \
def.u.fs_d.precision		= (PREC); \
def.u.fs_d.display_flags	= (DISP); \
def.u.fs_d.fs_flags			|= (WANT_PHASE) ? PF_FSliderFlag_WANT_PHASE : 0; \
def.u.fs_d.curve_tolerance	= AEFX_AUDIO_DEFAULT_CURVE_TOLERANCE;\
def.u.fs_d.useExponent		= true;\
def.u.fs_d.exponent			= EXPONENT;\
def.uu.id = (ID); \
if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
} while (0)

enum { PF_Precision_INTEGER, PF_Precision_TENTHS, PF_Precision_HUNDREDTHS, PF_Precision_THOUSANDTHS, PF_Precision_TEN_THOUSANDTHS };

#define PF_ADD_CHECKBOX(NAME_A, NAME_B, DFLT, FLAGS, ID)\
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_CHECKBOX; \
		PF_STRNNCPY(def.name, NAME_A, sizeof(def.name)); \
		def.u.bd.u.nameptr  = (NAME_B); \
		def.u.bd.value		= (DFLT); \
		def.u.bd.dephault	= (PF_Boolean)(def.u.bd.value); \
		def.flags			|= (FLAGS); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

// safer newer version
#define PF_ADD_CHECKBOXX(NAME, DFLT, FLAGS, ID)\
	do {\
		AEFX_CLR_STRUCT(def); \
		PF_ADD_CHECKBOX(NAME, "", DFLT, FLAGS, ID); \
	} while (0)

#define PF_ADD_BUTTON(PARAM_NAME, BUTTON_NAME, PUI_FLAGS, PARAM_FLAGS, ID)\
	do {\
		AEFX_CLR_STRUCT(def); \
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type		= PF_Param_BUTTON; \
		PF_STRNNCPY(def.name, PARAM_NAME, sizeof(def.name)); \
		def.u.button_d.u.namesptr  = (BUTTON_NAME); \
		def.flags			= (PARAM_FLAGS); \
		def.ui_flags		= (PUI_FLAGS); \
		def.uu.id			= (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_ADD_ANGLE(NAME, DFLT, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_ANGLE; \
		PF_STRNNCPY(def.name, (NAME), sizeof(def.name) ); \
		def.u.ad.value = def.u.ad.dephault = (PF_Fixed)((DFLT) * 65536.0); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)


#define PF_ADD_NULL(NAME, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_NO_DATA; \
		PF_STRNNCPY(def.name, (NAME), sizeof(def.name) ); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)


#define PF_ADD_POPUP(NAME, CHOICES, DFLT, STRING, ID) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_POPUP; \
		PF_STRNNCPY(def.name, (NAME), sizeof(def.name) ); \
		def.u.pd.num_choices = (CHOICES); \
		def.u.pd.dephault = (DFLT); \
		def.u.pd.value = def.u.pd.dephault; \
		def.u.pd.u.namesptr = (STRING); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)
	
#define PF_ADD_LAYER(NAME, DFLT, ID) \
	do	{\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_LAYER; \
		PF_STRNNCPY(def.name, (NAME), sizeof(def.name) ); \
		def.u.ld.dephault = (DFLT); \
		def.uu.id = ID;\
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)			

#define PF_ADD_255_SLIDER(NAME, DFLT, ID)\
	PF_ADD_SLIDER( (NAME), 0, 255, 0, 255, (DFLT), (ID))

#define PF_ADD_PERCENT(NAME, DFLT, ID)\
	PF_ADD_FIXED( (NAME), 0, 100, 0, 100, (DFLT), 1, 1, 0, (ID))

#define PF_ADD_POINT(NAME, X_DFLT, Y_DFLT, RESTRICT_BOUNDS, ID) \
	do	{\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_POINT; \
		PF_STRNNCPY(def.name, (NAME), sizeof(def.name) ); \
		def.u.td.restrict_bounds = RESTRICT_BOUNDS;\
		def.u.td.x_value = def.u.td.x_dephault = (X_DFLT << 16); \
		def.u.td.y_value = def.u.td.y_dephault = (Y_DFLT << 16); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_ADD_POINT_3D(NAME, X_DFLT, Y_DFLT, Z_DFLT, ID) \
	do	{\
		AEFX_CLR_STRUCT(def); \
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_POINT_3D; \
		PF_STRNNCPY(def.name, (NAME), sizeof(def.name) ); \
		def.u.point3d_d.x_value = def.u.point3d_d.x_dephault = X_DFLT; \
		def.u.point3d_d.y_value = def.u.point3d_d.y_dephault = Y_DFLT; \
		def.u.point3d_d.z_value = def.u.point3d_d.z_dephault = Y_DFLT; \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_ADD_TOPIC(NAME, ID) \
	do	{\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_GROUP_START; \
		PF_STRNNCPY(def.name, (NAME), sizeof(def.name) ); \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

#define PF_END_TOPIC(ID) \
	do	{\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_GROUP_END; \
		def.uu.id = (ID); \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)	

#define PF_ADD_VERSIONED_FLAG(NAME) \
	do {\
		PF_Err	priv_err = PF_Err_NONE; \
		def.param_type = PF_Param_CHECKBOX; \
		def.name[0] = 0; \
		def.u.bd.u.nameptr  = (NAME); \
		def.u.bd.value = true; \
		def.u.bd.dephault = false; \
		def.flags = PF_ParamFlag_USE_VALUE_FOR_OLD_PROJECTS; \
		def.ui_flags = PF_PUI_INVISIBLE; \
		if ((priv_err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return priv_err; \
	} while (0)

// newer safer version
#define PF_ADD_TOPICX(NAME, FLAGS, ID) \
	do {\
		AEFX_CLR_STRUCT(def); \
		def.flags = (FLAGS); \
		PF_ADD_TOPIC(NAME, ID); \
	} while (0)

#define PF_ADD_POPUPX(NAME, NUM_CHOICES, DFLT, ITEMS_STRING, FLAGS, ID) \
	do {															\
		AEFX_CLR_STRUCT(def);										\
		def.flags = (FLAGS);										\
		PF_ADD_POPUP(NAME, NUM_CHOICES, DFLT, ITEMS_STRING, ID);	\
	} while (0)

enum { PF_ParamFlag_NONE=0 };		// SBI:AE_Effect.h

#define PF_ADD_FLOAT_SLIDERX_DISABLED(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, DFLT, PREC, DISP, FLAGS, ID)	\
	do {																										\
	AEFX_CLR_STRUCT(def);																					\
	def.flags = (FLAGS);\
	def.ui_flags = PF_PUI_DISABLED;\
	PF_ADD_FLOAT_SLIDER(NAME, VALID_MIN, VALID_MAX, SLIDER_MIN, SLIDER_MAX, 0, DFLT, PREC, DISP, 0, ID);	\
	} while (0)

namespace fxparam_utility {

	template <typename T>
	inline int RoundF(T	x)
	{
		int	ret;

		if (x > 0) {
			ret = (int)(x + (T)0.5);
		} else {
			if ((int)(x + (T)0.5) == (x + (T)0.5)) {
				ret = (int)x;
			} else {
				ret = (int)(x - (T)0.5);
			}
		}

		return ret;
	}
};

inline PF_Err PF_AddPointControl(PF_InData *in_data,
								 const char *nameZ,
								 float x_defaultF,			// 0-1
								 float y_defaultF,		// 0-1
								 bool restrict_boundsB,
								 PF_ParamFlags param_flags,
								 PF_ParamUIFlags pui_flags,
								 A_long param_id)
{
    PF_ParamDef		def = {{0}};
	namespace du = fxparam_utility;

	def.flags = param_flags;
	def.ui_flags = pui_flags;

	PF_ADD_POINT(nameZ, du::RoundF(x_defaultF*100), du::RoundF(y_defaultF*100), restrict_boundsB, param_id);	// has error return in macro

	return PF_Err_NONE;
}


#endif // H_PARAM_UTILS

```

---

<h2>Begin File: Smart_Utils.cpp</h2>

```cpp
/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007-2023 Adobe Inc.                                  */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Inc. and its suppliers, if                    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Inc. and its                    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Inc.            */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

#include "Smart_Utils.h"


PF_Boolean IsEmptyRect(const PF_LRect *r){
	return (r->left >= r->right) || (r->top >= r->bottom);
}

void UnionLRect(const PF_LRect *src, PF_LRect *dst)
{
	if (IsEmptyRect(dst)) {
		*dst = *src;
	} else if (!IsEmptyRect(src)) {
		dst->left 	= mmin(dst->left, src->left);
		dst->top  	= mmin(dst->top, src->top);
		dst->right 	= mmax(dst->right, src->right);
		dst->bottom = mmax(dst->bottom, src->bottom);
	}
}

PF_Boolean
IsEdgePixel(
	PF_LRect	*rectP,
	A_long		x,
	A_long		y)
{
	PF_Boolean 	x_hitB = FALSE,
				y_hitB = FALSE;
				
	x_hitB = ((x == rectP->left)  || (x == rectP->right));
	
	y_hitB = ((y == rectP->top)  || (y == rectP->bottom));
	
	if (x_hitB){
		y_hitB = ((y >= rectP->top) && (y <= rectP->bottom));
	} else {
		if (y_hitB){
			x_hitB = ((x >= rectP->left) && (x <= rectP->right));
		}
	}
	return (x_hitB && y_hitB);
}
```

---

<h2>Begin File: Smart_Utils.h</h2>

```cpp
/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007-2023 Adobe Inc.                                  */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Inc. and its suppliers, if                    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Inc. and its                    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Inc.            */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

#include "AE_Effect.h"
#include "SPTypes.h"

#ifndef mmin
	#define mmin(a,b) ((a) < (b) ? (a) : (b))
	#define mmax(a,b) ((a) > (b) ? (a) : (b))
#endif

PF_Boolean IsEmptyRect(const PF_LRect *r);

void UnionLRect(const PF_LRect *src, PF_LRect *dst);

PF_Boolean IsEdgePixel(PF_LRect	*rectP, A_long x, A_long y);
```

---

<h2>Begin File: String_Utils.h</h2>

```cpp
/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/



/* String_Utils.h  */


#pragma once 

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif
A_char	*GetStringPtr(int strNum);
#ifdef __cplusplus
}
#endif

#define	STR(_foo)	GetStringPtr(_foo)







#endif /* STRING_UTILS_H */
```

---

