from PyShiftCore import *
import cv2


print("Imported cv2 version:", cv2.__version__) # Check if cv2 is imported correctly

app.beginUndoGroup(); # Begins undo group. Takes a string as argument. Use func name or something here, otherwise will use default
name = app.project.name # The name of the project
version = app.version # App version, not fully implemented
path = app.project.path # Path to the project
item = app.project.activeItem

if isinstance(item, CompItem): # Check if item is a CompItem
    frame = item.frameAtTime(5) # frame is returned as a standard numpy array
    cv2.imshow("frame", frame) # Show the frame for demonstration purposes
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    
app.endUndoGroup(); # end Undo group, no args. 

# debug prints
print("Project name:", name)
print("Version:", version)
print("Project path:", path)
