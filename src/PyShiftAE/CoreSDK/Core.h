#pragma once
#include "AEConfig.h"
#ifdef AE_OS_WIN
#include <windows.h>
#endif

#include "SuiteManager.h" //Singleton class that manages the suite handler

#include "PT_Err.h"
#include "AE_Macros.h"

#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>

/*
 * File: Core.h
 * Description: Header file for Core.cpp. Contains template definitions and function prototypes used in Core.cpp.
 *
 * Guidelines for Contributors:
 * 1. Consistency: Maintain consistency with the existing template and function definitions.
 * 2. Clarity: Ensure that all new templates and functions are clearly documented and easy to understand.
 * 3. Compatibility: Any additions should be compatible with the existing code structure and logic in Core.cpp.
 */

struct size {
	int width;
	int height;
};

struct ImageData {
    std::shared_ptr<std::vector<uint8_t>> data;
    int width;
    int height;
    int channels;

    // Default constructor
    ImageData() : width(0), height(0), channels(0), data(std::make_shared<std::vector<uint8_t>>()) {}

    // Constructor with shared_ptr and dimensions
    ImageData(std::shared_ptr<std::vector<uint8_t>> d, int w, int h, int c)
        : data(std::move(d)), width(w), height(h), channels(c) {}

    // Constructor with initialization
    ImageData(int w, int h, int c) : width(w), height(h), channels(c) {
        data = std::make_shared<std::vector<uint8_t>>(w * h * c);
    }
};


template<typename T>
struct Result {
    T value; // The result of the function, if any
    A_Err error; // The error code returned by the AE functions

    // Constructor for error-only results
    Result(A_Err err) : error(err) {}

    // Constructor for results with a value and an error code
    Result(T val, A_Err err) : value(val), error(err) {}

    // Default constructor for no-error (assumes default-constructible T)
    Result() : value(T()), error(A_Err_NONE) {}
};

// Specialization for void since it cannot be constructed or returned
template<>
struct Result<void> {
    A_Err error;

    // Constructor for error-only results
    explicit Result(A_Err err = A_Err_NONE) : error(err) {}
};


//APP RELATED FUNCS
Result<void> ReportInfo(std::string info); //Use to report Information in a dialog box

Result<void> StartUndoGroup(std::string undo_name); //Starts undo group, pass string of custom name

Result<void> EndUndoGroup(); //Ends undo group, no args

Result<A_UTF16Char> AppVersion(); //gets app version

Result<AEGP_ProjectH> getProject(); //gets handle to the project (1 per AE session)

Result<void> ExecuteCommand(int commandId); //executes a command by ID




// PROJECT RELATED FUNCS

Result<AEGP_ItemH> getActiveItem(); //gets the active item (null if multiple selected) (can be any type of item)

Result<AEGP_RenderOptionsH> getRenderOptions(Result<AEGP_ItemH> itemH);

Result<AEGP_RenderOptionsH> setTime(Result<AEGP_RenderOptionsH> roH, float time);

Result<AEGP_RenderOptionsH> getWorldType(Result<AEGP_RenderOptionsH> roH);

Result<AEGP_FrameReceiptH> renderAndCheckoutFrame(Result<AEGP_RenderOptionsH> roH);

Result<AEGP_WorldH> getReceiptWorld(Result<AEGP_FrameReceiptH> receiptH);

Result<void> checkinFrame(Result<AEGP_FrameReceiptH> receiptH);

Result<PF_Pixel8*> getBaseAddr8(Result<AEGP_WorldH> frameH);

Result<size> getSize(Result<AEGP_WorldH> frameH);

Result<void> disposeRenderOptions(Result <AEGP_RenderOptionsH> roH);




// ITEM(GENERAL) RELATED FUNCS
Result<AEGP_ItemType> getItemType(Result<AEGP_ItemH> itemH); //gets the type of ItemH

Result<std::string> getItemName(Result<AEGP_ItemH> itemH); //gets the name of ItemH

Result<void> setItemName(Result<AEGP_ItemH> itemH, const std::string& name); //sets the name of ItemH

Result<A_long> getUniqueItemID(Result<AEGP_ItemH> itemH); //gets the unique, unchanging ID for an item



//COMP RELATED FUNCS
Result<int> getNumLayers(Result<AEGP_CompH> compH);

Result<AEGP_CompH> getCompFromItem(Result<AEGP_ItemH> itemH);



//LAYER RELATED FUNCS
Result<int> getLayerIndex(Result<AEGP_LayerH> layerH);

Result<std::string> getLayerName(Result<AEGP_LayerH> layerH);

Result<std::string> getLayerSourceName(Result<AEGP_LayerH> layerH);

Result<void> setLayerName(Result<AEGP_LayerH> layerH, const std::string& name);

Result<AEGP_LayerH> getLayerFromComp(Result<AEGP_CompH> compH, int index);
