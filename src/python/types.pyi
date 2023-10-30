# Use AETypes.d.ts as a guide for creating the python version of the AEType class

from typing import Any, Union, List, Optional
from definitions import *

class ThreeDColorValue:
    pass  # Define the ThreeDColorValue class later

class Property:
    pass  # Define the Property class later

class PropertyGroup:
    pass  # Define the PropertyGroup class later

class MaskPropertyGroup(PropertyGroup):
    pass  # Define the MaskPropertyGroup class later

_PropertyClasses = Union[Property, PropertyGroup, MaskPropertyGroup]

class CameraLayer(Layer):
    pass  # Define the CameraLayer class later

class LayerCollection(Collection):
    index = List[Layer]
    name = List[Layer]
    
    def add(self, item: AVItem, duration: float) -> AVLayer:
        pass
    
    def addNull(self, duration: float) -> AVLayer:
        pass
    
    def addSolid(self, color: ThreeDColorValue, name: str, width: int, height: int, pixelAspect: float, duration: float) -> AVLayer:
        pass
    
    def addBoxTest(self, size: Union[float, float], sourceText: str) -> TextLayer:
        pass
    
    def addText(self, sourceText: str) -> TextLayer:
        pass
    
    def addCamera(self, name: str, centerPoint: [float, float]) -> CameraLayer:
        pass
    
    def addLight(self, name: str, centerPoint: [float, float]) -> LightLayer:
        pass
    
    def addShape(self) -> ShapeLayer:
        pass
    
    def byName(self, name: str) -> Layer:
        pass
    
    def precompose(self, layerIndices: List[int], name: str, moveAllAttributes: bool, leaveAllAttributes: bool) -> CompItem:
        pass

class Layer(PropertyGroup):
    id: int
    index: int
    time: float
    hasVideo: bool
    active: bool
    nullLayer: bool
    selectedProperties: List[_PropertyClasses]
    containingComp: CompItem
    isNameSet: bool
    
    # read-write
    name: str
    parent: Layer
    startTime: float
    stretch: float
    inPoint: float
    outPoint: float
    enabled: bool
    solo: bool
    shy: bool
    locked: bool
    comment: str
    label: int
    autoOrient: AutoOrientType
    
    def moveToBeginning(self) -> None:
        pass
    
    def moveToEnd(self) -> None:
        pass
    
    def moveAfter(self, layer: Layer) -> None:
        pass
    
    def moveBefore(self, layer: Layer) -> None:
        pass
    
    def duplicate(self) -> Layer:
        pass
    
    def activeAtTime(self, time: float) -> bool:
        pass
    
    def setParentWithJump(self, newParent: Layer) -> None:
        pass
    
    def applyPreset(self, presetName: File) -> None:
        pass
    
    ## add shortcuts/transform shortcuts
class AVLayer(Layer):
    source: any
    isNameFromSource: bool
    height: int
    width: int
    canSetCollapseTransformation: bool
    frameBlending: bool
    canSetTimeRemapEnabled: bool
    hasAudio: bool
    audioActive: bool
    isTrackMatte: bool
    hasTrackMatte: bool
    
    # read-write
    audioEnabled: bool
    motionBlur: bool
    effectsActive: bool
    adjustmentLayer: bool
    environementLayer: bool
    guideLayer: bool
    threeDLayer: bool
    threeDPerChar: bool
    collapseTransformation: bool
    frameBlendingType: FrameBlendingType
    timeRemapEnabled: bool
    blendingMode: BlendingMode
    preserveTransparency: bool
    samplingQuality: LayerSamplingQuality
    trackMatteType: TrackMatteType
    quality: LayerQuality
    
    def audioActivateAtTime(self, time: float) -> bool:
        pass
    
    def calculateTransformFromPoints(self, pointTopLeft: [float, float, float], pointTopRight: [float, float, float], pointBottomRight: [float, float, float]) -> None:
        pass
    
    def sourcePointToComp(self, point: [float, float]) -> [float, float]:
        pass
    
    def compPointToSource(self, point: [float, float]) -> [float, float]:
        pass
    
    def openInViewer(self) -> Viewer:
        pass
    
    def replaceSource(self, newSource: FootageSource, fixExpressions: bool) -> None:
        pass
    
    def sourceRectAtTime(self, time: float, extents: bool) -> [float, float, float, float]:
        pass
    
    def addToMotionGraphicsTemplate(self, comp: CompItem) -> None:
        pass
    
    def addToMotionGraphicsTemplateAs(self, comp: CompItem, name: str) -> None:
        pass
    
    def canAddtoMotionGraphicsTemplate(self, comp: CompItem) -> bool:
        pass
    
class CompItem(AVItem):
    # read only
    numLayers: int
    activeCamera: Optional[CameraLayer]
    layers: LayerCollection
    markerProperty: MarkerValueProperty
    selectedLayers: List[Layer]
    selectedProperties: List[_PropertyClasses]
    renderers: List[str]
    motionGraphicsTemplateControllerCount: int

    # read-write
    frameDuration: float
    workAreaStart: float
    workAreaDuration: float
    hideShyLayers: bool
    motionBlur: bool
    draft3d: bool
    frameBlending: bool
    preserveNestedFrameRate: bool
    preserveNestedResolution: bool
    bgColor: ThreeDColorValue
    displayStartTime: float
    resolutionFactor: Union[float, float]
    shutterAngle: float
    shutterPhase: float
    motionBlurSamplesPerFrame: float
    motionBlurAdaptiveSampleLimit: float
    motionGraphicsTemplateName: str
    renderer: str
    dropFrame: bool
    displayStartFrame: int
    
    def duplicate(self) -> CompItem:
        pass

    def layer(self, index: int) -> Layer:
        pass
    
    def layer(self, otherLayer: Layer, relIndex: int) -> Layer:
        pass
    
    def layer(self, name: str) -> Layer:
        pass
    
    def openInViewer(self) -> Viewer:
        pass
    
    def saveFrame(self, time: float, file: File) -> None:
        pass
    
    def ramPreviewTest(self, unknown: any, zoom: float, exposure: float) -> None:
        pass
    
    def exportAsMotionGraphicsTemplate(self, doOverWriteFileIfExisting: bool, file: File) -> bool:
        pass
    
    def openInEssentialGraphics(self) -> None:
        pass
    
    def getMotionGraphicsTemplateController(self, index: int) -> str:
        pass
    
    def setMotionGraphicsControllerName(self, index: int, name: str) -> None:
        pass
    
class FootageItem:
    pass  # Define the FootageItem class later

class FolderItem(Item):
    items: ItemCollection
    numItems: int
    
    def __getitem__(self, index: int) -> Item:
        pass
    
    def __getitem__(self, name: str) -> Item:
        pass

class AVItem(Item):
    proxySource: FootageSource
    usedIn: List[CompItem]
    hasVideo: bool
    hasAudio: bool
    footageMissing: bool
    isMediaReplacementCompatible: bool
    width: int
    height: int
    pixelAspect: float
    frameRate: float
    frameDuration: float
    duration: float
    useProxy: bool
    time: float
    
    def setProxy(self, file: File) -> None:
        pass
    
    def setProxyWithSequence(self, file: File, forceAlphabetical: bool) -> None:
        pass
    
    def setProxyWithSolid(self, color: ThreeDColorValue, name: str, width: int, height: int, pixelAspect: float) -> None:
        pass
    
    def setProxywithPlaceholder(self, name: str, width: int, height: int, frameRate: float, duration: float) -> None:
        pass
    
    def setProxytoNone(self) -> None:
        pass
    
_ItemClasses = Union[CompItem,  FootageItem,  FolderItem]

class FootageSource:
    pass  # Define the FootageSource class later

class Project:
    pass  # Define the Project class later

class Viewer:
    pass  # Define the Viewer class later

class Settings:
    pass  # Define the Settings class later

class Preferences:
    def deletePref(self, section: str, key: str, type: PREFType = None) -> None:
        pass
    
    def getPrefAsBool(self, section: str, key: str, type: PREFType = None) -> bool:
        pass
    
    def getPrefAsFloat(self, section: str, key: str, type: PREFType = None) -> float:
        pass
    
    def getPrefAsLong(self, section: str, key: str, type: PREFType = None) -> int:
        pass
    
    def getPrefAsString(self, section: str, key: str, type: PREFType = None) -> str:
        pass
    
    def havePref(self, section: str, key: str, type: PREFType = None) -> bool:
        pass
    
    def reload(self) -> None:
        pass
    
    def savePrefAsBool(self, section: str, key: str, value: bool, type: PREFType = None) -> None:
        pass
    
    def savePrefAsFloat(self, section: str, key: str, value: float, type: PREFType = None) -> None:
        pass
    
    def savePrefAsLong(self, section: str, key: str, value: int, type: PREFType = None) -> None:
        pass
    
    def savePrefAsString(self, section: str, key: str, value: str, type: PREFType = None) -> None:
        pass
    
    def saveToDisk(self) -> None:
        pass

class Collection:
    length: int
    
class ItemCollection(Collection):
    items = List[_ItemClasses]
    
    ## method to get items by index
    def __getitem__(self, index: int) -> _ItemClasses:
        pass
    
    ## method to get items by name
    def __getitem__(self, name: str) -> _ItemClasses:
        pass
    
    def addComp(self, name: str, width: int, height: int, pixelAspect: float, duration: float, frameRate: float) -> CompItem:
        pass
    
    def addFolder(self, name: str) -> FolderItem:
        pass
    
class Folder:
    pass  # Define the Folder class later

class File:
    pass  # Define the File class later

class _Swatch:
    pass  # Define the _Swatch class later

class app:
    project: Project
    version: str
    buildName: str
    buildNumber: int
    isWatchFolder: bool
    isRenderEngine: bool
    language: Language
    settings: Settings
    isoLanguage: str
    memoryInUse: int
    preferences: Preferences
    activeViewer: Optional[Viewer]
    effects: List[dict]  # List of dictionaries with keys 'displayName', 'matchName', 'version', 'category'
    availableGPUAccelTypes: GpuAccelType
    disableRendering: bool
    onError: Optional[str]
    exitCode: int
    exitAfterLaunchAndEval: bool
    saveProjectOnCrash: bool

    def newProject(self) -> Union[Project, None]:
        pass

    def open(self, file: Optional[File] = None) -> Union[Project, None]:
        pass

    def quit(self) -> None:
        pass

    def watchFolder(self, folder_object_to_watch: Folder) -> None:
        pass

    def pauseWatchFolder(self, pause: bool) -> None:
        pass

    def endWatchFolder(self) -> None:
        pass

    def purge(self, target: PurgeTarget) -> None:
        pass

    def beginUndoGroup(self, undoString: str) -> None:
        pass

    def endUndoGroup(self) -> None:
        pass

    def beginSuppressDialogs(self) -> None:
        pass

    def endSuppressDialogs(self, alert: bool) -> None:
        pass

    def setMemoryUsageLimits(self, imageCachePercentage: int, maximumMemoryPercentage: int) -> None:
        pass

    def setSavePreferencesOnQuit(self, doSave: bool) -> None:
        pass

    def activate(self) -> None:
        pass

    def scheduleTask(self, stringToExecute: str, delay: int, repeat: bool) -> int:
        pass

    def cancelTask(self, taskID: int) -> None:
        pass

    def parseSwatchFile(self, file: File) -> _Swatch:
        pass

    def findMenuCommandId(self, str: str) -> int:
        pass

    def executeCommand(self, id: int) -> None:
        pass

    def getenv(self, name: str) -> str:
        pass

    def setTimeout(self, func: callable, delay: int) -> int:
        pass

    def cancelTimeout(self, id: int) -> None:
        pass
    
class Item:
    # read-only
    id : int
    typeName : str
    
    # read-write
    name : str
    comment: str
    parentFolder: FolderItem
    selected: bool
    label: int
    
    def addGuide(self, orientationType: int, position: int) -> int:
        pass
    
    def removeGuide(self, guideIndex: int) -> None:
        pass
    
    def setGuide(self, position: int, guideIndex: int) -> None:
        pass
    
    def remove(self) -> None:
        pass
    
    
    