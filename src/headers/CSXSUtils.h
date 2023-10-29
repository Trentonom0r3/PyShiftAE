#pragma once
#include <minwindef.h>
#include <string>

// MyFunctions.h
#pragma once
enum EventScope {
	kEventScope_Global = 0,
	kEventScope_Application = 1,
	kEventScope_LastValue = 0x7FFFFFFF
};

enum EventErrorCode {
	kEventErrorCode_Success = 0,
	kEventErrorCode_OperationFailed = 1,
	kEventErrorCode_Unknown = 2,
	kEventErrorCode_LastValue = 0x7FFFFFFF
};

struct Event {
	const char* type;
	EventScope scope;
	const char* appId;
	const char* extensionId;
	const char* data;
};

typedef int (*PlugPlugDispatchEventFn)(Event*);
typedef void (*EventListenerFn)(const Event* const event, void* const context);

// In your .h file
extern HMODULE hModule;
HMODULE LoadDLL(std::string fileName);
int SendEvent(const char* EventType, const char* TargetApp, const char* ExtensionID, const char* Data);
EventErrorCode DispatchAdobeEvent(const char* type, const char* appId, const char* extensionId, const char* data);
int RegisterEventListener(const char* type); //, EventListenerFn listener, void* context); <- change to pass custom function