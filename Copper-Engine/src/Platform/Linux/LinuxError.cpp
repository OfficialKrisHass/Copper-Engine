#include "cupch.h"
#include "Engine/Input/Error.h"

#include <ImGui/imgui.h>

namespace Copper::Input::Error {

    PopupResult WarningPopup(const std::string& title, const std::string& description) {

        ImGui::Begin(title.c_str());

        ImGui::Text(description.c_str());
        if(ImGui::Button("Ok")) { ImGui::End(); return PopupResult::Ok; }
        if(ImGui::Button("Cancel")) { ImGui::End(); return PopupResult::Cancel; }

        ImGui::End();

        return PopupResult::Invalid;

    }
    PopupResult ErrorPopup(const std::string& title, const std::string& description) {

        ImGui::Begin(title.c_str());

        ImGui::Text(description.c_str());
        if(ImGui::Button("Ok")) { ImGui::End(); return PopupResult::Ok; }

        ImGui::End();

        return PopupResult::Invalid;

    }

}