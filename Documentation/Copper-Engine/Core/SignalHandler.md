# SignalHandler
A basic signal handler where you can Register handler functions to certain signals, and those functions will be fired when the process receives the given signal.

## Details
Header: Engine/Core/SignalHandler.h
Source: Engine/Core/SignalHandler.cpp
Namespace: Copper
Type: Namespace

## Enums
Signal : uint32 - Copper representation of the most commonly used signals. 

## Types
void(*HandlerFunc)(int32) - A function pointer passed to RegisterHandler. Is given the signal as an int32 as a parameter.

## Functions
void RegisterHandler(Signal signal, HandlerFunc handler) - Registers handler as the handler function to be called when signal is received. Simple wrapper for signal() from csignal.
