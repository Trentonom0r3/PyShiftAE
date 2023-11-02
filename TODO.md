# Current Progress

- Embedded Python 3.11 Interpreter, able to use numpy and other libs.
- Implemented basic version of 'App' and 'Project' classes.
- Usage is the same as extendscript "app.project, app.project.name, app.version, app.executeCommand(cmdID)"

## TODO

- Implement basic version of 'Item' class and all of its subclasses.
- Implement ItemCollection.
- Set things up so we can use 
  ```
  items = app.project.items
  comps = []
  for item in items:
    if (isInstance item, FootageItem):
      img = item.frame(frame_idx)  # returns frame into numpy array
      new_img = some_img_manipulation_function(img);
      item.replace(new_img)
    elif (isInstance item, CompItem):
      comp = item.comp
      comps.append(comp)
    elif (isInstance item, FolderItem):
      pass
  '''

- From there, first ensure everything works as properly in its basic state, then continue expanding the library.

- After the library is to a decent size, write python stub files for pip install, so the user can easily write their python scripts in an IDE.

- Finally, implement a method akin to 'csinterface.evalscript()' to provide an interface for CEP extension to use.

- Eventually, add some way to control scripting without AE being open, and from scripts running in cmd prompt.
  
        
