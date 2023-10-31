# Use AETypes.d.ts as a guide for creating the python version of the AEType class

from typing import Any, Generic, Union, List, Optional, TypeVar
from definitions import *

class ThreeDColorValue:
    pass  # Define the ThreeDColorValue class later

T = TypeVar("T", bound="UnknownPropertyType")

class NoValueType(PropertyClassMembers):
    propertyValueType: PropertyValueType.NO_VALUE
    value: Any

ColorValue = List[float]  # [number, number, number, number]
ThreeDColorValue = List[float]  # [number, number, number]

class ColorType(PropertyClassMembers):
    propertyValueType: PropertyValueType.COLOR
    value: ColorValue

class BooleanType(PropertyClassMembers):
    propertyValueType: PropertyValueType.OneD
    value: bool

# ... and so on for other types like OneDType, TwoDType, etc.


class PropertyClassMembers:
    # The class will take dynamic attributes, possibly defined by the Property class
    
    def __init__(self, **kwargs):
        for key, value in kwargs.items():
            setattr(self, key, value)

class UnknownPropertyType(PropertyClassMembers):
    propertyValueType: PropertyValueType
    value: Any


class Property(Generic[T]):
    propertyValueType: PropertyValueType
    value: Any
    hasMin: bool
    hasMax: bool
    minValue: Any
    maxValue: Any
    isSpatial: bool
    canVaryOverTime: bool
    isTimeVarying: bool
    numKeys: int
    unitsText: str
    expressionError: str
    selectedKeys: List[int]
    propertyIndex: int
    isSeparationFollower: bool
    isSeparationLeader: bool
    separationDimension: int
    separationLeader: Property(TwoDProperty)
    isDropdownEffect: bool
    alternateSource: AVItem
    canSetAlternateSource: bool
    
    # read-write
    expression: str
    dimensionsSeparated: bool
    
    def __init__(self, propertyValueType: PropertyValueType, value: Any, hasMin: bool, ...):
        self.propertyValueType = propertyValueType
        self.value = value
        self.hasMin = hasMin
        # ... initialize other attributes

    def valueAtTime(self, time: float, preExpression: bool) -> Any:
        pass  # Implementation

    # ... and so on for other methods


class PropertyBase:
    matchName: str
    propertyIndex: int
    propertyDepth: int
    propertyType: PropertyType
    parentProperty: PropertyGroup
    isModified: bool
    canSetEnabled: bool
    active: bool
    elided: bool
    isEffect: bool
    isMask: bool
    
    # read-write
    name: str
    enabled: bool
    selected: bool
    
    def getPropertyGroup(self, countUp: int) -> PropertyGroup:
        pass
    
    def remove(self) -> None:
        pass
    
    def moveTo(self, newIndex: int) -> None:
        pass
    
    def duplicate(self) -> Property:
        pass
    
    def setProperty(self, index: int) -> None:
        pass
    
    def setProperty(self, name: str) -> None:
        pass
    
class PropertyGroup(PropertyBase):
    numProperties: int
    
    def getProperty(self, index: int) -> _PropertyClasses:
        pass
    
    def getProperty(self, name: str) -> _PropertyClasses:
        pass
    
    def canAddProperty(self, name: str) -> bool:
        pass
    
    def addProperty(self, name: str) -> _PropertyClasses:
        pass
    
    
class MaskPropertyGroup(PropertyGroup):
    pass  # Define the MaskPropertyGroup class later

_PropertyClasses = Union[Property, PropertyGroup, MaskPropertyGroup]

class CameraLayer(Layer):
    cameraOption: _CameraOptionsGroup

class LayerCollection(Collection):
    def get_index(self, index: int) -> Layer:
        pass
    
    def get_name(self, name: str) -> Layer:
        pass
    
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
    numItems: int
    selection: List[_ItemClasses]
    renderQueue: RenderQueue
    # read-write
    bitsPerChannel: int
    transparencyGridThumbnails: bool
    displayStartFrame: int
    gpuAccelType: GpuAccelType
    linearBlending: bool
    xmpPacket: str
    framesCountType: FramesCountType
    feetFramesFilmType: FeetFramesFilmType
    framesUseFeetFrames: bool
    footageTimecodeDisplayStartType: FootageTimecodeDisplayStartType
    timeDisplayType: TimeDisplayType
    toolType: ToolType
    workingGamma: float
    workingSpace: str
    linearizeWorkingSpace: bool
    compensateForSceneReferredProfiles: bool
    expressionEngine: str
    #iinit with AEGP_ProjectH
    def __init__(self, file: Optional[File] = None):
        self.file = file
        self.rootFolder = FolderItem()
        self.items = ItemCollection()
        self.activeItem = "GET_ACTIVE_ITEM_C++"
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        pass
    
    def item(self, index: int) -> _ItemClasses:
        pass
    
    def itemById(self, id: int) -> _ItemClasses:
        pass
    
    def consolidateFootage(self) -> int:
        pass
    
    def removeUnusedFootage(self) -> int:
        pass
    
    def reduceProject(self, array_of_items: List[_ItemClasses]) -> int:
        pass
    
    def close(self, closeOptions: CloseOptions) -> bool:
        pass
    
    def save(self, file: Optional[File] = None) -> None:
        pass
    
    def saveWithDialog(self) -> bool:
        pass
    
    def importPlaceholder(self, name: str, width: int, height: int, frameRate: float, duration: float) -> PlaceholderItem:
        pass
    
    def importFile(self, importOptions: ImportOptions) -> _ItemClasses:
        pass
    
    def importFileWithDialog(self) -> List[_ItemClasses]:
        pass
    
    def showWindow(self, doShow: bool) -> None:
        pass
    
    def autoFixExpressions(self, oldText: str, newText: str) -> None:
        pass
    
    def newTeamProject(self, teamProjectName: str, description: str) -> bool:
        pass
    
    def openTeamProject(self, teamProjectName: str) -> bool:
        pass
    
    def shareTeamProject(self, comment: str) -> bool:
        pass
    
    def syncTeamProject(self) -> bool:
        pass
    
    def closeTeamProject(self) -> bool:
        pass
    
    def convertTeamProjectToProject(self, project_file: File) -> bool:
        pass
    
    def listTeamProjects(self) -> List[str]:
        pass
    
    def isTeamProjectOpen(self, teamProjectName: str) -> bool:
        pass
    
    def isAnyTeamProjectOpen(self) -> bool:
        pass
    
    def isTeamProjectEnabled(self) -> bool:
        pass
    
    def isLoggedInToTeamProject(self) -> bool:
        pass
    
    def isSyncCommandEnabled(self) -> bool:
        pass
    
    def isShareCommandEnabled(self) -> bool:
        pass
    
    def isResolveCommandEnabled(self) -> bool:
        pass
    
    def resolveConflict(self, ResolveType: ResolveType) -> bool:
        pass
    
    def listColorProfiles(self) -> List[str]:
        pass
    
    def setDefaultImportFolder(self, folder: Folder) -> bool:
        pass
    
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

class App:
    project: Project("GetProjC++")  # Call c function to populate all of these attributes
    version: str        # Populate Project the same way
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
    
    
    