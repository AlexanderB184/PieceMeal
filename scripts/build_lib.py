from sys import platform
from os import path, listdir, system

if platform == "linux" or platform == "linux2":
    COMPILER = "gcc"
    DEFAULT_FLAGS = ["-Wall", "-Werror", "-O3"]
    SHARED_LIBRARY_FLAGS = ["-fPIC", "-shared"]
    SHARED_LIBRARY_EXTENSION = ".so"
    NAME_FLAG = "-o"
elif platform == "darwin":
    print("MacOS not supported")
    exit(1)
elif platform == "win32":
    COMPILER = "cl"
    DEFAULT_FLAGS = ["/Wall", "/WX", "/O2"]
    SHARED_LIBRARY_FLAGS = ["/LD"]
    SHARED_LIBRARY_EXTENSION = ".dll"
    NAME_FLAG = "/Fe"


LIB = path.join("build", "libchess")

def list_c_files(dir: str) -> 'list[str]':
    return [path.join(dir, file) for file in listdir(dir) if file[-2:] == ".c"]

def compile_library():
    files = list_c_files("src")
    target = LIB + SHARED_LIBRARY_EXTENSION
    cmd = " ".join([COMPILER, *files, *DEFAULT_FLAGS, *SHARED_LIBRARY_FLAGS, NAME_FLAG, target])
    print(f"Executing: {cmd}")
    result = system(cmd)
    if result != 0:
        print("Failed to compile library.")
        exit(1)
    print(f"Library compiled successfully to {target}")

if __name__ == "__main__":
    compile_library()