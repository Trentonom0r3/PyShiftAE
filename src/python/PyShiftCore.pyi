# PyShiftCore.pyi

from typing import Any, List, Optional, Union
import numpy as np

class Item:
    name: str
    duration: float
    time: float

    def getName(self) -> str: ...
    def setName(self, name: str) -> None: ...
    
class FootageItem(Item):
    pass

class FolderItem(Item):
    pass

class CompItem(Item):
    def frameAtTime(self, time: float) -> np.ndarray: ...
    def replaceFrameAtTime(self, img: np.ndarray, time: float) -> None: ...

class Project:
    name: str
    path: str
    activeItem: Item

class App:
    version: str
    project: Project

    def beginUndoGroup(self, undo_name: str = "Default Undo Group Name") -> None: ...
    def endUndoGroup(self) -> None: ...
    def executeCommand(self, commandId: int) -> None: ...

# Variable representing the app instance
app: App
