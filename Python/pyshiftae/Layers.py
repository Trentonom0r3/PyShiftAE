import PyFx # type: ignore
from .Properties import *
from .Items import *
from typing import Union

class Layer(PropertyGroup):
    def __init__(self, layer: PyFx.LayerPtr) -> None:
        super().__init__(PyFx.DynamicStreamSuite().GetNewStreamRefForLayer(layer))
        self.layer = layer
        
    @staticmethod
    def active_layer() -> Union['Layer', 'AVLayer', 'CameraLayer', 'LightLayer', 'TextLayer', 'VectorLayer', None]:
        layer_ptr = PyFx.LayerSuite().GetActiveLayer()
        if layer_ptr:
            return LayerFactory.create_layer(layer_ptr)
        
    @property
    def name(self) -> str:
        return PyFx.LayerSuite().GetLayerName(self.layer)
    
    @name.setter
    def name(self, value: str) -> None:
        PyFx.LayerSuite().SetLayerName(self.layer, value)
        
    @property
    def index(self) -> int:
        return PyFx.LayerSuite().GetLayerIndex(self.layer)
    
    @index.setter
    def index(self, value: int) -> None:
        PyFx.LayerSuite().ReorderLayer(self.layer, value)
        
    @property
    def source_item(self) -> Item:
        return ItemFactory.create_item(PyFx.LayerSuite().GetLayerSourceItem(self.layer))
    
    @property
    def parent_comp(self) -> CompItem:
        return CompItem(PyFx.LayerSuite().GetLayerParentComp(self.layer))
    
    @property
    def quality(self) -> PyFx.LayerQual:
        return PyFx.LayerSuite().GetLayerQuality(self.layer)
    
    @quality.setter
    def quality(self, value: PyFx.LayerQual) -> None:
        PyFx.LayerSuite().SetLayerQuality(self.layer, value)
        
    @property
    def video_active(self) -> bool:
        return PyFx.LayerSuite().GetLayerFlags(self.layer) & PyFx.LayerFlag.VIDEO_ACTIVE
    
    @property
    def audio_active(self) -> bool:
        return PyFx.LayerSuite().GetLayerFlags(self.layer) & PyFx.LayerFlag.AUDIO_ACTIVE
    
    @property
    def current_time(self) -> float:
        return PyFx.LayerSuite().GetLayerCurrentTime(self.layer)
    
    @property
    def duration(self) -> float:
        return PyFx.LayerSuite().GetLayerDuration(self.layer)
    
    @property
    def offset(self) -> float:
        return PyFx.LayerSuite().GetLayerOffset(self.layer)
    
    @offset.setter
    def offset(self, value: float) -> None:
        PyFx.LayerSuite().SetLayerOffset(self.layer, value)
        
    @property
    def in_point(self) -> float:
        return PyFx.LayerSuite().GetLayerInPoint(self.layer)
    
    @in_point.setter
    def in_point(self, value: float) -> None:
        PyFx.LayerSuite().SetLayerInPointAndDuration(self.layer, value, self.duration)
        
    @property
    def stretch(self) -> float:
        return PyFx.LayerSuite().GetLayerStretch(self.layer)
    
    @stretch.setter
    def stretch(self, value: float) -> None:
        PyFx.LayerSuite().SetLayerStretch(self.layer, value)
        
    @property
    def flag(self) -> PyFx.LayerFlag:
        return PyFx.LayerSuite().GetLayerFlags(self.layer)
    
    @flag.setter
    def flag(self, value: PyFx.LayerFlag) -> None:
        PyFx.LayerSuite().SetLayerFlag(self.layer, value)
        
    @property
    def is_3d(self) -> bool:
        return PyFx.LayerSuite().IsLayer3D(self.layer)
    
    @property
    def is_2d(self) -> bool:
        return not self.is_3d()
    
    @property
    def parent(self) -> 'Layer':
        return LayerFactory.create_layer(PyFx.LayerSuite().GetLayerParent(self.layer))
    
    @parent.setter
    def parent(self, value: 'Layer') -> None:
        PyFx.LayerSuite().SetLayerParent(self.layer, value.layer)
        
    @property
    def sampling_quality(self) -> PyFx.LayerSamplingQual:
        return PyFx.LayerSuite().GetLayerSamplingQuality(self.layer)
    
    @sampling_quality.setter
    def sampling_quality(self, value: PyFx.LayerSamplingQual) -> None:
        PyFx.LayerSuite().SetLayerSamplingQuality(self.layer, value)
        
    def duplicate(self) -> 'Layer':
        return LayerFactory.create_layer(PyFx.LayerSuite().DuplicateLayer(self.layer))
    
    def delete(self) -> None:
        PyFx.LayerSuite().DeleteLayer(self.layer)
        
    def num_effects(self) -> int:
        return PyFx.EffectSuite().getLayerNumEffects(self.layer)
    
    def get_property(self, stream: PyFx.LayerStream) -> Union[BaseProperty, OneDProperty, TwoDProperty, ThreeDProperty, ColorProperty, MarkerProperty, LayerIDProperty, MaskIDProperty, MaskOutlineProperty, TextDocumentProperty]:
        return PropertyFactory.create_property(PyFx.StreamSuite().GetNewLayerStream(self.layer, stream))
    
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
        return PyFx.LayerSuite().GetLayerObjectType(self.layer)
    
    def copy_to_comp(self, comp: CompItem) -> 'Layer':
        if PyFx.LayerSuite().IsAddLayerValid(self.source_item.item, comp.comp):
            return LayerFactory.create_layer(PyFx.LayerSuite().AddLayer(self.source_item.item, comp.comp))

class AVLayer(Layer):
    def __init__(self, layer: PyFx.LayerPtr) -> None:
        super().__init__(PyFx.DynamicStreamSuite().GetNewStreamRefForLayer(layer))
        self.layer = layer
        
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
        super().__init__(PyFx.DynamicStreamSuite().GetNewStreamRefForLayer(layer))
        self.layer = layer
        
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
        super().__init__(PyFx.DynamicStreamSuite().GetNewStreamRefForLayer(layer))
        self.layer = layer
        
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
        super().__init__(PyFx.DynamicStreamSuite().GetNewStreamRefForLayer(layer))
        self.layer = layer
        
    def getObjectType(self) -> PyFx.ObjectType:
        return PyFx.ObjectType.TEXT
    
class VectorLayer(Layer):
    def __init__(self, layer: PyFx.LayerPtr) -> None:
        super().__init__(PyFx.DynamicStreamSuite().GetNewStreamRefForLayer(layer))
        self.layer = layer
        
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
        
        