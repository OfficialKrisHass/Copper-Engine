# Summary
Core header file that includes all of the core files, types, defines, functions, etc that are used in almost every part of the Engine. This header is included in every Engine header file.

# Header
Engine/Core/Core.h
# Source
No source
# Namespace
No namespace
# Type
Header file

# Functions
const fs::path& ExecutableFolder()#Returns the path of the folder where the current executable is located.

bool Editor::IsRuntimeRunning()#Only when CU_EDITOR is defined, returns true if the runtime is running (play button has been pressed).

# Defines
FLAG(bit)#Returns a number where only the bit at position bit is 1.

IN_RUNTIME(action)#When CU_EDITOR is defined, action is executed only when the runtime is running (the play button was pressed). Otherwise action is always executed.
IN_NOT_RUNTIME(action)#When CU_EDITOR is defined, action is executed only when the runtime is NOT running. Otherwise empty (action is never executed).
IN_RUNTIME_BEGIN#When CU_EDITOR is defined, begins a block where the following lines of code (until a IN_RUNTIME_END) is executed only when the runtime is running. Otherwise empty.
IN_RUNTIME_END#When CU_EDITOR is defined, ends the previous IN_RUNTIME_BEGIN block. Otherwise empty.

# Types
int8#A 1 byte signed integer.
uint8#A 1 byte unsigned integer.

int16#A 2 byte signed integer
uint16#A 2 byte unsigned integer.

int32#A 4 byte signed integer.
uint32#A 4 byte unsigned integer.

int64#An 8 byte signed integer.
uint64#An 8 byte unsigned integer
