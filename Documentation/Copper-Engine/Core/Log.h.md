# Log.h
Core header for logging messages to the console. Contains the log defines and Logger class

## Details
Header: Engine/Core/Log.h
Source: Engine/Core/Log.cpp
Namespace: Copper
Type: Header

Included in Engine/Core/Core.h so no need to include manually.

## Defines
Log(...) - Only if CU_DEBUG is defined (otherwise empty), Logs the provided arguments as a normal log message.
LogWarn(...) - Only if CU_DEBUG is defined (otherwise empty), Logs the provided arguments as a warning log message.
LogError(...) - Only if CU_DEBUG is defined (otherwise empty), Logs the provided arguments as a error log message.
LogSuccess(...) - Only if CU_DEBUG is defined (otherwise empty), Logs the provided arguments as a success log message.

## Classes
Logger - static Logger implementation and handy functions.
