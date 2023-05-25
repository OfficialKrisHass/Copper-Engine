#pragma once

#include "Engine/Core/Core.h"

namespace Copper::Input {

    enum PopupResult : uint32_t {

        Ok,
        Cancel,
        Closed,
        Waiting,
        Invalid,

    };

    PopupResult WarningPopup(const std::string& title, const std::string& description);
    PopupResult ErrorPopup(const std::string& title, const std::string& description);
    
}