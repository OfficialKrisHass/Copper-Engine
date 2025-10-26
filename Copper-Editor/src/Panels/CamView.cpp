#include "CamView.h"

#include "Core/EditorApp.h"

#include "Projects/Project.h"

#include <Engine/Core/Window.h>

#include <Engine/Input/Input.h>

#include <Engine/Renderer/FrameBuffer.h>

#include <ImGui/imgui.h>

namespace Editor {

    void CamView::UIRender() {

        CUP_FUNCTION();
        CUP_START_FRAME("Cam view");

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        bool open = ImGui::Begin("Cam View");
        ImGui::PopStyleVar();

        if (!open) {

            ImGui::End();

            CUP_END_FRAME();
            return;

        }
        if (!GetProject().IsValid() || GetScene()->GetMainCamera() == nullptr) {

            ImGui::Text("No Camera available!");

            ImGui::End();

            CUP_END_FRAME();
            return;

        }

        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        SetWindowSize(UVector2I(static_cast<uint32>(windowSize.x), static_cast<uint32>(windowSize.y)));

        ImGui::Image(static_cast<ImTextureID>((uint64) GetMainFBO().GetColorAttachmentID(0)), windowSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
        if (GetEditorState() == EditorState::Play && !m_isAcceptingInput && ImGui::IsItemClicked()) {

            m_isAcceptingInput = true;

            Input::SetCursorLocked(true);
            Input::SetCursorVisible(false);

        }

        ImGui::End();

        CUP_END_FRAME();

    }

}
