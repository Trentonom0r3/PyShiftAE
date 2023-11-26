#include "Core.h"
/*
 * File: Core.cpp
 * Description: This file contains the core logic wrappers for the After Effects SDK. It is designed to be simple and heavily abstracted for reusability.
 *
 * Guidelines for Contributors:
 * 1. Simplicity: Functions should be straightforward, performing a single task.
 * 2. Suite Access: Always access suites via `SuiteManager::GetInstance().GetSuiteHandler()`.
 * 3. Plugin ID: Retrieve the plugin ID using `SuiteManager::GetInstance().GetPluginID()`. Check for null pointers.
 * 4. Error Handling: Use the `Result<T>` class for returning both values and error codes.
 *	 `Result<void>` for functions that do not return a value.
 *	'Result<T>' is a template class that contains a value of type T and an error code of type A_Err.
 * 5. Abstraction: Maintain a high level of abstraction. Complex functionalities should be delegated to 'PyCore.cpp'.
 * 6. Reusability: Write reusable code. Many functions will be used in various combinations in 'PyCore.cpp'.
 * 7. Do not modify the standard set unless absolutely necessary.
 * 
 * Error Handling:
 * 1. Use the `Result<T>` class for returning both values and error codes.
 * `Result<void>` for functions that do not return a value.
 * 2. `Result<T>` is a template class that contains a value of type T and an error code of type A_Err.
 * 3. The `Result<T>` class is defined in 'Core.h'.
 * 4. The `Result<T>` class is used to return both values and error codes from functions.
 * 5. The `Result<T>` class is used to return error codes from functions that do not return a value.
 * 
 * See Existing code and header file for examples.
 *
 */

 // Convert seconds to A_Time considering the frame rate
A_Time ConvertFloatToATime(float seconds, float frameRate) {
	A_Time aTime;
	float timescale = frameRate * 100;  // Scale based on frame rate
	aTime.value = static_cast<A_long>(seconds * timescale);
	aTime.scale = static_cast<A_u_long>(timescale);
	return aTime;
}

// Convert A_Time to float (seconds)
float ConvertATimeToFloat(const A_Time& aTime) {
	if (aTime.scale == 0) {
		throw std::runtime_error("Scale cannot be zero in A_Time conversion.");
	}
	return static_cast<float>(aTime.value) / static_cast<float>(aTime.scale);
}

// Convert frame number to A_Time
A_Time ConvertFrameToATime(int frameNumber, float frameRate) {
	return ConvertFloatToATime(static_cast<float>(frameNumber) / frameRate, frameRate);
}


std::string convertUTF16ToUTF8(const A_UTF16Char* utf16String) {
	if (utf16String == nullptr) {
		return "";
	}

	int utf8Length = WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<const wchar_t*>(utf16String), -1, nullptr, 0, nullptr, nullptr);
	if (utf8Length <= 0) {
		return "";
	}

	std::vector<char> utf8String(utf8Length);
	WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<const wchar_t*>(utf16String), -1, utf8String.data(), utf8Length, nullptr, nullptr);
	return std::string(utf8String.begin(), utf8String.end() - 1);
}

std::vector<A_UTF16Char> convertUTF8ToUTF16(const std::string& utf8String) {
	int utf16Length = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, nullptr, 0);
	if (utf16Length <= 0) {
		return std::vector<A_UTF16Char>();
	}

	std::vector<wchar_t> utf16Buffer(utf16Length);
	MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), -1, utf16Buffer.data(), utf16Length);

	std::vector<A_UTF16Char> utf16String(utf16Buffer.begin(), utf16Buffer.end());
	return utf16String;
}

AEGP_ProjBitDepth ConvertToProjBitDepth(const std::string& bitDepthStr) {
	if (bitDepthStr == "8") return AEGP_ProjBitDepth_8;
	if (bitDepthStr == "16") return AEGP_ProjBitDepth_16;
	if (bitDepthStr == "32") return AEGP_ProjBitDepth_32;
	throw std::runtime_error("Invalid bit depth string: " + bitDepthStr);
}
