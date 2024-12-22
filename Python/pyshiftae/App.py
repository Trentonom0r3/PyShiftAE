import operator
import PyFx # type: ignore
import os 
from typing import Any, Iterator, List, Tuple, Union

class App:
    _suite = PyFx.UtilitySuite()
    def __init__(self) -> None:
        pass
    
    def report_info(self, message: str) -> None:
        self._suite.reportInfo(message)
        