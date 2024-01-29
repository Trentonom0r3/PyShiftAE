
### Class: `Layer`
-------------
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

  -----------------
### Class: `LayerCollection`
---------------------
A collection of layers within a given composition. Access items just like you would any other list (albeit slightly altered)

- **Methods:**
  - `append(layer: Item)` **->** `Layer`: Adds a created Item to the comp's layers. Returns the added Layer.
  - `insert(layer: Item, index: int)` **->** `Layer` : Adds a created item to the comp's layers at the specified index. Returns the added Layer.
  - `remove(layer: Layer)` **->** `NONE` : Removes a layer given a layer object.
  - `pop(index: int)` **->** `NONE`: Removes a layer based on index. If no index is provided, removes the last layer.
--------------------
