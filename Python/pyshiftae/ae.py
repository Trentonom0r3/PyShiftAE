import operator
import PyFx # type: ignore
import os 
from typing import Any, Iterator, List, Tuple, Union

class AssetManager:
    def __init__(self) -> None:
        pass

    def import_asset(self, file_path: str, name: str) -> PyFx.ItemPtr:
        suite = PyFx.FootageSuite()
        item = None
        footage = None
        if os.path.splitext(file_path)[1]: # This is a single file
            footage = suite.newFootage(file_path, PyFx.FootageLayerKey(nameAC=name), None, PyFx.InterpretationStyle.NO_DIALOG_NO_GUESS)
        elif os.path.isdir(file_path): # This is a sequence
            footage = suite.newFootage(file_path, PyFx.FootageLayerKey(nameAC=name), PyFx.FileSequenceImportOptions().sequence(), PyFx.InterpretationStyle.NO_DIALOG_NO_GUESS)
        else:
            raise Exception("Invalid file path")
        if footage:
            item = suite.addFootageToProject(footage, PyFx.ProjSuite().GetProjectRootFolder(PyFx.ProjSuite().GetProjectByIndex(0)))
        return item

    def replace_asset(self, old_asset: PyFx.ItemPtr, new_file_path: str) -> None:
        suite = PyFx.FootageSuite()
        new_footage = self.import_asset(new_file_path, "")
        if new_footage:
            suite.replaceItemMainFootage(suite.getMainFootageFromItem(old_asset), new_footage)
        else:
            raise Exception("Failed to import new footage")

    
class App:
    _suite = PyFx.UtilitySuite()
    def __init__(self) -> None:
        pass
    
    def report_info(self, message: str) -> None:
        self._suite.reportInfo(message)
        
class Item:
    _suite = PyFx.ItemSuite()
    def __init__(self, item: PyFx.ItemPtr) -> None:
        self.item = item
    
    @staticmethod
    def active_item() -> Union['Item', 'CompItem', 'FootageItem', None]:
        item_ptr = PyFx.ItemSuite().GetActiveItem()
        if item_ptr:
            return ItemFactory.create_item(item_ptr)
        
    @property
    def name(self) -> str:
        try :
            name = self._suite.GetItemName(self.item)
        except Exception as e:
            raise e
        return name   
    
    @name.setter
    def name(self, value: str) -> None:
        self._suite.SetItemName(self.item, value)
        
    @property
    def type(self) -> PyFx.ItemType:
        return self._suite.GetItemType(self.item)
    
    @property
    def parent_folder(self) -> 'FolderItem':
        return FolderItem(self._suite.GetItemParentFolder(self.item))
    
    @parent_folder.setter
    def parent_folder(self, folder: 'FolderItem') -> None:
        self._suite.SetItemParentFolder(self.item, folder.item)
        
    @property
    def duration(self) -> float:
        return self._suite.GetItemDuration(self.item)
    
    @property
    def current_time(self) -> float:
        return self._suite.GetItemCurrentTime(self.item)
    
    @current_time.setter
    def current_time(self, value: float) -> None:
        self._suite.SetItemCurrentTime(self.item, value)
        
    @property
    def comment(self) -> str:
        return self._suite.GetItemComment(self.item)
    
    @comment.setter
    def comment(self, value: str) -> None:
        self._suite.SetItemComment(self.item, value)
        
    @property
    def label(self) -> int:
        return self._suite.GetItemLabel(self.item)
    
    @label.setter
    def label(self, value: int) -> None:
        self._suite.SetItemLabel(self.item, value)
        
    @property
    def dimensions(self) -> tuple[int, int]:
        return self._suite.GetItemDimensions(self.item)
    
    @property
    def pixel_aspect(self) -> float:
        return self._suite.GetItemPixelAspectRatio(self.item)
    
    @property
    def selected(self) -> bool:
        return self._suite.IsItemSelected(self.item)
    
    @selected.setter
    def selected(self, value: bool) -> None:
        self._suite.SelectItem(self.item, value)
        
    @property
    def missing(self) -> bool:
        return self._suite.GetItemFlags(self.item) & PyFx.ItemFlag.MISSING
    
    @property
    def has_proxy(self) -> bool:
        return self._suite.GetItemFlags(self.item) & PyFx.ItemFlag.HAS_PROXY
    
    @property
    def using_proxy(self) -> bool:
        return self._suite.GetItemFlags(self.item) & PyFx.ItemFlag.USING_PROXY
    
    @using_proxy.setter
    def use_proxy(self, value: bool) -> None:
        self._suite.SetItemUseProxy(self.item, value)
        
    @property
    def missing_proxy(self) -> bool:
        return self._suite.GetItemFlags(self.item) & PyFx.ItemFlag.MISSING_PROXY
    
    @property
    def has_video(self) -> bool:
        return self._suite.GetItemFlags(self.item) & PyFx.ItemFlag.HAS_VIDEO
    
    @property
    def has_audio(self) -> bool:
        return self._suite.GetItemFlags(self.item) & PyFx.ItemFlag.HAS_AUDIO
    
    @property
    def still(self) -> bool:
        return self._suite.GetItemFlags(self.item) & PyFx.ItemFlag.STILL
    
    @property
    def has_active_audio(self) -> bool:
        return self._suite.GetItemFlags(self.item) & PyFx.ItemFlag.HAS_ACTIVE_AUDIO
    
    @use_proxy.setter
    def use_proxy(self, value: bool) -> None:
        self._suite.SetItemUseProxy(self.item, value)
        
    def delete(self) -> None:
        self._suite.DeleteItem(self.item)
        
    def __repr__(self) -> str:
        return f"{self.__class__.__name__}({self.name})"
    
    def __str__(self) -> str:
        return self.name
    
    def __eq__(self, other: 'Item') -> bool:
        return self.item == other.item
    
    def __ne__(self, other: 'Item') -> bool:
        return self.item != other.item
    
    #override python garbage collection, ensuring nothing happens.
    def __del__(self) -> None:
        pass
    
    
class FolderItem(Item):
    def __init__(self, item: PyFx.ItemPtr) -> None:
        super().__init__(item)
        
    @property
    def children(self) -> 'ItemCollection':
        return ItemCollection.create(self.item)
    
    def add_item(self, item: Item) -> None:
        self._suite.SetItemParentFolder(item.item, self.item)
        
    def remove_item(self, item: Item) -> None:
        self._suite.SetItemParentFolder(item.item, None)
        
class CompItem(Item):
    _compSuite = PyFx.CompSuite()
    def __init__(self, item: Union[PyFx.ItemPtr, PyFx.CompPtr]) -> None:
        self.comp = item if isinstance(item, PyFx.CompPtr) else PyFx.CompSuite().GetCompFromItem(item)
        super().__init__(item if isinstance(item, PyFx.ItemPtr) else PyFx.CompSuite().GetItemFromComp(item))
        
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
        return self._compSuite.GetCompDownsampleFactor(self.comp)
    
    @downsample_factor.setter
    def downsample_factor(self, value: PyFx.DownsampleFactor) -> None:
        self._compSuite.SetCompDownsampleFactor(self.comp, value)
        
    @property
    def background_color(self) -> tuple[float, float, float, float]:
        color : PyFx.ColorVal = self._compSuite.GetCompBGColor(self.comp)
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

class ItemCollection(list):
    _ROOT_FOLDER = None
    def __init__(self) -> None:
        super().__init__()

    @classmethod 
    def create(cls, root_folder: PyFx.ItemPtr) -> 'ItemCollection':
        collection = cls()
        current_item = root_folder
        cls._ROOT_FOLDER = root_folder
        proj = PyFx.ProjSuite().GetProjectByIndex(0)
        while current_item:
            if PyFx.ItemSuite().GetItemParentFolder(current_item) == root_folder:
                item = ItemFactory.create_item(current_item)
                collection.append(item)
            current_item = PyFx.ItemSuite().GetNextProjItem(proj, current_item)
        return collection
    
    def __getitem__(self, key: any) -> Item:
        if isinstance(key, int):
            return super().__getitem__(key)
        for item in self:
            if self._suite.GetItemName(item.item) == key:
                return item
        raise KeyError(f"Item with name '{key}' not found")
    
    def append(self, object: Item) -> None:
        if self._suite.GetItemParentFolder(object.item) != self._ROOT_FOLDER:
            self._suite.SetItemParentFolder(object.item, self._ROOT_FOLDER) #set the parent folder
        super().append(object) #append the object to the list
        
    def remove(self, object: Item) -> None:
        self._suite.DeleteItem(object.item) #delete the item
        super().remove(object)
        
    def clear(self) -> None:
        for item in self:
            self._suite.DeleteItem(item.item)
        super().clear()
        
    def __iter__(self) -> Iterator:
        return super().__iter__()
    
    def get_item(self, **criteria) -> Item:
        """
        Retrieve an item from the collection based on specified criteria.
        
        Args:
            **criteria (dict): Key-value pairs where keys are item attributes
                               and values are the expected values. Supports nested
                               attributes using dot notation.

        Returns:
            Item: The first item that matches all criteria.
        
        Raises:
            KeyError: If no item matching the criteria is found.
        """
        def match(item: Item, attr_path: str, value: Any) -> bool:
            attr = operator.attrgetter(attr_path)
            try:
                return attr(item) == value
            except AttributeError:
                return False
        
        for item in self:
            if all(match(item, key, val) for key, val in criteria.items()):
                return item

        raise KeyError(f"Item with criteria '{criteria}' not found")
    
    def get_items(self, **criteria) -> 'ItemCollection':
        """
        Retrieve items from the collection based on specified criteria.
        
        Args:
            **criteria (dict): Key-value pairs where keys are item attributes
                               and values are the expected values. Supports nested
                               attributes using dot notation.

        Returns:
            ItemCollection: A collection of items that match all criteria.
        """
        def match(item: Item, attr_path: str, value: Any) -> bool:
            attr = operator.attrgetter(attr_path)
            try:
                return attr(item) == value
            except AttributeError:
                return False
            
        items = ItemCollection()
        for item in self:
            if all(match(item, key, val) for key, val in criteria.items()):
                items.append(item)
        return items
    
    def __repr__(self) -> str:
        return f"{self.__class__.__name__}({super().__repr__()})"
    
    def __str__(self) -> str:
        return f"{self.__class__.__name__}({super().__str__()})"
    
    def __len__(self) -> int:
        return super().__len__()
  

class BaseProperty:
    _suite = PyFx.StreamSuite()
    _dyn_suite = PyFx.DynamicStreamSuite()
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        self.property = property
        
    @property
    def name(self) -> str:
        return self._suite.GetStreamName(self.property)
    
    @name.setter
    def name(self, value: str) -> None:
        self._dyn_suite.SetStreamName(self.property, value)

    @property
    def type(self) -> PyFx.StreamType:
        return self._suite.GetStreamType(self.property)
    
    @property
    def group_type(self) -> PyFx.StreamGroupingType:
        return self._dyn_suite.GetStreamGroupingType(self.property)
    
    @property
    def match_name(self) -> str:
        return self._dyn_suite.GetMatchname(self.property)
    
    def duplicate(self) -> Union['BaseProperty', 'PropertyGroup', 'OneDProperty', 'TwoDProperty', 'ThreeDProperty', 'ColorProperty', 'MarkerProperty', 'LayerIDProperty', 'MaskIDProperty', 'MaskOutlineProperty', 'TextDocumentProperty']:
        new_stream = self._dyn_suite.DuplicateStream(self.property)
        stream = self._dyn_suite.GetNewStreamRefByIndex(self.property, new_stream)
        return PropertyFactory.create_property(stream)
    
    def reorder(self, index: int) -> None:
        self._dyn_suite.ReorderStream(self.property, index)
        
    def get_property(self, name: str) -> Union['BaseProperty', 'PropertyGroup', 'OneDProperty', 'TwoDProperty', 'ThreeDProperty', 'ColorProperty', 'MarkerProperty', 'LayerIDProperty', 'MaskIDProperty', 'MaskOutlineProperty', 'TextDocumentProperty']:
        stream = self._dyn_suite.GetNewStreamRefByMatchname(self.property, name)
        return PropertyFactory.create_property(stream)
    
    def get_property_by_index(self, index: int) -> Union['BaseProperty', 'PropertyGroup', 'OneDProperty', 'TwoDProperty', 'ThreeDProperty', 'ColorProperty', 'MarkerProperty', 'LayerIDProperty', 'MaskIDProperty', 'MaskOutlineProperty', 'TextDocumentProperty']:
        stream = self._dyn_suite.GetNewStreamRefByIndex(self.property, index)
        return PropertyFactory.create_property(stream)
    
    def add_property(self, name: str) -> None:
        if self._dyn_suite.CanAddStream(self.property, name):
            self._dyn_suite.AddStream(self.property, name)
            
    def remove_property(self, name: str) -> None:
        stream = self._dyn_suite.GetNewStreamRefByMatchname(self.property, name)
        if stream:
            self._dyn_suite.DeleteStream(stream)
            
    def remove_property_by_index(self, index: int) -> None:
        stream = self._dyn_suite.GetNewStreamRefByIndex(self.property, index)
        if stream:
            self._dyn_suite.DeleteStream(stream)
            
    def num_keys(self) -> int:
        return PyFx.KeyframeSuite().GetStreamNumKFs(self.property)
    
    def __repr__(self) -> str:
        return f"{self.__class__.__name__}({self.name})"
    
    def __str__(self) -> str:
        return self.name

class PropertyGroup(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
            
    def num_properties(self) -> int:
        return self._dyn_suite.GetNumStreamsInGroup(self.property)
    
    def __repr__(self) -> str:
        return f"{self.__class__.__name__}({self.name})"
    
    def __str__(self) -> str:
        return self.name
    
    def __getitem__(self, key: Union[int, str]) -> Union['BaseProperty', 'PropertyGroup', 'OneDProperty', 'TwoDProperty', 'ThreeDProperty', 'ColorProperty', 'MarkerProperty', 'LayerIDProperty', 'MaskIDProperty', 'MaskOutlineProperty', 'TextDocumentProperty']:
        if isinstance(key, int):
            return self.get_property_by_index(key)
        elif isinstance(key, str):
            return self.get_property(key)
        else:
            raise TypeError("Key must be an int or a str")
        
    def __iter__(self):
        for i in range(self.num_properties()):
            yield self.get_property_by_index(i)
            
    def __len__(self) -> int:
        return self.num_properties()
    
    def __contains__(self, item: Union['BaseProperty', 'PropertyGroup', 'OneDProperty', 'TwoDProperty', 'ThreeDProperty', 'ColorProperty', 'MarkerProperty', 'LayerIDProperty', 'MaskIDProperty', 'MaskOutlineProperty', 'TextDocumentProperty']) -> bool:
        for i in range(self.num_properties()):
            if self.get_property_by_index(i) == item:
                return True
        return False
    
    def append(self, item: Union['BaseProperty', 'PropertyGroup', 'OneDProperty', 'TwoDProperty', 'ThreeDProperty', 'ColorProperty', 'MarkerProperty', 'LayerIDProperty', 'MaskIDProperty', 'MaskOutlineProperty', 'TextDocumentProperty']) -> None:
        self.add_property(item.name)
        
    def insert(self, index: int, item: Union['BaseProperty', 'PropertyGroup', 'OneDProperty', 'TwoDProperty', 'ThreeDProperty', 'ColorProperty', 'MarkerProperty', 'LayerIDProperty', 'MaskIDProperty', 'MaskOutlineProperty', 'TextDocumentProperty']) -> None:
        self.add_property(item.name)
        self.reorder(index)
        
    def remove(self, item: Union['BaseProperty', 'PropertyGroup', 'OneDProperty', 'TwoDProperty', 'ThreeDProperty', 'ColorProperty', 'MarkerProperty', 'LayerIDProperty', 'MaskIDProperty', 'MaskOutlineProperty', 'TextDocumentProperty']) -> None:
        self.remove_property(item.name)
        
    def clear(self) -> None:
        for i in range(self.num_properties()):
            self.remove_property_by_index(i)
    
    def reverse(self) -> None:
        for i in range(self.num_properties()):
            self.reorder(i)
            
    def sort(self, key=None, reverse=False) -> None:
        for i in range(self.num_properties()):
            self.reorder(i)

class OneDProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> float:
        val = self._suite.GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        return val.value()
    
    def set_value(self, value: float) -> None:
        self._suite.SetStreamValue(self.property, PyFx.StreamValue2(self.property, value))
        
    def __getitem__(self, time: float) -> float:
        return self.get_value(PyFx.LTimeMode.CompTime, time, False)
    
    def __setitem__(self, time: float, value: float) -> None:
        self.set_value(value)
        
class TwoDProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> tuple[float, float]:
        val = self._suite.GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        two_d : PyFx.TwoDVal = val.value()
        return two_d.to_tuple()
    
    def set_value(self, value: tuple[float, float]) -> None:
        self._suite.SetStreamValue(self.property, PyFx.StreamValue2(self.property, value))
        
    def __getitem__(self, time: float) -> tuple[float, float]:
        return self.get_value(PyFx.LTimeMode.CompTime, time, False)
    
    def __setitem__(self, time: float, value: tuple[float, float]) -> None:
        self.set_value(value)
        
class ThreeDProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> tuple[float, float, float]:
        val = self._suite.GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        three_d_tuple = val.value()
        return three_d_tuple
    
    def set_value(self, value: tuple[float, float, float]) -> None:
        self._suite.SetStreamValue(self.property, PyFx.StreamValue2(self.property, value))
        
    def __getitem__(self, time: float) -> tuple[float, float, float]:
        return self.get_value(PyFx.LTimeMode.CompTime, time, False)
    
    def __setitem__(self, time: float, value: tuple[float, float, float]) -> None:
        self.set_value(value)
        
class ColorProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> tuple[float, float, float, float]:
        val = self._suite.GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        color : PyFx.ColorVal = val.value()
        return color.to_tuple()
    
    def set_value(self, value: tuple[float, float, float, float]) -> None:
        self._suite.SetStreamValue(self.property, PyFx.StreamValue2(self.property, value))
        
class MarkerProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> 'PyFx.Marker':
        val = self._suite.GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        return PyFx.Marker(val.value())
    
    def add_marker(self, time: float) -> 'PyFx.Marker':
        idx = PyFx.KeyframeSuite().InsertKeyframe(self.property, PyFx.LTimeMode.CompTime, PyFx.Time(time))
        marker = PyFx.MarkerSuite().getNewMarker()
        val = PyFx.StreamValue2(self.property, marker)
        PyFx.KeyframeSuite().SetKeyframeValue(self.property, idx, val)
        return PyFx.Marker(marker)
    
    def __getitem__(self, time: float) -> 'PyFx.Marker':
        return self.get_value(PyFx.LTimeMode.CompTime, time, False)
    
class LayerIDProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> int:
        val = self._suite.GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        return val.value()
    
class MaskIDProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> int:
        val = self._suite.GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        return val.value()
    
class MaskOutlineProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> 'PyFx.MaskOutline':
        val = self._suite.GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        return PyFx.MaskOutline(val.value())
    
class TextDocumentProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> 'PyFx.TextDocument':
        val = self._suite.GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        return PyFx.TextDocument(val.value())
    
    
class PropertyFactory:
    
    @staticmethod
    def create_property(property: PyFx.StreamRefPtr) -> Union[PropertyGroup, BaseProperty, OneDProperty, TwoDProperty, ThreeDProperty, ColorProperty, MarkerProperty, LayerIDProperty, MaskIDProperty, MaskOutlineProperty, TextDocumentProperty]:
        group_type = PyFx.DynamicStreamSuite().GetStreamGroupingType(property)
        if group_type == PyFx.StreamGroupingType.INDEXED_GROUP or group_type == PyFx.StreamGroupingType.NAMED_GROUP:
            return PropertyGroup(property)
        elif group_type == PyFx.StreamGroupingType.LEAF:
            stream_type = PyFx.StreamSuite().GetStreamType(property)
            if stream_type == PyFx.StreamType.OneD:
                return OneDProperty(property)
            elif stream_type == PyFx.StreamType.TwoD or stream_type == PyFx.StreamType.TwoD_SPATIAL:
                return TwoDProperty(property)
            elif stream_type == PyFx.StreamType.ThreeD or stream_type == PyFx.StreamType.ThreeD_SPATIAL:
                return ThreeDProperty(property)
            elif stream_type == PyFx.StreamType.COLOR:
                return ColorProperty(property)
            elif stream_type == PyFx.StreamType.MARKER:
                return MarkerProperty(property)
            elif stream_type == PyFx.StreamType.LAYER_ID:
                return LayerIDProperty(property)
            elif stream_type == PyFx.StreamType.MASK_ID:
                return MaskIDProperty(property)
            elif stream_type == PyFx.StreamType.MASK:
                return MaskOutlineProperty(property)
            elif stream_type == PyFx.StreamType.TEXT_DOCUMENT:
                return TextDocumentProperty(property)
            else:
                return BaseProperty(property)
        return None
    
 
class Layer(PropertyGroup):
    _Lsuite = PyFx.LayerSuite()
    def __init__(self, layer: PyFx.LayerPtr) -> None:
        super().__init__(self._dyn_suite.GetNewStreamRefForLayer(layer))
        self.layer = layer
        
    @staticmethod
    def active_layer() -> Union['Layer', 'AVLayer', 'CameraLayer', 'LightLayer', 'TextLayer', 'VectorLayer', None]:
        layer_ptr = PyFx.LayerSuite().GetActiveLayer()
        if layer_ptr:
            return LayerFactory.create_layer(layer_ptr)
        
    @property
    def name(self) -> str:
        return self._Lsuite.GetLayerName(self.layer)
    
    @name.setter
    def name(self, value: str) -> None:
        self._Lsuite.SetLayerName(self.layer, value)
        
    @property
    def index(self) -> int:
        return self._Lsuite.GetLayerIndex(self.layer)
    
    @index.setter
    def index(self, value: int) -> None:
        self._Lsuite.ReorderLayer(self.layer, value)
        
    @property
    def source_item(self) -> Item:
        return ItemFactory.create_item(self._Lsuite.GetLayerSourceItem(self.layer))
    
    @property
    def parent_comp(self) -> CompItem:
        return CompItem(self._Lsuite.GetLayerParentComp(self.layer))
    
    @property
    def quality(self) -> PyFx.LayerQual:
        return self._Lsuite.GetLayerQuality(self.layer)
    
    @quality.setter
    def quality(self, value: PyFx.LayerQual) -> None:
        self._Lsuite.SetLayerQuality(self.layer, value)
        
    @property
    def video_active(self) -> bool:
        return self._Lsuite.GetLayerFlags(self.layer) & PyFx.LayerFlag.VIDEO_ACTIVE
    
    @property
    def audio_active(self) -> bool:
        return self._Lsuite.GetLayerFlags(self.layer) & PyFx.LayerFlag.AUDIO_ACTIVE
    
    @property
    def current_time(self) -> float:
        return self._Lsuite.GetLayerCurrentTime(self.layer)
    
    @property
    def duration(self) -> float:
        return self._Lsuite.GetLayerDuration(self.layer)
    
    @property
    def offset(self) -> float:
        return self._Lsuite.GetLayerOffset(self.layer)
    
    @offset.setter
    def offset(self, value: float) -> None:
        self._Lsuite.SetLayerOffset(self.layer, value)
        
    @property
    def in_point(self) -> float:
        return self._Lsuite.GetLayerInPoint(self.layer)
    
    @in_point.setter
    def in_point(self, value: float) -> None:
        self._Lsuite.SetLayerInPointAndDuration(self.layer, value, self.duration)
        
    @property
    def stretch(self) -> float:
        return self._Lsuite.GetLayerStretch(self.layer)
    
    @stretch.setter
    def stretch(self, value: float) -> None:
        self._Lsuite.SetLayerStretch(self.layer, value)
        
    @property
    def flag(self) -> PyFx.LayerFlag:
        return self._Lsuite.GetLayerFlags(self.layer)
    
    @flag.setter
    def flag(self, value: PyFx.LayerFlag) -> None:
        self._Lsuite.SetLayerFlag(self.layer, value)
        
    @property
    def is_3d(self) -> bool:
        return self._Lsuite.IsLayer3D(self.layer)
    
    @property
    def is_2d(self) -> bool:
        return not self.is_3d()
    
    @property
    def parent(self) -> 'Layer':
        return LayerFactory.create_layer(self._Lsuite.GetLayerParent(self.layer))
    
    @parent.setter
    def parent(self, value: 'Layer') -> None:
        self._Lsuite.SetLayerParent(self.layer, value.layer)
        
    @property
    def sampling_quality(self) -> PyFx.LayerSamplingQual:
        return self._Lsuite.GetLayerSamplingQuality(self.layer)
    
    @sampling_quality.setter
    def sampling_quality(self, value: PyFx.LayerSamplingQual) -> None:
        self._Lsuite.SetLayerSamplingQuality(self.layer, value)
        
    def duplicate(self) -> 'Layer':
        return LayerFactory.create_layer(self._Lsuite.DuplicateLayer(self.layer))
    
    def delete(self) -> None:
        self._Lsuite.DeleteLayer(self.layer)
        
    def num_effects(self) -> int:
        return PyFx.EffectSuite().getLayerNumEffects(self.layer)
    
    def get_property(self, stream: PyFx.LayerStream) -> Union[BaseProperty, OneDProperty, TwoDProperty, ThreeDProperty, ColorProperty, MarkerProperty, LayerIDProperty, MaskIDProperty, MaskOutlineProperty, TextDocumentProperty]:
        return PropertyFactory.create_property(self._suite.GetNewLayerStream(self.layer, stream))
    
    @property
    def anchor_point(self) -> ThreeDProperty:
        return self.get_property(PyFx.LayerStream.ANCHORPOINT)
    
    @property
    def position(self) -> ThreeDProperty:
        return self.get_property(PyFx.LayerStream.POSITION)
    
    @property
    def rotation(self) -> ThreeDProperty:
        return self.get_property(PyFx.LayerStream.ROTATION)
    
    @property
    def marker(self) -> MarkerProperty:
        return self.get_property(PyFx.LayerStream.MARKER)
    
    @property
    def scale(self) -> ThreeDProperty:
        return self.get_property(PyFx.LayerStream.SCALE)
    
    @property
    def opacity(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.OPACITY)
    
    @property
    def rotation_x(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.ROTATE_X)
    
    @property
    def rotation_y(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.ROTATE_Y)
    
    @property
    def rotation_z(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.ROTATE_Z)
    
    @property
    def text(self) -> TextDocumentProperty:
        return self.get_property(PyFx.LayerStream.TEXT)
    
    @property
    def type(self) -> PyFx.ObjectType:
        return self._Lsuite.GetLayerObjectType(self.layer)
    
    def copy_to_comp(self, comp: CompItem) -> 'Layer':
        if self._Lsuite.IsAddLayerValid(self.source_item.item, comp.comp):
            return LayerFactory.create_layer(self._Lsuite.AddLayer(self.source_item.item, comp.comp))

class AVLayer(Layer):
    def __init__(self, layer: PyFx.LayerPtr) -> None:
        super().__init__(layer)
        
    def getObjectType(self) -> PyFx.ObjectType:
        return PyFx.ObjectType.AV
    
    def casts_shadows(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.CASTS_SHADOWS)
    
    def accepts_lights(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.ACCEPTS_LIGHTS)
    
    def ambient_coeff(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.AMBIENT_COEFF)
    
    def diffuse_coeff(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.DIFFUSE_COEFF)
    
    def specular_intensity(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.SPECULAR_INTENSITY)
    
    def specular_shininess(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.SPECULAR_SHININESS)
    
    def light_transmission(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.LIGHT_TRANSMISSION)
    
    def metal(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.METAL)
    
    def reflection_intensity(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.REFLECTION_INTENSITY)
    
    def reflection_sharpness(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.REFLECTION_SHARPNESS)
    
    def reflection_rolloff(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.REFLECTION_ROLLOFF)
    
    def transparency_coeff(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.TRANSPARENCY_COEFF)
    
    def transparency_rolloff(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.TRANSPARENCY_ROLLOFF)
    
    def index_of_refraction(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.INDEX_OF_REFRACTION)
    
    def extrusion_bevel_style(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.EXTRUSION_BEVEL_STYLE)
    
    def extrusion_bevel_direction(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.EXTRUSION_BEVEL_DIRECTION)
    
    def extrusion_bevel_depth(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.EXTRUSION_BEVEL_DEPTH)
    
    def extrusion_hole_bevel_depth(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.EXTRUSION_HOLE_BEVEL_DEPTH)
    
    def extrusion_depth(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.EXTRUSION_DEPTH)
    
    def plane_curvature(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.PLANE_CURVATURE)
    
    def plane_subdivision(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.PLANE_SUBDIVISION)
    
class CameraLayer(Layer):
    def __init__(self, layer: PyFx.LayerPtr) -> None:
        super().__init__(layer)
     
    def getObjectType(self) -> PyFx.ObjectType:
        return PyFx.ObjectType.CAMERA
    
    def zoom(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.ZOOM)
    
    def depth_of_field(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.DEPTH_OF_FIELD)
    
    def focus_distance(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.FOCUS_DISTANCE)
    
    def aperture(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.APERTURE)
    
    def blur_level(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.BLUR_LEVEL)
    
    def iris_shape(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.IRIS_SHAPE)
    
    def iris_rotation(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.IRIS_ROTATION)
    
    def iris_roundness(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.IRIS_ROUNDNESS)
    
    def iris_aspect_ratio(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.IRIS_ASPECT_RATIO)
    
    def iris_diffraction_fringe(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.IRIS_DIFFRACTION_FRINGE)
    
    def iris_highlight_gain(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.IRIS_HIGHLIGHT_GAIN)
    
    def iris_highlight_threshold(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.IRIS_HIGHLIGHT_THRESHOLD)
    
    def iris_highlight_saturation(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.IRIS_HIGHLIGHT_SATURATION)
    
class LightLayer(Layer):
    def __init__(self, layer: PyFx.LayerPtr) -> None:
        super().__init__(layer)
        
    def getObjectType(self) -> PyFx.ObjectType:
        return PyFx.ObjectType.LIGHT
    
    def color(self) -> ColorProperty:
        return self.get_property(PyFx.LayerStream.COLOR)
    
    def intensity(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.INTENSITY)
    
    def cone_angle(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.CONE_ANGLE)
    
    def cone_feather(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.CONE_FEATHER)
    
    def shadow_darkness(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.SHADOW_DARKNESS)
    
    def shadow_diffusion(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.SHADOW_DIFFUSION)
    
    def light_falloff_type(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.LIGHT_FALLOFF_TYPE)
    
    def light_falloff_start(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.LIGHT_FALLOFF_START)
    
    def light_falloff_distance(self) -> OneDProperty:
        return self.get_property(PyFx.LayerStream.LIGHT_FALLOFF_DISTANCE)
    
class TextLayer(Layer):
    def __init__(self, layer: PyFx.LayerPtr) -> None:
        super().__init__(layer)
        
    def getObjectType(self) -> PyFx.ObjectType:
        return PyFx.ObjectType.TEXT
    
class VectorLayer(Layer):
    def __init__(self, layer: PyFx.LayerPtr) -> None:
        super().__init__(layer)
        
    def getObjectType(self) -> PyFx.ObjectType:
        return PyFx.ObjectType.VECTOR
    
class LayerFactory:
    
    @staticmethod
    def create_layer(layer: PyFx.LayerPtr) -> Union[Layer, AVLayer, CameraLayer, LightLayer, TextLayer, VectorLayer]:
        type = PyFx.LayerSuite().GetLayerObjectType(layer)
        if type == PyFx.ObjectType.AV:
            return AVLayer(layer)
        elif type == PyFx.ObjectType.CAMERA:
            return CameraLayer(layer)
        elif type == PyFx.ObjectType.LIGHT:
            return LightLayer(layer)
        elif type == PyFx.ObjectType.TEXT:
            return TextLayer(layer)
        elif type == PyFx.ObjectType.VECTOR:
            return VectorLayer(layer)
        else:
            return Layer(layer)
        
class LayerCollection(list):
    _base_comp : PyFx.CompPtr = None
    def __init__(self) -> None:
        super().__init__()
        
    @classmethod
    def create(cls, base_comp: PyFx.CompPtr) -> 'LayerCollection':
        collection = cls()
        cls._base_comp = base_comp
        suite = PyFx.LayerSuite()
        num_layers = suite.GetCompNumLayers(base_comp)
        for i in range(num_layers):
            layer_ptr = suite.GetCompLayerByIndex(base_comp, i)
            layer = LayerFactory.create_layer(layer_ptr)
            collection.append(layer)
        return collection
    
    def add_null(self, name: str, duration: float = 0.0) -> Layer:
        if duration < 0:
            duration = self._suite.GetItemDuration(PyFx.CompSuite().GetItemFromComp(self._base_comp))
        null = PyFx.CompSuite().CreateNullInComp(self._base_comp, name, duration)
        layer = LayerFactory.create_layer(null)
        super().append(layer)
        return layer
    
    def add_solid(self, name: str, color: tuple[float, float, float, float], width: int, height: int, duration: float = 0.0) -> Layer:
        if duration < 0:
            duration = self._suite.GetItemDuration(PyFx.CompSuite().GetItemFromComp(self._base_comp))
        solid = PyFx.CompSuite().CreateSolidInComp(self._base_comp, name, width, height, PyFx.ColorVal(color), PyFx.Time(duration))
        layer = LayerFactory.create_layer(solid)
        super().append(layer)
        return layer
    
    def add_camera(self, name: str, center_point : tuple[float, float] = (0.0, 0.0)) -> Layer:
        camera = PyFx.CompSuite().CreateCameraInComp(self._base_comp, name, PyFx.FloatPoint(center_point[0], center_point[1]))
        layer = LayerFactory.create_layer(camera)
        super().append(layer)
        return layer
    
    def add_light(self, name: str, center_point : tuple[float, float] = (0.0, 0.0)) -> Layer:
        light = PyFx.CompSuite().CreateLightInComp(self._base_comp, name, PyFx.FloatPoint(center_point[0], center_point[1]))
        layer = LayerFactory.create_layer(light)
        super().append(layer)
        return layer
            
    def extend(self, layers: 'LayerCollection') -> None:
        for layer in layers:
            self.append(layer.source_item)
            super().append(layer)
            
    def insert(self, index: int, layer: Layer) -> None:
        if self._suite.IsAddLayerValid(layer.source_item, self._base_comp):
            new_layer = Layer(self._suite.AddLayer(layer.source_item, self._base_comp))
            self._suite.ReorderLayer(new_layer, index)
            super().insert(index, new_layer)
            
    def remove(self, layer: Layer) -> None:
        self._suite.DeleteLayer(layer.layer)
        super().remove(layer)
    
    def clear(self) -> None:
        for layer in self:
            self._suite.DeleteLayer(layer.layer)
        super().clear()
        
    def pop(self, index: int) -> Layer:
        layer = super().pop(index)
        self._suite.DeleteLayer(layer.layer)
        return layer
    
    def reverse(self) -> None:
        for i, layer in enumerate(self):
            self._suite.ReorderLayer(layer.layer, len(self) - i)
        super().reverse()
        
    def sort(self, key=None, reverse=False) -> None:
        super().sort(key, reverse)
        for i, layer in enumerate(self):
            self._suite.ReorderLayer(layer.layer, i)
            
    def __getitem__(self, key: any) -> Layer:
        if isinstance(key, int):
            return super().__getitem__(key)
        for layer in self:
            if self._suite.GetLayerName(layer.layer) == key:
                return layer
        raise KeyError(f"Layer with name '{key}' not found")
    
    def __repr__(self) -> str:
        return f"{self.__class__.__name__}({super().__repr__()})"
    
    def __str__(self) -> str:
        return f"{self.__class__.__name__}({super().__str__()})"
    
    def __len__(self) -> int:
        return super().__len__()
    
    def __iter__(self) -> Iterator:
        return super().__iter__()
    
    def get_layer(self, **criteria) -> Layer:
        """
        Retrieve a layer from the collection based on specified criteria.
        
        Args:
            **criteria (dict): Key-value pairs where keys are layer attributes
                               and values are the expected values. Supports nested
                               attributes using dot notation.

        Returns:
            Layer: The first layer that matches all criteria.
        
        Raises:
            KeyError: If no layer matching the criteria is found.
        """
        def match(layer: Layer, attr_path: str, value: Any) -> bool:
            attr = operator.attrgetter(attr_path)
            try:
                return attr(layer) == value
            except AttributeError:
                return False
        
        for layer in self:
            if all(match(layer, key, val) for key, val in criteria.items()):
                return layer

        raise KeyError(f"Layer with criteria '{criteria}' not found")
    
    def get_layers(self, **criteria) -> 'LayerCollection':
        """
        Retrieve layers from the collection based on specified criteria.
        
        Args:
            **criteria (dict): Key-value pairs where keys are layer attributes
                               and values are the expected values. Supports nested
                               attributes using dot notation.

        Returns:
            LayerCollection: A collection of layers that match all criteria.
        """
        def match(layer: Layer, attr_path: str, value: Any) -> bool:
            attr = operator.attrgetter(attr_path)
            try:
                return attr(layer) == value
            except AttributeError:
                return False
            
        layers = LayerCollection()
        for layer in self:
            if all(match(layer, key, val) for key, val in criteria.items()):
                layers.append(layer)
        return layers
    

class UndoGroup:
    def __init__(self, name: str) -> None:
        self.name = name
        PyFx.UtilitySuite().startUndoGroup(name)
        
    def __enter__(self) -> 'UndoGroup':
        return self
    
    def __exit__(self, exc_type, exc_value, traceback) -> None:
        PyFx.UtilitySuite().endUndoGroup()
        
class QuietErrors:
    def __init__(self) -> None:
        PyFx.UtilitySuite().startQuietErrors()
        
    def __enter__(self) -> 'QuietErrors':
        return self
    
    def __exit__(self, exc_type, exc_value, traceback) -> None:
        PyFx.UtilitySuite().endQuietErrors()
        
#decorator to start an undo group
def undo_group(name: str):
    def decorator(func):
        def wrapper(*args, **kwargs):
            with UndoGroup(name):
                return func(*args, **kwargs)
        return wrapper
    return decorator

#decorator to start quiet errors
def quiet_errors(func):
    def wrapper(*args, **kwargs):
        with QuietErrors():
            return func(*args, **kwargs)
    return wrapper


class Effect:
    def __init__(self, effect: PyFx.EffectRefPtr) -> None:
        self.effect = effect

    @classmethod
    def apply(cls, layer: Layer, match_name: str) -> 'Effect':
        #check name and match name
        current_key = 0
        num_effects = PyFx.EffectSuite().getNumInstalledEffects()
      #  App().report_info(f"num_effects: {num_effects}")
        first_eff = PyFx.EffectSuite().getNextInstalledEffect(current_key)
        current_key = first_eff
        for i in range(num_effects):
            current_match_name = PyFx.EffectSuite().getEffectMatchName(current_key)
            current_name = PyFx.EffectSuite().getEffectName(current_key)
            if current_match_name == match_name or current_name == match_name:
                effect_ref = PyFx.EffectSuite().applyEffect(layer.layer, current_key)
                return Effect(effect_ref)
            current_key = PyFx.EffectSuite().getNextInstalledEffect(current_key)
        return None

    @property
    def name(self) -> str:
        return PyFx.EffectSuite().getEffectName(self.effect)

    @property
    def match_name(self) -> str:
        return PyFx.EffectSuite().getEffectMatchName(self.effect)

    @property
    def category(self) -> str:
        return PyFx.EffectSuite().getEffectCategory(self.effect)

    def param(self, name: Union[int, str]) -> Union[BaseProperty, OneDProperty, TwoDProperty, ThreeDProperty, ColorProperty, PropertyGroup]:
        if isinstance(name, int):
            stream = self._suite.GetNewEffectStreamByIndex(self.effect, name)
            return PropertyFactory.create_property(stream)
        else:
            for i in range(self._suite.GetEffectNumParamStreams(self.effect)):
                current_name = self._suite.GetStreamName(self._suite.GetNewEffectStreamByIndex(self.effect, i), True)
                current_match_name = self._dyn_suite.GetMatchname(self._suite.GetNewEffectStreamByIndex(self.effect, i))
                if current_name == name or current_match_name == name:
                    stream = self._suite.GetNewEffectStreamByIndex(self.effect, i)
                    return PropertyFactory.create_property(stream)
            return None
    
    def duplicate(self) -> 'Effect':
        effect_ref = PyFx.EffectSuite().duplicateEffect(self.effect)
        return Effect(effect_ref)
        
class Mask:
    def __init__(self, mask: PyFx.MaskRefPtr) -> None:
        self.mask = mask

    @classmethod
    def getMask(cls, layer: Layer, maskIndex: int) -> 'Mask':
        maskref = PyFx.MaskSuite().getLayerMaskByIndex(layer.layer, maskIndex)
        return Mask(maskref)

    def invert(self) -> bool:
        return PyFx.MaskSuite().getMaskInvert(self.mask)

    def setInvert(self, invert: bool) -> None:
        PyFx.MaskSuite().setMaskInvert(self.mask, invert)

    @property
    def mode(self) -> PyFx.MaskMode:
        return PyFx.MaskSuite().getMaskMode(self.mask)

    @mode.setter
    def mode(self, mode: PyFx.MaskMode) -> None:
        PyFx.MaskSuite().setMaskMode(self.mask, mode)

    @property
    def outline(self) -> MaskOutlineProperty:
        property = self.getProperty(PyFx.MaskStream.OUTLINE)
        return property

    @property
    def motionBlurState(self) -> PyFx.MaskMBlur:
        return PyFx.MaskSuite().getMaskMotionBlurState(self.mask)

    @property
    def opacity(self) -> OneDProperty:
        property = self.getProperty(PyFx.MaskStream.OPACITY)
        return property

    @property
    def feather(self) -> TwoDProperty:
        property = self.getProperty(PyFx.MaskStream.FEATHER)
        return property

    @property
    def expansion(self) -> OneDProperty:
        property = self.getProperty(PyFx.MaskStream.EXPANSION)
        return property

    @property
    def featherFalloff(self) -> PyFx.MaskFeatherFalloff:
        return PyFx.MaskSuite().getMaskFeatherFalloff(self.mask)

    @featherFalloff.setter
    def featherFalloff(self, featherFalloff: PyFx.MaskFeatherFalloff) -> None:
        PyFx.MaskSuite().setMaskFeatherFalloff(self.mask, featherFalloff)

    @property
    def id(self) -> int:
        return PyFx.MaskSuite().getMaskID(self.mask)

    @property
    def color(self) -> tuple[float, float, float, float]:
        return PyFx.MaskSuite().getMaskColor(self.mask).to_tuple()

    @color.setter
    def color(self, color: tuple[float, float, float, float]) -> None:
        PyFx.MaskSuite().setMaskColor(self.mask, PyFx.ColorVal(color))
        
    @property
    def lockState(self) -> bool:
        return PyFx.MaskSuite().getMaskLockState(self.mask)
    
    @lockState.setter
    def lockState(self, lock: bool) -> None:
        PyFx.MaskSuite().setMaskLockState(self.mask, lock)
        
    @property
    def isRotoBezier(self) -> bool:
        return PyFx.MaskSuite().getMaskIsRotoBezier(self.mask)
    
    @isRotoBezier.setter
    def isRotoBezier(self, isRotoBezier: bool) -> None:
        PyFx.MaskSuite().setMaskIsRotoBezier(self.mask, isRotoBezier)
        
    def getProperty(self, property: PyFx.MaskStream) -> Union[BaseProperty, MaskOutlineProperty, OneDProperty, TwoDProperty]:
        stream = self._suite.GetNewMaskStream(self.mask, property)
        return PropertyFactory.create_property(stream)


class Project:
    _suite = PyFx.ProjSuite()
    def __init__(self, proj : PyFx.ProjectPtr = None) -> None:
        if proj is None: # if no project is passed in, get the first project
            proj = self._suite.GetProjectByIndex(0)  # get the first project
        self.proj = proj  # store the project
        
    @classmethod # class method to create a new project
    def new(cls, name: str, path: str) -> 'Project':
        proj = PyFx.ProjSuite().NewProject(name)
        if path:
            PyFx.ProjSuite().SaveProjectToPath(proj, path)
        return cls(proj)
    
    @classmethod # class method to open a project
    def open(cls, path: str) -> 'Project':
        proj = PyFx.ProjSuite().OpenProjectFromPath(path)
        return cls(proj)
    
    @property # property to get the name of the project
    def path(self) -> str:
        return self._suite.GetProjectPath(self.proj)
    
    @property # property to see if the project has been modified
    def dirty(self) -> bool:
        return self._suite.ProjectIsDirty(self.proj)
    
    @property # property to get the bit depth of the project
    def bit_depth(self) -> PyFx.ProjBitDepth:
        
        return self._suite.GetProjectBitDepth(self.proj)
    
    @bit_depth.setter # property setter to set the bit depth of the project
    def bit_depth(self, value) -> None:
        self._suite.SetProjectBitDepth(self.proj, value)
        
    @property
    def items(self) -> ItemCollection:
        return ItemCollection.create(self._suite.GetProjectRootFolder(self.proj))
         
    def save(self, path: str) -> None:
        self._suite.SaveProjectToPath(self.proj, path)
        
    def Import(self, path: str) -> None:
        self._suite.ImportFile(self.proj, path)
        
        
            
            
            
            
            