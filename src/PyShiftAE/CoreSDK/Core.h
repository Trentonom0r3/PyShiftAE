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
#include <algorithm>


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
	float width;
    float height;
};

struct ImageData {
    std::shared_ptr<std::vector<uint8_t>> data;
    float width;
    float height;
    int channels;

    // Default constructor
    ImageData() : width(0), height(0), channels(0), data(std::make_shared<std::vector<uint8_t>>()) {}

    // Constructor with shared_ptr and dimensions
    ImageData(std::shared_ptr<std::vector<uint8_t>> d, int w, int h, int c)
        : data(std::move(d)), width(w), height(h), channels(c) {}

    // Constructor with initialization
    ImageData(float w, float h, int c) : width(w), height(h), channels(c) {
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

/*


Note that A_Time values are rationals 
and thus do not map exactly into floating point 
so you may end up with off-by-one frame issues 
depending on how the floating point time values are used.
If you need precision then I suggest doing your time operations directly with rationals.

*/
A_Time ConvertFloatToATime(float seconds, float frameRate);

float ConvertATimeToFloat(const A_Time& aTime);

std::string convertUTF16ToUTF8(const A_UTF16Char* utf16String);

std::vector<A_UTF16Char> convertUTF8ToUTF16(const std::string& utf8String);

AEGP_ProjBitDepth ConvertToProjBitDepth(const std::string& bitDepthStr);
