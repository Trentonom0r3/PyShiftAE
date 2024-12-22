
import PyFx # type: ignore
import Collection

class Project:
    _suite = PyFx.ProjSuite()
    def __init__(self, proj : PyFx.ProjectPtr = None) -> None:
        if proj is None: # if no project is passed in, get the first project
            proj = self._suite.GetProjectByIndex(0)  # get the first project
        self.proj = proj  # store the project
        
    @classmethod # class method to create a new project
    def new(cls, name: str, path: str) -> 'Project':
        proj = PyFx.ProjSuite().NewProject(name)
        if path:
            PyFx.ProjSuite().SaveProjectToPath(proj, path)
        return cls(proj)
    
    @classmethod # class method to open a project
    def open(cls, path: str) -> 'Project':
        proj = PyFx.ProjSuite().OpenProjectFromPath(path)
        return cls(proj)
    
    @property # property to get the name of the project
    def path(self) -> str:
        return self._suite.GetProjectPath(self.proj)
    
    @property # property to see if the project has been modified
    def dirty(self) -> bool:
        return self._suite.ProjectIsDirty(self.proj)
    
    @property # property to get the bit depth of the project
    def bit_depth(self) -> PyFx.ProjBitDepth:
        
        return self._suite.GetProjectBitDepth(self.proj)
    
    @bit_depth.setter # property setter to set the bit depth of the project
    def bit_depth(self, value) -> None:
        self._suite.SetProjectBitDepth(self.proj, value)
        
    @property
    def items(self) -> Collection.ItemCollection:
        return Collection.ItemCollection.create(self._suite.GetProjectRootFolder(self.proj))
         
    def save(self, path: str) -> None:
        self._suite.SaveProjectToPath(self.proj, path)
        
    def Import(self, path: str) -> None:
        self._suite.ImportFile(self.proj, path)
        
        

