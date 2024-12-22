import operator
from typing import Any, Iterator
import PyFx # type: ignore
from .Items import *
from .Layers import *

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
            if PyFx.ItemSuite().GetItemName(item.item) == key:
                return item
        raise KeyError(f"Item with name '{key}' not found")
    
    def append(self, object: Item) -> None:
        if PyFx.ItemSuite().GetItemParentFolder(object.item) != self._ROOT_FOLDER:
            PyFx.ItemSuite().SetItemParentFolder(object.item, self._ROOT_FOLDER) #set the parent folder
        super().append(object) #append the object to the list
        
    def remove(self, object: Item) -> None:
        PyFx.ItemSuite().DeleteItem(object.item) #delete the item
        super().remove(object)
        
    def clear(self) -> None:
        for item in self:
            PyFx.ItemSuite().DeleteItem(item.item)
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
    
class LayerCollection(list):
    _base_comp : PyFx.CompPtr = None
    def __init__(self) -> None:
        super().__init__()
        
    @classmethod
    def create(cls, base_comp: PyFx.CompPtr) -> 'LayerCollection':
        collection = cls()
        cls._base_comp = base_comp
        num_layers = PyFx.LayerSuite().GetCompNumLayers(base_comp)
        for i in range(num_layers):
            layer_ptr = PyFx.LayerSuite().GetCompLayerByIndex(base_comp, i)
            layer = LayerFactory.create_layer(layer_ptr)
            collection.append(layer)
        return collection
    
    def add_null(self, name: str, duration: float = 0.0) -> Layer:
        if duration < 0:
            duration = PyFx.ItemSuite().GetItemDuration(PyFx.CompSuite().GetItemFromComp(self._base_comp))
        null = PyFx.CompSuite().CreateNullInComp(self._base_comp, name, duration)
        layer = LayerFactory.create_layer(null)
        super().append(layer)
        return layer
    
    def add_solid(self, name: str, color: tuple[float, float, float, float], width: int, height: int, duration: float = 0.0) -> Layer:
        if duration < 0:
            duration = PyFx.ItemSuite().GetItemDuration(PyFx.CompSuite().GetItemFromComp(self._base_comp))
        solid = PyFx.CompSuite().CreateSolidInComp(self._base_comp, name, width, height, PyFx.ColorVal.from_tuple(color), duration)
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
        
    def append(self, item: Item) -> None:
        if PyFx.LayerSuite().IsAddLayerValid(item.item, self._base_comp):
            layer = Layer(PyFx.LayerSuite().AddLayer(item.item, self._base_comp))
            super().append(layer)
            
    def extend(self, layers: 'LayerCollection') -> None:
        for layer in layers:
            self.append(layer.source_item)
            super().append(layer)
            
    def insert(self, index: int, layer: Layer) -> None:
        if PyFx.LayerSuite().IsAddLayerValid(layer.source_item, self._base_comp):
            new_layer = Layer(PyFx.LayerSuite().AddLayer(layer.source_item, self._base_comp))
            PyFx.LayerSuite().ReorderLayer(new_layer, index)
            super().insert(index, new_layer)
            
    def remove(self, layer: Layer) -> None:
        PyFx.LayerSuite().DeleteLayer(layer.layer)
        super().remove(layer)
    
    def clear(self) -> None:
        for layer in self:
            PyFx.LayerSuite().DeleteLayer(layer.layer)
        super().clear()
        
    def pop(self, index: int) -> Layer:
        layer = super().pop(index)
        PyFx.LayerSuite().DeleteLayer(layer.layer)
        return layer
    
    def reverse(self) -> None:
        for i, layer in enumerate(self):
            PyFx.LayerSuite().ReorderLayer(layer.layer, len(self) - i)
        super().reverse()
        
    def sort(self, key=None, reverse=False) -> None:
        super().sort(key, reverse)
        for i, layer in enumerate(self):
            PyFx.LayerSuite().ReorderLayer(layer.layer, i)
            
    def __getitem__(self, key: any) -> Layer:
        if isinstance(key, int):
            return super().__getitem__(key)
        for layer in self:
            if PyFx.LayerSuite().GetLayerName(layer.layer) == key:
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
    
        
        
            
            
            
            
            
            