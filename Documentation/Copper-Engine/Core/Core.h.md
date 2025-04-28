# Core.h
Core header file that includes all of the core files, types, defines, functions, etc that are used in almost every part of the Engine.

## Details
Header: Engine/Core/Core.h
Source: No source
Type: Header

This header is included in every Engine header file.

## Functions
extern bool Editor::IsRuntimeRunning() - Only when CU_EDITOR is defined, returns true if the runtime is running (play button has been pressed) and false if not.

const fs::path& ExecutableFolder() - Returns the directory where the current executable is located. Definition in Engine/Core/Args.cpp

## Defines
FLAG(bit) = (1 << bit) - Used to create a flag where only the bit at position bit is 1.

IN_RUNTIME(action) = if (Editor::IsRuntimeRunning()) { action; } - Only when CU_EDITOR is defined (otherwise empty), Executes action only if runtime is running.
IN_NOT_RUNTIME(action) = if (!Editor::IsRuntimeRunning()) { action; } - Only when CU_EDITOR is defined (otherwise empty), Executes action only if runtime is not running.
IN_RUNTIME_BEGIN = if (Editor::IsRuntimeRunning()) { - Only when CU_EDITOR is defined (otherwise empty), Begins a scope, where all the lines until the first IN_RUNTIME_END execute only if the runtime is running.
IN_RUNTIME_END = } - Only when CU_EDITOR is defined (otherwise empty), Simply ends the previous IN_RUNTIME_BEGIN.

## Typedefs
char int8 - 8 bit signed integer.
unsigned char uint8 - 8 bit unsigned integer.

short int16 - 16 bit signed integer.
unsigned short uint16 - 16 bit unsigned integer.

int int32 - 32 bit signed integer.
unsigned int uint32 - 32 bit unsigned integer.

long long int64 - 64 bit signed integer.
unsigned long long uint64 - 64 bit unsigned integer.
