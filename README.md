# PyShiftAE
PyShiftAE is an experimental, Open Source library designed to transcend the boundaries of the built-in ExtendScript library in Adobe After Effects (AE), heralding a new era of scripting possibilities in AE. By offering a Python library as an alternative to ExtendScript, PyShiftAE not only enriches the existing feature set but simplifies and amplifies the scripting experience.

# ChangeLog
All Updates should be assumed to come with updated documentation, and updated binary .aex file located in ```dist```, unless otherwise specified. 
ChangeLog will contain a brief description of changes. See Wiki API docs for more info. 
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
    
## Contents

- [Problem Statement](#problem-statement)
- [Why PyShiftAE?](#why-pyshiftae)
- [Impact](#impact)
- [Challenges](#challenges-ahead)
- [Documentation](#documentation)
- [Contributing](#contributing)
    - [Issues](#reporting-issues)
    - [Pull Requests](#submitting-pull-requests)
    - [Guidelines](#code-guidelines)
    - [Testing](#testing)
    - [Docs](#docs)
    - [Community](#community-interaction)
    - [Legal](#legal)
    - [Setup](#environment-setup)
    - [Review Process](#review-process)
    - [Acknowledgements](#acknowledgements)
      
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
  
### Contributing

Contributions from the community are welcome, and I would be pleased to have them. Please follow this guide for contributing to PyShiftAE.

#### Reporting Issues

- Please make sure to check for existing issues before creating a new one. Your issue might have already been reported and possibly even fixed in the `main` branch.
- When creating an issue, please provide a clear and descriptive title, and explain the problem in detail. Include any relevant code snippets or error messages.

#### Submitting Pull Requests

- Fork the repository and create a new branch for your feature or bug fix.
- There's no strict coding style guideline at the moment, but please keep your code clean and well-commented. Following the general structure and commenting style of the existing code would be appreciated. To further this, detailed comments help both myself, and others learn and collaborate better.
- Submit your pull request against the `main` branch. If there is a `develop` branch or other relevant branches in the future, please redirect your pull request accordingly.

#### Code Guidelines

- Your code should be well-commented with docstrings for classes and methods.
- Please try to keep your code as clean and straightforward as possible.
- It would be beneficial to follow common Python conventions and keep a consistent coding style. (On the API exposure side)

#### Testing

- At the moment, there is not a specific testing framework in place. However, I would appreciate if you could provide simple tests or usage examples to demonstrate the functionality of your contribution. Eg: scripts used, video demonstrations, etc;

#### Docs

- You can follow the existing documentation as a format guide.
- When adding new features, it would be great to include relevant documentation updates alongside your code contributions.

#### Community Interaction

- Discussion around the project will primarily occur here on GitHub. Feel free to ask questions or discuss anything related to the project in the issue tracker.
- You can get in touch with me directly via the contact information provided on GitHub.

#### Legal

- By contributing, you agree that your contributions will be licensed under the same license as the project (AGPL 3.0).
- Ensure that you have the right to submit your contribution and that your code does not infringe any other person's or entity's rights.

#### Environment Setup

- The project is currently set up for Microsoft x64, using pybind11 x64 static, linked to Python 3.11 (debug and release). Follow the instructions provided in the [WIKI](https://github.com/Trentonom0r3/PyShiftAE/wiki/Building-from-source) for setting up your development environment.

#### Review Process

- Contributions will be reviewed by the community and maintainers. Constructive feedback will be provided, and once your contribution is accepted, it will be merged into the `main` branch.
- Given the experimental nature of the project, contributors who provide robust, well-thought-out and well-tested contributions might find themselves reviewing other contributions as well.

#### Acknowledgements

- All contributors will be acknowledged in the project repository. Your contributions, no matter how small, are valuable to us and the community.


