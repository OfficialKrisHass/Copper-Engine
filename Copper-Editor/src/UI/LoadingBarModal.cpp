#include "LoadingBarModal.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

#define BAR_WIDTH 600
#define BAR_HEIGHT 20

namespace Editor {

    void LoadingBar::DrawUI(){

        CUP_FUNCTION();

        static const ImU32 backgroundColor = ImGui::GetColorU32(ImGuiCol_FrameBg);
        static const ImU32 barColor = ImGui::GetColorU32(ImGuiCol_FrameBgHovered);

        if (!m_open)
            Open();

        // Ensure the modal has the correct size and is centered.

        ImGuiContext& context = *GImGui;
        const ImGuiStyle& style = context.Style;

        const float width = BAR_WIDTH + style.WindowPadding.x * 2.0f;
        const float height = BAR_HEIGHT + ImGui::GetTextLineHeightWithSpacing() + style.WindowPadding.y * 2.0f + ImGui::GetFrameHeight();
        const ImGuiViewport* viewport = ImGui::GetCurrentWindow()->WasActive ? ImGui::GetCurrentWindow()->Viewport : ImGui::GetMainViewport();

        ImGui::SetNextWindowSize(ImVec2(width, height));
        ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Once, ImVec2(0.5f, 0.5f));

        // Begin modal

        if (!ImGui::BeginPopupModal(m_title.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) return;
        
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return ImGui::EndPopup();

        const ImGuiID id = window->GetID(m_title.c_str());

        // Status

        ImGui::Text("%s", m_statusMsg.c_str());

        // Calculate and add rect. AddRectFilled only draws the rect, it does not move the cursor and doesn't add a clipping region and stuff.
        // Which is why we call ItemSize and ItemAdd (These names are seriously misleading btw :))
        
        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = ImVec2(BAR_WIDTH, BAR_HEIGHT);

        const ImRect bb = ImRect(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ImGui::ItemSize(bb, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, id))
            return ImGui::EndPopup();

        // Draw the actual bars

        window->DrawList->AddRectFilled(bb.Min, bb.Max, backgroundColor);
        window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + size.x * m_status, bb.Max.y), barColor);

        // Finish

        ImGui::EndPopup();

    }

    void LoadingBar::Open() {

        CUP_FUNCTION();

        ImGui::OpenPopup(m_title.c_str());

        m_open = true;

    }

}
