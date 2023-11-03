# PyShiftAE
PyShiftAE is an experimental library designed to transcend the boundaries of the built-in ExtendScript library in Adobe After Effects (AE), heralding a new era of scripting possibilities in AE. By offering a Python library as an alternative to ExtendScript, PyShiftAE not only enriches the existing feature set but simplifies and amplifies the scripting experience.

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
  
# Documentation 
- (API-Reference)[https://github.com/Trentonom0r3/PyShiftAE/wiki/API-Reference]
- (CSXS)[https://github.com/Trentonom0r3/PyShiftAE/wiki/CSXS-Utils]
- (Demos)[https://github.com/Trentonom0r3/PyShiftAE/wiki/Demos]

## Contributing

Contributions from the community are welcome, and I would be pleased to have them. Please follow this guide for contributing to PyShiftAE.

### Reporting Issues

- Please make sure to check for existing issues before creating a new one. Your issue might have already been reported and possibly even fixed in the `main` branch.
- When creating an issue, please provide a clear and descriptive title, and explain the problem in detail. Include any relevant code snippets or error messages.

### Submitting Pull Requests

- Fork the repository and create a new branch for your feature or bug fix.
- There's no strict coding style guideline at the moment, but please keep your code clean and well-commented. Following the general structure and commenting style of the existing code would be appreciated. To further this, detailed comments help both myself, and others learn and collaborate better.
- Submit your pull request against the `main` branch. If there is a `develop` branch or other relevant branches in the future, please redirect your pull request accordingly.

### Code Guidelines

- Your code should be well-commented with docstrings for classes and methods.
- Please try to keep your code as clean and straightforward as possible.
- It would be beneficial to follow common Python conventions and keep a consistent coding style. (On the API exposure side)

### Testing

- At the moment, there is not a specific testing framework in place. However, I would appreciate if you could provide simple tests or usage examples to demonstrate the functionality of your contribution. Eg: scripts used, video demonstrations, etc;

### Documentation

- You can follow the existing documentation as a format guide.
- When adding new features, it would be great to include relevant documentation updates alongside your code contributions.

### Community Interaction

- Discussion around the project will primarily occur here on GitHub. Feel free to ask questions or discuss anything related to the project in the issue tracker.
- You can get in touch with me directly via the contact information provided on GitHub.

### Legal

- By contributing, you agree that your contributions will be licensed under the same license as the project (AGPL 3.0).
- Ensure that you have the right to submit your contribution and that your code does not infringe any other person's or entity's rights.

### Environment Setup

- The project is currently set up for Microsoft x64, using pybind11 x64 static, linked to Python 3.11 (debug and release). Follow the instructions provided in the WIKI for setting up your development environment.

### Review Process

- Contributions will be reviewed by the community and maintainers. Constructive feedback will be provided, and once your contribution is accepted, it will be merged into the `main` branch.
- Given the experimental nature of the project, contributors who provide robust, well-thought-out and well-tested contributions might find themselves reviewing other contributions as well.

### Acknowledgements

- All contributors will be acknowledged in the project repository. Your contributions, no matter how small, are valuable to us and the community.


