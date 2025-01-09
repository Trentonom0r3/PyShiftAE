"""
setup.py for pyshiftae

Generates a Python-wheel that is version-specific (e.g., cp311, cp312)
by using a custom bdist_wheel command. We also use --version=... if supplied on the CLI.
"""

import sys
import os
import setuptools
from setuptools import setup, find_packages
from wheel.bdist_wheel import bdist_wheel as _bdist_wheel

# Detect the current Python version
current_version = f"python{sys.version_info.major}.{sys.version_info.minor}"

# Path to your .aex plugin for the current Python version
plugin_path = f"plugin/{current_version}/PyShiftAE.aex"
print(os.path.abspath(plugin_path))
# Default version
DEFAULT_VERSION = "0.1.0"

# Extract any --version=... from the command line
explicit_version = None
for arg in sys.argv:
    if arg.startswith("--version="):
        explicit_version = arg.split("=")[1]
# If found, remove it from sys.argv so it doesn't confuse setuptools
if explicit_version:
    sys.argv = [arg for arg in sys.argv if not arg.startswith("--version=")]

class bdist_wheel_versioned(_bdist_wheel):
    """
    Custom bdist_wheel command to:
      1) Mark the wheel as non-pure (root_is_pure = False).
      2) Set the Python tag to cp<major><minor> if you’re on CPython
         (e.g., cp311 for Python 3.11).
    """
    def finalize_options(self):
        super().finalize_options()
        self.root_is_pure = False  # Mark this as not a pure-Python package

        # If CPython, set python_tag = cp<major><minor>.
        # e.g. for 3.11 => cp311
        if hasattr(sys, "implementation") and sys.implementation.name == "cpython":
            major = sys.version_info.major
            minor = sys.version_info.minor
            self.python_tag = f"cp{major}{minor}"
        else:
            # Fallback if not CPython
            self.python_tag = self.python_tag

setup(
    name="pyshiftae",
    version=explicit_version or DEFAULT_VERSION,
    description="A Python library for After Effects automation",
    author="Trenton Flanagan",
    author_email="spigonvids@gmail.com",
    url="https://github.com/Trentonom0r3/PyShiftAE",

    # 1) Find all subpackages in "pyshiftae", etc.
    packages=find_packages(where="."),
    # 2) Also treat these top-level .py files as modules to be installed
    py_modules=["plugin_installer", "file_mover"],

    package_dir={"": "."},
    include_package_data=True,
    package_data={
        # Make sure the plugin is included in the wheel
        "pyshiftae": [plugin_path],
    },

    # 3) The console script now references plugin_installer at top-level
    entry_points={
        "console_scripts": [
            "psc-install=plugin_installer:main",
        ],
    },

    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires=">=3.11, <3.14",

    # Use our custom bdist_wheel
    cmdclass={"bdist_wheel": bdist_wheel_versioned},

    zip_safe=False,
)
