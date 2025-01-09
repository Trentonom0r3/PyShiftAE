# plugin_installer.py

import os
import sys
import shutil

class PluginInstaller:
    """
    A class to handle the installation of .aex plugins, using only os/shutil
    to locate and copy the plugin file. It does not import other modules, so
    we avoid the 'PyFx' issue entirely.
    """

    def __init__(self, plugin_name: str, install_dir: str):
        """
        Initialize the PluginInstaller instance.

        :param plugin_name: Name of the .aex plugin file.
        :param install_dir: Target directory where the plugin will be installed.
        """
        self.plugin_name = plugin_name
        self.install_dir = install_dir

    def get_plugin_path(self) -> str:
        """
        Get the absolute path to the .aex plugin, assuming there's a "plugins" folder
        in the same directory as this file (i.e., pyshiftae/plugins).

        :return: Absolute path to the plugin.
        :raises FileNotFoundError: If the plugin path does not exist.
        """
        current_file_dir = os.path.dirname(os.path.abspath(__file__))
        #plugin_dir = os.path.join(current_file_dir, "pyshiftae",      #python version #"plugin")  # i.e. pyshiftae/plugins
        plugin_dir = os.path.join(current_file_dir, "pyshiftae", "plugin")  # i.e. pyshiftae/plugins
        python_version = f"python{sys.version_info.major}.{sys.version_info.minor}"
        plugin_dir = os.path.join(plugin_dir, python_version)
        plugin_path = os.path.join(plugin_dir, self.plugin_name)

        if not os.path.isfile(plugin_path):
            raise FileNotFoundError(f"Could not find plugin '{self.plugin_name}' at: {plugin_path}")
        return plugin_path

    def install_plugin(self):
        """
        Install the .aex plugin to the target directory.
        """
        plugin_path = self.get_plugin_path()
        os.makedirs(self.install_dir, exist_ok=True)  # Ensure target directory exists

        try:
            shutil.copy(plugin_path, self.install_dir)
            print(f"Plugin '{self.plugin_name}' installed successfully to {self.install_dir}")
        except Exception as e:
            raise RuntimeError(f"Failed to install plugin '{self.plugin_name}': {e}")

def main():
    """
    CLI for installing .aex plugins, intentionally avoiding importing the main library.
    """
    print("PyShiftAE Plugin Installer\n")
    plugin_name = "PyShiftAE.aex".strip()
    install_dir = input("Enter the target installation directory (e.g., Adobe After Effects Plugins folder): ").strip()
    #commonly found at C:\Program Files\Adobe\Adobe After Effects {VERSION}\Support Files\Plug-ins\Effects
#TODO Make prettier with better info for the user.
    installer = PluginInstaller(plugin_name, install_dir)

    try:
        installer.install_plugin()
    except Exception as e:
        print(f"Error: {e}")

    # Section for extra information or actions
    print("\nAdditional Info:")
    print("- Ensure After Effects is restarted to load the new plugin.")
    print("- Contact support if the plugin does not appear.")

if __name__ == "__main__":
    main()
