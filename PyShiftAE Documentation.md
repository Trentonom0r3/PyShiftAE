

# PyShiftAE Documentation

## Module: PyShiftCore

# **Core Access Classes**

### Class: `Manifest`

The `Manifest` class is used to connect PyShiftAE Extensions with CEP and After Effects. 
If a field does not have a value you wish to place, simply forego altering it. 

- **Attributes:**
  - `name` **->** `str`: The Name of your Manifest. Should Match the `comp.psc.EXTENSIONNAME` folder's name. 
  - `version` **->** `str` : The version of your extension. 
  - `author` **->** `str` :  The author of the extension.
  - `description` **->** `str` : A brief description of what your extension's purpose is. 
  - `entry` **->** `str` : The main entry file for your extension. Must be named `entry.py`. 
  - `dependencies` **->** `list[str]`: A list of pip-installable names for your extension's module dependencies. 

### Class: `App`

The `App` class represents the main application interface, providing access to projects and global application functions. `App` may be instantiated, or you may use the provided global object; `app`.

- **Attributes:**
  - `project` **->** `Project`: (read/write) The current project being manipulated within the application.

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

### Class: `project`

Represents a project within the application, providing methods to interact with various project elements like compositions, footage, and folders.

- **Attributes:**
  - `activeItem` **->** `Item`: (read-only) The currently active item in the project.
  - `name` **->** `name: str`: (read-only) The name of the project.
  - `path` **->** `path: str`: (read-only) The file path of the project.
  - `items` **->** `ProjectCollection`: (read-only) The items in the project.
      - **Usage Notes:** It is necessary to assign `items` to a new variable before using it.
      - 
      ```python
        from PyShiftCore import *
        items = app.project.items
        if isinstance(items, ProjectCollection):
          for item in items:


        from PyShiftCore import *
        project = app.project
        for item in project.items:
          
      ```


- **Methods:**
  - `saveAs(path: str)` **->** `void` : Saves the project to the specified path. Allows the user to specify a new location and name for the project file.
  
# **Item Classes!**

### Class: `Item`

Abstract base class for various types of items in a project, such as compositions, footage, and folders.

- **Attributes:**
  - `name` **->** `name: str`: (read/write) The name of the item.
  - `width` **->** `float`: (read-only) The width of the Item.
  - `height` **->** `float`: (read-only) The height of the Item.
  - `duration` **->** `float`: Duration of the Item, in Item time.
  - `time` **->** 'float' : The Current time of the Item, in Item time.
  - `selected` **->** `bool` : (read-write) Boolean value indicating if the layer is selected or not.
 
### Class: `CompItem` (Inherits from `Item`)

Represents a composition item, which is a collection of layers.
- **Constructor:**
  - `CompItem(name: str, width: float, height: float, frameRate: float, duration: float, aspectRatio: float)` **->** `CompItem`
- **Attributes:**
  - `layers` **->** `LayerCollection`: (read-only) A `LayerCollection` object containing the layers of the composition.
  - `layer`**->** `LayerCollection`: (read-only) A `LayerCollection` object containing the layers of the composition.
  - `selectedLayers` **->** `LayerCollection`: (read-only) A `LayerCollection` object containing the selected layers of the composition.
  - `selectedlayer` **->** `LayerCollection`: (read-only) A `LayerCollection` object containing the selected layers of the composition.
      
      
      ```python
        from PyShiftCore import *
        comp = app.project.activeItem
        if isinstance(comp, CompItem):
          layers = comp.layers
          for layer in layers:

        from PyShiftCore import *
        comp = app.project.activeItem
        if isinstance(comp, CompItem):
          for layer in comp.layers:
          
      ```
  - `numLayers`**->** `int`: (read-only) The number of layers in the composition.
  - `width`**->** `float`: (read/write) The width of the CompItem.
  - `height`**->** `float`: (read/write) The height of the CompItem.
  - `duration`**->** `float`: (read/write) The duration of the CompItem.
  - `frameRate`**->** `float`: (read/write) The frame rate of the CompItem.

### Class: `FootageItem` (Inherits from `Item`)

Represents a footage item in a project. Footage items are used as sources for layers.

- **Constructor:**
  - `FootageItem(name: str, path: str)` **->** `FootageItem`

- **Attributes:**
  - `path` : (read-only) The path to the FootageItem. 

### Class: `FolderItem` (Inherits from `Item`)

Represents a folder item in a project, allowing organization of various items.

- **Attributes:**
  - `children`**->** `ItemCollection`: (read-only) The child items of the folder. Returns and ItemCollection Object.

       
      ```python
        from PyShiftCore import *
        items = app.project.items
        for item in items:
          if isinstance(item, FolderItem):
            children = item.children
            for child in children:
              # do something 

   
        from PyShiftCore import *
        project = app.project
        for item in project.items: 
          if isinstance(item, FolderItem):
            for child in item.children:

      ```

 
### Class: `SolidItem` (Inherits from `FootageItem`)

A Solid Item in a project. 

- **Constructor**:
  - `SolidItem(name: str, width: float, height: float, red: float, green: float, blue: float, alpha: float, duration: float)` **->** `SolidItem`

# **Collection Classes!**

### Class: `LayerCollection`

A collection of layers within a given composition. Access items just like you would any other list (albeit slightly altered)

- **Methods:**
  - `append(layer: Item)` **->** `Layer`: Adds a created Item to the comp's layers. Returns the added Layer.
  - `insert(layer: Item, index: int)` **->** `Layer` : Adds a created item to the comp's layers at the specified index. Returns the added Layer.
  - `remove(layer: Layer)` **->** `NONE` : Removes a layer given a layer object.
  - `pop(index: int)` **->** `NONE`: Removes a layer based on index. If no index is provided, removes the last layer.


### Class: `ProjectCollection`

Represents a collection of projects.

- **Methods:**
  - `append(item: Item)` **->** List[Item]: Adds an item to the ProjectCollection. Returns the ProjectCollection as a list
  - `remove(item: Item)` **->** List[Item]: Removes an item from the ProjectCollection. Returns the ProjectCollection as a list.

### Class: `ItemCollection`

Represents a collection of items.

- **Methods:**
  - `append(item: Item)` **->** List[Item]: Adds an item to the item collection. Returns the Item Collection as a list. 
  - `remove(item: Item)` **->** List[Item]: Removes an item from the item collection. Returns the Item Collection as a list.

# **Layer Classes!**

### Class: `Layer`

Represents a layer within a composition, providing methods to access and modify layer properties.

- **Attributes:**
  - `name`**->** `name: str`: (read/write) The name of the layer.
  - `index`**->** `index: int`: (read/write) The index of the layer within its composition. Changing the index can reorder layers.
  - `source`**->** `sourceItem: FootageItem`: returns a FootageItem object for the layer.
  - `sourceName`**->** `name: str`: (read-only) The source name of the layer.
  - `time` **->** `float`: (read-only) The current time of the layer in the layer's time coordinate system. (in seconds) 
  - `compTime`**->** `float` : (read-only) The current time of the layer in the composition's time coordinate system.  (in seconds)
  - `inPoint` **->** `float`: (read-only) The in-point of the layer in the layer's time coordinate system.  (in seconds) `
  - `compInPoint`**->** `float` : (read-only) The in-point of the layer in the composition's time coordinate system.  (in seconds)
  - `duration` **->** `float`: (read-only) The duration of the layer.  (in seconds)
  - `compDuration`**->** `float` : (read-only) The duration of the layer in the composition's time coordinate system.  (in seconds)
  - `quality(enum value)` **->** `string`: (read/write) The quality setting of the layer. Options of "NONE", "WIREFRAME", "DRAFT", or "BEST". (layer.quality = BEST, etc)
  - `startTime` **->** `float`: (read/write) The time offset of the layer.
  - `video_active` **->** `bool`: (read-write) Determines whether the layer's video is active. Setting this to `True` activates the layer's video, and setting it to `False` deactivates it.
  - `audio_active`**->** `bool`: (read-write) Determines whether the layer's audio is active. Setting this to `True` activates the layer's audio, and setting it to `False` deactivates it.
  - `effects_active`**->** `bool`: (read-write) Indicates whether effects on the layer are active. Toggle with `True` or `False`.
  - `motion_blur`**->** `bool`: (read-write) Controls the motion blur setting for the layer. Enable or disable with `True` or `False`.
  - `frame_blending`**->** `bool`: (read-write) Toggles frame blending for the layer. Set to `True` to enable, `False` to disable.
  - `locked`**->** `bool`: (read-write) Lock or unlock the layer. `True` locks the layer, preventing changes, and `False` unlocks it.
  - `shy`**->** `bool`: (read-write) Sets the layer's shy status. `True` makes the layer shy (hidden in the UI), and `False` makes it visible.
  - `collapse`**->** `bool`: (read-write) Toggles the collapse transformation for the layer. Set to `True` to enable, `False` to disable.
  - `auto_orient_rotation`**->** `bool`: (read-write) Controls the auto-orientation towards rotation for the layer. `True` to enable, `False` to disable.
  - `adjustment_layer`**->** `bool`: (read-write) Defines whether the layer is an adjustment layer. Set to `True` for adjustment layer, `False` otherwise.
  - `time_remapping`**->** `bool`: (read-write) Enables or disables time remapping for the layer. `True` to enable, `False` to disable.
  - `layer_is_3d`**->** `bool`: (read-write) Specifies if the layer is a 3D layer. `True` for 3D, `False` for 2D.
  - `look_at_camera`**->** `bool`: (read-write) Determines whether the layer is set to look at the camera. `True` to enable, `False` to disable.
  - `look_at_poi`**->** `bool`: (read-write) Toggles the layer's orientation towards the point of interest. `True` for active, `False` for inactive.
  - `solo`**->** `bool`: (read-write) Toggles the solo status of the layer. `True` solos the layer, `False` unsolos it.
  - `markers_locked`**->** `bool`: (read-write) Controls whether the markers on the layer are locked. `True` locks them, `False` unlocks.
  - `null_layer`**->** `bool`: (read-write) Indicates if the layer is a null object layer. `True` for null layer, `False` otherwise.
  - `hide_locked_masks`**->** `bool`: (read-write) Controls the visibility of locked masks. `True` hides them, `False` shows them.
  - `guide_layer`**->** `bool`: (read-write) Sets the layer as a guide layer. `True` to set as guide, `False` otherwise.
  - `advanced_frame_blending`**->** `bool`: (read-write) Enables or disables advanced frame blending. `True` to enable, `False` to disable.
  - `sublayers_render_separately`**->** `bool`: (read-write) Controls whether sublayers render separately. `True` for separate rendering, `False` for combined.
  - `environment_layer`**->** `bool`: (read-write) Determines if the layer is an environment layer. `True` to set as environment layer, `False` otherwise.

- **Methods:**
  - `delete()` : Deletes the layer from the composition.
  - `duplicate()` **->** `FootageItem`: Creates a duplicate of the layer.
