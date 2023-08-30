#include "cupch.h"
#include "Engine/Input/Popup.h"

#include "Engine/Core/Engine.h"

#include "Engine/UI/ImGui.h"

#include <ImGui/imgui.h>
#include <portable-file-dialogs.h>

namespace Copper::Input {

    PopupResult QuestionPopup(const std::string& title, const std::string& description, bool cancel) {

        auto m = pfd::message(title, description, cancel ? pfd::choice::yes_no_cancel : pfd::choice::yes_no, pfd::icon::question);

        switch (m.result()) {

            case pfd::button::yes: return PopupResult::Yes;
            case pfd::button::no: return PopupResult::No;
            case pfd::button::cancel: return PopupResult::Cancel;
            default: return PopupResult::Invalid;

        }

    }
    PopupResult InfoPopup(const std::string& title, const std::string& description, bool cancel) {

        auto m = pfd::message(title, description, cancel ? pfd::choice::ok_cancel : pfd::choice::ok, pfd::icon::info);

        switch (m.result()) {

            case pfd::button::ok: return PopupResult::Ok;
            case pfd::button::cancel: return PopupResult::Cancel;
            default: return PopupResult::Invalid;

        }

    }
    PopupResult WarningPopup(const std::string& title, const std::string& description) {
        
        auto m = pfd::message(title, description, pfd::choice::yes_no_cancel, pfd::icon::warning);
        pfd::button btn = m.result();
        Log((int32_t) btn);

        switch(btn) {

            case pfd::button::yes: return PopupResult::Yes;
            case pfd::button::no: return PopupResult::No;
            case pfd::button::cancel: return PopupResult::Cancel;
            default: return PopupResult::Invalid;

        }

    }
    PopupResult ErrorPopup(const std::string& title, const std::string& description) {

        auto m = pfd::message(title, description, pfd::choice::ok, pfd::icon::error);

        switch(m.result()) {

            case pfd::button::ok: return PopupResult::Ok;
            default: return PopupResult::Invalid;

        }

    }

}