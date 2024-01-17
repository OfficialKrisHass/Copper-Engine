#pragma once

#include "Engine/Core/Core.h"

namespace Copper::Input {

    enum PopupResult : int32_t {

        Ok,
        Yes,
        No,
        Retry,
        Abort,
        Ignore,
        Cancel,

        Invalid

    };

    PopupResult QuestionPopup(const std::string& title, const std::string& description, bool hasCancel = false);
    PopupResult InfoPopup(const std::string& title, const std::string& description, bool hasCancel = false);
    PopupResult WarningPopup(const std::string& title, const std::string& description, bool hasCancel = false);
    PopupResult ErrorPopup(const std::string& title, const std::string& description);
    
}