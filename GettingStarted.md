# Getting Started:
----------------------
Assuming you've had a successful install, let's start up After Effects.

Within After Effects, `PyShiftAE` affects two areas for menus:

- `File` **->** `Run Script(.py)`
- `Window` **->** `EXTENSION_NAME`

The `File` menu can be used to execute standalone python scripts. 
The `Window` menu is used to open any `CEPy` extensions that you may have installed. 

In any case, unless you're writing an extension that displays or gets data without interaction with AE, you will be using the `PyShiftAE` library. 

All scripts should start by instantiating the application object.
```py
import PyShiftAE as psc
app = psc.App()  # Now you have access to all AE features.
```

## API Reference:
- [Manifest]()
- [App]()
- [Project]()
- [ProjectCollection]()
- [Item]()
    - [FolderItem]()
        - [ItemCollection]()
    - [SolidItem]()
    - [CompItem]()
        - [LayerCollection]()
    - [FootageItem]()
- [Layer]()
