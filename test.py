from PyShiftCore import *
import traceback
try:
    import numpy
except:
    traceback.print_exc()

print("Imported Numpy!")

app.beginUndoGroup();
name = app.project.name
version = app.version
path = app.project.path
app.executeCommand(2080);
print("executing command");
app.endUndoGroup();

# Do something with the name, version, and path
print("Project name:", name)
print("Version:", version)
print("Project path:", path)
