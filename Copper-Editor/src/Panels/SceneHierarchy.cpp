#include "SceneHierarchy.h"

#include "Core/EditorApp.h"
#include "Core/Utils/ModelLoader.h"

#include "Panels/Properties.h"

#include "Engine/Renderer/Primitives.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

#define MOVE_RECT_HEIGHT 4.0f

using namespace Copper;

namespace Editor {

    uint32 clickedEntityID = INVALID_ENTITY_ID;

    Scene* SceneHierarchy::m_scene = nullptr;

    void SceneHierarchy::UI() {

        CUP_START_FRAME("Scene Hierarchy");

        if (ImGui::BeginPopupContextWindow("##Scene Hierarchy")) {

            PopupWindow();
            ImGui::EndPopup();

        }

        for (InternalEntity* entity : EntityView(GetScene())) {

            if (!entity) continue;
            if (entity->GetTransform()->Parent()) continue;

            DrawEntityNode(entity);

        }

        RemoveParentTarget();

        CUP_END_FRAME();

    }

    void SceneHierarchy::DrawEntityNode(InternalEntity* entity) {

        ImGui::PushID((uint32) (uint64) entity);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (Properties::GetSelectedDataType() == Properties::SelectedDataType::Entity && Properties::GetSelectedData().entity == entity)
            flags |= ImGuiTreeNodeFlags_OpenOnArrow;

        bool opened = ImGui::TreeNodeEx(entity, flags, entity->name.c_str());

        if (ImGui::IsItemClicked())
            clickedEntityID = entity->ID();

        if (ImGui::IsMouseReleased(0) && ImGui::IsItemHovered() && entity->ID() == clickedEntityID)
            Properties::SetSelectedEntity(entity);

        if (ImGui::BeginDragDropSource()) {

            uint32 data = entity->ID();

            ImGui::SetDragDropPayload("SCH_ENTITY_NODE", &data, sizeof(uint32), ImGuiCond_Once);
            ImGui::EndDragDropSource();

        }
        if (ImGui::BeginDragDropTarget()) {

            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_ENTITY_NODE"))
                entity->GetTransform()->AddChild(GetEntityFromID(*static_cast<uint32*>(payload->Data))->GetTransform());

            ImGui::EndDragDropTarget();

        }

        if (ImGui::BeginPopupContextItem()) {

            if (ImGui::MenuItem("Remove")) {

                m_scene->RemoveEntity(entity);

                if (Properties::GetSelectedDataType() == Properties::SelectedDataType::Entity && Properties::GetSelectedData().entity == entity)
                    Properties::ClearSelectedData();

                ImGui::EndPopup();
                ImGui::PopID();

                return;

            }

            ImGui::EndPopup();

        }

        if (opened) {

            for (uint32 i = 0; i < entity->GetTransform()->NumOfChildren(); i++) {

                DrawEntityNode(entity->GetTransform()->GetChild(i)->GetEntity());

            }

            ImGui::TreePop();

        }

        ImGui::PopID();

    }
    void SceneHierarchy::PopupWindow() {

        if (ImGui::MenuItem("Entity", 0, false, m_scene)) {
            
            Properties::SetSelectedEntity(m_scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one));
            SetChanges(true);

        }

        ImGui::Separator();

        if (ImGui::BeginMenu("3D Objects")) {

            if (ImGui::MenuItem("Plane", 0, false, m_scene)) {

                Entity selectedEntity = m_scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one, "Plane");

                MeshRenderer* renderer = selectedEntity->AddComponent<MeshRenderer>();
                renderer->mesh = PlaneMesh();
                renderer->material = Material::WhiteMaterial();

                SetChanges(true);
                Properties::SetSelectedEntity(selectedEntity);

            }
            if (ImGui::MenuItem("Cube", 0, false, m_scene)) {

                Entity selectedEntity = m_scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one, "Cube");

                MeshRenderer* renderer = selectedEntity->AddComponent<MeshRenderer>();
                renderer->mesh = CubeMesh();
                renderer->material = Material::WhiteMaterial();

                SetChanges(true);
                Properties::SetSelectedEntity(selectedEntity);

            }

            ImGui::EndMenu();

        }

        if (ImGui::MenuItem("Light", 0, false, m_scene)) {

            Entity selectedEntity = m_scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one, "Light");
            Light* l = selectedEntity->AddComponent<Light>();

            SetChanges(true);
            Properties::SetSelectedEntity(selectedEntity);

        }
        if (ImGui::MenuItem("Camera", 0, false, m_scene)) {

            Entity selectedEntity = m_scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one, "Camera");
            Camera* c = selectedEntity->AddComponent<Camera>();

            SetChanges(true);
            Properties::SetSelectedEntity(selectedEntity);

        }

    }

    void SceneHierarchy::RemoveParentTarget() {

        const ImVec2 regionMax = ImGui::GetWindowContentRegionMax();
        const ImRect windowRect{ { ImGui::GetWindowContentRegionMin().x + 1, ImGui::GetItemRectMax().y + 2 }, { regionMax.x, regionMax.y + 80 } };
        if (!ImGui::BeginDragDropTargetCustom(windowRect, ImGuiID(310320231753))) return;

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_ENTITY_NODE"))
            ((InternalEntity*) payload->Data)->GetTransform()->SetParent(nullptr);

        ImGui::EndDragDropTarget();

    }

    void SceneHierarchy::SetScene(Scene* scene) {

        CUP_FUNCTION();

        m_scene = scene;
        Properties::ClearSelectedData();

    }

}
