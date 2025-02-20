import os
import re

LICENSE = """// MIT License
// 
// Copyright (c) 2025 Alexander John Brown
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE."""

INCLUDE_PATTERN = re.compile(r'^#include\s+"([^"]+)"$')

def list_c_files(dir: str) -> 'list[str]':
    return [os.path.join(dir, file) for file in os.listdir(dir) if file[-2:] == ".c"]

def list_h_files(dir: str) -> 'list[str]':
    return [os.path.join(dir, file) for file in os.listdir(dir) if file[-2:] == ".h"]

def expand_file(file, included_files):
    if file in included_files:
        return ""

    included_files.add(file)

    with open(file, "r") as f:
        lines = f.readlines()
        # Process includes in the included file
        expanded_code = f"""// Expanded from {file}
"""
        for line in lines:
            match = INCLUDE_PATTERN.match(line.strip())
            if match:
                expanded_code += expand_file(os.path.normpath(os.path.join(os.path.dirname(file),match.group(1))), included_files)
            else:
                expanded_code += line

        return expanded_code + "\n"

def compile_library():
    included_files = set()
    src_dirs = ["src/chess"]
    files = []
    for dir in src_dirs:
        files += list_c_files(dir)

    codeBlob = f"""{LICENSE}
//
// File Generated By Build Script
//

"""

    for file in files:
        codeBlob += expand_file(file, included_files)
    with open("PieceMealLib.c", "w") as out:
        out.write(codeBlob)




if __name__ == "__main__":
    compile_library()