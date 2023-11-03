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
- Managing threading/loops:
   > Currently, running ```item.frameAtTime(idx)``` in a loop leads to an unresponsive app and ultimately a crash.
   > While it works fine on its own in a single run, I'm unsure as to why the loop causes the crash. (I'd assume its memory/lifetime related)
   > Could possibly simply be a limitation of the suites being used or AEGPs in general, looking into this.
   > ```item.frameAtTime(idx)``` runs by taking the activeItem (typically the comp) and getting the ```AEGP_WorldH``` from the specified time.
   > 	It then converts into a custom ImageData struct, passes to python, converts to numpy, and returns. 
   > Couple ideas for solutions are;
   > 	a) Figure out a different combination of suites to use.
   > 	b) Create an effect plugin (possibly built into the AEGP?) to attach to layers, and limit ```item.frameAtTime(idx)``` to prevent being called from loops

   
- See [TODO](https://github.com/Trentonom0r3/PyShiftAE/blob/main/TODO.md)
  
## Contributing
I really would like this to be a community project. Posting ideas, thoughts, functionality suggestions, workflow suggestions, etc, would be very much appreciated.

- I don't expect anyone to delve too deep into this, but any sort of "reverse-engineering" or "hacking" to get more info regarding internal AEFX workings would also help
  - This is how I figured out how to call/create/listen to CSXS events directly from native code. 

# Documentation 
- (API-Reference)[https://github.com/Trentonom0r3/PyShiftAE/wiki/API-Reference]
- (CSXS)[https://github.com/Trentonom0r3/PyShiftAE/wiki/CSXS-Utils]
- (Demos)[https://github.com/Trentonom0r3/PyShiftAE/wiki/Demos]
