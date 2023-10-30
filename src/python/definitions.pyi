from enum import Enum

class _AppVersion:
    CS3: float = 8.0
    CS4: float = 9.0
    CS5: float = 10.0
    CS5_5: float = 10.5
    CS6: float = 11.0
    CC: float = 12.0
    CC2014: float = 13.0
    CC2015: float = 13.5
    CC2015_1: float = 13.6
    CC2015_2: float = 13.7
    CC2015_3: float = 13.8
    CC2017: float = 14.0
    CC2018: float = 15.0
    CC2019: float = 16.0
    CC2020: float = 17.0

class commandID(Enum):
#FILE
  NewProject = 2
  NewFolder = 2139
  NewAdobePhotoshopFile = 3147
  NewMAXONCINEMA4DFile = 4007
  OpenProject = 3
  OpenRecentProject1 = 2330
  OpenRecentProject2 = 2331
  OpenRecentProject3 = 2332
  OpenRecentProject4 = 2333
  OpenRecentProject5 = 2334
  BrowseInBridge = 3689

  Close = 4
  CloseProject = 3154
  Save = 5
  SaveAs = 6
  SaveACopy = 2166
  SaveACopyAsXML = 3785
  IncrementAndSave = 3088
  Revert = 7

  ImportFile = 2003
  ImportMultipleFiles = 2236
  ImportPlaceholder = 2126
  ImportSolid = 3000
  ImportRecentFootage1 = 2310
  ImportRecentFootage2 = 2311
  ImportRecentFootage3 = 2312
  ImportRecentFootage4 = 2313
  ImportRecentFootage5 = 2314
  ImportRecentFootage6 = 2315
  ImportRecentFootage7 = 2316
  ImportRecentFootage8 = 2317
  ImportRecentFootage9 = 2318
  ImportRecentFootage10 = 2319
  ExportAddToAdobeMediaEncoderQueue = 3800
  ExportAddToRenderQueue = 2161

  AddFootageToComp = 2005
  NewCompFromSelection = 2796

  CollectFiles = 2482
  ConsolidateAllFootage = 2107
  RemoveUnusedFootage = 2109
  ReduceProject = 2735
  FindMissingEffects = 4002
  FindMissingFonts = 4003
  FindMissingFootage = 4004
  WatchFolder = 2457

  RunScriptFile = 8000
  OpenScriptEditor = 8001

  CreateProxyStill = 2778
  CreateProxyMovie = 2779
  SetProxyFile = 2003
  SetProxyNone = 2119
  InterpretFootageMain = 2077
  InterpretFootageProxy = 2103
  InterpretFootageRememberInterpretation = 2254
  InterpretFootageApplyInterpretation = 2255
  ReplaceFootageFile = 2003
  ReplaceFootageWithLayeredComp = 3070
  ReplaceFootagePlaceholder = 2126
  ReplaceFootageSolid = 3000
  ReloadFootage = 2257
  RevealInExplorer = 2562
  RevealInFinder = 2562
  RevealInBridge = 3690

  ProjectSettings = 2611

#EDIT
  Undo = 16
  Redo = 2035
  Redo2 = 17

  Cut = 18
  Copy = 19
  CopyWithPropertyLinks = 10310
  CopyExpressionOnly = 53
  Paste = 20
  Clear = 21

  Duplicate = 2080
  SplitLayer = 2158
  LiftWorkArea = 2613
  ExtractWorkArea = 2614
  SelectAll = 23
  DeselectAll = 2004

  PurgeAllMemoryAndDiskCache = 10200
  PurgeAllMemory = 2373
  PurgeUndo = 2371
  PurgeImageCacheMemory = 2372
  PurgeSnapshot = 2481

  EditOriginal = 2142
  EditInAdobeAudition = 3697

  TemplatesRenderSettings = 2149
  TemplatesOutputModule = 2150
  PasteMochaMask = 5006

#COMPOSITION
  NewComposition = 2000

  CompositionSettings = 2007
  SetPosterTime = 2012
  TrimCompToWorkArea = 2360
  CropCompToRegionOfInterest = 2997
  AddToAdobeMediaEncoderQueue = 3800
  AddToRenderQueue = 2161
  AddOutputModule = 2154

  SaveFrameAs = 2233
  SaveFrameAsPhotoshopLayers = 5001
  PreRender = 2780

  CompositionFlowchart = 2258
  CompositionMiniFlowchart = 3792

#LAYER
  NewText = 2836
  NewSolid = 2038
  NewLight = 2563
  NewCamera = 2564
  NewNullObject = 2767
  NewShapeLayer = 3736
  NewAdjustmentLayer = 2279
  # NewAdobePhotoshopFile = 3147
  #/ NewMAXONCINEMA4DFile = 4007
  LayerSettings = 2021

  OpenLayer = 3784
  OpenLayerSource = 2523
  # RevealInExplorer = 2562
  # RevealInFinder = 2562

  NewMask = 2367
  ResetMask = 2448
  RemoveMask = 2368
  RemoveAllMasks = 2369
  UnlockAllMasks = 2456
  LockOtherMasks = 2455
  HideLockedMasks = 2524

  QualityBest = 2045
  QualityDraft = 2044
  QualityWireframe = 2042
  QualityBilinear = 10207
  QualityBicubic = 10208

  HideOtherVideo = 2054
  ShowAllVideo = 2055
  UnlockAllLayers = 2244

  FlipHorizontal = 3766
  FlipVertical = 3767
  CenterInView = 3819
  CenterAnchorPointInLayerContent = 10312
  FitToComp = 2156
  FitToCompWidth = 2732
  FitToCompHeight = 2733
  EnableTimeRemapping = 2153
  TimeReverseLayer = 2135
  TimeStretch = 2024
  FreezeFrame = 3695
  AddMarker = 2157

  LayerStylesConvertToEditableStyles = 3740
  LayerStylesShowAll = 3743
  LayerStylesRemoveAll = 2072
  LayerStylesDropShadow = 9000
  LayerStylesInnerShadow = 9001
  LayerStylesOuterGlow = 9002
  LayerStylesInnerGlow = 9003
  LayerStylesBevelAndEmboss = 9004
  LayerStylesSatin = 9005
  LayerStylesColorOverlay = 9006
  LayerStylesGradientOverlay = 9007
  LayerStylesStroke = 9008

  GroupShapes = 3741
  UngroupShapes = 3742

  ConvertToEditableText = 3799
  CreateShapesFromText = 3781
  CreateMasksFromText = 2933
  CreateShapesFromVectorLayer = 3973
  CreateStereo3DRig = 3843
  CreateOrbitNull = 3844
  LinkFocusDistanceToPointOfInterest = 3845
  LinkFocusDistanceToLayer = 3847
  SetFocusDistanceToLayer = 3846
  AutoTrace = 3044
  PreCompose = 2071

#ANIMATION
  SaveAnimationPreset = 3075
  ApplyAnimationPreset = 2450
  RecentAnimationPreset1 = 2460
  RecentAnimationPreset2 = 2461
  RecentAnimationPreset3 = 2462
  RecentAnimationPreset4 = 2463
  RecentAnimationPreset5 = 2464
  BrowsePresets = 3691

  ConvertAudioToKeyframes = 5015
  ConvertExpressionToKeyframes = 2639
  RPFCameraImport = 5018
  SequenceLayers = 5003
  TimeReverseKeyframes = 3693

  RemoveAllTextAnimators = 3058

  AddExpression = 2702
  SeparateDimensions = 3764
  TrackCamera = 3983
  TrackInMochaAE = 5007
  WarpStabilizerVFX = 3986
  TrackMotion = 2568
  TrackMask = 10311
  TrackThisProperty = 2643

  RevealPropertiesWithKeyframes = 2387
  RevealPropertiesWithAnimation = 4011
  RevealAllModifiedProperties = 2771

# VIEW
  ZoomIn = 2092
  ZoomOut = 2093

  ResolutionFull = 2048
  ResolutionHalf = 2047
  ResolutionThird = 2081
  ResolutionQuarter = 2046
  ResolutionCustom = 2049

  UseDisplayColorManagement = 3704

  ShowRulers = 2280

  ShowGuides = 2274
  SnapToGuides = 2286
  LockGuides = 2275
  ClearGuides = 2276

  ShowGrid = 2277
  SnapToGrid = 2278

  ShowLayerControls = 2435

#WINDOW
  Align = 5022
  Audio = 2029
  Brushed = 3014
  Character = 3011
  EffectsAndPresets = 3718
  Info = 2028
  MaskInterpolation = 5027
  MediaBrowser = 4013
  Metadata = 3788
  MotionSketch = 5024
  Paint = 3045
  Paragraph = 3012
  Preview = 2031
  Progress = 4005
  Smoother = 5028
  Tools = 2010
  Tracker = 5005
  Wiggler = 5030
  
class AlphaMode(Enum):
    IGNORE = 5413
    PREMULTIPLIED = 5414
    STRAIGHT = 5412
    
class AutoOrientType(Enum):
    ALONG_PATH = 4213
    CAMERA_OR_POINT_OF_INTEREST = 4214
    CHARACTERS_TOWARD_CAMERA = 4215
    NO_AUTO_ORIENT = 4212
    
class BlendingMode(Enum):
    ADD = 5220
    ALPHA_ADD = 5244
    CLASSIC_COLOR_BURN = 5219
    CLASSIC_COLOR_DODGE = 5225
    CLASSIC_DIFFERENCE = 5234
    COLOR = 5238
    COLOR_BURN = 5218
    COLOR_DODGE = 5224
    DANCING_DISSOLVE = 5214
    DARKEN = 5215
    DARKER_COLOR = 5247
    DIFFERENCE = 5233
    DISSOLVE = 5213
    DIVIDE = 5249
    EXCLUSION = 5235
    HARD_LIGHT = 5228
    HARD_MIX = 5232
    HUE = 5236
    LIGHTEN = 5221
    LIGHTER_COLOR = 5246
    LINEAR_BURN = 5217
    LINEAR_DODGE = 5223
    LINEAR_LIGHT = 5229
    LUMINESCENT_PREMUL = 5245
    LUMINOSITY = 5239
    MULTIPLY = 5216
    NORMAL = 5212
    OVERLAY = 5226
    PIN_LIGHT = 5231
    SATURATION = 5237
    SCREEN = 5222
    SILHOUETE_ALPHA = 5242
    SILHOUETTE_LUMA = 5243
    SOFT_LIGHT = 5227
    STENCIL_ALPHA = 5240
    STENCIL_LUMA = 5241
    SUBTRACT = 5248
    VIVID_LIGHT = 5230
    
class ChannelType(Enum):
    CHANNEL_ALPHA = 7816
    CHANNEL_ALPHA_BOUNDARY = 7822
    CHANNEL_ALPHA_OVERLAY = 7821
    CHANNEL_BLUE = 7815
    CHANNEL_BLUE_COLORIZE = 7819
    CHANNEL_GREEN = 7814
    CHANNEL_GREEN_COLORIZE = 7818
    CHANNEL_RED = 7813
    CHANNEL_RED_COLORIZE = 7817
    CHANNEL_RGB = 7812
    CHANNEL_RGB_STRAIGHT = 7820

class CloseOptions(Enum):
    DO_NOT_SAVE_CHANGES = 1212
    PROMPT_TO_SAVE_CHANGES = 1213
    SAVE_CHANGES = 1214

class FastPreviewType(Enum):
    FP_ADAPTIVE_RESOLUTION = 8013
    FP_DRAFT = 8014
    FP_FAST_DRAFT = 8015
    FP_OFF = 8012
    FP_WIREFRAME = 8016

class FieldSeparationType(Enum):
    LOWER_FIELD_FIRST = 5614
    OFF = 5613
    UPPER_FIELD_FIRST = 5612
    
class FootageTimecodeDisplayStartType(Enum):
    FTCS_START_0 = 2213
    FTCS_USE_SOURCE_MEDIA = 2212
    
class FrameBlendingType(Enum):
    FRAME_MIX = 4013
    NO_FRAME_BLEND = 4012
    PIXEL_MOTION = 4014
    
class FramesCountType(Enum):
    FC_START_0 = 2612
    FC_START_1 = 2612
    FC_TIMECODE_CONVERSION = 2614
    
class GetSettingsFormat(Enum):
    NUMBER = 3413
    NUMBER_SETTABLE = 3414
    SPEC = 3412
    STRING = 3415
    STRING_SETTABLE = 3416

class GpuAccelType(Enum):
    CUDA = 1813
    METAL = 1814
    OPENCL = 1812
    SOFTWARE = 1816
    VULKAN = 1815

class ImportAsType(Enum):
    COMP = 3814
    COMP_CROPPED_LAYERS = 3812
    FOOTAGE = 3813
    PROJECT = 3815

class KeyframeInterpolationType(Enum):
    BEZIER = 6613
    HOLD = 6614
    LINEAR = 6612

class Language(Enum):
    CHINESE = 1619
    ENGLISH = 1612
    FRENCH = 1615
    GERMAN = 1614
    ITALIAN = 1616
    JAPANESE = 1613
    KOREAN = 1618
    PORTUGUESE = 1621
    RUSSIAN = 1620
    SPANISH = 1617

class LayerQuality(Enum):
    BEST = 4614
    DRAFT = 4613
    WIREFRAME = 4612

class LayerSamplingQuality(Enum):
    BICUBIC = 4813
    BILINEAR = 4812

class LightType(Enum):
    AMBIENT = 4415
    PARALLEL = 4412
    POINT = 4414
    SPOT = 4413

class LogType(Enum):
    ERRORS_AND_PER_FRAME_INFO = 3214
    ERRORS_AND_SETTINGS = 3213
    ERRORS_ONLY = 3212

class MaskFeatherFalloff(Enum):
    FFO_LINEAR = 7213
    FFO_SMOOTH = 7212

class MaskMode(Enum):
    ADD = 6813
    DARKEN = 6817
    DIFFERENCE = 6818
    INTERSECT = 6815
    LIGHTEN = 6816
    NONE = 6812
    SUBTRACT = 6814

class MaskMotionBlur(Enum):
    OFF = 7014
    ON = 7013
    SAME_AS_LAYER = 7012

class ParagraphJustification(Enum):
    CENTER_JUSTIFY = 7415
    FULL_JUSTIFY_LASTLINE_CENTER = 7418
    FULL_JUSTIFY_LASTLINE_FULL = 7419
    FULL_JUSTIFY_LASTLINE_LEFT = 7416
    FULL_JUSTIFY_LASTLINE_RIGHT = 7417
    LEFT_JUSTIFY = 7413
    MULTIPLE_JUSTIFICATIONS = 7412
    RIGHT_JUSTIFY = 7414

class PostRenderAction(Enum):
    IMPORT = 3613
    IMPORT_AND_REPLACE_USAGE = 3614
    NONE = 3612
    SET_PROXY = 3615

class PREFType(Enum):
    PREF_Type_MACHINE_INDEPENDENT = 8813
    PREF_Type_MACHINE_INDEPENDENT_COMPOSITION = 8816
    PREF_Type_MACHINE_INDEPENDENT_OUTPUT = 8815
    PREF_Type_MACHINE_INDEPENDENT_RENDER = 8814
    PREF_Type_MACHINE_SPECIFIC = 8812
    PREF_Type_MACHINE_SPECIFIC_PAINT = 8818
    PREF_Type_MACHINE_SPECIFIC_TEXT = 8817

class PropertyType(Enum):
    INDEXED_GROUP = 6214
    NAMED_GROUP = 6213
    PROPERTY = 6212

class PropertyValueType(Enum):
    COLOR = 6418
    CUSTOM_VALUE = 6419
    LAYER_INDEX = 6421
    MARKER = 6420
    MASK_INDEX = 6422
    NO_VALUE = 6412
    OneD = 6417
    SHAPE = 6423
    TEXT_DOCUMENT = 6424
    ThreeD = 6414
    ThreeD_SPATIAL = 6413
    TwoD = 6416
    TwoD_SPATIAL = 6415

class PulldownMethod(Enum):
    ADVANCE_24P = 6013
    PULLDOWN_3_2 = 6012
    
class PulldownPhase(Enum):
    OFF = 5813
    SSWWW = 5814
    SWWWS = 5815
    SWWWW_24P_ADVANCE = 5821
    WSSWW = 5812
    WSWWW_24P_ADVANCE = 5820
    WWSSW = 5817
    WWSWW_24P_ADVANCE = 5819
    WWWSS = 5816
    WWWSW_24P_ADVANCE = 5818
    WWWWS_24P_ADVANCE = 5822

class PurgeTarget(Enum):
    ALL_CACHES = 1412
    IMAGE_CACHES = 1415
    SNAPSHOT_CACHES = 1414
    UNDO_CACHES = 1413    

class ResolveType(Enum):
    ACCEPT_THEIRS = 8612
    ACCEPT_THEIRS_AND_COPY = 8613
    ACCEPT_YOURS = 8614
    
class RQItemStatus(Enum):
    DONE = 3019
    ERR_STOPPED = 3018
    NEEDS_OUTPUT = 3013
    QUEUED = 3015
    RENDERING = 3016
    UNQUEUED = 3014
    USER_STOPPED = 3017
    WILL_CONTINUE = 3012
    
class TimeDisplayType(Enum):
    FRAMES = 2013
    TIMECODE = 2012
    
class ToolType(Enum):
    Tool_Arrow = 9012
    Tool_CameraDollyCamera = 9017
    Tool_CameraDollyToCursor = 9048
    Tool_CameraDollyTowardsCursor = 9047
    Tool_CameraMaya = 9014
    Tool_CameraOrbit = 9015
    Tool_CameraOrbitCamera = 9015
    Tool_CameraOrbitCursor = 9044
    Tool_CameraOrbitScene = 9045
    Tool_CameraPanCamera = 9016
    Tool_CameraPanCursor = 9046
    Tool_CameraTrackXY = 9016
    Tool_CameraTrackZ = 9017
    Tool_CloneStamp = 9019
    Tool_Eraser = 9020
    Tool_Feather = 9032
    Tool_Hairbrush = 9042
    Tool_Hand = 9021
    Tool_Magnify = 9022
    Tool_Oval = 9026
    Tool_Paintbrush = 9018
    Tool_PanBehind = 9023
    Tool_Pen = 9031
    Tool_PenConvert = 9035
    Tool_PenMinus = 9034
    Tool_PenPlus = 9033
    Tool_Pin = 9036
    Tool_PinAdvanced = 9043
    Tool_PinBend = 9038
    Tool_PinDepth = 9040
    Tool_PinStarch = 9037
    Tool_Polygon = 9027
    Tool_Quickselect = 9041
    Tool_Rect = 9024
    Tool_Rotate = 9013
    Tool_RoundedRect = 9025
    Tool_Star = 9028
    Tool_TextH = 9029
    Tool_TextV = 9030
    
class TrackMatteType(Enum):
    ALPHA = 5013
    ALPHA_INVERTED = 5014
    LUMA = 5015
    LUMA_INVERTED = 5016
    NO_TRACK_MATTE = 5012
    
class ViewerType(Enum):
    VIEWER_COMPOSITION = 7612
    VIEWER_FOOTAGE = 7614
    VIEWER_LAYER = 7613
    
    