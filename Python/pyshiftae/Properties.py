import PyFx # type: ignore
from typing import Union

class BaseProperty:
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        self.property = property
        
    @property
    def name(self) -> str:
        return PyFx.StreamSuite().GetStreamName(self.property)
    
    @name.setter
    def name(self, value: str) -> None:
        PyFx.DynamicStreamSuite().SetStreamName(self.property, value)

    @property
    def type(self) -> PyFx.StreamType:
        return PyFx.StreamSuite().GetStreamType(self.property)
    
    @property
    def group_type(self) -> PyFx.StreamGroupingType:
        return PyFx.DynamicStreamSuite().GetStreamGroupingType(self.property)
    
    @property
    def match_name(self) -> str:
        return PyFx.DynamicStreamSuite().GetMatchname(self.property)
    
    def duplicate(self) -> Union['BaseProperty', 'PropertyGroup', 'OneDProperty', 'TwoDProperty', 'ThreeDProperty', 'ColorProperty', 'MarkerProperty', 'LayerIDProperty', 'MaskIDProperty', 'MaskOutlineProperty', 'TextDocumentProperty']:
        new_stream = PyFx.DynamicStreamSuite().DuplicateStream(self.property)
        stream = PyFx.DynamicStreamSuite().GetNewStreamRefByIndex(self.property, new_stream)
        return PropertyFactory.create_property(stream)
    
    def reorder(self, index: int) -> None:
        PyFx.DynamicStreamSuite().ReorderStream(self.property, index)
        
    def get_property(self, name: str) -> Union['BaseProperty', 'PropertyGroup', 'OneDProperty', 'TwoDProperty', 'ThreeDProperty', 'ColorProperty', 'MarkerProperty', 'LayerIDProperty', 'MaskIDProperty', 'MaskOutlineProperty', 'TextDocumentProperty']:
        stream = PyFx.DynamicStreamSuite().GetNewStreamRefByMatchname(self.property, name)
        return PropertyFactory.create_property(stream)
    
    def get_property_by_index(self, index: int) -> Union['BaseProperty', 'PropertyGroup', 'OneDProperty', 'TwoDProperty', 'ThreeDProperty', 'ColorProperty', 'MarkerProperty', 'LayerIDProperty', 'MaskIDProperty', 'MaskOutlineProperty', 'TextDocumentProperty']:
        stream = PyFx.DynamicStreamSuite().GetNewStreamRefByIndex(self.property, index)
        return PropertyFactory.create_property(stream)
    
    def add_property(self, name: str) -> None:
        if PyFx.DynamicStreamSuite().CanAddStream(self.property, name):
            PyFx.DynamicStreamSuite().AddStream(self.property, name)
            
    def remove_property(self, name: str) -> None:
        stream = PyFx.DynamicStreamSuite().GetNewStreamRefByMatchname(self.property, name)
        if stream:
            PyFx.DynamicStreamSuite().DeleteStream(stream)
            
    def remove_property_by_index(self, index: int) -> None:
        stream = PyFx.DynamicStreamSuite().GetNewStreamRefByIndex(self.property, index)
        if stream:
            PyFx.DynamicStreamSuite().DeleteStream(stream)
            
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
        return PyFx.DynamicStreamSuite().GetNumStreamsInGroup(self.property)
    
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
        val = PyFx.StreamSuite().GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        return val.value()
    
    def set_value(self, value: float) -> None:
        PyFx.StreamSuite().SetStreamValue(self.property, PyFx.StreamValue2Ptr(self.property, value))
        
    def __getitem__(self, time: float) -> float:
        return self.get_value(PyFx.LTimeMode.CompTime, time, False)
    
    def __setitem__(self, time: float, value: float) -> None:
        self.set_value(value)
        
class TwoDProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> tuple[float, float]:
        val = PyFx.StreamSuite().GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        two_d : PyFx.TwoDVal = val.value()
        return two_d.to_tuple()
    
    def set_value(self, value: tuple[float, float]) -> None:
        PyFx.StreamSuite().SetStreamValue(self.property, PyFx.StreamValue2Ptr(self.property, value))
        
    def __getitem__(self, time: float) -> tuple[float, float]:
        return self.get_value(PyFx.LTimeMode.CompTime, time, False)
    
    def __setitem__(self, time: float, value: tuple[float, float]) -> None:
        self.set_value(value)
        
class ThreeDProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> tuple[float, float, float]:
        val = PyFx.StreamSuite().GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        three_d : PyFx.ThreeDVal = val.value()
        return three_d.to_tuple()
    
    def set_value(self, value: tuple[float, float, float]) -> None:
        PyFx.StreamSuite().SetStreamValue(self.property, PyFx.StreamValue2Ptr(self.property, value))
        
    def __getitem__(self, time: float) -> tuple[float, float, float]:
        return self.get_value(PyFx.LTimeMode.CompTime, time, False)
    
    def __setitem__(self, time: float, value: tuple[float, float, float]) -> None:
        self.set_value(value)
        
class ColorProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> tuple[float, float, float, float]:
        val = PyFx.StreamSuite().GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        color : PyFx.ColorVal = val.value()
        return color.to_tuple()
    
    def set_value(self, value: tuple[float, float, float, float]) -> None:
        PyFx.StreamSuite().SetStreamValue(self.property, PyFx.StreamValue2Ptr(self.property, value))
        
class MarkerProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> 'PyFx.Marker':
        val = PyFx.StreamSuite().GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        return PyFx.Marker(val.value())
    
    def add_marker(self, time: float) -> 'PyFx.Marker':
        idx = PyFx.KeyframeSuite().InsertKeyframe(self.property, PyFx.LTimeMode.CompTime, PyFx.Time(time))
        marker = PyFx.MarkerSuite().getNewMarker()
        val = PyFx.StreamValue2Ptr(self.property, marker)
        PyFx.KeyframeSuite().SetKeyframeValue(self.property, idx, val)
        return PyFx.Marker(marker)
    
    def __getitem__(self, time: float) -> 'PyFx.Marker':
        return self.get_value(PyFx.LTimeMode.CompTime, time, False)
    
class LayerIDProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> int:
        val = PyFx.StreamSuite().GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        return val.value()
    
class MaskIDProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> int:
        val = PyFx.StreamSuite().GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        return val.value()
    
class MaskOutlineProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> 'PyFx.MaskOutline':
        val = PyFx.StreamSuite().GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
        return PyFx.MaskOutline(val.value())
    
class TextDocumentProperty(BaseProperty):
    def __init__(self, property: PyFx.StreamRefPtr) -> None:
        super().__init__(property)
        
    def get_value(self, time_mode: PyFx.LTimeMode, time: float, pre_expression: bool) -> 'PyFx.TextDocument':
        val = PyFx.StreamSuite().GetNewStreamValue(self.property, time_mode, PyFx.Time(time), pre_expression)
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
    