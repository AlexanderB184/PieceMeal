import build_lib
from os import path, system

class Test:
    def __init__(self, name: str, args: 'list[str]'):
        self.name = name
        self.args = args

tests = [
    Test("test_is_check", []),
    Test("test_is_legal", []),
    Test("test_is_pseudo_legal", []),
    Test("test_make_and_unmake", []),
    Test("test_move_gen", []),
    Test("test_notation", []),
    Test("test_save_pos", ["quiet"]),
    Test("test_zobrist", []),
    Test("test_correctness", []),
]

def compile_and_run_test(test: Test):
    source_file = path.join("tests", test.name) + ".c"
    target_file = path.join("build", test.name) + ".bin"

    cmd = " ".join([
        build_lib.COMPILER,
        source_file,
        *build_lib.DEFAULT_FLAGS,
        "-Iinclude",
        "-Lbuild",
        "-Wl,-rpath=build",
        "-lchess",
        build_lib.NAME_FLAG,
        target_file
    ])
    print(f"Executing: {cmd}")
    result = system(cmd)
    if result != 0:
        print(f"Failed to compile {test.name}.")
        exit(1)
    print(f"Compiled {test.name} successfully. Running it now...")
    cmd = " ".join([target_file, *test.args])
    system(cmd)

if __name__ == "__main__":
    for test in tests:
        print(f"running test: {test.name} {' '.join(test.args)}")
        compile_and_run_test(test)