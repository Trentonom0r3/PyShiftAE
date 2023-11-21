# PyShiftAE Documentation
Unlike a LOT of ExtendScript, EVERYTHING will be documented here.
## Module: PyShiftCore

### Class: `App`
- **Attributes:**
  - `project` (read-only): The current project.
- **Methods:**
  - `beginUndoGroup(undo_name: str = "Default Undo Group Name")`: Begins a new undo group with the specified name.
  - `endUndoGroup()`: Ends the current undo group.
  - `executeCommand(commandId: int)`: Executes a command with the specified command ID.
  - `reportInfo(info: str)`: Displays an alert box displaying the given string.

   - **TODO:**
    - **Methods:**
      - `startQuietErrors()`: Silences Errors. Must be balanced with `endQuietErrors`
      - `endQuietErrors()`: Re-enables errors.
      - `writeToConsole(info: str)` : writes to the OS Console.
      - `newProject()`: Creates a new project. NOTE: Will close the current project without saving it first!
      - `openProject(file_path: str)` : Opens a project given path to the .aep file.
      - `sendEvent(event: str, host: str, sender: str, message: str)` : Sends a CSXS event.
      - `addEventListener(event: str)` : Adds a CSXS event Listener to the python script. 
      - `removeEventListener(event: str)` : Removes a CSXS Event Listener.

### Class: `Project`
- **Attributes:**
  - `activeItem` (read-only): The active item in the project.

   - **TODO:**
    - **Attributes:**
      - `name` (read-only): The name of the project. 
      - `path` (read-only): The path to the .aep file for the project.
      - `root` (read-only): The root of the project, treated as a folder.
      - `bitDepth` (read-write): The Project's Bit Depth.
      - **Methods:**
        - `save(file_path: str = None)`: Saves the project. If `file_path` is provided, saves the project to the specified path. If `file_path` is not provided, saves the project to its current location or prompts for a location if the project is new and unsaved. The `file_path` parameter is optional.

### Class: `Item`
- **Attributes:**
  - `name` (read-write): The name of the item.
  - `type` (read-only): The Item Type. (Comp, Footage, Folder)

### Class: `FootageItem` (Inherits from `Item`)
- **Attributes:**

### Class: `FolderItem` (Inherits from `Item`)
- **Attributes:**

### Class: `CompItem` (Inherits from `Item`)
- **Attributes:**
  - `layer` or `layers` (read-write): A list of all layer objects in comp. Access using `layer[0]` or `layers`, the latter of which returns a list which can then be accessed via `for layer in layers`. 

   - **TODO:**
     - **Attributes:**
    - `selectedLayers` (read-write) : A list of selected layers in the comp.

### Class: `Layer`
- **Attributes:**
  - `name` (read-write): The name of the layer. 
  - `index` (read-only): The index of the layer.
  - `sourceName` (read-only): The Source name of the layer.
  
     - **TODO:**
      - **Attributes:**
      - `quality` (read-write): The layer's quality setting. 
      - `duration` (read-write): Duration of the layer. 
      
