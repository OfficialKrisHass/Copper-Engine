#pragma once

#include "Engine/Core/Core.h"

// Safe wrapper for the command line arguments
namespace Copper::Args {

    void Setup(uint32 argc, char* argv[]);

    uint32 Count();
    const std::string& Get(uint32 index);

}