#include "Viewport.h"

#include "Core/EditorApp.h"
#include "Core/Change.h"
#include "Core/ChangeHandler.h"

#include "Panels/Properties.h"

#include <Engine/Components/RigidBody.h>

#include <Engine/Renderer/Renderer.h>

#include <Engine/Physics/Raycast.h>

#include <Engine/Input/Input.h>

#include <Engine/Utilities/Math.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGuizmo/ImGuizmo.h>

namespace Editor {

    Raycast::Data hitData;

    void Viewport::Initialize() {

        CUP_FUNCTION();

        CU_ASSERT(ImGui::FindWindowSettingsByID(ImHashStr("Viewport")) != nullptr, "Could not get Window Settings for Viewport window");

        ImVec2ih size = ImGui::FindWindowSettingsByID(ImHashStr("Viewport"))->Size;
        float tabBarHeight = 18.0f + ImGui::GetStyle().FramePadding.y * 2;
        m_size = UVector2I(size.x, size.y - static_cast<uint32>(tabBarHeight));

        m_fbo.Create(m_size, { FrameBuffer::Attachment::Format::RGB8, FrameBuffer::Attachment::Format::RedInteger });
        m_sceneCam = SceneCamera(m_size, this);

    }

    void Viewport::Update() {

        CUP_FUNCTION();

        if (m_fbo.GetSize() != m_size) {
            
            m_fbo.Resize(m_size);
            m_sceneCam.Resize(m_size);

        }

        m_fbo.Bind();

        RendererAPI::ClearColor(Color(0.18f, 0.18f, 0.18f));
        m_fbo.ClearAttachment(1, INVALID_ENTITY_ID);

        if (GetScene() == nullptr || !GetScene()->IsInitialized()) {

            m_fbo.Unbind();
            return;

        }

        m_sceneCam.Update();
        GetScene()->Render(&m_sceneCam);

        // When the left mouse button is clicked, the window has to be open, interactions can't be blocked and the cursor needs to be in the window are
        // and not hovering over the gizmos for the selection of the entity to be successful. 

        if (m_open && !IsInteractionBlocked() && Input::GetKeyState(KeyCode::Mouse0) == KeyState::Pressed
            && m_mousePos.x > -1 && m_mousePos.y > -1 && m_mousePos.x < m_size.x && m_mousePos.y < m_size.y && !ImGuizmo::IsOver()) {

            uint32 id = m_fbo.ReadPixel(1, m_mousePos.x, m_mousePos.y);
            InternalEntity* selectedEntity = GetEntityFromID(id);

            if (selectedEntity != nullptr)
                Properties::SetSelectedEntity(Entity(GetEntityFromID(id)));

        }

        m_fbo.Unbind();

    }
    void Viewport::UIRender() {

        CUP_FUNCTION();
        CUP_START_FRAME("Viewport");

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        m_open = ImGui::Begin(m_name.c_str());
        ImGui::PopStyleVar();

        m_focused = ImGui::IsWindowFocused();

        if (!m_open) {

            ImGui::End();

            CUP_END_FRAME();

            return;

        }

        //TODO: Either Change ImGui To use UVector2I or edit Copper Code to use ImVec2
        //      so that we don't have to allocate memory for the UVector2I
        float tabBarHeight = ImGui::GetCursorPos().y;
        ImVec2 windowPos = ImGui::GetWindowPos();

        if (!m_firstFrame) {

            ImVec2 windowSize = ImGui::GetContentRegionAvail();
            if (m_size.x != windowSize.x || m_size.y != windowSize.y)
                m_size = UVector2I(static_cast<uint32>(windowSize.x), static_cast<uint32>(windowSize.y));

        }

        m_firstFrame = false;

        m_centre = m_size / 2;
        m_centre.x += (uint32) windowPos.x;
        m_centre.y += (uint32) windowPos.y;

        ImVec2 mousePos = ImGui::GetMousePos();
        m_mousePos.x = static_cast<int32>(mousePos.x - windowPos.x);
        m_mousePos.y = static_cast<int32>(mousePos.y - windowPos.y - tabBarHeight);
        m_mousePos.y = m_size.y - m_mousePos.y;

        ImGui::Image(static_cast<ImTextureID>((uint64) m_fbo.GetColorAttachmentID(0)), { static_cast<float>(m_size.x), static_cast<float>(m_size.y) }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        if (Properties::GetSelectedData().type == SelectedData::Type::Entity) {

            InternalEntity* selectedEntity = Properties::GetSelectedData().entity;
            CU_ASSERT(selectedEntity, "Selected entity is invalid");

            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(windowPos.x, windowPos.y, static_cast<float>(m_size.x), m_size.y + tabBarHeight);

            Matrix4 projection = m_sceneCam.CreateProjectionMatrix();
            Matrix4 view = m_sceneCam.CreateViewMatrix();
            Matrix4 transform = selectedEntity->GetTransform()->GetTransformMatrix();

            bool snap = Input::GetKeyState(KeyCode::LeftControl) == KeyState::Down;
            float snapValues[3] = { 0.5f, 0.5f, 0.5f };

            ImGuizmo::Manipulate(&view.cols[0].x, &projection.cols[0].x, static_cast<ImGuizmo::OPERATION>(m_gizmo.operation), static_cast<ImGuizmo::MODE>(m_gizmo.globalMode), &transform.cols[0].x, nullptr, snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing()) {

                if (!m_gizmo.active) {

                    m_gizmo.active = true;

                    m_gizmo.savedPosition = selectedEntity->GetTransform()->GetPosition();
                    m_gizmo.savedRotation = selectedEntity->GetTransform()->GetRotation();
                    m_gizmo.savedScale = selectedEntity->GetTransform()->GetScale();

                }

                Vector3 position, scale;
                Quaternion rotation;
                Math::DecomposeTransform(transform, position, rotation, scale);

                selectedEntity->GetTransform()->SetGlobalPosition(position);
                selectedEntity->GetTransform()->SetGlobalRotation(rotation);
                selectedEntity->GetTransform()->SetGlobalScale(scale);

                if (RigidBody* rb = selectedEntity->GetComponent<RigidBody>()) {

                    rb->SetPosition(position);
                    rb->SetRotation(rotation);

                }

            } else if (m_gizmo.active) {

                m_gizmo.active = false;

                if (m_gizmo.savedPosition != selectedEntity->GetTransform()->GetPosition()) {

                    Change& change = AddChange(Change::Type::EntityMoved);
                    change << selectedEntity->GetID() << m_gizmo.savedPosition << selectedEntity->GetTransform()->GetPosition();

                } else if (m_gizmo.savedRotation != selectedEntity->GetTransform()->GetRotation()) {

                    Change& change = AddChange(Change::Type::EntityRotated);
                    change << selectedEntity->GetID() << m_gizmo.savedRotation << selectedEntity->GetTransform()->GetRotation();

                } else if (m_gizmo.savedScale != selectedEntity->GetTransform()->GetScale()) {

                    Change& change = AddChange(Change::Type::EntityScaled);
                    change << selectedEntity->GetID() << m_gizmo.savedScale << selectedEntity->GetTransform()->GetScale();

                }

            }

            if (ImGui::IsWindowFocused() && Input::GetKeyState(KeyCode::Delete) == KeyState::Pressed) {

                RemoveEntity(selectedEntity);
                SetChanges();

            }

        }

        m_sceneCam.SetCanLook(ImGui::IsItemHovered());

        // Gizmo controls
        // TODO: Add icons instead of text

        //static const float buttonSize = 20.f;
        const float buttonSize = ImGui::CalcTextSize("W").x + ImGui::GetStyle().FramePadding.x * 4.0f;

        ImGui::SetCursorPos({ ImGui::GetStyle().WindowPadding.x, ImGui::GetStyle().WindowPadding.y + tabBarHeight });
        if (ImGui::Button("P", { buttonSize, buttonSize }))
            m_gizmo.operation = Gizmo::Translate;

        ImGui::SameLine();
        if (ImGui::Button("R", { buttonSize, buttonSize }))
           m_gizmo.operation = Gizmo::Rotate; 

        ImGui::SameLine();
        if (ImGui::Button("S", { buttonSize, buttonSize }))
           m_gizmo.operation = Gizmo::Scale; 

        ImGui::PushStyleVarX(ImGuiStyleVar_ItemSpacing, ImGui::GetStyle().ItemSpacing.x * 2);

        ImGui::SameLine();
        if (ImGui::Button("W", { buttonSize, buttonSize }))
           m_gizmo.globalMode = true;

        ImGui::PopStyleVar();

        ImGui::SameLine();
        if (ImGui::Button("L", { buttonSize, buttonSize }))
           m_gizmo.globalMode = false;

        
        ImGui::End();

        CUP_END_FRAME();

    }

}
