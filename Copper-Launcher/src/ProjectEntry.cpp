#include "ProjectEntry.h"

#include "Fonts.h"

#include "PersistentData.h"

#include "Utils.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

#ifdef CU_LINUX
#include <sys/types.h>
#include <unistd.h>
#elif CU_WINDOWS
#include <Windows.h>
#include <processthreadsapi.h>
#endif

namespace Launcher {

    constexpr float ProjectTabHeight = 85.0f;

    static ImGuiID heldID = ImGuiID(0);

    extern void OnWindowClose();

    void ProjectEntry::Render() const {

        const ImGuiID id = ImGui::GetID(m_name.c_str());
        const ImVec2 size = { ImGui::GetContentRegionAvail().x, ProjectTabHeight };
        const bool isHovered = heldID == ImGuiID(0) && ImGui::GetHoveredID() == id;
        const bool isHeld = heldID == id;

        if (isHeld)
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
        else if (isHovered)
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { WindowPadding, 0 });
        ImGui::BeginChildFrame(id, size, ImGuiWindowFlags_NoScrollbar);
        ImGui::PopStyleVar();

        if (isHovered || isHeld)
            ImGui::PopStyleColor();

        const ImRect tabRect = { ImGui::GetItemRectMin(), ImGui::GetItemRectMin() + size };

        ImGui::PushFont(Fonts::SubtitleFont());
        ImGui::Text(m_name.c_str());
        ImGui::PopFont();

        ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - ImGui::GetTextLineHeight());

        ImGui::PushFont(Fonts::SmallTextFont());
        ImGui::Text(m_directory.c_str());
        ImGui::PopFont();

        bool held = false;
        if (ImGui::ButtonBehavior(tabRect, id, nullptr, &held, ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnRelease))
            LaunchEditor(m_directory);

        if (held)
            heldID = id;
        else if (heldID == id && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            heldID = ImGuiID(0);

        ImGui::EndChildFrame();

    }

}
