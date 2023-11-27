# PyShiftAE
Python for After Effects! PyShiftAE is an experimental, Open Source library designed to transcend the boundaries of the built-in ExtendScript library in Adobe After Effects (AE), heralding a new era of scripting possibilities in AE. By offering a Python library as an alternative to ExtendScript, PyShiftAE not only enriches the existing feature set but simplifies and amplifies the scripting experience. PyShiftAE is NOT a wrapper around extendscript. Instead, it is a python library written around the c++ SDK.

# ChangeLog
All Updates should be assumed to come with updated documentation, and updated binary .aex file located in ```dist```, unless otherwise specified. 
ChangeLog will contain a brief description of changes. See Wiki API docs for more info. 

- [11.27.23]
    - Added `effects_active`, `frame_blending`, `locked`, `shy`,
    `collapse`, `auto_orient_rotation`, `adjustment_layer`, `time_remapping`, 
    `layer_is_3d`, `look_at_camera`, `look_at_poi`, `solo`, `markers_locked`,
    `null_layer`, `hide_locked_masks`, `guide_layer`, `advanced_frame_blending`, 
    `sublayers_render_separately`, and `environment_layer` read-write attributes to ```Layer``` class.
    
    - Slight modification to `quality` attribute, now takes global attributes of "WIREFRAME", "DRAFT", or "BEST". 
        - ex; layer.quality = BEST
- [11.26.23]
    - Added several attributes and methods to ```Layer``` class. See Docs. 
    - Added ```saveAs``` to ```Project``` Class.
    - Added ```newSolid``` to ```CompItem``` Class. 
- [11.22.23]
    - Added ```addLayer``` method to ```CompItem```. 
    - Added ```addFolder```, ```addFootage```, and ```addComp``` to ```Project```.
    - Adjusted ```index``` attribute of ```Layer``` to be read-write. Changing this adjust other layers as well.
- [11.21.23]
    - Added ```layer/layers``` attributes to ```CompItem```.
    - Added basic methods (name, index) to ```layer/layers```.
        - Can access as standard python list (for layer in layers, layer[0], etc)
    - Adjusted ```app.reportInfo``` to work with str, int, float, etc.
    - Updated Documentation with **TODO** attributes and methods. 
    - Updated Task_Utils.h to send ```AEGP_CauseIdleRoutinesToBeCalled()``` when enqueueing a message, leading to faster execution.
    
- [11.19.23] 
    - Removed ```frameAtTime``` and ```replaceFrameAtTime``` methods.
        - These will be reimplemented in a different way using other subclasses. 
    - Full Refactor to Separate Python Thread.
    - Refactored into ```CoreSDK```, ```CoreLib```, and ```PyUtils``` folders.
        - ```CoreSDK``` contains the base wrappers for the AE SDK. 
            -See docstring in ```Core.cpp```
        - ```CoreLib``` contains the class implementations for ```CoreSDK```
            - This is a bit special. It utilizes a thread safe message queue to wrap the functions.
        -  ```PyUtils``` contains the python init, script running, and embedded module.
    - Overall refactoring to ensure separation of concerns and easier maintainability. 
    
## Contents

- [Problem Statement](#problem-statement)
- [Why PyShiftAE?](#why-pyshiftae)
- [Impact](#impact)
- [Challenges](#challenges-ahead)
- [Documentation](#documentation)
- [Contributing](https://github.com/Trentonom0r3/PyShiftAE/blob/main/CONTRIBUTING.md)
    - [Issues](https://github.com/Trentonom0r3/PyShiftAE/blob/main/CONTRIBUTING.md#reporting-issues)
    - [Pull Requests](https://github.com/Trentonom0r3/PyShiftAE/blob/main/CONTRIBUTING.md#submitting-pull-requests)
    - [Guidelines](https://github.com/Trentonom0r3/PyShiftAE/blob/main/CONTRIBUTING.md#code-guidelines)
    - [Testing](https://github.com/Trentonom0r3/PyShiftAE/blob/main/CONTRIBUTING.md#testing)
    - [Docs](https://github.com/Trentonom0r3/PyShiftAE/blob/main/CONTRIBUTING.md#docs)
    - [Community](https://github.com/Trentonom0r3/PyShiftAE/blob/main/CONTRIBUTING.md#community-interaction)
    - [Legal](https://github.com/Trentonom0r3/PyShiftAE/blob/main/CONTRIBUTING.md#legal)
    - [Setup](https://github.com/Trentonom0r3/PyShiftAE/blob/main/CONTRIBUTING.md#environment-setup)
    - [Review Process](https://github.com/Trentonom0r3/PyShiftAE/blob/main/CONTRIBUTING.md#review-process)
    - [Acknowledgements](https://github.com/Trentonom0r3/PyShiftAE/blob/main/CONTRIBUTING.md#acknowledgements)
      
### Problem Statement
ExtendScript, the established scripting language in AE, poses certain limitations particularly around sophisticated tasks such as pixel or audio data manipulation. Its capabilities are somewhat confined, often demanding a transition to the more complex C++ SDK for advanced functionalities.

### Why PyShiftAE?
- **Ease of Use**: Python, being a more intuitive and widely adopted language compared to ExtendScript, lowers the entry barrier for scriptwriters. 
- **Extended Functionality**: PyShiftAE is not a mere wrapper around ExtendScript; it's a full-fledged library offering novel functionalities and a simplified approach to existing ones.
- **Community Empowerment**: With PyShiftAE, a broader spectrum of developers including CEP developers and those proficient in Python can effortlessly script in AE, thus democratizing advanced scripting capabilities.
- **Innovation Unleashed**: Direct integration with APIs, machine learning libraries, and more becomes a reality with PyShiftAE, paving the way for innovative scripts and extensions.

### Impact
The ripple effect of PyShiftAE in the AE community could be profound. Scriptwriters could venture into more advanced coding territories, and CEP extension authors could craft hybrid extensions blending SDK features via PyShiftAE, thereby catalyzing a wave of innovative solutions in the AE ecosystem.

## Challenges Ahead
- Memory & Lifetime Management: Ensuring robust state storage while averting memory leaks is a paramount concern.
- Feature Parity: Retaining all the functionality from ExtendScript, albeit with an enhanced and simplified interface, is a significant challenge.
  
## Documentation 
- [API-Reference](https://github.com/Trentonom0r3/PyShiftAE/wiki/API-Reference)
- [CSXS](https://github.com/Trentonom0r3/PyShiftAE/wiki/CSXS-Utils)
- [Demos](https://github.com/Trentonom0r3/PyShiftAE/wiki/Demos)
- [Building from source](https://github.com/Trentonom0r3/PyShiftAE/wiki/Building-from-source)
- [Pre-Compiled .aex Binary](https://github.com/Trentonom0r3/PyShiftAE/blob/main/dist/PyShiftAE.aex)
  - MUST have Python 3.11 installed to path.
  


- All contributors will be acknowledged in the project repository. Your contributions, no matter how small, are valuable to us and the community.


