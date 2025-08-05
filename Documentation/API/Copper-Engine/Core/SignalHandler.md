# Summary
A basic signal handler where you can Register handler functions to certain signals, and those functions will be fired when the process receives the given signal.

# Header
Engine/Core/SignalHandler.h
# Source
Engine/Core/SignalHandler.cpp
# Namespace
Copper
# Type
Namespace

# Types
HandlerFunc#A function pointer with a void return type and an int32 argument (the received signal).

# Functions
void RegisterHandler(Signal signal, HandlerFunc handler)#Registers handler as the handler function to be called when signal is received
