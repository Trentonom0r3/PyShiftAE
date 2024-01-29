# The Project Class:
----------------------
### Summary:

<p>The project class provides access to everything within the currently open project. Currently, there can only be one project open at a time. Access this through the application class. The project class should be used in nearly every script.<p>


### Class: `Project`
-------------------
```py
app = App()
project = app.project
```

Represents a project within the application, providing methods to interact with various project elements like compositions, footage, and folders.

- **Attributes:**
  - `activeItem` **->** `Item`: 
    - `project.activeItem`: (read-only) The currently active item in the project.


  - `activeLayer` **->** `Layer`:
    - `project.activeLayer`: (read-only) The currently active layer in the project.


  - `name` **->** `name: str`: 
    - `project.name`: (read-only) The name of the project.


  - `path` **->** `path: str`:
    - `project.path`: (read-only) The file path of the project.


  - `items` **->** `ProjectCollection`: 
    - `project.items`: (read-only) The items in the project.


  - `selectedItems` **->** `ProjectCollection`:                                    
     - `project.selectedItems`: (read-only) The selected items in the project. 

- **Methods:**
    - `saveAs(path: str)` : Saves the project to the specified path.
--------------------
# The ProjectCollection Class:
---------------

### Summary:

<p>The ProjectCollection class provides a way to manipulate collections of items in the project via pythonic list interfaces. Usage is (mostly) the same as is with standard python lists. Can and (should) only be accessed through `project.items` or `project.selectedItems`.<p>

-------------------
### Class: `ProjectCollection`:
```py
app = App()
items = app.project.items
```
Represents a collection of projects.

- **Methods:**
  - `append(item: Item)` **->** List[Item]: 
    - `items.append(Item)`: Adds an item to the ProjectCollection. Returns the ProjectCollection as a list


  - `remove(item: Item)` **->** List[Item]:
    - `items.remove(Item)`: Removes an item from the ProjectCollection. Returns the ProjectCollection as a list.
