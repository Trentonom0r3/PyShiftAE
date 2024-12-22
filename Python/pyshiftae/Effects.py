import PyFx # type: ignore
from typing import Union
from .Layers import Layer
from .Properties import *

class Effect:
    def __init__(self, effect: PyFx.EffectRefPtr) -> None:
        self.effect = effect

    @classmethod
    def apply(cls, layer: Layer, match_name: str) -> 'Effect':
        #check name and match name
        current_key = 0
        num_effects = PyFx.EffectSuite().getNumInstalledEffects()
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
            stream = PyFx.StreamSuite().GetNewEffectStreamByIndex(self.effect, name)
            return PropertyFactory.create_property(stream)
        else:
            for i in range(PyFx.StreamSuite().GetEffectNumParamStreams(self.effect)):
                current_name = PyFx.StreamSuite().GetStreamName(PyFx.StreamSuite().GetNewEffectStreamByIndex(self.effect, i), True)
                current_match_name = PyFx.DynamicStreamSuite().GetMatchname(PyFx.StreamSuite().GetNewEffectStreamByIndex(self.effect, i))
                if current_name == name or current_match_name == name:
                    stream = PyFx.StreamSuite().GetNewEffectStreamByIndex(self.effect, i)
                    return PropertyFactory.create_property(stream)
            return None
    
    def duplicate(self) -> 'Effect':
        effect_ref = PyFx.EffectSuite().duplicateEffect(self.effect)
        return Effect(effect_ref)