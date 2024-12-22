
import sys

import random
import string
import pyshiftae as ae  

# Utility function to generate random strings
def random_string(length=10):
    return ''.join(random.choice(string.ascii_letters) for _ in range(length))

def stress_test_layers_and_effects():
    """
    This stress test focuses on:
    - Creating a large number of layers.
    - Applying random effects to those layers.
    - Inserting keyframes for various layer properties.
    """
    effect_names = [
        "ADBE Gaussian Blur 2",     # Gaussian Blur
        "ADBE Camera Lens Blur",    # Camera Lens Blur
        "ADBE Motion Blur",         # Directional Blur
        "ADBE Depth Matte",         # Depth Matte
        "ADBE Radial Blur",         # Radial Blur
        "ADBE Sharpen",             # Sharpen
        "ADBE Brightness & Contrast 2",  # Brightness & Contrast
        "ADBE CurvesCustom",        # Curves
        "ADBE Vibrance",            # Vibrance
        "ADBE Tint",                # Tint
        "ADBE Change Color",        # Change Color
        "ADBE Exposure2",           # Exposure
        "ADBE Unsharp Mask2",       # Unsharp Mask
        "ADBE Displacement Map",    # Displacement Map
        "ADBE Twirl",               # Twirl
        "ADBE Turbulent Displace",  # Turbulent Displace
        "ADBE Liquify",             # Liquify
        "ADBE Polar Coordinates",   # Polar Coordinates
        "ADBE Lens Flare",          # Lens Flare
        "ADBE Cartoonify",          # Cartoon
        "ADBE Wave Warp",           # Wave Warp
        "ADBE Glow",                # Glow
        "ADBE Echo",                # Echo
        "ADBE Gradient Wipe",       # Gradient Wipe
        "ADBE Venetian Blinds",     # Venetian Blinds
        "ADBE Timewarp",            # Timewarp
    ]
    
    # Assume we are working with the active composition
    comp = ae.Layer.active_layer().parent_comp  # Get the parent composition of the active layer
 #   ae.App().report_info(f"Active Composition: {comp.name}")
    # Stress test by adding random layers and applying effects
    num_layers = 5  # Large number of layers to test performance
    for i in range(num_layers):
        layer_name = random_string()
        
        # Add solid layers with random colors
        layer = comp.layers.add_solid(
            name=layer_name,
            color=(random.random(), random.random(), random.random(), 1),  # Random color
            width=1920,
            height=1080,
            duration=10  # 10 seconds duration
        )
   #     ae.App().report_info(f"Added Layer {layer_name}")

        # Apply random effects to the layer
        num_effects = random.randint(5, 10)  # Applying 5-10 effects per layer
        for j in range(num_effects):
            effect_name = random.choice(effect_names)
            effect = ae.Effect.apply(layer, effect_name)
           # ae.App().report_info(f"Applied Effect {effect_name} to Layer {layer_name}")
#
            # Set random effect parameter values (assuming the effect has such parameters)
           # for param_index in range(ae.StreamSuite().GetEffectNumParamStreams(effect.effect)):
              #  ae.App().report_info(f"Setting Parameter {param_index} for Effect {effect_name} on Layer {layer_name}")

def stress_test_keyframes():
    """
    This stress test focuses on inserting keyframes for different properties.
    """
    # Assume the composition is already loaded and we have access to layers
    comp = ae.Layer.active_layer().parent_comp

    # Fetch all layers in the composition
    for layer in comp.layers:
        # Insert random keyframes for the layer's position
        num_keyframes = 100  # Large number of keyframes to stress test
        for keyframe_idx in range(num_keyframes):
            time = random.uniform(0, 10)  # Random time for keyframe within 10 seconds
            position = (random.uniform(-500, 500), random.uniform(-500, 500), random.uniform(-500, 500))  # Random position
            layer.position.set_value(position)
           # ae.App().report_info(f"Added Keyframe {keyframe_idx} for Layer {layer.name} at Time {time} with Position {position}")

        # Similarly, add keyframes for other properties like opacity, scale, etc.
        for keyframe_idx in range(num_keyframes):
            time = random.uniform(0, 10)  # Random time for keyframe within 10 seconds
            opacity = random.uniform(0, 100)  # Random opacity value
            layer.opacity.set_value(opacity)
          #  ae.App().report_info(f"Added Keyframe {keyframe_idx} for Layer {layer.name} at Time {time} with Opacity {opacity}")

if __name__ == "__main__":
    import traceback
    try:
       
        stress_test_keyframes()
    except Exception as e:
        ae.App().report_info(f"Error: {e}")
        ae.App().report_info(traceback.format_exc())