import os, re
from collections import namedtuple
from typing import Generator

# Define named tuples
Signature = namedtuple('Signature', ['returnType', 'name', 'parameters', 'comment'])
FilePathName = namedtuple('FilePathName', ['path', 'nameWithoutExtension', 'headerExists', 'testing'])

signatureRunAll = Signature("void", "RunAll", "", "// Run all tests in this module")

def SignatureToString(signature: Signature) -> str:
    return f"{signature.returnType} {signature.name}({signature.parameters})"

def GenerateHeader(structName: str, methods: str, headerExists: bool) -> str:

    includeHeader = f"""#include "{structName}.h"\n""" if headerExists else ""

    return f"""// Auto-generate begin. Do not modify!
#ifndef H_Gen_{structName}
#define H_Gen_{structName}

#include "MyC.h.gen"
{includeHeader}
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

#include <stdlib.h>

#define new(typename) $##typename.Constructor(malloc(sizeof(typename)))
#define new1(typename,a1) $##typename.Constructor(malloc(sizeof(typename)),a1)
#define new2(typename,a1,a2) $##typename.Constructor(malloc(sizeof(typename)),a1,a2)
#define new3(typename,a1,a2,a3) $##typename.Constructor(malloc(sizeof(typename)),a1,a2,a3)
#define delete(typename,self) free($##typename.Destructor(self))

#define	$(x) if (x) return NULL

#endif
// Auto-generate end. Do not modify!"""

def GenerateCFile(structName: str, signatures: list[Signature], headerExists: bool, generateRunAll: bool) -> str:
    
    runAll = ""
    if generateRunAll:
        calls = "\n".join(f"\t{methodName}();" for _, methodName, _, _ in signatures)
        runAll = f"""static {SignatureToString(signatureRunAll)}
{{
{calls}
}}
"""
        signatures = [signatureRunAll]
    
    members = []
    for _, methodName, _, _ in signatures:
        members.append(f"\t.{methodName} = &{methodName}")
        
    members = ",\n".join(members)


    return f"""// Auto-generate begin. Do not modify!
#include "{structName}.h{"" if headerExists else ".gen"}"
{runAll}
_Namespace{structName} const ${structName} =
{{
{members}
}};
// Auto-generate end. Do not modify!"""

def FindCFiles(directory: str) -> Generator[FilePathName, None, None]:
    """
    Finds all the C files with #include "MyObject.c.gen" or #undef line
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

            for line in lines:
                testing = "#undef MyC_Test" in line
                if f'#include "{fileName}.gen"' in line or testing:
                    yield (path, fileName[:-2], os.path.exists(path[:-2]+".h"), testing)
                    break

def ParseSignatures(path: str, testing: bool) -> Generator[Signature, None, None]:
    """
    Parses the .c file and extracts all the function signatures
    to be used in the namespace file

    Matches format: `void method(int param1, int param2)` with
        indefinite number of parameters
    """
    
    lines = "".join(x for x in open(path))
    # match form '// comment\n void MethodName(type1 name1, type2 name2)'
    r  = "" if testing else "(//.+?)\\r?\\n"
    r += "static (\w+\s*\*?)\s+([A-Z]\w*)\(((?:,?\s*(?:\w+\s*\*?)\s+\w+)*)\)"
    for found in re.finditer(r, lines, flags=re.MULTILINE):
        if testing:
            yield Signature(found.group(1), found.group(2), found.group(3), "")
        else:
            yield Signature(found.group(2), found.group(3), found.group(4), found.group(1))

def MakeFunctionPointers(matches: list[Signature], testing: bool) -> Generator[str, None, None]:
    """
    Transforms each Signature into function pointer string
        Ex: void f(int a, char b) -> void (*f)(int, char)
    """
    for match in matches:
        arguments = match[2]
        methodName = match[1]
        returnType = match[0]
        comment = match[3]
        #if (len(arguments) == 0):
        #    arguments = "void"
        if not testing:
            yield comment
        yield f"{returnType} (*{methodName})({arguments});"

def main():
    print("\nMyC Python Pre-Build script begin.\n")
    generatedFilesCount = 0

    # Generate all the namespace headers and c files
    for path, structName, headerExists, testing in FindCFiles("."):

        signatures = list(ParseSignatures(path, testing))

        # Generate header
        methods = "\n".join(f"\t{x}" for x in MakeFunctionPointers([signatureRunAll] if testing else signatures, testing))
        headerFileName = f"{structName}.h.gen"
        with open(headerFileName, "w") as file:
            print(f"Generating file {headerFileName}")
            file.write(GenerateHeader(structName, methods, headerExists))
            generatedFilesCount += 1

        # Generate C file
        cFileName = f"{structName}.c.gen"
        with open(cFileName, "w") as file:
            print(f"Generating file {cFileName}")
            file.write(GenerateCFile(structName, signatures, headerExists, testing))
            generatedFilesCount += 1

    if generatedFilesCount > 0:
        # Generate macro keywords header
        with open("MyC.h.gen", "w") as file:
            print(f"Generating file MyC.h.gen")
            file.write(GenerateMacroFile())

    print(f"\n{generatedFilesCount} file(s) generated.")
    print("\nMyC Python Pre-Build script end.\n")

main()