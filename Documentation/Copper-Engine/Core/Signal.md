# Summary
A simple enum representation of the signals a process can receive.

# Header
Engine/Core/SignalHandler.h
# Source
No source
# Namespace
Copper::SignalHandler
# Type
Enum

# Values
Abort#Abnormal termination, usually means something is wrong.
Termination#Normal termination.
Interrupt#The process received an interrupt either from the code, or from the Operating System.
Segfault#Segmentation fault, wrong memory access, attempt to write to 0x0, etc. Means you messed up kiddo.
IllegalInstruction#Whoop Whoop, that's the sound of the police. Means you attempted to perform a garbage, or priviliged instruction, which usually means a corrupted executable
FloatingPointExc#Misleading name, thrown when an invalid math operation (division by zero) or a floating point over/underflow happened.
