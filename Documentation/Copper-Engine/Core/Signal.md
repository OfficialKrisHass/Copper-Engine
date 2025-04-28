# Signal
A simple enum representation of the signals a process can receive.

## Details
Header: Engine/Core/SignalHandler.h
Source: No source
Namespace: Copper::SignalHandler
Type: Enum

## Values
Abort = 0 - Abnormal termination, usually means something is wrong.
Termination = 1 - Normal termination.
Interrupt = 2 - The process received an interrupt either from the code, or from the Operating System.
Segfault = 3 - Segmentation fault, wrong memory access, attempt to write to 0x0, etc. Means you messed up kiddo.
IllegalInstruction = 4 - Whoop Whoop, that's the sound of the police. Means you attempted to perform a garbage, or priviliged instruction, which usually means a corrupted executable
FloatingPointExc = 5 - Misleading name, thrown when an invalid math operation (division by zero) or a floating point over/underflow happened.
