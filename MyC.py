import os, re
from collections import namedtuple
from typing import Generator

# Define named tuples
Signature = namedtuple('Signature', ['returnType', 'name', 'parameters'])
FilePathName = namedtuple('FilePathName', ['path', 'nameWithoutExtension'])

def GenerateHeader(structName: str, methods: str) -> str:
    return f"""// Auto-generate begin. Do not modify!
#ifndef H_Gen_{structName}
#define H_Gen_{structName}

#include "MyC.h.gen"
#include "{structName}.h"

typedef struct
{{
{methods}
}}
_Namespace{structName};

extern _Namespace{structName} const ${structName};

#endif
// Auto-generate end. Do not modify!"""

def GenerateMacroFile() -> str:
    return """// Auto-generate begin. Do not modify!
#ifndef H_MyC
#define H_MyC

#define new(typename) $##typename.Constructor(malloc(sizeof(typename)))
#define new1(typename,a1) $##typename.Constructor(malloc(sizeof(typename)),a1)
#define new2(typename,a1,a2) $##typename.Constructor(malloc(sizeof(typename)),a1,a2)
#define new3(typename,a1,a2,a3) $##typename.Constructor(malloc(sizeof(typename)),a1,a2,a3)
#define delete(typename,self) free($##typename.Destructor(self))

#define	$(x) if (x) return NULL

#endif
// Auto-generate end. Do not modify!"""

def GenerateCFile(structName: str, signatures: list[Signature]) -> str:
    members = []
    for _, methodName, _ in signatures:
        members.append(f"\t.{methodName} = &{methodName}")

    members = ",\n".join(members)

    return f"""// Auto-generate begin. Do not modify!
#include "{structName}.h"

_Namespace{structName} const ${structName} =
{{
{members}
}};
// Auto-generate end. Do not modify!"""

def FindCFiles(directory: str) -> Generator[FilePathName, None, None]:
    """
    Finds all the C files with #include "MyObject.c.gen" line
    that will be used for namespace generation.
    """
    for root, _, files in os.walk(directory):
        for fileName in files:
            if not fileName.endswith(".c"):
                continue

            path = os.path.join(root, fileName)

            file = open(path, 'r')
            lines = file.readlines()
            file.close()

            for line in lines[::-1]:
                if f'#include "{fileName}.gen"' in line:
                    yield (path, fileName[:-2])
                    break

def ParseSignatures(path: str) -> Generator[Signature, None, None]:
    """
    Parses the .c file and extracts all the function signatures
    to be used in the namespace file

    Matches format: `void method(int param1, int param2)` with
        indefinite number of parameters
    """
    with open(path) as file:
        for line in file:
            # match form 'void MethodName(type1 name1, type2 name2)'
            found = re.search(r"(\w+\s*\*?)\s+([A-Z]\w*)\(((?:,?\s*(?:\w+\s*\*?)\s+\w+)*)\)", line)
            if (found):
                yield Signature(found.group(1), found.group(2), found.group(3))

def MakeFunctionPointers(matches: list[Signature]) -> Generator[str, None, None]:
    """
    Transforms each Signature into function pointer string
        Ex: void f(int a, char b) -> void (*f)(int, char)
    """
    for match in matches:
        arguments = match[2]
        methodName = match[1]
        returnType = match[0]
        if (len(arguments) == 0):
            arguments = "void"
        yield f"{returnType} (*{methodName})({arguments})"

def main():
    print("\nMyC Python Pre-Build script begin.\n")
    generatedFilesCount = 0

    # Generate all the namespace headers and c files
    for path, structName in FindCFiles("."):

        signatures = list(ParseSignatures(path))

        # Generate header
        methods = "\n".join(f"\t{x};" for x in MakeFunctionPointers(signatures))
        headerFileName = f"{structName}.h.gen"
        with open(headerFileName, "w") as file:
            print(f"Generating file {headerFileName}")
            file.write(GenerateHeader(structName, methods))
            generatedFilesCount += 1

        # Generate C file
        cFileName = f"{structName}.c.gen"
        with open(cFileName, "w") as file:
            print(f"Generating file {cFileName}")
            file.write(GenerateCFile(structName, signatures))
            generatedFilesCount += 1

    if generatedFilesCount > 0:
        # Generate macro keywords header
        with open("MyC.h.gen", "w") as file:
            print(f"Generating file MyC.h.gen")
            file.write(GenerateMacroFile())

    print(f"\n{generatedFilesCount} file(s) generated.")
    print("\nMyC Python Pre-Build script end.\n")

main()