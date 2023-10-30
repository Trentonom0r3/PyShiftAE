# Use AETypes.d.ts as a guide for creating the python version of the AEType class

from typing import Union, List, Optional
from definitions import *

class ThreeDColorValue:
    pass  # Define the ThreeDColorValue class later

class File:
    pass  # Define the File class later

class FootageSource:
    pass  # Define the FootageSource class later

class Property:
    pass  # Define the Property class later

class PropertyGroup:
    pass  # Define the PropertyGroup class later

class MaskPropertyGroup:
    pass  # Define the MaskPropertyGroup class later

_PropertyClasses = Union[Property, PropertyGroup, MaskPropertyGroup]

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


class GpuAccelType:
    pass  # Define the GpuAccelType enum later

class Folder:
    pass  # Define the Folder class later

class File:
    pass  # Define the File class later

class Language:
    pass  # Define the Language enum later

class PurgeTarget:
    pass  # Define the PurgeTarget enum later

class _Swatch:
    pass  # Define the _Swatch class later

class Application:
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
    