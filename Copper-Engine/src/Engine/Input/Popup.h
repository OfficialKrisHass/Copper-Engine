#pragma once

#include "Engine/Core/Core.h"

namespace Copper::Input {

    enum PopupResult : uint32 {

        Invalid = 0,

        Ok,
        Yes,
        No,
        Retry,
        Abort,
        Ignore,
        Cancel,

    };

    PopupResult QuestionPopup(const std::string& title, const std::string& description, bool hasCancel = false);
    PopupResult InfoPopup(const std::string& title, const std::string& description, bool hasCancel = false);
    PopupResult WarningPopup(const std::string& title, const std::string& description, bool hasCancel = false);
    PopupResult ErrorPopup(const std::string& title, const std::string& description);
    
}