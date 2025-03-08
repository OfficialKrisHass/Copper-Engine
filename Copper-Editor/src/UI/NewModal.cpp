#include "NewModal.h"

#include <ImGui/imgui.h>

using namespace Copper;

namespace Editor::NewModal {

    bool open = false;

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

        ImGui::Text("Input the name of then ew thing:");

        ImGui::EndPopup();

    }

    static const char* options[] = { "Folder", "Script", "Material" };
    static constexpr int optionsCount = IM_ARRAYSIZE(options);
    static int selectedOption = -1;

    void Options() {

        CUP_FUNCTION();
        ImGui::BeginChild("##Options", ImVec2(70.0f * ImGui::GetContentRegionAvail().x / 100.0f, 90.0f * ImGui::GetContentRegionAvail().y / 100.0f), true);

        ImGui::PushItemWidth(-1);

        ImVec2 size = ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y);
        if (ImGui::BeginListBox("##OptionsList", size)) {

            for (int i = 0; i < optionsCount; i++) {

                ImGui::PushID(i);

                const bool selected = i == selectedOption;
                if (ImGui::Selectable(options[i], selected))
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

        ImGui::Text("Here is some info about the selected option");

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
