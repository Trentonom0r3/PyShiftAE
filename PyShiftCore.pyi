from typing import Optional, List

class app:
    project: Project

    def beginUndoGroup(self, undo_name: str = "Default Undo Group Name") -> None: ...
    def endUndoGroup(self) -> None: ...
    def reportInfo(self, info: str) -> None: ...

class Project:
    activeItem: Optional[Item]
    name: str
    path: str

    def addFolder(self, name: str = "New Folder") -> None: ...
    def addComp(self, name: str, width: int, height: int, frameRate: float, duration: float, aspectRatio: float) -> CompItem: ...
    def addFootage(self, path: str) -> FootageItem: ...
    def saveAs(self, path: str) -> None: ...

class Item:
    name: str
    width: int
    height: int

    def getName(self) -> str: ...
    def setName(self, name: str) -> None: ...

class Layer:
    name: str
    index: int
    sourceName: str
    time: float
    compTime: float
    inPoint: float
    compInPoint: float
    duration: float
    compDuration: float
    quality: str
    startTime: float
    video_active: bool
    audio_active: bool
    effects_active: bool
    motion_blur: bool
    frame_blending: bool
    locked: bool
    shy: bool
    collapse: bool
    auto_orient_rotation: bool
    adjustment_layer: bool
    time_remapping: bool
    layer_is_3d: bool
    look_at_camera: bool
    look_at_poi: bool
    solo: bool
    markers_locked: bool
    null_layer: bool
    hide_locked_masks: bool
    guide_layer: bool
    advanced_frame_blending: bool
    sublayers_render_separately: bool
    environment_layer: bool

    def delete(self) -> None: ...
    def duplicate(self) -> Layer: ...

class CompItem(Item):
    layers: Optional[LayerCollection]
    numLayers: int
    def __init__(self, name: str, width: int, height: int, frameRate: float, duration: float, aspectRatio: float) -> None: ...

class FootageItem(Item):
    def __init__(self, name: str, path: str, index: int) -> None: ...
    
class LayerCollection:
    def __getitem__(self, index: int) -> Layer: ...
    def __setitem__(self, index: int, layer: Layer) -> None: ...
    def __len__(self) -> int: ...
    def append(self, item: Item, index: int = -1) -> Layer: ...
    def insert(self, item: Item, index: int) -> Layer: ...
    def remove(self, layer: Layer) -> Layer: ...
    def pop(self, index: int = -1) -> Layer: ...
    
class SolidItem(FootageItem):
    def __init__(self, name: str, width: float, height: float, red: float, green: float, blue: float, alpha: float, duration: float) -> None: ...
    
    