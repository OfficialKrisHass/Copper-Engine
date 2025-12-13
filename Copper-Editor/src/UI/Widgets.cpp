#include "Widgets.h"

#include "Core/EditorApp.h"

#include <Engine/Renderer/Texture.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

namespace Editor::UI {

    void InfoLabel(const Texture& icon, const char* text) {

        static const ImGuiStyle& style = ImGui::GetStyle();

        ImGui::PushStyleColor(ImGuiCol_ChildBg, style.Colors[ImGuiCol_FrameBg]);

        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 6));

        ImGui::BeginChild("InfoLabel", ImVec2(0, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);

        const float textHeight = ImGui::GetTextLineHeight();
        const float iconSize = textHeight * 1.3f;
        ImGui::Image(static_cast<ImTextureID>((intptr_t) icon.GetID()), ImVec2(iconSize, iconSize), {0, 1}, {1, 0});

        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (textHeight - iconSize) * 0.5f);
        ImGui::Text(text);

        ImGui::EndChild();

        ImGui::PopStyleColor();
        ImGui::PopStyleVar(3);

    }

}
