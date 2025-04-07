#include "SceneHierarchy.h"

#include "Core/EditorApp.h"
#include "Core/ChangeHandler.h"
#include "Core/Clipboard.h"

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

        if (ImGui::BeginPopupContextWindow("##Scene Hierarchy"))
            PopupWindow();

        for (InternalEntity* entity : EntityView(GetScene())) {

            if (!entity) continue;
            if (entity->GetTransform()->Parent()) continue;

            DrawEntityNode(entity);

        }

        RemoveParentTarget();

        if (Properties::GetSelectedData().type == SelectedData::Type::Entity && ImGui::IsWindowFocused() && Input::GetKeyState(KeyCode::Delete) == KeyState::Pressed) {

            CU_ASSERT(Properties::GetSelectedData().entity, "Selected entity is invalid, can't delete!");

            m_entityToRemove = Properties::GetSelectedData().entity;
            Properties::ClearSelectedData();

            SetChanges();

        }

        if (m_entityToRemove) {

            m_scene->RemoveEntity(m_entityToRemove);
            m_entityToRemove = Entity();

        }


        CUP_END_FRAME();

    }

    void SceneHierarchy::DrawEntityNode(InternalEntity* entity) {

        ImGui::PushID((uint32) (uint64) entity);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 min = { pos.x + ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.x, pos.y };
        ImVec2 max = { pos.x + ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, pos.y + ImGui::GetTextLineHeight() };

        if (ImGui::IsMouseHoveringRect(min, max)) {

            switch (Input::GetKeyState(KeyCode::Mouse0)) {

            case KeyState::Pressed: clickedEntityID = entity->ID(); break;
            case KeyState::Released: Properties::SetSelectedEntity(entity); break;
            default: break;

            }

        }

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (Properties::GetSelectedData().type == SelectedData::Type::Entity && Properties::GetSelectedData().entity == entity)
            flags |= ImGuiTreeNodeFlags_Selected;

        bool opened = ImGui::TreeNodeEx(entity, flags, entity->name.c_str());

        if (ImGui::BeginDragDropSource()) {

            uint32 data = entity->ID();

            ImGui::SetDragDropPayload("SCH_ENTITY_NODE", &data, sizeof(uint32), ImGuiCond_Once);
            ImGui::EndDragDropSource();

        }
        if (ImGui::BeginDragDropTarget()) {

            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_ENTITY_NODE")) {

                Entity child = GetEntityFromID(*static_cast<uint32*>(payload->Data));
                entity->GetTransform()->AddChild(child->GetTransform());

                SetChanges();

            }

            ImGui::EndDragDropTarget();

        }

        if (ImGui::BeginPopupContextItem())
            EntityPopup(entity);

        if (opened) {

            for (uint32 i = 0; i < entity->GetTransform()->NumOfChildren(); i++) {

                DrawEntityNode(entity->GetTransform()->GetChild(i)->GetEntity());

            }

            ImGui::TreePop();

        }

        ImGui::PopID();

    }

    void SceneHierarchy::CommonPopup(Entity& newEntity) {

        CUP_FUNCTION();

        if (ImGui::MenuItem("Paste", nullptr, false, !ClipboardEmpty()))
            newEntity = PasteFromClipboard();

        ImGui::Separator();

        if (ImGui::MenuItem("Entity", 0, false, m_scene))
            newEntity = m_scene->CreateEntity();

        if (ImGui::BeginMenu("3D Objects")) {

            if (ImGui::MenuItem("Plane", 0, false, m_scene)) {

                newEntity = m_scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one, "Plane");
                MeshRenderer* renderer = newEntity->AddComponent<MeshRenderer>();

                renderer->mesh = PlaneMesh();
                renderer->material = Material::WhiteMaterial();

            }
            if (ImGui::MenuItem("Cube", 0, false, m_scene)) {

                newEntity = m_scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one, "Cube");
                MeshRenderer* renderer = newEntity->AddComponent<MeshRenderer>();

                renderer->mesh = CubeMesh();
                renderer->material = Material::WhiteMaterial();

            }

            ImGui::EndMenu();

        }

        if (ImGui::MenuItem("Light", 0, false, m_scene)) {

            newEntity = m_scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one, "Light");
            Light* l = newEntity->AddComponent<Light>();

        }
        if (ImGui::MenuItem("Camera", 0, false, m_scene)) {

            newEntity = m_scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one, "Camera");
            Camera* c = newEntity->AddComponent<Camera>();

        }

        if (newEntity) {

            Properties::SetSelectedEntity(newEntity);
            SetChanges();

        } 

    }
    void SceneHierarchy::EntityPopup(Entity entity) {

        CUP_FUNCTION();

        if (ImGui::MenuItem("Remove")) {

            if (Properties::GetSelectedData().type == SelectedData::Type::Entity && Properties::GetSelectedData().entity == entity)
                Properties::ClearSelectedData();

            m_entityToRemove = entity;
            SetChanges();

        }

        if (ImGui::MenuItem("Copy"))
            CopyToClipboard(entity);

        Entity newEntity;

        CommonPopup(newEntity);
        if (newEntity)
            newEntity->GetTransform()->SetParent(entity->GetTransform());

        ImGui::EndPopup();

    }
    void SceneHierarchy::PopupWindow() {

        Entity newEntity;
        CommonPopup(newEntity);

        ImGui::EndPopup();

    }

    void SceneHierarchy::RemoveParentTarget() {

        const ImVec2 regionMax = ImGui::GetWindowContentRegionMax();
        const ImRect windowRect{ { ImGui::GetWindowContentRegionMin().x + 1, ImGui::GetItemRectMax().y + 2 }, { regionMax.x, regionMax.y + 80 } };

        if (!ImGui::BeginDragDropTargetCustom(windowRect, ImGuiID(310320231753))) return;

        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_ENTITY_NODE")) {

            Entity entity = GetEntityFromID(*static_cast<uint32*>(payload->Data));
            entity->GetTransform()->SetParent(nullptr);

            SetChanges();

        }
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FB_MODEL")) {

            Entity entity = CreateEntity();

            MeshRenderer* meshRenderer = entity->AddComponent<MeshRenderer>();
            CU_ASSERT(meshRenderer != nullptr, "Could not add mesh renderer to entity {}", *entity);

            meshRenderer->mesh = *static_cast<MeshAsset*>(payload->Data);

            SetChanges();

        }

        ImGui::EndDragDropTarget();

    }

    void SceneHierarchy::SetScene(Scene* scene) {

        CUP_FUNCTION();

        m_scene = scene;

    }

}
