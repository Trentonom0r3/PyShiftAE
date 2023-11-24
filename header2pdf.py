import os

def find_source_files(root_dir, extensions=('.h', '.cpp')):
    """
    Traverse through the directory to find files with the given extensions.

    Args:
    root_dir (str): The root directory to start searching from.
    extensions (tuple): File extensions to look for.

    Returns:
    list: A list of paths to files with the specified extensions.
    """
    files_list = []
    for subdir, dirs, files in os.walk(root_dir):
        for file in files:
            if file.endswith(extensions):
                files_list.append(os.path.join(subdir, file))
    return files_list

def compile_files_to_md(files_list, output_md_path):
    """
    Compiles the content of the given files into a single Markdown file.

    Args:
    files_list (list): List of file paths to compile.
    output_md_path (str): Path to the output Markdown file.
    """
    with open(output_md_path, 'w', encoding='utf-8') as md_file:
        for file_path in files_list:
            filename = os.path.basename(file_path)  # Extract filename
            md_file.write(f"<h2>Begin File: {filename}</h2>\n\n```cpp\n")

            with open(file_path, 'r', encoding='utf-8') as file:
                md_file.write(file.read())

            md_file.write("\n```\n\n---\n\n")

# Example usage
root_directory = "C:\\Users\\tjerf\\source\\repos\\PyShiftAE\\Util"  # Replace with your directory path
output_md = 'C:\\Users\\tjerf\\source\\repos\\PyShiftAE\\AE_SDK_HEADERS02.md'

source_files = find_source_files(root_directory)
compile_files_to_md(source_files, output_md)


