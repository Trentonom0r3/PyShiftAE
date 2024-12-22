import PyFx # type: ignore
from .Collection import ItemCollection, LayerCollection
from .AssetManager import AssetManager
from typing import Union

class Item:
    def __init__(self, item: PyFx.ItemPtr) -> None:
        self.item = item
    
    @staticmethod
    def active_item() -> Union['Item', 'CompItem', 'FootageItem', None]:
        item_ptr = PyFx.ItemSuite().GetActiveItem()
        if item_ptr:
            return ItemFactory.create_item(item_ptr)
        
    @property
    def name(self) -> str:
        return PyFx.ItemSuite().GetItemName(self.item)
    
    @name.setter
    def name(self, value: str) -> None:
        PyFx.ItemSuite().SetItemName(self.item, value)
        
    @property
    def type(self) -> PyFx.ItemType:
        return PyFx.ItemSuite().GetItemType(self.item)
    
    @property
    def parent_folder(self) -> 'FolderItem':
        return FolderItem(PyFx.ItemSuite().GetItemParentFolder(self.item))
    
    @parent_folder.setter
    def parent_folder(self, folder: 'FolderItem') -> None:
        PyFx.ItemSuite().SetItemParentFolder(self.item, folder.item)
        
    @property
    def duration(self) -> float:
        return PyFx.ItemSuite().GetItemDuration(self.item)
    
    @property
    def current_time(self) -> float:
        return PyFx.ItemSuite().GetItemCurrentTime(self.item)
    
    @current_time.setter
    def current_time(self, value: float) -> None:
        PyFx.ItemSuite().SetItemCurrentTime(self.item, value)
        
    @property
    def comment(self) -> str:
        return PyFx.ItemSuite().GetItemComment(self.item)
    
    @comment.setter
    def comment(self, value: str) -> None:
        PyFx.ItemSuite().SetItemComment(self.item, value)
        
    @property
    def label(self) -> int:
        return PyFx.ItemSuite().GetItemLabel(self.item)
    
    @label.setter
    def label(self, value: int) -> None:
        PyFx.ItemSuite().SetItemLabel(self.item, value)
        
    @property
    def dimensions(self) -> tuple[int, int]:
        return PyFx.ItemSuite().GetItemDimensions(self.item)
    
    @property
    def pixel_aspect(self) -> float:
        return PyFx.ItemSuite().GetItemPixelAspectRatio(self.item)
    
    @property
    def selected(self) -> bool:
        return PyFx.ItemSuite().IsItemSelected(self.item)
    
    @selected.setter
    def selected(self, value: bool) -> None:
        PyFx.ItemSuite().SelectItem(self.item, value)
        
    @property
    def missing(self) -> bool:
        return PyFx.ItemSuite().GetItemFlags(self.item) & PyFx.ItemFlag.MISSING
    
    @property
    def has_proxy(self) -> bool:
        return PyFx.ItemSuite().GetItemFlags(self.item) & PyFx.ItemFlag.HAS_PROXY
    
    @property
    def using_proxy(self) -> bool:
        return PyFx.ItemSuite().GetItemFlags(self.item) & PyFx.ItemFlag.USING_PROXY
    
    @using_proxy.setter
    def use_proxy(self, value: bool) -> None:
        PyFx.ItemSuite().SetItemUseProxy(self.item, value)
        
    @property
    def missing_proxy(self) -> bool:
        return PyFx.ItemSuite().GetItemFlags(self.item) & PyFx.ItemFlag.MISSING_PROXY
    
    @property
    def has_video(self) -> bool:
        return PyFx.ItemSuite().GetItemFlags(self.item) & PyFx.ItemFlag.HAS_VIDEO
    
    @property
    def has_audio(self) -> bool:
        return PyFx.ItemSuite().GetItemFlags(self.item) & PyFx.ItemFlag.HAS_AUDIO
    
    @property
    def still(self) -> bool:
        return PyFx.ItemSuite().GetItemFlags(self.item) & PyFx.ItemFlag.STILL
    
    @property
    def has_active_audio(self) -> bool:
        return PyFx.ItemSuite().GetItemFlags(self.item) & PyFx.ItemFlag.HAS_ACTIVE_AUDIO
    
    @use_proxy.setter
    def use_proxy(self, value: bool) -> None:
        PyFx.ItemSuite().SetItemUseProxy(self.item, value)
        
    def delete(self) -> None:
        PyFx.ItemSuite().DeleteItem(self.item)
        
    def __repr__(self) -> str:
        return f"{self.__class__.__name__}({self.name})"
    
    def __str__(self) -> str:
        return self.name
    
    def __eq__(self, other: 'Item') -> bool:
        return self.item == other.item
    
    def __ne__(self, other: 'Item') -> bool:
        return self.item != other.item
    
    
class FolderItem(Item):
    def __init__(self, item: PyFx.ItemPtr) -> None:
        super().__init__(item)
        
    @property
    def children(self) -> 'ItemCollection':
        return ItemCollection.create(self.item)
    
    def add_item(self, item: Item) -> None:
        PyFx.ItemSuite().SetItemParentFolder(item.item, self.item)
        
    def remove_item(self, item: Item) -> None:
        PyFx.ItemSuite().SetItemParentFolder(item.item, None)
        
class CompItem(Item):
    def __init__(self, item: PyFx.ItemPtr) -> None:
        super().__init__(item)
        self.comp = PyFx.CompSuite().GetCompFromItem(item)
        
    @classmethod
    def most_recent(cls) -> 'CompItem':
        return cls(PyFx.CompSuite().GetMostRecentlyUsedComp())
    
    @classmethod
    def create(cls, name: str, width: int, height: int, pixel_aspect: float, duration: float, frame_rate: float, parent: Item = None) -> 'CompItem':
        comp = PyFx.CompSuite().CreateComp(name, width, height, pixel_aspect, duration, frame_rate)
        if parent:
            PyFx.ItemSuite().SetItemParentFolder(comp, parent)
        return cls(comp)
    
    @property
    def layers(self) -> 'LayerCollection':
        return LayerCollection.create(self.comp)
    
    @property
    def downsample_factor(self) -> PyFx.DownsampleFactor:
        return PyFx.CompSuite().GetCompDownsampleFactor(self.comp)
    
    @downsample_factor.setter
    def downsample_factor(self, value: PyFx.DownsampleFactor) -> None:
        PyFx.CompSuite().SetCompDownsampleFactor(self.comp, value)
        
    @property
    def background_color(self) -> tuple[float, float, float, float]:
        color : PyFx.ColorVal = PyFx.CompSuite().GetCompBGColor(self.comp)
        return color.to_tuple()
    
    @background_color.setter
    def background_color(self, value: tuple[float, float, float, float]) -> None:
        PyFx.CompSuite().SetCompBGColor(self.comp, PyFx.ColorVal(value))
        
    @property
    def show_layer_name_or_source_name(self) -> bool:
        return PyFx.CompSuite().GetShowLayerNameOrSourceName(self.comp)
    
    @show_layer_name_or_source_name.setter
    def show_layer_name_or_source_name(self, value: bool) -> None:
        PyFx.CompSuite().SetShowLayerNameOrSourceName(self.comp, value)
        
    @property
    def show_blend_modes(self) -> bool:
        return PyFx.CompSuite().GetShowBlendModes(self.comp)
    
    @show_blend_modes.setter
    def show_blend_modes(self, value: bool) -> None:
        PyFx.CompSuite().SetShowBlendModes(self.comp, value)
        
    @property
    def frame_rate(self) -> float:
        return PyFx.CompSuite().GetCompFramerate(self.comp)
    
    @frame_rate.setter
    def frame_rate(self, value: float) -> None:
        PyFx.CompSuite().SetCompFrameRate(self.comp, value)
        
    def __repr__(self) -> str:
        return f"{self.__class__.__name__}({self.name})"
    
    def __str__(self) -> str:
        return self.name
    
    def __eq__(self, other: 'CompItem') -> bool:
        return self.comp == other.comp
    
    def __ne__(self, other: 'CompItem') -> bool:
        return self.comp != other.comp

class FootageItem(Item):
    def __init__(self, item: PyFx.ItemPtr) -> None:
        super().__init__(item)
        self.footage = PyFx.FootageSuite().getMainFootageFromItem(item)
        
    @classmethod
    def create(cls, path: str, name: str) -> 'FootageItem':
        item : PyFx.ItemPtr = AssetManager().import_asset(path, name)
        return ItemFactory.create_item(item)
            
    @classmethod
    def create_placeholder(cls, path: str, width: int, height: int, duration: float, platform: PyFx.Platform = PyFx.Platform.WIN) -> 'FootageItem':
        footage = PyFx.FootageSuite().newPlaceholderFootage(path, width, height, duration, platform)
        item = PyFx.FootageSuite().addFootageToProject(footage, PyFx.ProjSuite().GetProjectRootFolder(PyFx.ProjSuite().GetProjectByIndex(0)))
        return cls(item)
    
    @classmethod
    def create_solid(cls, name: str, width: int, height: int, color: tuple[float, float, float, float]) -> 'FootageItem':
        footage = PyFx.FootageSuite().newSolidFootage(name, width, height, PyFx.ColorVal(color))
        item = PyFx.FootageSuite().addFootageToProject(footage, PyFx.ProjSuite().GetProjectRootFolder(PyFx.ProjSuite().GetProjectByIndex(0)))
        return cls(item)
    
    @property
    def num_files(self) -> tuple[int, int]:
        return PyFx.FootageSuite().getFootageNumFiles(self.footage)
    
    def path(self, frame_num: int = 0, index: int = 0) -> str:
        return PyFx.FootageSuite().getFootagePath(self.footage, frame_num, index)
    
    def set_proxy(self, footage: 'FootageItem') -> None:
        PyFx.FootageSuite().setItemProxyFootage(self.footage, footage.footage)
        
    def replace_from_path(self, path: str) -> None:
        AssetManager().replace_asset(self.item, path)
        
    def __repr__(self) -> str:
        return f"{self.__class__.__name__}({self.name})"
    
    def __str__(self) -> str:
        return self.name
    
    def __eq__(self, other: 'FootageItem') -> bool:
        return self.footage == other.footage
    
    def __ne__(self, other: 'FootageItem') -> bool:
        return self.footage != other.footage
    
class ItemFactory:
    """
    Factory class for creating Item objects.
    """
    @staticmethod
    def create_item(item: PyFx.ItemPtr) -> Union[Item, FolderItem, CompItem, FootageItem]:
        type = PyFx.ItemSuite().GetItemType(item) #get the item type
        if type == PyFx.ItemType.FOLDER: 
            return FolderItem(item)
        elif type == PyFx.ItemType.COMP:
            return CompItem(item)
        elif type == PyFx.ItemType.FOOTAGE:
            return FootageItem(item)
        else:
            return Item(item)
