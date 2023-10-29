#include <windows.h>
#include <iostream>
#include "Win/CSXSUtils.h"


HMODULE hModule = NULL; // Global DLL handle


HMODULE LoadDLL(std::string DLLpath) {

    LPCSTR resultStr = DLLpath.c_str();  // Convert the path string to LPCSTR
	// Now resultStr contains the path string returned by the ExtendScript function
	hModule = LoadLibrary(resultStr);  // Use the dynamic path here


	if (!hModule) {
		std::cerr << "Failed to load DLL." << std::endl;
		return NULL;
	}

	return hModule;
}


EventErrorCode DispatchAdobeEvent(const char* type, const char* appId, const char* extensionId, const char* data) {
	try {
		if (!hModule) {
			std::cerr << "Failed to load DLL." << std::endl;
			return kEventErrorCode_OperationFailed;
		}

		PlugPlugDispatchEventFn DispatchEvent = reinterpret_cast<PlugPlugDispatchEventFn>(GetProcAddress(hModule, "PlugPlugDispatchEvent"));

		Event my_event = {
			type,
			kEventScope_Application,
			appId,
			extensionId,
			data
		};

		EventErrorCode result = static_cast<EventErrorCode>(DispatchEvent(&my_event));
		//FreeLibrary(hModule);

		return result;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		return kEventErrorCode_OperationFailed;
	}
}



int SendEvent(const char* EventType, const char* TargetApp, const char* ExtensionID, const char* Data) {
	EventErrorCode result = DispatchAdobeEvent(EventType, TargetApp, ExtensionID, Data);
	if (result == kEventErrorCode_Success) {
		std::cout << "Event dispatched successfully." << std::endl;
		return 1;
	}
	else {
		std::cerr << "Failed to dispatch event. Error code: " << result << std::endl;
		return 0;
	}
}

//Private function to handle events
void MyEventListener(const Event* const event, void* const context) {
	std::cout << "Received event: " << event->type << std::endl;

	if (event->data) {
		if (strcmp(event->data, "Hello from JSX!") == 0) {
			std::cout << "Event data is correct." << std::endl;
			int res = SendEvent("com.adobe.csxs.events.MyCustomEvent", "AEFT", "getimg", "Hello from C++");
			if (res == kEventErrorCode_Success) {
				std::cout << "Event sent successfully." << std::endl;
			}
			else {
				std::cerr << "Failed to send event." << std::endl;
			}
		}
		else {
			std::cerr << "Event data is incorrect." << std::endl;
		}

	}
}

//Register event listener "com.adobe.csxs.events......"

int RegisterEventListener(const char* EventType) {
	try {
	typedef EventErrorCode(*PlugPlugAddEventListenerFn)(const char*, const EventListenerFn, void* const);
	PlugPlugAddEventListenerFn AddEventListener = reinterpret_cast<PlugPlugAddEventListenerFn>(GetProcAddress(hModule, "PlugPlugAddEventListener"));


	// Register the event listener
	EventErrorCode result = AddEventListener(EventType, MyEventListener, nullptr);

	// Check the result
	if (result == kEventErrorCode_Success) {
		std::cout << "Event listener added successfully." << std::endl;
	}
	else {
		std::cerr << "Failed to add event listener. Error code: " << result << std::endl;
	}

	return 0;
}
catch (const std::exception& e) {
	std::cerr << "Exception: " << e.what() << std::endl;
	return 1;
}
}