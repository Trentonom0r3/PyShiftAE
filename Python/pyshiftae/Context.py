import PyFx # type: ignore  

class UndoGroup:
    def __init__(self, name: str) -> None:
        self.name = name
        PyFx.UtilitySuite().startUndoGroup(name)
        
    def __enter__(self) -> 'UndoGroup':
        return self
    
    def __exit__(self, exc_type, exc_value, traceback) -> None:
        PyFx.UtilitySuite().endUndoGroup()
        
class QuietErrors:
    def __init__(self) -> None:
        PyFx.UtilitySuite().startQuietErrors()
        
    def __enter__(self) -> 'QuietErrors':
        return self
    
    def __exit__(self, exc_type, exc_value, traceback) -> None:
        PyFx.UtilitySuite().endQuietErrors()
        
#decorator to start an undo group
def undo_group(name: str):
    def decorator(func):
        def wrapper(*args, **kwargs):
            with UndoGroup(name):
                return func(*args, **kwargs)
        return wrapper
    return decorator

#decorator to start quiet errors
def quiet_errors(func):
    def wrapper(*args, **kwargs):
        with QuietErrors():
            return func(*args, **kwargs)
    return wrapper
