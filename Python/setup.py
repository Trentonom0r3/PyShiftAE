from setuptools import setup, find_packages

setup(
    name="pyshiftae",  # Name of your package
    version="0.1.0",   # Initial version
    description="A Python library for After Effects automation",
    author="Trenton Flanagan",
    author_email="spigonvids@gmail.com",
    url="https://github.com/Trentonom0r3/PyShiftAE",  # Replace with your project's URL
    packages=find_packages(where="."),  # Automatically find sub-packages
    package_dir={"": "."},  # Root directory for the package
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires=">=3",  # Python version compatibility
)

