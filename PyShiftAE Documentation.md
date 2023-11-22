
# PyShiftAE Documentation

## Module: PyShiftCore

### Class: `App`

The `App` class represents the main application interface, providing access to projects and global application functions.

- **Attributes:**
  - `project`: (read/write) The current project being manipulated within the application.

- **Methods:**
  - `beginUndoGroup(undo_name: str = "Default Undo Group Name")`: Begins a new undo group with a given name. Useful for grouping a set of operations that can be undone together.
  - `endUndoGroup()`: Ends the currently active undo group. Should be called after the completion of operations started with `beginUndoGroup`.
  - `reportInfo(info: str)`: Displays an information dialog with a given string. Useful for debugging and user notifications.

- **Example Usage:**
  ```python
  from PyShiftCore import *

  app.beginUndoGroup("My Undo Group")
  # Perform some operations here
  app.endUndoGroup()
  app.reportInfo("Operations Completed")
  ```

### Class: `Project`

Represents a project within the application, providing methods to interact with various project elements like compositions, footage, and folders.

- **Attributes:**
  - `activeItem`: (read-only) The currently active item in the project.
  - `name`: (read/write) The name of the project.
  - `path`: (read/write) The file path of the project.

- **Methods:**
  - `addFolder(name: str = "New Folder")`: Adds a new folder to the project with the specified name.
  - `addComp(name: str, width: int, height: int, frameRate: float, duration: int, aspectRatio: float)`: Adds a new composition to the project with specified parameters.
  - `addFootage(path: str)`: Adds new footage to the project from a specified file path.

- **Example Usage:**
  ```python
  from PyShiftCore import *

  proj = app.project  # get the project

  proj.addFolder("My Folder")  # add a folder to the project bin
  proj.addComp("My Composition", 1920, 1080, 24.0, 10, 1.0)  # add a composition to the project bin
  proj.addFootage("C:\path\to\footage.mov")  # adds footage to the project bin
  ```

### Class: `Item`

Abstract base class for various types of items in a project, such as compositions, footage, and folders.

- **Attributes:**
  - `name`: (read/write) The name of the item.

- **Methods:**
  - `frameAtTime(time: float) -> py::array_t<uint8_t>`: Returns a frame at a specified time as a NumPy array.

### Class: `Layer`

Represents a layer within a composition, providing methods to access and modify layer properties.

- **Attributes:**
  - `name`: (read/write) The name of the layer.
  - `index`: (read/write) The index of the layer within its composition. Changing the index can reorder layers.
  - `sourceName`: (read-only) The source name of the layer.

### Class: `CompItem` (Inherits from `Item`)

Represents a composition item, which is a collection of layers.

- **Attributes:**
  - `layers`: (read-only) A list of all layer objects in the composition.
  - `numLayers`: (read-only) The number of layers in the composition.

- **Methods:**
  - `addLayer(name: str = "New Layer", path: str = NULL, index: int = -1)`: Adds a new layer to the composition with specified parameters.

- **Example Usage:**
  ```python
  from PyShiftCore import *

  comp = app.project.activeItem  # check for the activeItem

  if isinstance(comp, CompItem):  # if comp is actually a composition
      comp.addLayer("name", "C:\\", "2)

  layers = comp.layers  # get the list of layers

  for layer in layers:  
      app.reportInfo(layer.name)
      layer.index = -1  # read-write. Change this to reorder the layers. 

  else:
      app.reportInfo("Select a Composition first!")

  ```

### Class: `FootageItem` (Inherits from `Item`)

Represents a footage item in a project.

### Class: `FolderItem` (Inherits from `Item`)

Represents a folder item in a project, allowing organization of various items.

- **Methods:**
  - `addFolder(name: str = "New Folder")`: Adds a new folder to the project.
