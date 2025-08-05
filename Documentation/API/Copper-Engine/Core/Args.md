# Summary
Wrapper for the command line arguments passed to the running executable. Does not include the path of the executable.

# Header
Engine/Core/Args.h
# Source
Engine/Core/Args.cpp
# Namespace
Copper
# Type
Namespace

# Functions
void Setup(uint32 argc, char* arv[])#Parses the command line arguments. Internal, gets called almost immediately at startup, DO NOT USE.
uint32 Count()#Returns the number of arguments.
const std::string& Get(uint32 index)#Returns the argument at the provided index.
const fs::path& ProjectPath()#Only when CU_EDITOR is defined, returns the project to open, if one was provided.
