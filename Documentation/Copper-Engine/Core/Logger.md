# Logger
A simple static class with the main logger functionality. This class should be rewritten because there is no need for it to be a class smh.

## Details
Header: Engine/Core/Log.h
Source: Engine/Core/Log.cpp
Namespace: Copper
Type: Class

## Functions
static void Initialize() - Internal, initializes the Logger. DO NOT USE.

static std::shared_ptr<spdlog::logger> GetLogger() - Retrieves a pointer to the spdlog::logger object.
static std::vector<std::string> GetLastLogMessages() - Returns a vector of the last logged messages. This should be removed as it's inefective.
