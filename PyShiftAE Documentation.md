
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
  - `name`: (read-only) The name of the project.
  - `path`: (read-only) The file path of the project.

- **Methods:**
  - `addFolder(name: str = "New Folder")`: Adds a new folder to the project with the specified name.
  - `addComp(name: str, width: int, height: int, frameRate: float, duration: int, aspectRatio: float)`: Adds a new composition to the project with specified parameters.
  - `addFootage(path: str)`: Adds new footage to the project from a specified file path.
  - `saveAs(path: str)` : Saves the project to the specified path. Allows the user to specify a new location and name for the project file.
  
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
  - `width`: (read-only) The width of the Item.
  - `height`: (read-only) The height of the Item.

- **Methods:**

### Class: `Layer`

Represents a layer within a composition, providing methods to access and modify layer properties.

- **Attributes:**
  - `name`: (read/write) The name of the layer.
  - `index`: (read/write) The index of the layer within its composition. Changing the index can reorder layers.
  - `sourceName`: (read-only) The source name of the layer.
  - `time` : (read-only) The current time of the layer in the layer's time coordinate system. (in seconds) 
  - `compTime` : (read-only) The current time of the layer in the composition's time coordinate system.  (in seconds)
  - `inPoint` : (read-only) The in-point of the layer in the layer's time coordinate system.  (in seconds) `
  - `compInPoint` : (read-only) The in-point of the layer in the composition's time coordinate system.  (in seconds)
  - `duration` : (read-only) The duration of the layer.  (in seconds)
  - `compDuration` : (read-only) The duration of the layer in the composition's time coordinate system.  (in seconds)
  - `quality` : (read/write) The quality setting of the layer. Options of "WIREFRAME", "DRAFT", or "BEST". (layer.quality = BEST, etc)
  - `startTime` : (read/write) The time offset of the layer.
  - `video_active`: (read-write) Determines whether the layer's video is active. Setting this to `True` activates the layer's video, and setting it to `False` deactivates it.
  - `audio_active`: (read-write) Determines whether the layer's audio is active. Setting this to `True` activates the layer's audio, and setting it to `False` deactivates it.
  - `effects_active`: (read-write) Indicates whether effects on the layer are active. Toggle with `True` or `False`.
  - `motion_blur`: (read-write) Controls the motion blur setting for the layer. Enable or disable with `True` or `False`.
  - `frame_blending`: (read-write) Toggles frame blending for the layer. Set to `True` to enable, `False` to disable.
  - `locked`: (read-write) Lock or unlock the layer. `True` locks the layer, preventing changes, and `False` unlocks it.
  - `shy`: (read-write) Sets the layer's shy status. `True` makes the layer shy (hidden in the UI), and `False` makes it visible.
  - `collapse`: (read-write) Toggles the collapse transformation for the layer. Set to `True` to enable, `False` to disable.
  - `auto_orient_rotation`: (read-write) Controls the auto-orientation towards rotation for the layer. `True` to enable, `False` to disable.
  - `adjustment_layer`: (read-write) Defines whether the layer is an adjustment layer. Set to `True` for adjustment layer, `False` otherwise.
  - `time_remapping`: (read-write) Enables or disables time remapping for the layer. `True` to enable, `False` to disable.
  - `layer_is_3d`: (read-write) Specifies if the layer is a 3D layer. `True` for 3D, `False` for 2D.
  - `look_at_camera`: (read-write) Determines whether the layer is set to look at the camera. `True` to enable, `False` to disable.
  - `look_at_poi`: (read-write) Toggles the layer's orientation towards the point of interest. `True` for active, `False` for inactive.
  - `solo`: (read-write) Toggles the solo status of the layer. `True` solos the layer, `False` unsolos it.
  - `markers_locked`: (read-write) Controls whether the markers on the layer are locked. `True` locks them, `False` unlocks.
  - `null_layer`: (read-write) Indicates if the layer is a null object layer. `True` for null layer, `False` otherwise.
  - `hide_locked_masks`: (read-write) Controls the visibility of locked masks. `True` hides them, `False` shows them.
  - `guide_layer`: (read-write) Sets the layer as a guide layer. `True` to set as guide, `False` otherwise.
  - `advanced_frame_blending`: (read-write) Enables or disables advanced frame blending. `True` to enable, `False` to disable.
  - `sublayers_render_separately`: (read-write) Controls whether sublayers render separately. `True` for separate rendering, `False` for combined.
  - `environment_layer`: (read-write) Determines if the layer is an environment layer. `True` to set as environment layer, `False` otherwise.
  
=======
  - `time` : (read-only) The current time of the layer in the layer's time coordinate system.
  - `compTime` : (read-only) The current time of the layer in the composition's time coordinate system.
  - `inPoint` : (read-only) The in-point of the layer in the layer's time coordinate system.
  - `compInPoint` : (read-only) The in-point of the layer in the composition's time coordinate system.
  - `duration` : (read-only) The duration of the layer.
  - `compDuration` : (read-only) The duration of the layer in the composition's time coordinate system.
  - `quality` : (read/write) The quality setting of the layer. Options of "wireframe", "draft", or "best". 
  - `offset` : (read/write) The time offset of the layer.

- **Methods:**
  - `delete()` : Deletes the layer from the composition.
  - `duplicate()` : Creates a duplicate of the layer.
  
### Class: `CompItem` (Inherits from `Item`)

Represents a composition item, which is a collection of layers.
- **Constructor:** (When Creating New Items):
  - `CompItem(name: str, height: int, width: int, frameRate: float, duration: float, aspectRatio: float)`
  ```python
  from PyShiftCore import *
  comp = CompItem(name: str, height: int, width: int, frameRate: float, duration: float, aspectRatio: float)
  ```
- **Attributes:**
  - `layers`: (read-write) `LayerCollection` object, accessed as a list. See `LayerCollection` for more info.

  - `numLayers`: (read-only) The number of layers in the composition.
  - `width`: (read-write) The width of the CompItem.
  - `height`: (read-write) The height of the CompItem.
  - `duration`: (read-write) The duration of the CompItem.
  - `frameRate`: (read-write) The frame rate of the CompItem.

- **Methods:**

### Class: `FootageItem` (Inherits from `Item`)

- **Constructor:**
  - `name: str`, `path: str`
  ```python
  from PyShiftCore import *
  new_item = FootageItem("name", "C:\\")
  
  ```

### Class: `FolderItem` (Inherits from `Item`)

Represents a folder item in a project, allowing organization of various items.

- **Methods:**
  - `addFolder(name: str = "New Folder")`: Adds a new folder to the project.

### Class: `LayerCollection`

A collection of layers within a given composition. Access items just like you would any other list (albeit slightly altered)

- **Methods:**
  - `append(layer: Item)` : Adds a created Item to the comp's layers.
  - `insert(layer: Item, index: int)` : Adds a created item to the comp's layers at the specified index.
  - `remove(layer: Layer)` : Removes a layer given a layer object.
    -
    ```python
    from PyShiftCore import *
    
    comp = app.project.activeItem
    
    layer = comp.layers[0]
    
    if layer.name == "some_name":
      comp.layers.remove(layer)

    for layer in comp.layers:
      # do something

    ```
  - `pop(index: int)` : Removes a layer based on index. If no index is provided, removes the last layer.


### Class: `SolidItem`

A Solid Layer.

- **Constructor**:
  - `SolidItem(name: str, width: float, height: float, red: float, green: float, blue: float, alpha: float, duration: float)`
