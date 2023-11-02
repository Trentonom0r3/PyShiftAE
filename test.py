import PyShiftCore
import traceback
try:
    import numpy
except:
    traceback.print_exc()

print("Imported Numpy!")

name = PyShiftCore.app.project.name
version = PyShiftCore.app.version
path = PyShiftCore.app.project.path

# Do something with the name, version, and path
print("Project name:", name)
print("Version:", version)
print("Project path:", path)
