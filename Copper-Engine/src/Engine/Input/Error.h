#pragma once

#include "Engine/Core/Core.h"

namespace Copper::Input::Error {

    int ErrorPopup(const std::string& title, const std::string& description);
    int WarningPopup(const std::string& title, const std::string& description);
    
}