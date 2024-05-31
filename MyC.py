import os, re
from collections import namedtuple
from typing import Generator

# Templates
template_macro = """\
// Auto-generate begin. Do not modify!
#ifndef H_MyC
#define H_MyC

#include <stdlib.h>

#define new(typename) typename##_Constructor(malloc(sizeof(typename)))
#define new1(typename,a1) typename##_Constructor1(malloc(sizeof(typename)),a1)
#define new2(typename,a1,a2) typename##_Constructor2(malloc(sizeof(typename)),a1,a2)
#define new3(typename,a1,a2,a3) typename##_Constructor3(malloc(sizeof(typename)),a1,a2,a3)

#define delete(typename,self) free(typename##_Destructor(self))


// Result related definitions
typedef void* ref;

#define Result(T) Result##T
#define TypedefResult(T) \\
typedef struct Result(T) \\
{ \\
	int code; \\
	void* value; \\
} \\
Result(T);

TypedefResult(ref);

enum ResultCode
{
    InvalidArgument = -3,
    AllocNull = -2,
	out_oldOfBounds = -1,
	OK = 0
};

#define	try_old(x) {\\
	Resultref result = x; \\
	if (result.code != 0) \\
		return result;

#define out_old(variable) \\
	variable = result.value;\\
}

#define end_old }

// New result system

#define CONCAT(a, b) a##b
#define UNIQUE_NAME(base) CONCAT(base, __COUNTER__)

#define ret(T) Result(T) _MyC_try_return_value

#define try(T, x, statement) _MyC_try(T, statement, UNIQUE_NAME(temp_result), x)
#define	_MyC_try(T, statement, result, x) \\
	Result(T) result = statement; \\
	if (result.code != 0) \\
	{ \\
		_MyC_try_return_value.code = result.code; \\
		return _MyC_try_return_value; \\
	} \\
	T x = result.value

#define run(statement) _MyC_run(statement, UNIQUE_NAME(temp_code))
#define _MyC_run(statement, temp) \\
	int temp = statement.code; \\
	if (temp != 0) \\
	{ \\
		_MyC_try_return_value.code = temp; \\
		return _MyC_try_return_value; \\
	} \\
    
#define OK(x) \\
    _MyC_try_return_value.code = OK, \\
    _MyC_try_return_value.value = x, \\
    _MyC_try_return_value

#endif
// Auto-generate end. Do not modify!"""

template_header = """\
// Auto-generate begin. Do not modify!
#ifndef H_Gen_{structName}
#define H_Gen_{structName}

#include "MyC.h.gen"
{includeHeader}

{methods}

#endif
// Auto-generate end. Do not modify!"""

template_c = """\
// Auto-generate begin. Do not modify!
{includeHeader}
{runAll}
// Auto-generate end. Do not modify!"""

template_runAll = """\
#include <stdio.h>
{signature}
{{
    size_t i = 0;
{calls}
    printf("\\n%d test cases on {structName} has successfully run.\\n\\n\\n", i);
}}
"""

# Define named tuples
Signature = namedtuple('Signature', ['returnType', 'name', 'parameters', 'comment'])
FilePathName = namedtuple('FilePathName', ['path', 'nameWithout_oldExtension', 'headerExists', 'testing'])

signatureRunAll = Signature("void", "RunAll", "", "// Run all tests in this module")

def GenerateHeader(structName: str, signatures: list[Signature], headerExists: bool, testing: bool) -> str:

    customRunAll = Signature(
        signatureRunAll.returnType,
        f"{structName}_{signatureRunAll.name}",
        signatureRunAll.parameters,
        signatureRunAll.comment
    )
    methods = "\n".join(f"{x}" for x in MakeFunctionDefinition([customRunAll] if testing else signatures, testing))
    includeHeader = f"""#include "{structName}.h"\n""" if headerExists else ""

    return template_header.format(
        structName = structName, 
        methods = methods,
        includeHeader = includeHeader
    )

def GenerateMacroFile() -> str:
    return template_macro

def GenerateCFile(structName: str, signatures: list[Signature], headerExists: bool) -> str:
    
    runAll = ""
    calls = "\n".join(f"\tprintf(\"Executing test case `{methodName}` ...\"); {methodName}(); puts(\"OK\"); i++;" for _, methodName, _, _ in signatures)
    runAll = template_runAll.format(
        signature = f"{signatureRunAll.returnType} {structName}_{signatureRunAll.name}({signatureRunAll.parameters})",
        calls = calls,
        structName = structName
    )
    signatures = [signatureRunAll]

    return template_c.format(
        includeHeader = f"""#include "{structName}.h{"" if headerExists else ".gen"}\"""",
        runAll = runAll
    )

def FindCFiles(directory: str) -> Generator[FilePathName, None, None]:
    """
    Finds all the C files with #include "MyObject.h.gen" or #undef line
    that will be used for namespace generation.
    """
    for root, _, files in os.walk(directory):
        for fileName in files:
            if not fileName.endswith(".c") and not fileName.endswith(".h"):
                continue

            path = os.path.join(root, fileName)

            file = open(path, 'r')
            lines = file.readlines()
            file.close()

            testing = "#undef MyC_Test" in lines[0]
            generateHeader = f'#include "{fileName}.gen"' in lines[-1]
            if testing:
                yield (path, fileName[:-2], os.path.exists(path[:-2]+".h"), testing)
                continue
            if generateHeader:
                yield (path[:-2]+".c", fileName[:-2], os.path.exists(path[:-2]+".h"), testing)
                continue

def ParseSignatures(path: str, testing: bool) -> Generator[Signature, None, None]:
    """
    Parses the .c file and extracts all the function signatures
    to be used in the namespace file

    Matches format: `void method(int param1, int param2)` with
        indefinite number of parameters
    """
    
    lines = "".join(x for x in open(path))
    
    # match form '// comment\n void MethodName(type1 name1, type2 name2)'
    r  = "" if testing else "(\/\/.+?)\r?\n"
    r += "((?:\[\[nodiscard\]\] )?\w+\s*\*?)\s+([A-Z]\w*)\(((?:,?\s*(?:\w+\s*\*?)\s+\w+)*)\)"
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

def MakeFunctionDefinition(matches: list[Signature], testing: bool) -> Generator[str, None, None]:

    for match in matches:
        arguments = match[2]
        methodName = match[1]
        returnType = match[0]
        comment = match[3]
        yield comment
        yield f"{returnType} {methodName}({arguments});"

def main():
    print("\nMyC Python Pre-Build script begin.\n")
    generatedFilesCount = 0

    # Generate all the namespace headers and c files
    for path, structName, headerExists, testing in FindCFiles("."):

        signatures = list(ParseSignatures(path, testing))

        # Generate header
        headerFileName = f"{structName}.h.gen"
        with open(headerFileName, "w") as file:
            print(f"Generating file {headerFileName}")
            file.write(GenerateHeader(structName, signatures, headerExists, testing))
            generatedFilesCount += 1

        # Generate C file for testing
        if testing:
            cFileName = f"{structName}.c.gen"
            with open(cFileName, "w") as file:
                print(f"Generating file {cFileName}")
                file.write(GenerateCFile(structName, signatures, headerExists))
                generatedFilesCount += 1

    if generatedFilesCount > 0:
        # Generate macro keywords header
        with open("MyC.h.gen", "w") as file:
            print(f"Generating file MyC.h.gen")
            file.write(GenerateMacroFile())

    print(f"\n{generatedFilesCount} file(s) generated.")
    print("\nMyC Python Pre-Build script end.\n")

main()