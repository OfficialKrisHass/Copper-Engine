#pragma once

#include "Engine/Core/Core.h"

namespace Copper::Input::Error {

    int ErrorPopup(std::string title, std::string description);
    int WarningPopup(std::string title, std::string description);
    
}