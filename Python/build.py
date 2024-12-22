import os
import subprocess

# Navigate to the directory containing setup.py
os.chdir(os.path.dirname(__file__))

# Run the build command
subprocess.run(["python", "setup.py", "sdist", "bdist_wheel"])
