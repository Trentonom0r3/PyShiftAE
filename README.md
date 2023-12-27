# PyShiftAE
Python for After Effects! PyShiftAE is an experimental, Open Source library designed to transcend the boundaries of the built-in ExtendScript library in Adobe After Effects (AE), heralding a new era of scripting possibilities in AE. 

- By offering a Python library as an alternative to ExtendScript, PyShiftAE not only enriches the existing feature set but simplifies and amplifies the scripting experience. PyShiftAE is NOT a wrapper around extendscript. Instead, it is a python library written around the c++ SDK.
  
- PyShiftAE not only allows scripting of AE through python, but integration of most (if not all) external python libraries, provided you have the installed. 
- As ways to script shapes/masks/text et alia are implemented, this will open doors to python data-driven animation within AE, utilization of various ML and AI libraries, and much, much more.
### Join the Official **PyShiftAE** Discord!
- Check it out [here](https://discord.gg/S2AE9Zfuxb)!

## Contents
- [Building from source](https://github.com/Trentonom0r3/PyShiftAE/wiki/Building-from-source)
- [Installation via installer(RECOMMENDED)](https://github.com/Trentonom0r3/PyShiftAE/wiki/Installing-from-.exe-(RECOMMENDED))
- [ChangeLog](#changelog)
- [Problem Statement](#problem-statement)
- [Why PyShiftAE?](#why-pyshiftae)
- [Impact](#impact)
- [Challenges](#challenges-ahead)
- [API-Reference](https://github.com/Trentonom0r3/PyShiftAE/wiki/API-Reference)
    - [CSXS Events](https://github.com/Trentonom0r3/PyShiftAE/wiki/CSXS-Utils)
    - [Demos](https://github.com/Trentonom0r3/PyShiftAE/wiki/Demos)
      
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
      - All contributors will be acknowledged in the project repository. Your contributions, no matter how small, are valuable to us and the community.

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
- Feature Parity: Retaining all the functionality from ExtendScript, albeit with an enhanced and simplified interface, is a significant challenge.
- `CEPy` Extensions: A play on `CEP`, `CEPy` extensions aim to provide an interface for pure python GUI creation, for those that don't care about having a pretty JS UI.
- Custom Effects: The ability to create and write your own custom effects. They function like effects, have keyframeable parameters like effects, etc. Users would write custom python callbacks, determining the behavior of their effect.

# ChangeLog
All Updates should be assumed to come with updated documentation, and an updated binary .aex file located in `dist`, unless otherwise specified. 
ChangeLog will contain a brief description of changes. See Wiki API docs for more info. 
- [12.22.23]
    - Added `replace` method to `FootageItem`.
    - Created `PSC.exe` to help install `PyShiftAE` in the correct location, with the correct python version. Find this in `/dist/`.
- [12.21.23]
    - Massive Overhaul and Feature Updates. Slight API doc changes. 
    - Included a Debug Console, found under `Window` -> `Python Console`.
    - Added to the [CEPy-Resources Repo](https://github.com/Trentonom0r3/CEPy-Resources)
      - See this repo for how to run PyShiftAE from CEP extensions, and instructions for setup. 
    - Fixed issues with using iterator, fixed some memory and pointer issues.
    - Added `layer.source`, use to get source Item, which is used to alter/change source item. 
    - Added `selectedLayer(s)` to `CompItem` class, shows you which layers are selected.
    - Added `selectedItems` to `Project` class.
    - Added `activeLayer` to `Project`.
    - Added `selected` boolean attribute to `Item` base class. 
    - Added a `build.py` script to help user generate the boilerplate for `CEP` integration. 
        - Again, see the [CEPy-Resources Repo](https://github.com/Trentonom0r3/CEPy-Resources).

- [11.29.23]
    - Added `ProjectCollection` and `ItemCollection` classes, similar to `LayerCollection`
    - Updated API Reference with some more detailed information. 
    - Added constructor to `FolderItem`. 
    - Removed Item creation methods from `Project` class in favor of constructor usage. 
    - Demonstration of recursive searching through `ProjectCollection` and `FolderItem` using a pythonic interace:
        
        ```python
        from PyShiftCore import *

        NAME = "NAME"
        PATH = "PATH"
        project = app.project

        items = project.items

        new_folder = FolderItem(NAME)  # create a new folder item

        new_footage = FootageItem(NAME, PATH)  # create a new footage item

        new_folder.children.append(new_footage)  # add the footage item to the folder item

        for item in items:  # loop through all items in the project
            if isinstance(item, FolderItem): # check if the item is a folder
                if item.name != "PyShiftAE": # check if the folder is not the PyShiftAE folder
                    item.children.append(new_folder) # add the new folder to the folder we found
            else:
                app.reportInfo(item.name) # report the name of the item

        for item in items: # loop through all items in the project
            if isinstance(item, FolderItem): # check if the item is a folder
                app.reportInfo("Folder Found!:" + item.name) # report the name of the folder
                children = item.children # get the children of the folder
                app.reportInfo("Looking for Child Items!") # report that we are looking for child items
                for child in children: # loop through all the children
                    app.reportInfo("Child Found!:" + child.name) # report the name of the child
                    if isinstance(child, FolderItem): # check if the child is a folder
                        app.reportInfo("Child is Folder!:" + child.name) # report that the child is a folder
                        children2 = child.children # get the children of the child
                        for child2 in children2: # loop through all the children of the child
                            app.reportInfo("Child Folder Child Item:" + child2.name) # report the name of the child of the child       
        ```
- [11.28.23 [Pt.2]]
    - Adjusted `LayerCollection` to have getter/setter methods, access as you normally would lists. 
    - Removed `addLayer` and `addSolid` from `CompItem` in favor of list manipulation.
    - Started on python stub files `.pyi`, contained under `PyShiftCore.pyi`.
    - Added `SolidItem` class.
        -
        ```python
        from PyShiftCore import *
        NAME = "NAME"
        WIDTH = 1920
        HEIGHT = 1080
        RED = 0.55  # Floating Point in range [0.00, 1.00]
        GREEN = 0.22  # Floating Point in range [0.00, 1.00]
        BLUE = 0.10  # Floating Point in range [0.00, 1.00]
        ALPHA = 1.0  # Floating Point in range [0.00, 1.00]
        DURATION = 10.0  # Floating Point in range [0.00, 1.00]
        solid = SolidItem(NAME, WIDTH, HEIGHT, RED, GREEN, BLUE, ALPHA, DURATION)

        active_comp_layers = app.project.activeItem.layers

        active_comp_layers.append(solid)

        for layer in active_comp_layers:
            # TODO: Implement Logic to have the following usage: if isinstance(layer, AVLayer):
            if layer.name == "NAME":
                app.reportInfo(layer.name)
        ```
- [11.28.23]
    - Added constructors for `CompItem`, `FootageItem`. 
        -
        ```python
        from PyShiftCore import *

        # declaring args for clarity

        NAME = "NAME"
        WIDTH = 1920
        HEIGHT = 1080
        FRAMERATE = 24.0
        DURATION = 10.0
        ASPECT_RATIO = 1.0

        ITEM_NAME = "TEST"
        PATH = "C:\\Users\\"

        # get the activeItem
        comp = app.project.activeItem

        new_comp = CompItem(NAME, WIDTH, HEIGHT, FRAMERATE, DURATION, ASPECT_RATIO) # returns a CompItem for use

        new_item = FootageItem(ITEM_NAME, PATH)

        # add items to the new comp
        layers = new_comp.layers
        layers.append(new_item)

        # add the new comp to the activeItem
        comp.layers.append(new_comp)

        ```
    - Adjusted `comp.layers/comp.layer` to return a `LayerCollection` object, presented as a standard list.
        - `append`, `insert`, `remove`, `pop`, all available, but slightly different from standard python.
            - See API reference.
    - Adjusted `app.reportInfo()` to take args more gracefully.
        - Previously, had to create a separate "info_str" arg to pass in.
        - Now you can simply run something like `app.reportInfo(comp.numLayers)`.
    
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
   
