# PyShiftAE
An experimental repository for research, ideas, and (hopefully) eventually, working code. See README.md for more info. 

# Goals
- The main Goal of PyShiftAE is to create a replacement library for extendscript, in python. 
- This is not going to be done via wrappers around JS calls, but an actual library creation. 
- Ideally, you could write and run external .py scripts, as well as use them internally. 
- There would be an interface for using PyShiftAE within CEP (and eventually UXP) extensions. (Call it Pyinterface.js, use Pyinterface.evalpy(), hehe)
- This would GREATLY extend the capabilities of scripting in AE -- image data, audio data, access to numpy, ML libraries, and other custom libraries. 
- Script entirely in python, and access functionality you never could have with CEP.

# Contributing
I really would like this to be a community project. Posting ideas, thoughts, functionality suggestions, workflow suggestions, etc, would be very much appreciated.

- I don't expect anyone to delve too deep into this, but any sort of "reverse-engineering" or "hacking" to get more info regarding internal AEFX workings would also help
  - This is how I figured out how to call/create/listen to CSXS events directly from native code. 

# Documentation 
Coming soon

# Usage
The files included in this repo currently demonstrate how to use the PlugPlug.Dll from the c++ side in order to send and receive csxs events. 
In addition the src/PyShiftAE directory is the source files for the plugin. 
You can go to File->Run Script (.py), and run the test script provided from within AE.
Make sure Python 3.11 x64 is available on PATH.


You can actually use the CSXS wrappers for any adobe app, provided you find a way to get the path to PlugPlug.Dll in your app.

You would then make sure the event listener is loaded whenever your plugin is initialized. 

So you'd want to include this;

https://github.com/Trentonom0r3/AE-SDK-CEP-UTILS/blob/main/AEGP/Grabba/Win/CSXSUtils.h

Make sure you add this to your project;

https://github.com/Trentonom0r3/AE-SDK-CEP-UTILS/blob/main/AEGP/Grabba/CSXUtils.cpp

and then the only other thing you would have to adjust would be this function in CSXUtils.cpp, basically just tell it what you want to listen for and what you want it to do when you hear it. It would also be a really good idea to add a condition to ignore whatever you're sending from the plugin itself, otherwise it'll get stuck in an infinite loop.

	
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
	

I would make a wrapper around it in your main code, and return the strings from the event listener so you don't have to worry about including SDK headers or anything.
Finally, you can create an event listener in your main code like this;

	
	char* RegisterEvent(const char* EventType)
	{
		std::string path;
		path = GetPlugPlugPath();
	
		LoadDLL(path);
	
		int result = RegisterEventListener(EventType);
	
		if (result == 1) {
			return "Success";
		}
		else {
			return "Error";
		}
	}
	
