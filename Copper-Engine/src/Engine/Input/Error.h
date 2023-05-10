#pragma once

#include "Engine/Core/Core.h"

namespace Copper::Input::Error {

    enum PopupResult : uint32_t {

        Ok,
        Cancel,
        Invalid

    };

    PopupResult WarningPopup(const std::string& title, const std::string& description);
    PopupResult ErrorPopup(const std::string& title, const std::string& description);
    
}