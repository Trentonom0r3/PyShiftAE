"""
build_wheels.py

Script to build version-specific wheels for multiple Python versions.
We call 'py -X.Y setup.py bdist_wheel' with an explicit version.
"""

import os
import subprocess
import shutil

# Define the list of Python versions and their explicit package versions
VERSION = "0.1.1"
build_targets = {
    "3.11": VERSION,
    "3.12": VERSION,
    "3.13": VERSION,
}

# Navigate to the directory containing setup.py
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
os.chdir(SCRIPT_DIR)

# Optionally clean the dist/ directory first
dist_dir = os.path.join(SCRIPT_DIR, "dist")
if os.path.exists(dist_dir):
    print(f"Removing existing dist/ directory at: {dist_dir}")
    shutil.rmtree(dist_dir)

for py_version, package_version in build_targets.items():
    print(f"--- Building for Python {py_version} with version {package_version} ---")
    # We’ll keep output in the same dist/ folder so that we end up with multiple wheels
    # If you want them separated, you could do another mechanism, e.g., rename after each build, etc.
    
    cmd = [
        "py",
        f"-{py_version}",
        "setup.py",
        "sdist",        # Source distribution
        "bdist_wheel",  # Build a wheel
        f"--version={package_version}"  # Pass our explicit version
    ]

    try:
        result = subprocess.run(cmd, check=True)
        print(f"Successfully built for Python {py_version}\n")
    except FileNotFoundError:
        print(f"Error: Python version {py_version} not found on this system.")
    except subprocess.CalledProcessError as e:
        print(f"Build failed for Python {py_version}: {e}")
        exit(1)

print("All builds completed successfully.")

# At the end, the dist/ folder should contain wheels named like:
#   pyshiftae-0.1.1-cp311-...whl
#   pyshiftae-0.1.2-cp312-...whl
#   pyshiftae-0.1.3-cp313-...whl
#
# plus any .tar.gz source distributions
