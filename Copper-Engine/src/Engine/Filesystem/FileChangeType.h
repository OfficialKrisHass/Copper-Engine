#pragma once

#include "Engine/Core/Core.h"

namespace Copper {

    enum class FileChangeType : uint8 {

        None = 0,

        Created,
        Changed,
        Deleted,

        RenamedOld,
        RenamedNew,

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

        LogError("Could not get string of invalud FileChangeType value. Value: '{}'", static_cast<uint8>(value));
        return "";

    }

}
