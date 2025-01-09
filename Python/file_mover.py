import os
import shutil
import stat

class FileMover:
    """
    A class to handle moving files to another location with optional permission handling.
    """

    def __init__(self, source_path: str, destination_path: str):
        """
        Initialize the FileMover instance.

        :param source_path: Path to the source file to be moved.
        :param destination_path: Target location where the file will be moved.
        """
        self.source_path = source_path
        self.destination_path = destination_path

    def check_source_exists(self) -> bool:
        """
        Check if the source file exists.

        :return: True if the source file exists, False otherwise.
        """
        return os.path.exists(self.source_path)

    def ensure_permissions(self):
        """
        Ensure that the file permissions are set to allow moving the file.
        """
        try:
            # Grant write and execute permissions to the file if not already set
            os.chmod(self.source_path, stat.S_IWRITE | stat.S_IREAD | stat.S_IEXEC)
        except PermissionError as e:
            raise PermissionError(f"Unable to set permissions on {self.source_path}: {e}")

    def move_file(self):
        """
        Move the source file to the destination location.

        :raises FileNotFoundError: If the source file does not exist.
        :raises PermissionError: If the file cannot be moved due to permission issues.
        """
        if not self.check_source_exists():
            raise FileNotFoundError(f"Source file does not exist: {self.source_path}")

        try:
            self.ensure_permissions()
            # Create destination directory if it does not exist
            os.makedirs(os.path.dirname(self.destination_path), exist_ok=True)
            # Move the file
            shutil.move(self.source_path, self.destination_path)
            print(f"File moved successfully from {self.source_path} to {self.destination_path}")
        except PermissionError as e:
            raise PermissionError(f"Failed to move file due to permission error: {e}")
        except Exception as e:
            raise RuntimeError(f"Failed to move file: {e}")

# Example Usage
if __name__ == "__main__":
    source = "path/to/source/file.txt"
    destination = "path/to/destination/file.txt"

    mover = FileMover(source, destination)
    try:
        mover.move_file()
    except Exception as e:
        print(f"Error: {e}")
