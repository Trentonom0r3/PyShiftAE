### Class: `Item`
_________
Abstract base class for various types of items in a project, such as compositions, footage, and folders.

- **Attributes:**
  - `name` **->** `name: str`: 
    - `item.name`: (read/write) The name of the item.
  - `width` **->** `float`: 
    - `item.width`: (read-only) The width of the Item.
  - `height` **->** `float`: 
    - `item.height`: (read-only) The height of the Item.
  - `duration` **->** `float`: 
    - `item.duration`: Duration of the Item, in Item time.
  - `time` **->** 'float' : The 
    - `item.time`:
    Current time of the Item, in Item time.
  - `selected` **->** `bool` : 
    - `item.selected`: (read-write) Boolean value indicating if the layer is selected or not.
 ________________
### Class: `CompItem` (Inherits from `Item`)
_____________________
Represents a composition item, which is a collection of layers.
```py
comp = CompItem("name", 1920, 1080, 24.0, 200.0, 1.0)
# or 
app = App()
item = app.project.activeItem
if isinstance(item, CompItem):
    comp = item
```
- **Constructor:**
  - `CompItem(name: str, width: float, height: float, frameRate: float, duration: float, aspectRatio: float)` **->** `CompItem`
- **Attributes:**
  - `layers` **->** `LayerCollection`: (read-only)
    - `comp.layers`:  A `LayerCollection` object containing the layers of the composition.
  - `layer`**->** `LayerCollection`: (read-only)
    - `comp.layer`: A `LayerCollection` object containing the layers of the composition.
  - `selectedLayers` **->** `LayerCollection`: (read-only)
    - `comp.selectedLayers`: A `LayerCollection` object containing the selected layers of the composition.
  - `selectedlayer` **->** `LayerCollection`: (read-only)
    - `comp.selectedLayers`: A  `LayerCollection` object containing the selected layers of the composition.

  - `numLayers`**->** `int`: 
    - `comp.numLayers`: (read-only) The number of layers in the composition.
  - `width`**->** `float`:
    - `comp.width`: (read/write) The width of the CompItem.
  - `height`**->** `float`: 
    - `comp.height`: (read/write) The height of the CompItem.
  - `duration`**->** `float`: 
    - `comp.duration`: (read/write) The duration of the CompItem.
  - `frameRate`**->** `float`: 
    - `comp.frameRate`: (read/write) The frame rate of the CompItem.
----------------------
### Class: `FootageItem` (Inherits from `Item`)

Represents a footage item in a project. Footage items are used as sources for layers.
```py
footage = FootageItem("Name", "C")
```
- **Constructor:**
  - `FootageItem(name: str, path: str)` **->** `FootageItem`

- **Attributes:**
  - `path` : (read-only) 
    - `footage.path`: The path to the FootageItem. 

- **Methods:** 
  - `replace(name:str, path:str)` **->** `FootageItem`:
    - `footage.replace("Name", "C:/")`: Replaces the Footage in a `FootageItem`.
--------------------

### Class: `FolderItem` (Inherits from `Item`)
-----------------------
Represents a folder item in a project, allowing organization of various items.

```py
folder = FolderItem("New_Folder")
```

- **Attributes:**
  - `children`**->** `ItemCollection`: (read-only) The child items of the folder. Returns and ItemCollection Object.
    ```py
    if isinstance(item, FolderItem):
        children = item.children
    ```
----------------------
### Class: `SolidItem` (Inherits from `FootageItem`)

A Solid Item in a project. 
```py 
item = SolidItem("NAME", (512,512), (1,1,1,1), 24.0)
```
- **Constructor**:
  - `SolidItem(name: str, (width, height): tuple[float,float), (r,g,b,a): tuple[float,float,float,float], duration: float)` **->** `SolidItem`
-------------------
### Class: `ItemCollection`
--------------
Represents a collection of items.

- **Methods:**
  - `append(item: Item)` **->** List[Item]: Adds an item to the item collection. Returns the Item Collection as a list. 
  - `remove(item: Item)` **->** List[Item]: Removes an item from the item collection. Returns the Item Collection as a list.
