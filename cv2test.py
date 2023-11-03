from PyShiftCore import *
import cv2
import numpy as np
import traceback

print("Imported cv2 version:", cv2.__version__)  # Check if cv2 is imported correctly

app.beginUndoGroup()
item = app.project.activeItem
length = item.duration * 12

if isinstance(item, CompItem):  # Check if item is a CompItem
    try:
        for i in range(1, int(length) - 1):  # Loop through all frames
            img = item.frameAtTime(i)  # Get frame
            new_img = np.random.randint(0, 255, (img.shape[0], img.shape[1], img.shape[2]), dtype=np.uint8)
            item.replaceFrameAtTime(new_img, i)  # Replace frame
    except Exception as e:
        print("Error:", str(e))
        traceback.print_exc()
else:
    print("Active item is not a CompItem")

app.endUndoGroup()
