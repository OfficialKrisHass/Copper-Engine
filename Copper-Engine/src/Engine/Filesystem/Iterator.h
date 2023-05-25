#pragma once

#include "Engine/Core/Core.h"

#ifdef CU_WINDOWS

#include <filesystem>

#else

#include <experimental/filesystem>

#endif

namespace Copper::Filesystem {

#ifdef CU_WINDOWS
    using DirectoryIterator = std::filesystem::directory_iterator;
    using DirectoryEntry = std::filesystem::directory_entry;
#else
    using DirectoryIterator = std::experimental::filesystem::directory_iterator;
    using DirectoryEntry = std::experimental::filesystem::directory_entry;
#endif

}