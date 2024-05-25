import os, re

def gen_header(struct_name, methods):
    return f"""// Auto-generate begin. Do not modify!
#ifndef H_Gen_{struct_name}
#define H_Gen_{struct_name}

#include "MyC.h.gen"
#include "{struct_name}.h"

typedef struct
{{
{methods}
}}
_Namespace{struct_name};

extern _Namespace{struct_name} const _namespace{struct_name};

#endif
// Auto-generate end. Do not modify!"""

# Finds all the header files that requests namespace generation
# with #include "MyObject.h.gen" statement
def find_headers(directory):
    included_files = []
    for root, _, files in os.walk(directory):
        for file in files:
            if not file.endswith(".h"):
                continue
            file_path = os.path.join(root, file)
            with open(file_path, 'r') as f:
                lines = f.readlines()
                for line in lines:
                    if f'#include "{file}.gen"' in line:
                        included_files.append((file_path, file[:-2]))
                        break
    return included_files

# Parses the .c file and extracts all the function signatures
# to be used in the namespace file
def parse_signatures(path, struct_name):
    path = path[:-1] + "c"
    matches = []
    with open(path) as file:
        for line in file:
            # match form 'static methodName(type1 name1, type2 name2)'
            found_matches = re.findall(r"static\s+(\w+\s*\*?)\s+(\w+)\(((?:,?\s*(?:\w+\s*\*?)\s+\w+)+)\)", line)
            matches.extend(found_matches)
    return matches

def make_function_pointers(matches):
    for match in matches:
        arguments = match[2]
        # match form 'type1 name1' or ', type2 name2'
        parameters = ", ".join(re.findall(r"(?:,?\s*(\w+\s*\*?)\s+\w+)", arguments))
        methodName = match[1][0].upper() + match[1][1:]
        returnType = match[0]
        yield f"\t{returnType} (*{methodName})({parameters});"

def make_namespace_struct(struct_name, matches):

    members = []
    for match in matches:
        methodName = match[1][0].upper() + match[1][1:]
        members.append(f"\t.{methodName} = &{match[1]}")

    members = ",\n".join(members)

    return f"""// Auto-generate begin. Do not modify!
#include "{struct_name}.h"

_Namespace{struct_name} const _namespace{struct_name} =
{{
{members}
}};
// Auto-generate end. Do not modify!"""
    
def main():
    print("\nMyC Python Pre-Build script begin.\n")
    files_generated = 0

    # Generate all the namespace headers and c files
    result = find_headers(".")
    for path, struct_name in result:
        
        matches = parse_signatures(path, struct_name)

        # Generate header
        methods = "\n".join(make_function_pointers(matches))
        header_name = f"{struct_name}.h.gen"
        with open(header_name, "w") as file:
            print(f"Generating file {header_name}")
            file.write(gen_header(struct_name, methods))
            files_generated += 1

        # Generate C file
        c_file_name = f"{struct_name}.c.gen"
        with open(c_file_name, "w") as file:
            print(f"Generating file {c_file_name}")
            file.write(make_namespace_struct(struct_name, matches))
            files_generated += 1

    if files_generated > 0:
        # Generate macro keywords header
        with open("MyC.h.gen", "w") as file:
            print(f"Generating file MyC.h.gen")
            file.write("""// Auto-generate begin. Do not modify!
#ifndef H_MyC
#define H_MyC

#define $(x) _namespace##x
#define new(x) $(x).Constructor(malloc(sizeof(x)))
#define delete(x,y) free($(x).Destructor(y))

#endif
// Auto-generate end. Do not modify!""")

    print(f"\n{files_generated} file(s) generated.")
    print("\nMyC Python Pre-Build script end.\n")

main()