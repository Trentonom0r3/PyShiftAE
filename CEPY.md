# CEPy Extensions - Pure Python Extensions for After Effects!
--------------------
## Summary:
<p>CEPy extensions are PyShiftAE's take on your standard CEP extensions. CEPy extensions provide a means to write extensions for After Effects in pure python-- no Javascript or Extendscript whatsoever!<p>

### Why Write in Python?:
<p>When I first started scripting After Effects, I had a hard time figuring out extendscript and Javascript-- I still do. Python is not only one of the fastest growing languages, but one of the most versatile and easy to learn. As an artist with an engineering background, I found python natural, and feel like others in my situation could greatly benefit from the ability to script using python!<p>

--------------------
### Requirements and Structure:
------------
CEPy extensions require the following folder structure:

```
com.psc.EXTENSION_NAME/
├── manifest.py -- REQUIRED
├── entry.py -- REQUIRED
├── utils.py -- RECOMMENDED
└── extras.ext -- OPTIONAL extras
```


For convenience, `PyShiftAE` provides a function, `create_template`, which can be used to generate the basic structure for creating extensions:

```py
def create_template(name = None, version = None, author = None, ##works fine
                   description = None, dependencies = None, out_path=None):
    """
    args (optional)
    ==============
    name: str -> Name of the Extension
    version: str -> Extension Version
    author: str -> Extension Author
    description: str -> Extension description
    dependencies: list[str] -> Extension Dependencies
    out_path: str -> Output path for Extension Template.
    """
    return None
```
------------
### Installation:
-------------
`PyShiftAE` provides another function, `install_extension`, which can be used to install extensions in the proper location.

```py
def install_extension(extension_path):
    """
    Installs the extension at the required path.
    """
    return None
```

Users may also manually install extensions by placing them in the `CEP` directory.

To do so, place the entire folder in this directory.<p> `com.psc.EXTENSION_NAME` **->** `C:\Program Files (x86)\Common Files\Adobe\CEP\extensions\`

## Starting out:
<p> Assuming you have the proper directory structure set up, you are ready to begin writing CEPy extensions! If you follow these steps, you should have no problem etting started!<p>

# Diving in:
### Summary:
The Manifest class is used to connect PyShiftAE Extensions with CEP and After Effects. If a field does not have a value you wish to place, simply forego altering it.

### Class: **Manifest**
---------------------
`manifest = Manifest()`
- **Attributes:**   
    - `name` **->** `str`: 
        - `manifest.name = "EXTENSION"`: The Name of your Manifest. Should Match the comp.psc.EXTENSIONNAME folder's name.

    - `version` **->** `str` : 
        - `manifest.version = "1.0.0"`: The version of your extension.

    - `author` **->** `str` : 
        - `manifest.author = "ME"`: The author of the extension.

    - `description` **->** `str` :
        - `manifest.description = ""`: A brief description of what your extension's purpose is.

    - `entry` **->** `str` : 
        - `manifest.entry = os.path.dirname(os.path.realpath(__file__)) + "\entry.py"`: The full path to the main entry file for your extension. Must be named entry.py.

    - `dependencies` **->** `list[str]`:
        - `manifest.dependencies = ["PyShiftAE", "Numpy", "Pandas"]` A list of pip-installable names for your extension's dependencies.

Next, write your `entry.py` file.<p>
For this, you can use any python GUI framework, just ensure your script will spawn the window when the script is ran by using an `if __name__ = "__main__"` block.

Your `entry.py` file will set up your GUI, execute it, and implement callbacks for buttons and such.

While it is possible to write your code to script AE within `entry.py`, it is recommended to do so in an external `utils.py` (or similarly named) file for separation of concerns, ease of use, and debugging purposes. 

You can simply import the entire `PyShiftAE` library, and have access to all After Effects related functionality. 

For more information and example extensions, you can look [here]().