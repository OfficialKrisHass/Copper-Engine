# Args
Namespace wrapper for the command line arguments passed to the running executable.

## Details
Header: Engine/Core/Args.h
Source: Engine/Core/Args.cpp
Namespace: Copper
Type: Namespace

Does not include the executable path.

## Functions
void Setup(uint32 argc, char* arv[]) - Internal, one of the first things that gets called, used to parse the arguments. DO NOT USE.

uint32 Count() - Returns the number of arguments (does not include the executable path).
const std::string& Get(uint32 index) - Returns the argument at index, returns an empty string when index is invalid.

const fs::path& ProjectPath() - Only when CU_EDITOR is defined, returns the path of the Project that should be opened, if provided.

## Errors
"Args::Setup has already been run" - logged when Args::Setup was called a second time (not allowed).
"Command line arguments count is 0 (no exec path)" - logged when argc == 0 (no command line arguments).
