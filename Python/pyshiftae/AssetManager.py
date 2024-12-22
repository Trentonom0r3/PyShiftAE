import PyFx # type: ignore
import os 

class AssetManager:
    def __init__(self) -> None:
        pass

    def import_asset(self, file_path: str, name: str) -> PyFx.ItemPtr:
        suite = PyFx.FootageSuite()
        item = None
        footage = None
        if os.path.splitext(file_path)[1]: # This is a single file
            footage = suite.newFootage(file_path, PyFx.FootageLayerKey(nameAC=name), None, PyFx.InterpretationStyle.NO_DIALOG_NO_GUESS)
        elif os.path.isdir(file_path): # This is a sequence
            footage = suite.newFootage(file_path, PyFx.FootageLayerKey(nameAC=name), PyFx.FileSequenceImportOptions().sequence(), PyFx.InterpretationStyle.NO_DIALOG_NO_GUESS)
        else:
            raise Exception("Invalid file path")
        if footage:
            item = suite.addFootageToProject(footage, PyFx.ProjSuite().GetProjectRootFolder(PyFx.ProjSuite().GetProjectByIndex(0)))
        return item

    def replace_asset(self, old_asset: PyFx.ItemPtr, new_file_path: str) -> None:
        suite = PyFx.FootageSuite()
        new_footage = self.import_asset(new_file_path, "")
        if new_footage:
            suite.replaceItemMainFootage(suite.getMainFootageFromItem(old_asset), new_footage)
        else:
            raise Exception("Failed to import new footage")
