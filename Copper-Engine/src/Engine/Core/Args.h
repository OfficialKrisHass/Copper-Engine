#pragma once

#include "Engine/Core/Core.h"

// Safe wrapper for the command line arguments, shouldn't include executable name
namespace Copper::Args {

    // Internal function aka DO NOT CALL THIS
    void Setup(uint32 argc, char* argv[]);

    uint32 Count();
    const std::string& Get(uint32 index);

}