#pragma once

#include "Engine/Core/Core.h"

// Wrapper for the command line arguments.
namespace Copper::Args {

    // Internal function aka DO NOT CALL THIS
    void Initialize(uint32 argc, char* argv[]);

    uint64 Count();
    const std::string& GetArgument(uint32 index);

}
