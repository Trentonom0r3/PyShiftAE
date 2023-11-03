# PyShiftAE
PyShiftAE is a pioneering library designed to transcend the boundaries of the built-in ExtendScript library in Adobe After Effects (AE), heralding a new era of scripting possibilities in AE. By offering a Python library as an alternative to ExtendScript, PyShiftAE not only enriches the existing feature set but simplifies and amplifies the scripting experience.

## Problem Statement
ExtendScript, the established scripting language in AE, poses certain limitations particularly around sophisticated tasks such as pixel or audio data manipulation. Its capabilities are somewhat confined, often demanding a transition to the more complex C++ SDK for advanced functionalities.

## Why PyShiftAE?
- **Ease of Use**: Python, being a more intuitive and widely adopted language compared to ExtendScript, lowers the entry barrier for scriptwriters. 
- **Extended Functionality**: PyShiftAE is not a mere wrapper around ExtendScript; it's a full-fledged library offering novel functionalities and a simplified approach to existing ones.
- **Community Empowerment**: With PyShiftAE, a broader spectrum of developers including CEP developers and those proficient in Python can effortlessly script in AE, thus democratizing advanced scripting capabilities.
- **Innovation Unleashed**: Direct integration with APIs, machine learning libraries, and more becomes a reality with PyShiftAE, paving the way for innovative scripts and extensions.

## Impact
The ripple effect of PyShiftAE in the AE community could be profound. Scriptwriters could venture into more advanced coding territories, and CEP extension authors could craft hybrid extensions blending SDK features via PyShiftAE, thereby catalyzing a wave of innovative solutions in the AE ecosystem.

## Distinguishing Features
Unlike existing Python libraries for AE which serve as wrappers around ExtendScript, PyShiftAE stands out as an independent library with its own suite of features. Here's a glimpse of how PyShiftAE can simplify and elevate the scripting experience in AE:
```
from PyShiftCore import *
import traceback

item = app.project.activeItem
length = item.duration

if isinstance(item, CompItem):  # Check if item is a CompItem
    try:
        for i in range( 1, int( length ) - 1 ):  # Loop through all frames

            img = item.frameAtTime(i)  # Get frame

            new_img = some_function() # manipulate the image

	    item.replaceFrameAtTime( new_img, i ) # replace frame at the given time

    except Exception as e:
        print("Error:", str(e))
        traceback.print_exc()
else:
    print("Active item is not a CompItem")
```

# Challenges Ahead
- Memory & Lifetime Management: Ensuring robust state storage while averting memory leaks is a paramount concern.
- Feature Parity: Retaining all the functionality from ExtendScript, albeit with an enhanced and simplified interface, is a significant challenge.
- See [TODO](https://github.com/Trentonom0r3/PyShiftAE/blob/main/TODO.md)
- 
# Contributing
I really would like this to be a community project. Posting ideas, thoughts, functionality suggestions, workflow suggestions, etc, would be very much appreciated.

- I don't expect anyone to delve too deep into this, but any sort of "reverse-engineering" or "hacking" to get more info regarding internal AEFX workings would also help
  - This is how I figured out how to call/create/listen to CSXS events directly from native code. 

# Documentation 
Coming soon

# Proof of Concept

### Simple Attribute Gathering, Command Execution:
 
https://github.com/Trentonom0r3/PyShiftAE/assets/130304830/31369ac6-a3a0-40e2-a78a-5a3936c60239

### New "frameAtTime" method, usage with cv2 demo:

https://github.com/Trentonom0r3/PyShiftAE/assets/130304830/26afed06-ef81-458c-ba4e-c5bc97af0d1e



# Usage
- The files included in this repo currently demonstrate how to use the PlugPlug.Dll from the c++ side in order to send and receive csxs events. 
- In addition, the src/PyShiftAE directory is the source files for the plugin. 
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
	
