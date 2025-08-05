# Summary
Core header for logging messages to the console. Contains the log defines and Logger class. Included in Engine/Core/Core.h

# Header
Engine/Core/Log.h
# Source
Engine/Core/Log.cpp
# Namespace
Copper
# Type
Header

# Defines
Log(...)#Only if CU_DEBUG is defined (otherwise empty), Logs the provided arguments as a normal log message.
LogWarn(...)#Only if CU_DEBUG is defined (otherwise empty), Logs the provided arguments as a warning log message.
LogError(...)#Only if CU_DEBUG is defined (otherwise empty), Logs the provided arguments as a error log message.
LogSuccess(...)#Only if CU_DEBUG is defined (otherwise empty), Logs the provided arguments as a success log message.
