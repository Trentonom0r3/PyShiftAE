from PyShiftCore import *
import cv2
import numpy as np

print("Imported cv2 version:", cv2.__version__) # Check if cv2 is imported correctly

app.beginUndoGroup(); # Begins undo group. Takes a string as argument. Use func name or something here, otherwise will use default
item = app.project.activeItem

if isinstance(item, CompItem): # Check if item is a CompItem
    frame = item.frameAtTime(5) # frame is returned as a standard numpy array
    img = np.random.randint(0, 255, (frame.shape[0], frame.shape[1], frame.shape[2]), dtype=np.uint8) # Create a random image
    item.replaceFrameAtTime(img, 5) # Replace frame at time 5 with the random image
    
app.endUndoGroup(); # end Undo group, no args. 

# debug prints

