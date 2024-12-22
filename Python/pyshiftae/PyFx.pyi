from typing import Any, Tuple, Union
from enum import Enum, auto

class MemFlag(Enum):
    NONE = auto() #* No Memory Flag.*#
    CLEAR = auto() #* Clear Memory Flag.*#
    QUIET = auto()

class Platform(Enum):
    WIN = auto() #* Windows Platform.*#
    MAC = auto()

class ProjBitDepth(Enum):
    _8 = auto()   
    _16 = auto()
    _32 = auto()
    NUM_VALID_DEPTHS = auto()

class CameraType(Enum):
    NONE = auto()
    PERSPECTIVE = auto()
    ORTHOGRAPHIC = auto()
    NUM_TYPES = auto()

class TimeDisplayType(Enum):
    TIMECODE = auto()
    FRAMES = auto()
    FEET_AND_FRAMES = auto()

class FilmSizeUnits(Enum):
    NONE = auto()
    HORIZONTAL = auto()
    VERTICAL = auto()
    DIAGONAL = auto()

class LightType(Enum):
    NONE = auto()
    PARALLEL = auto()
    SPOT = auto()
    POINT = auto()
    AMBIENT = auto()
    RESERVED1 = auto()
    NUM_TYPES = auto()

class FootageSignature(Enum):
    NONE = auto()
    MISSING = auto()
    SOLID = auto()

class LightFalloffType(Enum):
    NONE = auto()
    SMOOTH = auto()
    INVERSE_SQUARE_CLAMPED = auto()

class FootageDepth(Enum):
    _1 = auto()
    _2 = auto()
    _4 = auto()
    _8 = auto()
    _16 = auto()
    _24 = auto()
    _30 = auto()
    _32 = auto()
    GRAY_2 = auto()
    GRAY_4 = auto()
    GRAY_8 = auto()
    _48 = auto()
    _64 = auto()
    GRAY_16 = auto()

class FramesPerFoot(Enum):
    _35MM = auto()
    _16MM = auto()

class TimeDisplayMode(Enum):
    TIMECODE = auto()
    FRAMES = auto()

class SourceTimecodeDisplayMode(Enum):
    ZERO = auto()
    SOURCE_TIMECODE = auto()

class FramesDisplayMode(Enum):
    ZERO_BASED = auto()
    ONE_BASED = auto()
    TIMECODE_CONVERSION = auto()

class SoundEncoding(Enum):
    UNSIGNED_PCM = auto()
    SIGNED_PCM = auto()
    FLOAT = auto()
    END = auto()
    BEGIN = auto()

class ItemType(Enum):
    NONE = auto()
    FOLDER = auto()
    COMP = auto()
    SOLID = auto()
    FOOTAGE = auto()
    NUM_TYPES = auto()

class ItemFlag(Enum):
    MISSING = auto()
    HAS_PROXY = auto()
    USING_PROXY = auto()
    MISSING_PROXY = auto()
    HAS_VIDEO = auto()
    HAS_AUDIO = auto()
    STILL = auto()
    HAS_ACTIVE_AUDIO = auto()

class Label(Enum):
    NONE = auto()
    NO_LABEL = auto()
    LABEL_1 = auto()
    LABEL_2 = auto()
    LABEL_3 = auto()
    LABEL_4 = auto()
    LABEL_5 = auto()
    LABEL_6 = auto()
    LABEL_7 = auto()
    LABEL_8 = auto()
    LABEL_9 = auto()
    LABEL_10 = auto()
    LABEL_11 = auto()
    LABEL_12 = auto()
    LABEL_13 = auto()
    LABEL_14 = auto()
    LABEL_15 = auto()
    LABEL_16 = auto()
    NUM_TYPES = auto()

class PersistentType(Enum):
    MACHINE_SPECIFIC = auto()
    MACHINE_INDEPENDENT = auto()
    MACHINE_INDEPENDENT_RENDER = auto()
    MACHINE_INDEPENDENT_OUTPUT = auto()
    MACHINE_INDEPENDENT_COMPOSITION = auto()
    MACHINE_SPECIFIC_TEXT = auto()
    MACHINE_SPECIFIC_PAINT = auto()
    MACHINE_SPECIFIC_EFFECTS = auto()
    MACHINE_SPECIFIC_EXPRESSION_SNIPPETS = auto()
    MACHINE_SPECIFIC_SCRIPT_SNIPPETS = auto()
    NUM_TYPES = auto()

class CompFlag(Enum):
    SHOW_ALL_SHY = auto()
    #* Show All Shy.*#
    ENABLE_MOTION_BLUR = auto()
    #* Enable Motion Blur.*#
    ENABLE_TIME_FILTER = auto()
    #* Enable Time Filter.*#
    GRID_TO_FRAMES = auto()
    #* Grid to Frames.*#
    GRID_TO_FIELDS = auto()
    #* Grid to Fields.*#
    USE_LOCAL_DSF = auto()
    #* Use Local DSF.*#
    DRAFT_3D = auto()
    #* Draft 3D.*#
    SHOW_GRAPH = auto()

class TransferFlags(Enum):
    PRESERVE_ALPHA = auto()
    RANDOMIZE_DISSOLVE = auto()

class TrackMatte(Enum):
    NO_TRACK_MATTE = auto()
    ALPHA = auto()
    NOT_ALPHA = auto()
    LUMA = auto()
    NOT_LUMA = auto()

class LayerQual(Enum):
    NONE = auto()
    WIREFRAME = auto()
    DRAFT = auto()
    BEST = auto()

class LayerSamplingQual(Enum):
    BILINEAR = auto()
    BICUBIC = auto()

class LayerFlag(Enum):
    NONE = auto()
    VIDEO_ACTIVE = auto()
    AUDIO_ACTIVE = auto()
    EFFECTS_ACTIVE = auto()
    MOTION_BLUR = auto()
    FRAME_BLENDING = auto()
    LOCKED = auto()
    SHY = auto()
    COLLAPSE = auto()
    AUTO_ORIENT_ROTATION = auto()
    ADJUSTMENT_LAYER = auto()
    TIME_REMAPPING = auto()
    LAYER_IS_3D = auto()
    LOOK_AT_CAMERA = auto()
    LOOK_AT_POI = auto()
    SOLO = auto()
    MARKERS_LOCKED = auto()
    NULL_LAYER = auto()
    HIDE_LOCKED_MASKS = auto()
    GUIDE_LAYER = auto()
    ADVANCED_FRAME_BLENDING = auto()
    SUBLAYERS_RENDER_SEPARATELY = auto()
    ENVIRONMENT_LAYER = auto()

class ObjectType(Enum):
    NONE = auto()
    AV = auto()
    LIGHT = auto()
    CAMERA = auto()
    TEXT = auto()
    VECTOR = auto()
    RESERVED1 = auto()
    RESERVED2 = auto()
    RESERVED3 = auto()
    RESERVED4 = auto()
    RESERVED5 = auto()
    NUM_TYPES = auto()

class LTimeMode(Enum):
    LayerTime = auto()
    CompTime = auto()

class LayerStream(Enum):
    SOURCE_TEXT = auto()
    ## Valid for all layer types
    ANCHORPOINT = auto()
    POSITION = auto()
    SCALE = auto()
    ROTATION = auto()
    ROTATE_Z = auto()
    OPACITY = auto()
    AUDIO = auto()
    MARKER = auto()
    TIME_REMAP = auto()
    ROTATE_X = auto()
    ROTATE_Y = auto()
    ORIENTATION = auto()
    TEXT = auto()
    ## only valid for AEGP_ObjectType = auto()
    DEPTH_OF_FIELD = auto()
    FOCUS_DISTANCE = auto()
    APERTURE = auto()
    BLUR_LEVEL = auto()
    IRIS_SHAPE = auto()
    IRIS_ROTATION = auto()
    IRIS_ROUNDNESS = auto()
    IRIS_ASPECT_RATIO = auto()
    IRIS_DIFFRACTION_FRINGE = auto()
    IRIS_HIGHLIGHT_GAIN = auto()
    IRIS_HIGHLIGHT_THRESHOLD = auto()
    IRIS_HIGHLIGHT_SATURATION = auto()
    ## only valid for AEGP_ObjectType = auto()
    COLOR = auto()
    CONE_ANGLE = auto()
    CONE_FEATHER = auto()
    SHADOW_DARKNESS = auto()
    SHADOW_DIFFUSION = auto()
    LIGHT_FALLOFF_TYPE = auto()
    LIGHT_FALLOFF_START = auto()
    LIGHT_FALLOFF_DISTANCE = auto()
    ## only valid for AEGP_ObjectType = auto()
    ACCEPTS_LIGHTS = auto()
    AMBIENT_COEFF = auto()
    DIFFUSE_COEFF = auto()
    SPECULAR_INTENSITY = auto()
    SPECULAR_SHININESS = auto()
    CASTS_SHADOWS = auto()
    LIGHT_TRANSMISSION = auto()
    METAL = auto()
    REFLECTION_INTENSITY = auto()
    REFLECTION_SHARPNESS = auto()
    REFLECTION_ROLLOFF = auto()
    TRANSPARENCY_COEFF = auto()
    TRANSPARENCY_ROLLOFF = auto()
    INDEX_OF_REFRACTION = auto()
    EXTRUSION_BEVEL_STYLE = auto()
    EXTRUSION_BEVEL_DIRECTION = auto()
    EXTRUSION_BEVEL_DEPTH = auto()
    EXTRUSION_HOLE_BEVEL_DEPTH = auto()
    EXTRUSION_DEPTH = auto()
    PLANE_CURVATURE = auto()
    PLANE_SUBDIVISION = auto()

class MaskStream(Enum):
    OUTLINE = auto()
    OPACITY = auto()
    FEATHER = auto()
    EXPANSION = auto()

class StreamFlag(Enum):
    NONE = auto()
    HAS_MIN = auto()
    HAS_MAX = auto()
    IS_SPATIAL = auto()

class KeyInterp(Enum):
    NONE = auto()
    LINEAR = auto()
    BEZIER = auto()
    HOLD = auto()

class KeyInterpMask(Enum):
    NONE = auto()
    LINEAR = auto()
    BEZIER = auto()
    HOLD = auto()
    CUSTOM = auto()
    ANY = auto()

class StreamType(Enum):
    NONE = auto()
    ThreeD_SPATIAL = auto()
    ThreeD = auto()
    TwoD_SPATIAL = auto()
    TwoD = auto()
    OneD = auto()
    COLOR = auto()
    ARB = auto()
    MARKER = auto()
    LAYER_ID = auto()
    MASK_ID = auto()
    MASK = auto()
    TEXT_DOCUMENT = auto()

class StreamGroupingType(Enum):
    NONE = auto()
    LEAF = auto()
    NAMED_GROUP = auto()
    INDEXED_GROUP = auto()

class DynStreamFlag(Enum):
    ACTIVE_EYEBALL = auto()
    HIDDEN = auto()
    DISABLED = auto()
    ELIDED = auto()
    SHOWN_WHEN_EMPTY = auto()
    SKIP_REVEAL_WHEN_UNHIDDEN = auto()

class KeyframeFlag(Enum):
    NONE = auto()
    TEMPORAL_CONTINUOUS = auto()
    TEMPORAL_AUTOBEZIER = auto()
    SPATIAL_CONTINUOUS = auto()
    SPATIAL_AUTOBEZIER = auto()
    ROVING = auto()

class MarkerStringType(Enum):
    COMMENT = auto()
    CHAPTER = auto()
    URL = auto()
    FRAME_TARGET = auto()
    CUE_POINT_NAME = auto()

class MarkerFlag(Enum):
    NONE = auto()
    NAVIGATION = auto()
    PROTECT_REGION = auto()

class EffectFlags(Enum):
    NONE = auto()
    ACTIVE = auto()
    AUDIO_ONLY = auto()
    AUDIO_TOO = auto()
    MISSING = auto()

class MaskMode(Enum):
    NONE = auto()
    ADD = auto()
    SUBTRACT = auto()
    INTERSECT = auto()
    LIGHTEN = auto()
    DARKEN = auto()
    DIFF = auto()
    ACCUM = auto()

class MaskMBlur(Enum):
    SAME_AS_LAYER = auto()
    OFF = auto()
    ON = auto()

class MaskFeatherFalloff(Enum):
    SMOOTH = auto()
    LINEAR = auto()

class MaskFeatherInterp(Enum):
    NORMAL = auto()
    HOLD_CW = auto()

class MaskFeatherType(Enum):
    OUTER = auto()
    INNER = auto()

class AlphaFlags(Enum):
    PREMUL = auto()
    INVERTED = auto()
    ALPHA_IGNORE = auto()

class PulldownPhase(Enum):
    NO_PULLDOWN = auto()
    WSSWW = auto()
    SSWWW = auto()
    SWWWS = auto()
    WWWSS = auto()
    WWSSW = auto()
    WWWSW = auto()
    WWSWW = auto()
    WSWWW = auto()
    SWWWW = auto()
    WWWWS = auto()

class LayerDrawStyle(Enum):
    LAYER_BOUNDS = auto()
    DOCUMENT_BOUNDS = auto()

class InterpretationStyle(Enum):
    NO_DIALOG_GUESS = auto()
    DIALOG_OK = auto()
    NO_DIALOG_NO_GUESS = auto()

class PluginPathType(Enum):
    PLUGIN = auto()
    USER_PLUGIN = auto()
    ALLUSER_PLUGIN = auto()
    APP = auto()

class RenderQueueState(Enum):
    STOPPED = auto()
    PAUSED = auto()
    RENDERING = auto()

class RenderItemStatus(Enum):
    NONE = auto()
    WILL_CONTINUE = auto()
    NEEDS_OUTPUT = auto()
    UNQUEUED = auto()
    QUEUED = auto()
    RENDERING = auto()
    USER_STOPPED = auto()
    ERR_STOPPED = auto()
    DONE = auto()

class LogType(Enum):
    NONE = auto()
    ERRORS_ONLY = auto()
    PLUS_SETTINGS = auto()
    PER_FRAME_INFO = auto()

class EmbeddingType(Enum):
    NONE = auto()
    NOTHING = auto()
    LINK = auto()
    LINK_AND_COPY = auto()

class PostRenderAction(Enum):
    NONE = auto()
    IMPORT = auto()
    IMPORT_AND_REPLACE_USAGE = auto()
    SET_PROXY = auto()

class OutputTypes(Enum):
    NONE = auto()
    VIDEO = auto()
    AUDIO = auto()

class VideoChannels(Enum):
    NONE = auto()
    RGB = auto()
    RGBA = auto()
    ALPHA = auto()

class StretchQuality(Enum):
    NONE = auto()
    LOW = auto()
    HIGH = auto()

class OutputColorType(Enum):
    STRAIGHT = auto()
    PREMUL = auto()

class WorldType(Enum):
    NONE = auto()
    W8 = auto()
    W16 = auto()
    W32 = auto()

class MatteMode(Enum):
    STRAIGHT = auto()
    PREMUL_BLACK = auto()
    PREMUL_BG_COLOR = auto()

class ChannelOrder(Enum):
    ARGB = auto()
    BGRA = auto()

class ItemQuality(Enum):
    DRAFT = auto()
    BEST = auto()

class CollectionItemType(Enum):
    NONE = auto()
    LAYER = auto()
    MASK = auto()
    EFFECT = auto()
    STREAM = auto()
    KEYFRAME = auto()
    MASK_VERTEX = auto()
    STREAMREF = auto()

class StreamCollectionItemType(Enum):
    NONE = auto()
    LAYER = auto()
    MASK = auto()
    EFFECT = auto()

class WindowType(Enum):
    NONE = auto()
    PROJECT = auto()
    COMP = auto()
    TIME_LAYOUT = auto()
    LAYER = auto()
    FOOTAGE = auto()
    RENDER_QUEUE = auto()
    QT = auto()
    DIALOG = auto()
    FLOWCHART = auto()
    EFFECT = auto()
    OTHER = auto()

class MenuID(Enum):
    NONE = auto()
    APPLE = auto()
    FILE = auto()
    EDIT = auto()
    COMPOSITION = auto()
    LAYER = auto()
    EFFECT = auto()
    WINDOW = auto()
    FLOATERS = auto()
    KF_ASSIST = auto()
    IMPORT = auto()
    SAVE_FRAME_AS = auto()
    PREFS = auto()
    EXPORT = auto()
    ANIMATION = auto()
    PURGE = auto()
    NEW = auto()



    

    
class AEHandle:
    def __init__(self, handle: Any, deleter: Any = None):
        self.handle = handle
        self.deleter = deleter
        
    def __del__(self):
        if self.deleter:
            self.deleter(self.handle)
            
    def __bool__(self):
        return self.handle is not None
    
    def get(self):
        return self.handle
    
    def remove_deleter(self):
        self.deleter = None
        
    def reset(self, new_handle = None):
        if self.deleter:
            self.deleter(self.handle)
        self.handle = new_handle
        
    def swap(self, other):
        self.handle, other.handle = other.handle, self.handle
        self.deleter, other.deleter = other.deleter, self.deleter
        
    def release(self):
        self.handle = None
        
    def __call__(self):
        return self.handle

class StreamRefPtr(AEHandle):
    pass
    
class ProjectPtr(AEHandle):
    pass

class ItemPtr(AEHandle):
    pass

class CompPtr(AEHandle):
    pass

class FootagePtr(AEHandle):
    pass

class LayerPtr(AEHandle):
    pass

class EffectRefPtr(AEHandle):
    pass

class MaskRefPtr(AEHandle):
    pass

class RenderLayerContextPtr(AEHandle):
    pass

class PersistentBlobPtr(AEHandle):
    pass

class MaskOutlineValPtr(AEHandle):
    pass

class CollectionPtr(AEHandle):
    pass

class Collection2Ptr(AEHandle):
    pass

class SoundDataPtr(AEHandle):
    pass

class AddKeyframesInfoPtr(AEHandle):
    pass

class RenderReceiptPtr(AEHandle):
    pass

class WorldPtr(AEHandle):
    pass

class RenderOptionsPtr(AEHandle):
    pass

class LayerRenderOptionsPtr(AEHandle):
    pass

class FrameReceiptPtr(AEHandle):
    pass

class RQItemRefPtr(AEHandle):
    pass

class OutputModuleRefPtr(AEHandle):
    pass

class TextDocumentPtr(AEHandle):
    pass

class MarkerValPtr(AEHandle):
    pass

class TextOutlinesPtr(AEHandle):
    pass

class PlatformWorldPtr(AEHandle):
    pass

class ItemViewPtr(AEHandle):
    pass

class ColorProfilePtr(AEHandle):
    pass

class ConstColorProfilePtr(AEHandle):
    pass

class TimeStampPtr(AEHandle):
    pass

class MemHandlePtr(AEHandle):
    pass

class StreamValue2(AEHandle):
    def __init__(self, streamref : StreamRefPtr, value: Union[float, Tuple[float, float], Tuple[float, float, float], Tuple[float, float, float, float]]):
        # actual implementation done on the C++ side
        pass
        
    def value(self) -> Union[float, Tuple[float, float], Tuple[float, float, float], Tuple[float, float, float, float]]:
        pass

class ColorVal:
    """ Represents a color value with RGBA components. """

    def __init__(self, r=0, g=0, b=0, a=0):
        self.red = r
        self.green = g
        self.blue = b
        self.alpha = a

    def __init__(self, color: Tuple[float, float, float, float]):
        pass

    def to_tuple(self) -> Tuple[float, float, float, float]:
        """ Convert the ColorVal instance to a tuple. """
        pass

class TimeDisplay3:
    """ Manages display settings for time formats. """

    def __init__(self, display_mode=TimeDisplayMode.FRAMES, footage_display_mode=SourceTimecodeDisplayMode.ZERO,
                 display_dropframe=False, use_feet_frames=False, timebase='0', frames_per_foot='0',
                 frames_display_mode=FramesDisplayMode.ONE_BASED):
        self.display_mode = display_mode
        self.footage_display_mode = footage_display_mode
        self.display_dropframe = display_dropframe
        self.use_feet_frames = use_feet_frames
        self.timebase = timebase
        self.frames_per_foot = frames_per_foot
        self.frames_display_mode = frames_display_mode

    def to_tuple(self) -> Tuple:
        """ Convert the TimeDisplay3 instance to a tuple. """
        pass

class SoundDataFormat:
    """ Represents sound data format details. """

    def __init__(self, sample_rate=0.0, encoding=SoundEncoding.BEGIN, bytes_per_sample=0, num_channels=0):
        self.sample_rate = sample_rate
        self.encoding = encoding
        self.bytes_per_sample = bytes_per_sample
        self.num_channels = num_channels

    def to_tuple(self) -> Tuple:
        """ Convert the SoundDataFormat instance to a tuple. """
        pass

class DownsampleFactor:
    """ Represents a downsample factor. """

    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

    def __init__(self, factor: Tuple[int, int]):
        pass

    def to_tuple(self) -> Tuple[int, int]:
        """ Convert the DownsampleFactor instance to a tuple. """
        pass
    
class LayerTransferMode:
    """ Represents layer transfer mode details. """

    def __init__(self, mode=0, flags=TransferFlags.PRESERVE_ALPHA, track_matte=TrackMatte.NO_TRACK_MATTE):
        self.mode = mode
        self.flags = flags
        self.track_matte = track_matte

    def to_tuple(self) -> Tuple:
        """ Convert the LayerTransferMode instance to a tuple. """
        pass
   
class OneDVal:
    def __init__(self, value=0):
        self.value = value
        
    def val(self):
        pass
    
    def to_tuple(self) -> Tuple:
        pass
    
class TwoDVal:
    """ Represents a 2D value. """

    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

    def __init__(self, value: Tuple[float, float]):
        pass
    
    def to_tuple(self) -> Tuple[float, float]:
        """ Convert the TwoDVal instance to a tuple. """
        pass
    
class ThreeDVal:
    """ Represents a 3D value. """

    def __init__(self, x=0, y=0, z=0):
        self.x = x
        self.y = y
        self.z = z

    def __init__(self, value: Tuple[float, float, float]):
        pass
    
    def to_tuple(self) -> Tuple[float, float, float]:
        """ Convert the ThreeDVal instance to a tuple. """
        pass
    
class KeyframeEase:
    """ Represents keyframe ease settings. """

    def __init__(self, speed=0, influence=0):
        self.speed = speed
        self.influence = influence

    def __init__(self, value: Tuple[float, float]):
        pass
    
    def to_tuple(self) -> Tuple[float, float]:
        """ Convert the KeyframeEase instance to a tuple. """
        pass
    
class MaskFeather:
    def __init__(self, segment=0, segment_sF=0, radiusF=0, ui_corner_angleF=0, tensionF=0,
                 interp=MaskFeatherInterp.NORMAL, type=MaskFeatherType.OUTER):
        self.segment = segment
        self.segment_sF = segment_sF
        self.radiusF = radiusF
        self.ui_corner_angleF = ui_corner_angleF
        self.tensionF = tensionF
        self.interp = interp
        self.type = type

    def to_tuple(self) -> Tuple:
        pass
    
class MaskVertex:
    def __init__(self, x=0, y=0, tan_in_x=0, tan_in_y=0, tan_out_x=0, tan_out_y=0):
        self.x = x
        self.y = y
        self.tan_in_x = tan_in_x
        self.tan_in_y = tan_in_y
        self.tan_out_x = tan_out_x
        self.tan_out_y = tan_out_y

    def to_tuple(self) -> Tuple[float, float, float, float, float, float]:
        pass
    
class LoopBehavior:
    def __init__(self, loops=0, reserved=0):
        self.loops = loops
        self.reserved = reserved

    def to_tuple(self) -> Tuple[int, int]:
        pass
    
class FootageLayerKey():
    def __init__(self, layer_idL=-1, layer_indexL=-1, nameAC="", layer_draw_style=LayerDrawStyle.LAYER_BOUNDS):
        self.layer_idL = layer_idL
        self.layer_indexL = layer_indexL
        self.nameAC = nameAC
        self.layer_draw_style = layer_draw_style
        
    def to_tuple(self) -> Tuple[int, int, str, LayerDrawStyle]:
        pass
    
    def default():
        pass

class FileSequenceImportOptions():
    def __init__(self, all_in_folderB=False, force_alphabeticalB=False, start_frameL=-1, end_frameL=-1):
        self.all_in_folderB = all_in_folderB
        self.force_alphabeticalB = force_alphabeticalB
        self.start_frameL = start_frameL
        self.end_frameL = end_frameL
        
    def to_tuple(self) -> Tuple[bool, bool, int, int]:
        pass
    
    def still(self):
        self.all_in_folderB = False
        return self
    
    def sequence(self):
        self.all_in_folderB = True
        self.force_alphabeticalB = True
        return self

class Time():
    def __init__(self, value=(0, 1)):
        self.value = value
     
    def __init__(self, value: float):
        pass
        
    def __init__(self, value: int):
        pass
        
    def to_seconds(self) -> float:
        pass
    
    def to_frames(self) -> int:
        pass

    
class Ratio():
    def __init__(self, num=0, den=1):
        self.num = num
        self.den = den
        
    def __init__(self, value: float):
        pass
        
    def __init__(self, value: int):
        pass
        
    def to_tuple(self) -> Tuple[int, int]:
        pass
    
    def value(self) -> Tuple[int, int]:
        pass

class FloatPoint:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y
        
    def to_tuple(self) -> Tuple[float, float]:
        pass
    
class FloatPoint3:
    def __init__(self, x=0, y=0, z=0):
        self.x = x
        self.y = y
        self.z = z
        
    def to_tuple(self) -> Tuple[float, float, float]:
        pass
    
class FloatRect:
    def __init__(self, left=0, top=0, right=0, bottom=0):
        self.left = left
        self.top = top
        self.right = right
        self.bottom = bottom
        
    def to_tuple(self) -> Tuple[float, float, float, float]:
        pass
    
class Matrix3:
    def __init__(self, mat=None):
        self.mat = mat
        
    
class Matrix4:
    def __init__(self, mat=None):
        self.mat = mat
        
    
class LegacyRect:
    def __init__(self, top=0, left=0, bottom=0, right=0):
        self.top = top
        self.left = left
        self.bottom = bottom
        self.right = right
        
    def to_tuple(self) -> Tuple[int, int, int, int]:
        pass
    
class LRect:
    def __init__(self, left=0, top=0, right=0, bottom=0):
        self.left = left
        self.top = top
        self.right = right
        self.bottom = bottom
        
    def to_tuple(self) -> Tuple[int, int, int, int]:
        pass
    
class LPoint:
    def __init__(self, x : int = 0, y : int = 0):
        self.x = x
        self.y = y
        
    def to_tuple(self) -> Tuple[int, int]:
        pass
    
class FloatPolar:
    def __init__(self, radius: float, angle: float):
        self.radius = radius
        self.angle = angle
        
    def to_tuple(self) -> Tuple[float, float]:
        pass

class Marker:
    def __init__(self, markerP: MarkerValPtr):
        self.markerP = markerP
        
    @classmethod
    def createMarker(cls):
        pass
    
    def duplicateMarker(self):
        pass
    
    def setFlag(self, flagType: MarkerFlag, valueB: bool):
        pass
    
    def getFlag(self, flagType: MarkerFlag) -> bool:
        pass
    
    def getString(self, strType: MarkerStringType) -> str:
        pass
    
    def setString(self, strType: MarkerStringType, unicodeP: str):
        pass
    
    def countCuePointParams(self) -> int:
        pass
    
    def getIndCuePointParam(self, param_indexL: int) -> Tuple[str, str]:
        pass
    
    def setIndCuePointParam(self, param_indexL: int, unicodeKeyP: str, unicodeValueP: str):
        pass
    
    def insertCuePointParam(self, param_indexL: int):
        pass
    
    def deleteIndCuePointParam(self, param_indexL: int):
        pass
    
    def setDuration(self, durationD: float):
        pass
    
    def getDuration(self) -> float:
        pass
    
    def setLabel(self, value: int):
        pass
    
    def getLabel(self) -> int:
        pass
    
class MaskOutline:
    def __init__(self, mask_outlineP: MaskOutlineValPtr):
        self.mask_outlineP = mask_outlineP
        
    def isOpen(self) -> bool:
        pass
    
    def setOpen(self, openB: bool):
        pass
    
    def numSegments(self) -> int:
        pass
    
    def getVertexInfo(self, which_pointL: int) -> MaskVertex:
        pass
    
    def setVertexInfo(self, which_pointL: int, vertexP: MaskVertex):
        pass
    
    def createVertex(self, insert_position):
        pass
    
    def deleteVertex(self, index: int):
        pass
    
    def numFeathers(self) -> int:
        pass
    
    def getFeatherInfo(self, which_featherL: int) -> MaskFeather:
        pass
    
    def setFeatherInfo(self, which_featherL: int, featherP: MaskFeather):
        pass
    
    def createFeather(self, featherP0: MaskFeather) -> int:
        pass
    
    def deleteFeather(self, index: int):
        pass
    
class TextDocument:
    def __init__(self, text_documentP: TextDocumentPtr):
        self.text_documentP = text_documentP
        
    def getText(self) -> str:
        pass
    
    def setText(self, unicodePS: str):
        pass
    
class ProjSuite:
    def __init__(self):
        pass
    
    def GetNumProjects(self) -> int:
        pass
    
    def GetProjectByIndex(self, projIndex: int) -> ProjectPtr:
        pass
    
    def GetProjectName(self, project: ProjectPtr) -> str:
        pass
    
    def GetProjectPath(self, project: ProjectPtr) -> str:
        pass
    
    def GetProjectRootFolder(self, project: ProjectPtr) -> ItemPtr:
        pass
    
    def SaveProjectToPath(self, project: ProjectPtr, path: str):
        pass
    
    def GetProjectTimeDisplay(self, project: ProjectPtr) -> TimeDisplay3:
        pass
    
    def SetProjectTimeDisplay(self, project: ProjectPtr, timeDisplay: TimeDisplay3):
        pass
    
    def ProjectIsDirty(self, project: ProjectPtr) -> bool:
        pass
    
    def SaveProjectAs(self, project: ProjectPtr, path: str):
        pass
    
    def NewProject(self) -> ProjectPtr:
        pass
    
    def OpenProjectFromPath(self, path: str) -> ProjectPtr:
        pass
    
    def GetProjectBitDepth(self, project: ProjectPtr) -> ProjBitDepth:
        pass
    
    def SetProjectBitDepth(self, project: ProjectPtr, bitDepth: ProjBitDepth):
        pass

class ItemSuite:
    def __init__(self):
        pass
    
    def GetFirstProjItem(self, project: ProjectPtr) -> ItemPtr:
        pass
    
    def GetNextProjItem(self, project: ProjectPtr, item: ItemPtr) -> ItemPtr:
        pass
    
    def GetActiveItem(self) -> ItemPtr:
        pass
    
    def IsItemSelected(self, item: ItemPtr) -> bool:
        pass
    
    def SelectItem(self, item: ItemPtr, select: bool, deselectOthers: bool):
        pass
    
    def GetItemType(self, item: ItemPtr) -> ItemType:
        pass
    
    def GetTypeName(self, itemType: ItemType) -> str:
        pass
    
    def GetItemName(self, item: ItemPtr) -> str:
        pass
    
    def SetItemName(self, item: ItemPtr, name: str):
        pass
    
    def GetItemID(self, item: ItemPtr) -> int:
        pass
    
    def GetItemFlags(self, item: ItemPtr) -> ItemFlag:
        pass
    
    def SetItemUseProxy(self, item: ItemPtr, useProxy: bool):
        pass
    
    def GetItemParentFolder(self, item: ItemPtr) -> ItemPtr:
        pass
    
    def SetItemParentFolder(self, item: ItemPtr, parentFolder: ItemPtr):
        pass
    
    def GetItemDuration(self, item: ItemPtr) -> Time:
        pass
    
    def GetItemCurrentTime(self, item: ItemPtr) -> Time:
        pass
    
    def GetItemDimensions(self, item: ItemPtr) -> Tuple[int, int]:
        pass
    
    def GetItemPixelAspectRatio(self, item: ItemPtr) -> Ratio:
        pass
    
    def DeleteItem(self, item: ItemPtr):
        pass
    
    def CreateNewFolder(self, name: str, parentFolder: ItemPtr) -> ItemPtr:
        pass
    
    def SetItemCurrentTime(self, item: ItemPtr, newTime: Time):
        pass
    
    def GetItemComment(self, item: ItemPtr) -> str:
        pass
    
    def SetItemComment(self, item: ItemPtr, comment: str):
        pass
    
    def GetItemLabel(self, item: ItemPtr) -> Label:
        pass
    
    def SetItemLabel(self, item: ItemPtr, label: Label):
        pass
    
    def GetItemMRUView(self, item: ItemPtr) -> ItemViewPtr:
        pass
    
    def GetItemViewPlaybackTime(self, itemView: ItemViewPtr) -> Tuple[Time, bool]:
        pass
    
class SoundDataSuite:
    def __init__(self):
        pass
    
    def NewSoundData(self, soundFormat: SoundDataFormat) -> SoundDataPtr:
        pass
    
    def GetSoundDataFormat(self, soundData: SoundDataPtr) -> SoundDataFormat:
        pass
    
    def LockSoundDataSamples(self, soundData: SoundDataPtr) -> Tuple:
        pass
    
    def UnlockSoundDataSamples(self, soundData: SoundDataPtr):
        pass
    
    def GetNumSamples(self, soundData: SoundDataPtr) -> int:
        pass

class CompSuite:
    def __init__(self):
        pass
    
    def GetCompFromItem(self, item: ItemPtr) -> CompPtr:
        pass
    
    def GetItemFromComp(self, comp: CompPtr) -> ItemPtr:
        pass
    
    def GetCompDownsampleFactor(self, comp: CompPtr) -> DownsampleFactor:
        pass
    
    def SetCompDownsampleFactor(self, comp: CompPtr, factor: DownsampleFactor):
        pass
    
    def GetCompBGColor(self, comp: CompPtr) -> ColorVal:
        pass
    
    def SetCompBGColor(self, comp: CompPtr, color: ColorVal):
        pass
    
    def GetCompFlags(self, comp: CompPtr) -> CompFlag:
        pass
    
    def GetShowLayerNameOrSourceName(self, comp: CompPtr) -> bool:
        pass
    
    def SetShowLayerNameOrSourceName(self, comp: CompPtr, showLayerName: bool):
        pass
    
    def GetShowBlendModes(self, comp: CompPtr) -> bool:
        pass
    
    def SetShowBlendModes(self, comp: CompPtr, showBlendModes: bool):
        pass
    
    def GetCompFramerate(self, comp: CompPtr) -> float:
        pass
    
    def SetCompFrameRate(self, comp: CompPtr, fps: float):
        pass
    
    def GetCompShutterAnglePhase(self, comp: CompPtr) -> Tuple[Ratio, Ratio]:
        pass
    
    def GetCompShutterFrameRange(self, comp: CompPtr, compTime: Time) -> Tuple[Time, Time]:
        pass
    
    def GetCompSuggestedMotionBlurSamples(self, comp: CompPtr) -> int:
        pass
    
    def SetCompSuggestedMotionBlurSamples(self, comp: CompPtr, samples: int):
        pass
    
    def GetCompMotionBlurAdaptiveSampleLimit(self, comp: CompPtr) -> int:
        pass
    
    def SetCompMotionBlurAdaptiveSampleLimit(self, comp: CompPtr, samples: int):
        pass
    
    def GetCompWorkAreaStart(self, comp: CompPtr) -> Time:
        pass
    
    def GetCompWorkAreaDuration(self, comp: CompPtr) -> Time:
        pass
    
    def SetCompWorkAreaStartAndDuration(self, comp: CompPtr, workAreaStart: Time, workAreaDuration: Time):
        pass
    
    def CreateSolidInComp(self, comp: CompPtr, name: str, width: int, height: int, color: ColorVal, duration: Time) -> LayerPtr:
        pass
    
    def CreateCameraInComp(self, comp: CompPtr, name: str, centerPoint: FloatPoint) -> LayerPtr:
        pass
    
    def CreateLightInComp(self, comp: CompPtr, name: str, centerPoint: FloatPoint) -> LayerPtr:
        pass
    
    def CreateComp(self, parentFolder: ItemPtr, name: str, width: int, height: int, pixelAspectRatio: Ratio, duration: Time, framerate: Ratio) -> CompPtr:
        pass
    
    def GetCompDisplayStartTime(self, comp: CompPtr) -> Time:
        pass
    
    def SetCompDisplayStartTime(self, comp: CompPtr, startTime: Time):
        pass
    
    def SetCompDuration(self, comp: CompPtr, duration: Time):
        pass
    
    def SetCompDimensions(self, comp: CompPtr, width: int, height: int):
        pass
    
    def SetCompPixelAspectRatio(self, comp: CompPtr, pixelAspectRatio: Ratio):
        pass
    
    def CreateTextLayerInComp(self, comp: CompPtr, newLayer: bool = True) -> LayerPtr:
        pass
    
    def CreateBoxTextLayerInComp(self, comp: CompPtr, boxDimensions: FloatPoint, newLayer: bool = True) -> LayerPtr:
        pass
    
    def CreateNullInComp(self, comp: CompPtr, name: str, duration: Time) -> LayerPtr:
        pass
    
    def DuplicateComp(self, comp: CompPtr) -> CompPtr:
        pass
    
    def GetCompFrameDuration(self, comp: CompPtr) -> Time:
        pass
    
    def GetMostRecentlyUsedComp(self) -> CompPtr:
        pass
    
    def CreateVectorLayerInComp(self, comp: CompPtr) -> LayerPtr:
        pass
    
    def GetNewCompMarkerStream(self, parentComp: CompPtr) -> StreamRefPtr:
        pass
    
    def GetCompDisplayDropFrame(self, comp: CompPtr) -> bool:
        pass
    
    def SetCompDisplayDropFrame(self, comp: CompPtr, dropFrame: bool):
        pass

class LayerSuite:
    def __init__(self):
        pass
    
    def GetCompNumLayers(self, comp: CompPtr) -> int:
        pass
    
    def GetCompLayerByIndex(self, comp: CompPtr, layerIndex: int) -> LayerPtr:
        pass
    
    def GetActiveLayer(self) -> LayerPtr:
        pass
    
    def GetLayerIndex(self, layer: LayerPtr) -> int:
        pass
    
    def GetLayerSourceItem(self, layer: LayerPtr) -> ItemPtr:
        pass
    
    def GetLayerSourceItemID(self, layer: LayerPtr) -> int:
        pass
    
    def GetLayerParentComp(self, layer: LayerPtr) -> CompPtr:
        pass
    
    def GetLayerName(self, layer: LayerPtr) -> Tuple[str, str]:
        pass
    
    def GetLayerQuality(self, layer: LayerPtr) -> LayerQual:
        pass
    
    def SetLayerQuality(self, layer: LayerPtr, quality: LayerQual):
        pass
    
    def GetLayerFlags(self, layer: LayerPtr) -> LayerFlag:
        pass
    
    def SetLayerFlag(self, layer: LayerPtr, singleFlag: LayerFlag, value: bool):
        pass
    
    def IsLayerVideoReallyOn(self, layer: LayerPtr) -> bool:
        pass
    
    def IsLayerAudioReallyOn(self, layer: LayerPtr) -> bool:
        pass
    
    def GetLayerCurrentTime(self, layer: LayerPtr, timeMode: LTimeMode) -> Time:
        pass
    
    def GetLayerInPoint(self, layer: LayerPtr, timeMode: LTimeMode) -> Time:
        pass
    
    def GetLayerDuration(self, layer: LayerPtr, timeMode: LTimeMode) -> Time:
        pass
    
    def SetLayerInPointAndDuration(self, layer: LayerPtr, timeMode: LTimeMode, inPoint: Time, duration: Time):
        pass
    
    def GetLayerOffset(self, layer: LayerPtr) -> Time:
        pass
    
    def SetLayerOffset(self, layer: LayerPtr, offset: Time):
        pass
    
    def GetLayerStretch(self, layer: LayerPtr) -> Ratio:
        pass
    
    def SetLayerStretch(self, layer: LayerPtr, stretch: Ratio):
        pass
    
    def GetLayerTransferMode(self, layer: LayerPtr) -> Tuple[TransferFlags, TrackMatte]:
        pass
    
    def SetLayerTransferMode(self, layer: LayerPtr, flags: TransferFlags, trackMatte: TrackMatte):
        pass
    
    def IsAddLayerValid(self, itemToAdd: ItemPtr, intoComp: CompPtr) -> bool:
        pass
    
    def AddLayer(self, itemToAdd: ItemPtr, intoComp: CompPtr) -> LayerPtr:
        pass
    
    def ReorderLayer(self, layer: LayerPtr, layerIndex: int):
        pass
    
    def GetLayerMaskedBounds(self, layer: LayerPtr, timeMode: LTimeMode, time: Time) -> FloatRect:
        pass
    
    def GetLayerObjectType(self, layer: LayerPtr) -> ObjectType:
        pass
    
    def IsLayer3D(self, layer: LayerPtr) -> bool:
        pass
    
    def IsLayer2D(self, layer: LayerPtr) -> bool:
        pass
    
    def IsVideoActive(self, layer: LayerPtr, timeMode: LTimeMode, time: Time) -> bool:
        pass
    
    def IsLayerUsedAsTrackMatte(self, layer: LayerPtr, fillMustBeActive: bool) -> bool:
        pass
    
    def DoesLayerHaveTrackMatte(self, layer: LayerPtr) -> bool:
        pass
    
    def ConvertCompToLayerTime(self, layer: LayerPtr, compTime: Time) -> Time:
        pass
    
    def ConvertLayerToCompTime(self, layer: LayerPtr, layerTime: Time) -> Time:
        pass
    
    def GetLayerDancingRandValue(self, layer: LayerPtr, compTime: Time) -> int:
        pass
    
    def GetLayerID(self, layer: LayerPtr) -> int:
        pass
    
    def GetLayerToWorldXform(self, layer: LayerPtr, compTime: Time) -> Matrix4:
        pass
    
    def GetLayerToWorldXformFromView(self, layer: LayerPtr, viewTime: Time, compTime: Time) -> Matrix4:
        pass
    
    def SetLayerName(self, layer: LayerPtr, newName: str):
        pass
    
    def GetLayerParent(self, layer: LayerPtr) -> LayerPtr:
        pass
    
    def SetLayerParent(self, layer: LayerPtr, parentLayer: LayerPtr):
        pass
    
    def DeleteLayer(self, layer: LayerPtr):
        pass
    
    def DuplicateLayer(self, origLayer: LayerPtr) -> LayerPtr:
        pass
    
    def GetLayerFromLayerID(self, parentComp: CompPtr, id: int) -> LayerPtr:
        pass
    
    def GetLayerLabel(self, layer: LayerPtr) -> Label:
        pass
    
    def SetLayerLabel(self, layer: LayerPtr, label: Label):
        pass
    
    def GetLayerSamplingQuality(self, layer: LayerPtr) -> LayerSamplingQual:
        pass
    
    def SetLayerSamplingQuality(self, layer: LayerPtr, quality: LayerSamplingQual):
        pass
    
    def GetTrackMatteLayer(self, layer: LayerPtr) -> LayerPtr:
        pass
    
    def SetTrackMatte(self, layer: LayerPtr, trackMatteLayer: LayerPtr, trackMatteType: TrackMatte):
        pass
    
    def RemoveTrackMatte(self, layer: LayerPtr):
        pass

class StreamSuite:
    def __init__(self):
        pass
    
    def IsStreamLegal(self, layer: LayerPtr, whichStream: LayerStream) -> bool:
        pass
    
    def CanVaryOverTime(self, stream: StreamRefPtr) -> bool:
        pass
    
    def GetValidInterpolations(self, stream: StreamRefPtr) -> KeyInterpMask:
        pass
    
    def GetNewLayerStream(self, layer: LayerPtr, whichStream: LayerStream) -> StreamRefPtr:
        pass
    
    def GetEffectNumParamStreams(self, effectRef: EffectRefPtr) -> int:
        pass
    
    def GetNewEffectStreamByIndex(self, effectRef: EffectRefPtr, paramIndex: int) -> StreamRefPtr:
        pass
    
    def GetNewMaskStream(self, maskRef: MaskRefPtr, whichStream: MaskStream) -> StreamRefPtr:
        pass
    
    def GetStreamName(self, stream: StreamRefPtr, forceEnglish: bool) -> str:
        pass
    
    def GetStreamUnitsText(self, stream: StreamRefPtr, forceEnglish: bool) -> str:
        pass
    
    def GetStreamProperties(self, stream: StreamRefPtr) -> Tuple[StreamFlag, float, float]:
        pass
    
    def IsStreamTimevarying(self, stream: StreamRefPtr) -> bool:
        pass
    
    def GetStreamType(self, stream: StreamRefPtr) -> StreamType:
        pass
    
    def GetNewStreamValue(self, stream: StreamRefPtr, timeMode: LTimeMode, time: Time, preExpression: bool) -> StreamValue2:
        pass
    
    def SetStreamValue(self, stream: StreamRefPtr, value: StreamValue2):
        pass
    
    def DuplicateStreamRef(self, stream: StreamRefPtr) -> StreamRefPtr:
        pass
    
    def GetUniqueStreamID(self, stream: StreamRefPtr) -> int:
        pass
    
class DynamicStreamSuite:
    def __init__(self):
        pass
    
    def GetNewStreamRefForLayer(self, layer: LayerPtr) -> StreamRefPtr:
        pass
    
    def GetNewStreamRefForMask(self, mask: MaskRefPtr) -> StreamRefPtr:
        pass
    
    def GetStreamDepth(self, stream: StreamRefPtr) -> int:
        pass
    
    def GetStreamGroupingType(self, stream: StreamRefPtr) -> StreamGroupingType:
        pass
    
    def GetNumStreamsInGroup(self, stream: StreamRefPtr) -> int:
        pass
    
    def GetDynamicStreamFlags(self, stream: StreamRefPtr) -> DynStreamFlag:
        pass
    
    def SetDynamicStreamFlag(self, stream: StreamRefPtr, oneFlag: DynStreamFlag, undoable: bool, set: bool):
        pass
    
    def GetNewStreamRefByIndex(self, parentGroup: StreamRefPtr, index: int) -> StreamRefPtr:
        pass
    
    def GetNewStreamRefByMatchname(self, parentGroup: StreamRefPtr, matchName: str) -> StreamRefPtr:
        pass
    
    def DeleteStream(self, stream: StreamRefPtr):
        pass
    
    def ReorderStream(self, stream: StreamRefPtr, newIndex: int):
        pass
    
    def DuplicateStream(self, stream: StreamRefPtr) -> int:
        pass
    
    def SetStreamName(self, stream: StreamRefPtr, newName: str):
        pass
    
    def CanAddStream(self, parentGroup: StreamRefPtr, matchName: str) -> bool:
        pass
    
    def AddStream(self, parentGroup: StreamRefPtr, matchName: str) -> StreamRefPtr:
        pass
    
    def GetMatchname(self, stream: StreamRefPtr) -> str:
        pass
    
    def GetNewParentStreamRef(self, stream: StreamRefPtr) -> StreamRefPtr:
        pass
    
    def GetStreamIsModified(self, stream: StreamRefPtr) -> bool:
        pass
    
    def IsSeparationLeader(self, stream: StreamRefPtr) -> bool:
        pass
    
    def AreDimensionsSeparated(self, leaderStream: StreamRefPtr) -> bool:
        pass
    
    def SetDimensionsSeparated(self, leaderStream: StreamRefPtr, separated: bool):
        pass
    
    def GetSeparationFollower(self, dimension: int, leaderStream: StreamRefPtr) -> StreamRefPtr:
        pass
    
    def IsSeparationFollower(self, stream: StreamRefPtr) -> bool:
        pass
    
    def GetSeparationLeader(self, followerStream: StreamRefPtr) -> StreamRefPtr:
        pass
    
    def GetSeparationDimension(self, stream: StreamRefPtr) -> int:
        pass
    
class KeyframeSuite:
    def __init__(self):
        pass
    
    def GetStreamNumKFs(self, stream: StreamRefPtr) -> int:
        pass
    
    def GetKeyframeTime(self, stream: StreamRefPtr, keyIndex: int, timeMode: LTimeMode) -> Time:
        pass
    
    def InsertKeyframe(self, stream: StreamRefPtr, timeMode: LTimeMode, time: Time) -> int:
        pass
    
    def DeleteKeyframe(self, stream: StreamRefPtr, keyIndex: int):
        pass
    
    def GetNewKeyframeValue(self, stream: StreamRefPtr, keyIndex: int) -> StreamValue2:
        pass
    
    def SetKeyframeValue(self, stream: StreamRefPtr, keyIndex: int, value: StreamValue2):
        pass
    
    def GetStreamValueDimensionality(self, stream: StreamRefPtr) -> int:
        pass
    
    def GetStreamTemporalDimensionality(self, stream: StreamRefPtr) -> int:
        pass
    
    def GetNewKeyframeSpatialTangents(self, stream: StreamRefPtr, keyIndex: int) -> Tuple[StreamValue2, StreamValue2]:
        pass
    
    def SetKeyframeSpatialTangents(self, stream: StreamRefPtr, keyIndex: int, inTan: StreamValue2, outTan: StreamValue2):
        pass
    
    def GetKeyframeTemporalEase(self, stream: StreamRefPtr, keyIndex: int, dimension: int) -> Tuple[KeyframeEase, KeyframeEase]:
        pass
    
    def SetKeyframeTemporalEase(self, stream: StreamRefPtr, keyIndex: int, dimension: int, inEase: KeyframeEase, outEase: KeyframeEase):
        pass
    
    def GetKeyframeFlags(self, stream: StreamRefPtr, keyIndex: int) -> KeyframeFlag:
        pass
    
    def SetKeyframeFlag(self, stream: StreamRefPtr, keyIndex: int, flag: KeyframeFlag, value: bool):
        pass
    
    def GetKeyframeInterpolation(self, stream: StreamRefPtr, keyIndex: int) -> Tuple[KeyInterp, KeyInterp]:
        pass
    
    def SetKeyframeInterpolation(self, stream: StreamRefPtr, keyIndex: int, inInterp: KeyInterp, outInterp: KeyInterp):
        pass
    
    def StartAddKeyframes(self, stream: StreamRefPtr) -> AddKeyframesInfoPtr:
        pass
        
    def AddKeyframes(self, akH: AddKeyframesInfoPtr, timeMode: LTimeMode, time: Time) -> int:
        pass
        
    def SetAddKeyframe(self, akH: AddKeyframesInfoPtr, keyIndex: int, value: StreamValue2):
        pass
        
    def EndAddKeyframes(self, akH: AddKeyframesInfoPtr):
        pass
        
    def GetKeyframeLabelColorIndex(self, stream: StreamRefPtr, keyIndex: int) -> int:
        pass
        
    def SetKeyframeLabelColorIndex(self, stream: StreamRefPtr, keyIndex: int, keyLabel: int):
        pass

class TextDocumentSuite:
    def __init__(self):
        pass
    
    def getNewText(self, text_documentP: TextDocumentPtr) -> str:
        pass
    
    def setText(self, text_documentP: TextDocumentPtr, unicodePS: str):
        pass
    
class MarkerSuite:
    def __init__(self):
        pass
    
    def getNewMarker(self) -> MarkerValPtr:
        pass
    
    def duplicateMarker(self, markerP: MarkerValPtr) -> MarkerValPtr:
        pass
    
    def setMarkerFlag(self, markerP: MarkerValPtr, flagType: MarkerFlag, valueB: bool):
        pass
    
    def getMarkerFlag(self, markerP: MarkerValPtr, flagType: MarkerFlag) -> bool:
        pass
    
    def getMarkerString(self, markerP: MarkerValPtr, strType: MarkerStringType) -> str:
        pass
    
    def setMarkerString(self, markerP: MarkerValPtr, strType: MarkerStringType, unicodeP: str):
        pass
    
    def countCuePointParams(self, markerP: MarkerValPtr) -> int:
        pass
    
    def getIndCuePointParam(self, markerP: MarkerValPtr, param_indexL: int) -> Tuple[str, str]:
        pass
    
    def setIndCuePointParam(self, markerP: MarkerValPtr, param_indexL: int, unicodeKeyP: str, unicodeValueP: str):
        pass
    
    def insertCuePointParam(self, markerP: MarkerValPtr, param_indexL: int):
        pass
    
    def deleteIndCuePointParam(self, markerP: MarkerValPtr, param_indexL: int):
        pass
    
    def setMarkerDuration(self, markerP: MarkerValPtr, durationPT: Time):
        pass
    
    def getMarkerDuration(self, markerP: MarkerValPtr) -> Time:
        pass
    
    def setMarkerLabel(self, markerP: MarkerValPtr, value: int):
        pass
    
    def getMarkerLabel(self, markerP: MarkerValPtr) -> int:
        pass
    
class TextLayerSuite:
    def __init__(self):
        pass
    
    def getNewTextOutlines(self, layer: LayerPtr, layer_time: Time) -> TextOutlinesPtr:
        pass
    
    def getNumTextOutlines(self, outlines: TextOutlinesPtr) -> int:
        pass
    
    #def getIndexedTextOutline(self, outlines: TextOutlinesPtr, path_index: int) -> PF_PathOutlinePtr:
       # pass

class EffectSuite:
    def __init__(self):
        pass
    
    def getLayerNumEffects(self, layer: LayerPtr) -> int:
        pass
    
    def getLayerEffectByIndex(self, layer: LayerPtr, layer_effect_index: int) -> EffectRefPtr:
        pass
    
    def getInstalledKeyFromLayerEffect(self, effect_ref: EffectRefPtr) -> int:
        pass
    
   # def getEffectParamUnionByIndex(self, effect_ref: EffectRefPtr, param_index: int) -> Tuple[PF_ParamType, PF_ParamDefUnion]:
      #  pass
    
    def getEffectFlags(self, effect_ref: EffectRefPtr) -> EffectFlags:
        pass
    
    def setEffectFlags(self, effect_ref: EffectRefPtr, effect_flags_set_mask: EffectFlags, effect_flags: EffectFlags):
        pass
    
    def reorderEffect(self, effect_ref: EffectRefPtr, effect_index: int):
        pass
    
   # def effectCallGeneric(self, effect_ref: EffectRefPtr, timePT: Time, effect_cmd: PF_Cmd, effect_extraPV):
    #    pass
    
    def applyEffect(self, layer: LayerPtr, installed_effect_key: int) -> EffectRefPtr:
        pass
    
    def deleteLayerEffect(self, effect_ref: EffectRefPtr):
        pass
    
    def getNumInstalledEffects(self) -> int:
        pass
    
    def getNextInstalledEffect(self, installed_effect_key: int) -> int:
        pass
    
    def getEffectName(self, installed_effect_key: int) -> str:
        pass
    
    def getEffectMatchName(self, installed_effect_key: int) -> str:
        pass
    
    def getEffectCategory(self, installed_effect_key: int) -> str:
        pass
    
    def duplicateEffect(self, original_effect_ref: EffectRefPtr) -> EffectRefPtr:
        pass
    
    def numEffectMask(self, effect_ref: EffectRefPtr) -> int:
        pass
    
    def getEffectMaskID(self, effect_ref: EffectRefPtr, mask_indexL: int) -> int:
        pass
    
    def addEffectMask(self, effect_ref: EffectRefPtr, id_val: int) -> StreamRefPtr:
        pass
    
    def removeEffectMask(self, effect_ref: EffectRefPtr, id_val: int):
        pass
    
    def setEffectMask(self, effect_ref: EffectRefPtr, mask_indexL: int, id_val: int) -> StreamRefPtr:
        pass
    
class MaskSuite:
    def __init__(self):
        pass
    
    def getLayerNumMasks(self, layer: LayerPtr) -> int:
        pass
    
    def getLayerMaskByIndex(self, layer: LayerPtr, mask_indexL: int) -> MaskRefPtr:
        pass
    
    def getMaskInvert(self, mask_ref: MaskRefPtr) -> bool:
        pass
    
    def setMaskInvert(self, mask_ref: MaskRefPtr, invertB: bool):
        pass
    
    def getMaskMode(self, mask_ref: MaskRefPtr) -> MaskMode:
        pass
    
    def setMaskMode(self, maskH: MaskRefPtr, mode: MaskMode):
        pass
    
    def getMaskMotionBlurState(self, mask_ref: MaskRefPtr) -> MaskMBlur:
        pass
    
    def setMaskMotionBlurState(self, mask_ref: MaskRefPtr, blur_state: MaskMBlur):
        pass
    
    def getMaskFeatherFalloff(self, mask_ref: MaskRefPtr) -> MaskFeatherFalloff:
        pass
    
    def setMaskFeatherFalloff(self, mask_ref: MaskRefPtr, feather_falloffP: MaskFeatherFalloff):
        pass
    
    def getMaskID(self, mask_ref: MaskRefPtr) -> int:
        pass
    
    def createNewMask(self, layerH: LayerPtr, mask_indexPL0: int) -> MaskRefPtr:
        pass
    
    def deleteMaskFromLayer(self, mask_ref: MaskRefPtr):
        pass
    
    def getMaskColor(self, mask_ref: MaskRefPtr) -> ColorVal:
        pass
    
    def setMaskColor(self, mask_ref: MaskRefPtr, colorP: ColorVal):
        pass
    
    def getMaskLockState(self, mask_ref: MaskRefPtr) -> bool:
        pass
    
    def setMaskLockState(self, mask_ref: MaskRefPtr, lockB: bool):
        pass
    
    def getMaskIsRotoBezier(self, mask_ref: MaskRefPtr) -> bool:
        pass
    
    def setMaskIsRotoBezier(self, mask_ref: MaskRefPtr, is_roto_bezierB: bool):
        pass
    
    def duplicateMask(self, orig_mask_refH: MaskRefPtr) -> MaskRefPtr:
        pass
    

class MaskOutlineSuite:
    def __init__(self):
        pass
    
    def isMaskOutlineOpen(self, mask_outlineH: MaskOutlineValPtr) -> bool:
        pass
    
    def setMaskOutlineOpen(self, mask_outlineH: MaskOutlineValPtr, openB: bool):
        pass
    
    def getMaskOutlineNumSegments(self, mask_outlineH: MaskOutlineValPtr) -> int:
        pass
    
    def getMaskOutlineVertexInfo(self, mask_outlineH: MaskOutlineValPtr, which_pointL: int) -> MaskVertex:
        pass
    
    def setMaskOutlineVertexInfo(self, mask_outlineH: MaskOutlineValPtr, which_pointL: int, vertexP: MaskVertex):
        pass
    
    def createVertex(self, mask_outlineH: MaskOutlineValPtr, insert_position: int):
        pass
    
    def deleteVertex(self, mask_outlineH: MaskOutlineValPtr, index: int):
        pass
    
    def getMaskOutlineNumFeathers(self, mask_outlineH: MaskOutlineValPtr) -> int:
        pass
    
    def getMaskOutlineFeatherInfo(self, mask_outlineH: MaskOutlineValPtr, which_featherL: int) -> MaskFeather:
        pass
    
    def setMaskOutlineFeatherInfo(self, mask_outlineH: MaskOutlineValPtr, which_featherL: int, featherP: MaskFeather):
        pass
    
    def createMaskOutlineFeather(self, mask_outlineH: MaskOutlineValPtr, featherP0: MaskFeather) -> int:
        pass
    
    def deleteMaskOutlineFeather(self, mask_outlineH: MaskOutlineValPtr, index: int):
        pass
    
class FootageSuite:
    def __init__(self):
        pass
    
    def getMainFootageFromItem(self, itemH: ItemPtr) -> FootagePtr:
        pass
    
    def getProxyFootageFromItem(self, itemH: ItemPtr) -> FootagePtr:
        pass
    
    def getFootageNumFiles(self, footageH: FootagePtr) -> Tuple[int, int]:
        pass
    
    def getFootagePath(self, footageH: FootagePtr, frame_numL: int, file_indexL: int) -> str:
        pass
    
    def getFootageSignature(self, footageH: FootagePtr) -> FootageSignature:
        pass
    
    def newFootage(self, pathZ: str, layer_infoP0: FootageLayerKey, sequence_optionsP0: FileSequenceImportOptions, interp_style: InterpretationStyle) -> FootagePtr:
        pass
    
    def addFootageToProject(self, footageH: FootagePtr, folderH: ItemPtr) -> ItemPtr:
        pass
    
    def setItemProxyFootage(self, footageH: FootagePtr, itemH: ItemPtr):
        pass
    
    def replaceItemMainFootage(self, footageH: FootagePtr, itemH: ItemPtr):
        pass
    
   # def getFootageInterpretation(self, itemH: ItemPtr, proxyB: bool) -> FootageInterp:
    #    pass
    
    #def setFootageInterpretation(self, itemH: ItemPtr, proxyB: bool, interpP: AEGP_FootageInterp):
    #    pass
    
    def getFootageLayerKey(self, footageH: FootagePtr) -> FootageLayerKey:
        pass
    
    def newPlaceholderFootage(self, nameZ: str, width: int, height: int, durationPT: Time) -> FootagePtr:
        pass
    
   # def newPlaceholderFootageWithPath(self, pathZ: str, path_platform: Platform, file_type: AEIO_FileType, widthL: int, heightL: int, durationPT: Time) -> FootagePtr:
    #    pass
    
    def newSolidFootage(self, nameZ: str, width: int, height: int, colorP: ColorVal) -> FootagePtr:
        pass
    
    def getSolidFootageColor(self, itemH: ItemPtr, proxyB: bool) -> ColorVal:
        pass
    
    def setSolidFootageColor(self, itemH: ItemPtr, proxyB: bool, colorP: ColorVal) -> None:
        pass
    
    def setSolidFootageDimensions(self, itemH: ItemPtr, proxyB: bool, widthL: int, heightL: int) -> None:
        pass
    
    def getFootageSoundDataFormat(self, footageH: FootagePtr) -> SoundDataFormat:
        pass
    
    def getFootageSequenceImportOptions(self, footageH: FootagePtr) -> FileSequenceImportOptions:
        pass

class UtilitySuite:
    def __init__(self):
        pass
    
    def reportInfo(self, info_string: str):
        pass
    
    def reportInfoUnicode(self, info_string: str):
        pass
    
    def getDriverPluginInitFuncVersion(self) -> Tuple[int, int]:
        pass
    
    def getDriverImplementationVersion(self) -> Tuple[int, int]:
        pass
    
    def startQuietErrors(self):
        pass
    
    def endQuietErrors(self, report_quieted_errorsB: bool):
        pass
    
    def getLastErrorMessage(self, buffer_size: int) -> str:
        pass
    
    def startUndoGroup(self, undo_name: str):
        pass
    
    def endUndoGroup(self):
        pass
    
    def getMainHWND(self):
        pass
    
    def showHideAllFloaters(self, include_tool_palB: bool):
        pass
    
    def getPaintPalForeColor(self) -> ColorVal:
        pass
    
    def getPaintPalBackColor(self) -> ColorVal:
        pass
    
    def setPaintPalForeColor(self, fore_color: ColorVal):
        pass
    
    def setPaintPalBackColor(self, back_color: ColorVal):
        pass
    
    def getCharPalFillColor(self) -> Tuple[bool, ColorVal]:
        pass
    
    def getCharPalStrokeColor(self) -> Tuple[bool, ColorVal]:
        pass
    
    def setCharPalFillColor(self, fill_color: ColorVal):
        pass
    
    def setCharPalStrokeColor(self, stroke_color: ColorVal):
        pass
    
    def charPalIsFillColorUIFrontmost(self) -> bool:
        pass
    
    def convertFpLongToHSFRatio(self, numberF: float) -> Ratio:
        pass
    
    def convertHSFRatioToFpLong(self, ratioR: Ratio) -> float:
        pass
    
    def causeIdleRoutinesToBeCalled(self):
        pass
    
    def getSuppressInteractiveUI(self) -> bool:
        pass
    
    def writeToOSConsole(self, text: str):
        pass
    
    def writeToDebugLog(self, subsystem: str, eventType: str, text: str):
        pass
    
    def getPluginPath(self, path_type: PluginPathType) -> str:
        pass
    
class RenderQueueSuite:
    def __init__(self):
        pass
    
    def addCompToRenderQueue(self, comp: CompPtr, path: str):
        pass
    
    def setRenderQueueState(self, state: RenderQueueState):
        pass
    
    def getRenderQueueState(self) -> RenderQueueState:
        pass
    
class RenderQueueItemSuite:
    def __init__(self):
        pass
    
    def getNumRQItems(self) -> int:
        pass
    
    def getRQItemByIndex(self, rq_item_index: int) -> RQItemRefPtr:
        pass
    
    def getNextRQItem(self, current_rq_item: RQItemRefPtr) -> RQItemRefPtr:
        pass
    
    def getNumOutputModulesForRQItem(self, rq_item: RQItemRefPtr) -> int:
        pass
    
    def getRenderState(self, rq_item: RQItemRefPtr) -> RenderItemStatus:
        pass
    
    def setRenderState(self, rq_item: RQItemRefPtr, status: RenderItemStatus):
        pass
    
    def getStartedTime(self, rq_item: RQItemRefPtr) -> Time:
        pass
    
    def getElapsedTime(self, rq_item: RQItemRefPtr) -> Time:
        pass
    
    def getLogType(self, rq_item: RQItemRefPtr) -> LogType:
        pass
    
    def setLogType(self, rq_item: RQItemRefPtr, logtype: LogType):
        pass
    
    def removeOutputModule(self, rq_item: RQItemRefPtr, outmod: OutputModuleRefPtr):
        pass
    
    def getComment(self, rq_item: RQItemRefPtr) -> str:
        pass
    
    def setComment(self, rq_item: RQItemRefPtr, comment: str):
        pass
    
    def getCompFromRQItem(self, rq_item: RQItemRefPtr) -> CompPtr:
        pass
    
    def deleteRQItem(self, rq_item: RQItemRefPtr):
        pass
    
class OutputModuleSuite:
    def __init__(self):
        pass
    
    def getOutputModuleByIndex(self, rq_itemH: RQItemRefPtr, outmod_indexL: int) -> OutputModuleRefPtr:
        pass
    
    def getEmbedOptions(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr) -> EmbeddingType:
        pass
    
    def setEmbedOptions(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr, embed_options: EmbeddingType):
        pass
    
    def getPostRenderAction(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr) -> PostRenderAction:
        pass
    
    def setPostRenderAction(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr, post_render_action: PostRenderAction):
        pass
    
    def getEnabledOutputs(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr) -> OutputTypes:
        pass
    
    def setEnabledOutputs(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr, enabled_types: OutputTypes):
        pass
    
    def getOutputChannels(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr) -> VideoChannels:
        pass
    
    def setOutputChannels(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr, output_channels: VideoChannels):
        pass
    
    def getStretchInfo(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr) -> Tuple[bool, StretchQuality, bool]:
        pass
    
    def setStretchInfo(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr, is_enabledB: bool, stretch_quality: StretchQuality):
        pass
    
    def getCropInfo(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr) -> Tuple[bool, LRect]:
        pass
    
    def setCropInfo(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr, enableB: bool, crop_rect: LRect):
        pass
    
    def getSoundFormatInfo(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr) -> Tuple[SoundDataFormat, bool]:
        pass
    
    def setSoundFormatInfo(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr, sound_format_info: SoundDataFormat, audio_enabledB: bool):
        pass
    
    def getOutputFilePath(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr) -> str:
        pass
    
    def setOutputFilePath(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr, path: str):
        pass
    
    def addDefaultOutputModule(self, rq_itemH: RQItemRefPtr) -> OutputModuleRefPtr:
        pass
    
    def getExtraOutputModuleInfo(self, rq_itemH: RQItemRefPtr, outmodH: OutputModuleRefPtr) -> Tuple[str, str, bool, bool]:
        pass
    
    