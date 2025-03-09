#include "NewModal.h"

#include "UI/NewModalData.h"

#include <ImGui/imgui.h>

#define NAME_MAX_LENGTH 64

using namespace Copper;

namespace Editor::NewModal {

    static bool open = false;
    static uint32 selectedOption = 0;

    static char nameInput[NAME_MAX_LENGTH];

    void Options();
    void Details();

    void UIRender() {

        CUP_FUNCTION();

        if (!open) return;

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (!ImGui::BeginPopupModal("New", &open)) return;

        Options();
        ImGui::SameLine();
        Details();

        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::InputText("##Name", nameInput, NAME_MAX_LENGTH);

        ImGui::EndPopup();

    }

    void Options() {

        CUP_FUNCTION();
        ImGui::BeginChild("##Options", ImVec2(70.0f * ImGui::GetContentRegionAvail().x / 100.0f, 90.0f * ImGui::GetContentRegionAvail().y / 100.0f), true);

        ImGui::PushItemWidth(-1);

        ImVec2 size = ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y);
        if (ImGui::BeginListBox("##OptionsList", size)) {

            for (int i = 0; i < Data::optionsCount; i++) {

                ImGui::PushID(i);

                const bool selected = i == selectedOption;
                if (ImGui::Selectable(Data::options[i], selected))
                    selectedOption = i;

                if (selected)
                    ImGui::SetItemDefaultFocus();

                ImGui::PopID();

            }

            ImGui::EndListBox();

        }

        ImGui::PopItemWidth();

        ImGui::EndChild();

    }
    void Details() {

        CUP_FUNCTION();

        ImGui::BeginChild("##Details", ImVec2(0, 90.0f * ImGui::GetContentRegionAvail().y / 100.0f));

        ImGui::TextWrapped(Data::optionsDetails[selectedOption]);

        ImGui::EndChild();

    }

    void Open() {

        CUP_FUNCTION();
        open = true;

        ImGui::OpenPopup("New");

    }
    void Close() {

        CUP_FUNCTION();
        open = false;

    }

}
