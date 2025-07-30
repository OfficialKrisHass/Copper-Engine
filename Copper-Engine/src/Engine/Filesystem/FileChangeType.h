#pragma once

#include "Engine/Core/Core.h"

namespace Copper {

    // File (or directory) changes detected by the File/DirWatches.
    enum class FileChangeType : uint8 {

        None = 0,

        Created, // Created, duh
        Changed, // Some change was made it's contents
        Deleted, // File was deleted or stopped being watched

        RenamedOld, // File was renamed or moved, and this is it's old path/name
        RenamedNew, // File was renamed or moved, and this is it's new path/name

    };

    inline constexpr const char* FileChangeTypeToString(FileChangeType value) {

        switch (value) {

            case FileChangeType::None: return "None";
            case FileChangeType::Created: return "Created";
            case FileChangeType::Changed: return "Changed";
            case FileChangeType::Deleted: return "Deleted";
            case FileChangeType::RenamedOld: return "Renamed old";
            case FileChangeType::RenamedNew: return "Renamed new";
            default: break;

        }

        LogError("Could not get string of invalid FileChangeType value. Value: '{}'", static_cast<uint8>(value));
        return "";

    }

}
