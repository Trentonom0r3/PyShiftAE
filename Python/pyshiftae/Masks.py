import PyFx # type: ignore
from .Properties import *
from .Items import *
from typing import Union
from .Layers import Layer

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
        stream = PyFx.StreamSuite().GetNewMaskStream(self.mask, property)
        return PropertyFactory.create_property(stream)
