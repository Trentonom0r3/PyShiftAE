

# The Application Class:
--------------------
### Summary:

<p>The application class provides access to nearly all functionality within After Effects.
In most cases, the Application class should be instantiated before performing any other processing.<p>

### Class: **App**
-------------------
`app = App()`
- **Attributes:**
  - `project` **->** `Project`: 
    - `project = app.project`: (read/write) The current project being manipulated within the application.
  

- **Methods:**
  - `beginUndoGroup(undo_name: str = "Default Undo Group Name")`:
    - `app.beginUndoGroup()` Begins a new undo group with a given name. Useful for grouping a set of operations that can be undone together.

  - `endUndoGroup()`:
    - `app.endUndoGroup()` Ends the currently active undo group. Should be called after the completion of operations started with `beginUndoGroup`.

  - `reportInfo(info: str)`:
    - `app.reportInfo("Hello World")` Displays an information dialog with a given string. Useful for debugging and user notifications.
  
  - `executeCommand(cmd: int)`: 
    - `app.executeCommand`: Executes the given menu command. 

#### Usage:

```py
import PyShiftAE as psc

app = psc.App()  # Create an Application Object

app.beginUndoGroup("My_Custom_Name")  # Start the Undo Group <----*

proj = app.project  # get the project
print(proj)  # print the proj, to show we have it.

app.executeCommand(0000)  # execute some command

app.endUndoGroup()  # Matches the previously called group <----*

app.reportInfo("Command Executed!")  # Displays a dialogue box in AE.

```