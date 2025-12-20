#include "Properties.h"

#include "Core/EditorApp.h"
#include "Core/ChangeHandler.h"

#include "Projects/Project.h"

#include "Assets/ProjectAssetDatabase.h"
#include "Assets/Serializer.h"

#include "UI/Widgets.h"
#include "UI/TypeWidgets.h"

#include <Engine/Components/BoxCollider.h>
#include <Engine/Components/Camera.h>
#include <Engine/Components/CapsuleCollider.h>
#include <Engine/Components/Light.h>
#include <Engine/Components/MeshRenderer.h>
#include <Engine/Components/RigidBody.h>
#include <Engine/Components/ScriptComponent.h>
#include <Engine/Components/SphereCollider.h>

#include <Engine/Scripting/Script.h>
#include <Engine/Scripting/ScriptingEngine.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>

#include <cstring>

#define EditField(type, func) type value; field.GetValue(scriptComponent, &value);\
                              if (func(field.GetName(), &value)) { field.SetValue(scriptComponent, &value); SetChanges(); }
#define EditRefField(type, func) type value; field.GetRefValue(scriptComponent, (void**) &value);\
                                 if (func(field.GetName(), &value)) { field.SetRefValue(scriptComponent, value); SetChanges(); }

#define FRAME_WIDTH 241
#define FRAME_HEIGHT 24

#define HEADER_MARGIN 0.0f

using namespace Copper;

namespace Editor {

    static const char* s_lightTypes[] = { "Point", "Directional" };

    SelectedData Properties::m_selectedData = SelectedData();

    template<typename T> static bool DrawComponent(const std::string& name, T* component);
    static bool DrawComponent(const std::string& name, Transform* component);

    void Properties::UI() {

        CUP_FUNCTION();
        CUP_START_FRAME("Properties");

        if (m_selectedData.type == SelectedData::Type::Entity)
            RenderEntity();
        else if (m_selectedData.type == SelectedData::Type::File)
            RenderFile();

        CUP_END_FRAME();

    }

    void Properties::RenderEntity() {

        CUP_FUNCTION();
        CU_ASSERT(m_selectedData.type == SelectedData::Type::Entity, "Can't render an entity when one isn't selected!");

        if (!m_selectedData.entity) return;

        static const ImGuiStyle& style = ImGui::GetStyle();
        ImGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, style.ItemSpacing.y + 1.0f);

        InternalEntity* entity = m_selectedData.entity;

        char buffer[128] = {};
        std::strncpy(buffer, entity->name.c_str(), sizeof(buffer));

        if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {

            entity->name = buffer;
            SetChanges();

        }

        if(DrawComponent("Transform", entity->GetTransform())) {

            Transform* transform = entity->GetTransform();

            Vector3 pos = transform->GetPosition();
            if (UI::EditVector3("Position", &pos)) {

                transform->SetPosition(pos);

                if (RigidBody* rb = entity->GetComponent<RigidBody>())
                    rb->SetPosition(pos);

                SetChanges();

            }

            Vector3 newRot = transform->GetRotation().EulerAngles();
            if (UI::EditVector3("Rotation", &newRot)) {

                Quaternion rot = Quaternion(newRot);
                transform->SetRotation(rot);

                if (RigidBody* rb = entity->GetComponent<RigidBody>())
                    rb->SetRotation(rot);

                SetChanges();

            }

            Vector3 scale = transform->GetScale();
            if (UI::EditVector3("Scale", &scale)) {

                transform->SetScale(scale);

                SetChanges();

            }

            ImGui::PopID();

        }

        if (MeshRenderer* renderer = entity->GetComponent<MeshRenderer>()) RenderMeshRenderer(renderer);
        if (Light* light = entity->GetComponent<Light>()) RenderLight(light);
        if (Camera* camera = entity->GetComponent<Camera>()) RenderCamera(camera);

        if (RigidBody* rb = entity->GetComponent<RigidBody>()) RenderRigidBody(rb);

        if (BoxCollider* collider = entity->GetComponent<BoxCollider>()) RenderBoxCollider(collider);
        if (SphereCollider* collider = entity->GetComponent<SphereCollider>()) RenderSphereCollider(collider);
        if (CapsuleCollider* collider = entity->GetComponent<CapsuleCollider>()) RenderCapsuleCollider(collider);

        if (ScriptComponent* scriptComponent = entity->GetComponent<ScriptComponent>()) RenderScriptComponent(scriptComponent);

        ImGui::Spacing();
        //ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        float width = ImGui::GetWindowWidth();
        ImVec2 size = ImVec2(125, 30);

        ImGui::SetCursorPosX((width - size.x) * 0.5f);
        if(ImGui::Button("Add Component", size)) {

            ImGui::OpenPopup("##AddComponent");

        }

        if(ImGui::BeginPopup("##AddComponent")) {

            Collider* collider = entity->GetComponent<Collider>();

            if (ImGui::MenuItem("Light")) {

                entity->AddComponent<Light>()->color.r = 0.5f;
                SetChanges();

            } else if (ImGui::MenuItem("Mesh Renderer")) {

                entity->AddComponent<MeshRenderer>();
                SetChanges();

            } else if (ImGui::MenuItem("Camera")) {

                entity->AddComponent<Camera>();
                SetChanges();

            } else if (ImGui::MenuItem("Rigid Body")) {

                RigidBody* rb = entity->AddComponent<RigidBody>();
                if (collider != nullptr)
                    rb->Initialize();

                SetChanges();

            } else if (ImGui::MenuItem("Box Collider") && collider == nullptr) {

                entity->AddComponent<BoxCollider>();
                if (RigidBody* rb = entity->GetComponent<RigidBody>())
                    rb->Initialize();

                SetChanges();

            } else if (ImGui::MenuItem("Sphere Collider") && collider == nullptr) {

                entity->AddComponent<SphereCollider>();
                if (RigidBody* rb = entity->GetComponent<RigidBody>())
                    rb->Initialize();

                SetChanges();

            } else if (ImGui::MenuItem("Capsule Collider") && collider == nullptr) {

                entity->AddComponent<CapsuleCollider>();
                if (RigidBody* rb = entity->GetComponent<RigidBody>())
                    rb->Initialize();

                SetChanges();

            }

            ImGui::Separator();

            const Scripting::ScriptMap& componentScripts = Scripting::ComponentScripts();
            for (Scripting::ScriptMap::const_iterator it = componentScripts.begin(); it != componentScripts.end(); ++it) {

                const Scripting::Script& script = it->second;
                if (!ImGui::MenuItem(it->first.c_str())) continue;

                ScriptComponent* component = entity->AddComponent<ScriptComponent>();
                component->Setup(&script);
                SetChanges();

                break;

            }

            ImGui::EndPopup();

        }

        ImGui::PopStyleVar();

    }
    void Properties::RenderFile() {

        CUP_FUNCTION();
        CU_ASSERT(m_selectedData.type == SelectedData::Type::File, "Can't render file when a file is not selected!");

        if (m_selectedData.file.empty()) return;

        ImGui::TextUnformatted(m_selectedData.file.string().c_str());
        ImGui::Separator();

        std::string extension = m_selectedData.file.extension().string();

        if (extension == ".mat")
            RenderMaterial();
        else {

            ImGui::TextWrapped("This extension is not supported, make sure you called a function for this specific extension!");
            return;

        }

        const ImGuiStyle& style = ImGui::GetStyle();
        ImGui::PushStyleVarX(ImGuiStyleVar_FramePadding, style.FramePadding.x * 6.0f);
        ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, style.FramePadding.y * 2.5f);

        const ImVec2* framePadding = (const ImVec2*) ImGui::GetStyleVarInfo(ImGuiStyleVar_FramePadding)->GetVarPtr((void*) &style);
        ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Save").x - framePadding->x - style.WindowPadding.x - style.ItemSpacing.x);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + style.ItemSpacing.y);

        if (ImGui::Button("Save"))
            SerializeFile();

        ImGui::PopStyleVar(2);

    }

    void Properties::SerializeFile() {

        CUP_FUNCTION();

        const UUID& asset = ProjectAssetDatabase::GetAssetFromPath(m_selectedData.file);
        if (!asset.IsValid()) {

            LogError("Can't serialize an asset that isn't loaded! Path: {}", m_selectedData.file);
            return;

        }

        if (m_selectedData.file.extension().string() == ".mat")
            AssetFile::SerializeMaterial(GetProject().GetAssetsPath() / m_selectedData.file, asset);

    }

    // Components

    void Properties::RenderMeshRenderer(Copper::MeshRenderer* renderer) {

        CUP_FUNCTION();

        if (!DrawComponent<MeshRenderer>("Mesh Renderer", renderer)) return;

        if (UI::EditMaterial("Material", &renderer->material))
            SetChanges();

        ImGui::PopID();

    }
    void Properties::RenderLight(Light* light) {

        CUP_FUNCTION();

        if (!DrawComponent<Light>("Light", light)) return;

        if (UI::EditDropDown("Type", s_lightTypes, LIGHT_TYPES, (uint8*) &light->type))
            SetChanges();

        if (UI::EditColor("Color", &light->color))
            SetChanges();
        if (UI::EditFloat("Intensity", &light->intensity))
            SetChanges();

        ImGui::PopID();

    }
    void Properties::RenderCamera(Camera* camera) {

        CUP_FUNCTION();

        if (!DrawComponent<Camera>("Camera", camera)) return;

        if (UI::EditFloat("FOV", &camera->fov))
            SetChanges();
        if (UI::EditFloat("Near plane", &camera->nearPlane))
            SetChanges();
        if (UI::EditFloat("Far plane", &camera->farPlane))
            SetChanges();

        ImGui::PopID();

    }

    void Properties::RenderRigidBody(RigidBody* rb) {

        CUP_FUNCTION();

        if (!DrawComponent<RigidBody>("Rigid Body", rb)) return;

        if (!rb->IsValid())
            UI::InfoLabel(GetWarnIcon(), "RigidBody is missing a Collider component!");

        if (UI::EditFloat("Mass (kg)", &rb->m_mass)) {

            rb->SetMass(rb->m_mass);
            SetChanges();

        }

        if (UI::EditBool("Static", &rb->m_static)) {

            rb->SetStatic(rb->m_static);
            SetChanges();

        }
        if (UI::EditBool("Gravity", &rb->m_gravity)) {

            rb->SetGravity(rb->m_gravity);
            SetChanges();

        }

        if (ImGui::TreeNode("Locks")) {

            // Position Lock

            if (UI::EditMask("Position", (uint32&) rb->m_lockMask, 3)) {

                rb->SetLockMask(rb->m_lockMask);
                SetChanges();

            }
            if (UI::EditMask("Rotation", (uint32&) rb->m_lockMask, 3, 3)) {

                rb->SetLockMask(rb->m_lockMask);
                SetChanges();

            }

            ImGui::TreePop();

        }

        ImGui::PopID();

    }

    void Properties::RenderCollider(Collider* collider) {

        CUP_FUNCTION();

        if (!collider->IsValid())
            UI::InfoLabel(GetWarnIcon(), "Collider is missing a RigidBody component!");

        if (UI::EditBool("Trigger", &collider->m_trigger)) {

            collider->SetTrigger(collider->m_trigger);
            SetChanges();

        }
        if (UI::EditVector3("Center", &collider->m_center)) {

            collider->SetCenter(collider->m_center);
            SetChanges();

        }

        ImGui::Separator();

    }
    void Properties::RenderBoxCollider(BoxCollider* collider) {

        CUP_FUNCTION();

        if (!DrawComponent<BoxCollider>("Box Collider", collider)) return;
        RenderCollider(collider);

        if (UI::EditVector3("Size", &collider->m_size)) {

            collider->SetSize(collider->m_size);
            SetChanges();

        }

        ImGui::PopID();

    }
    void Properties::RenderSphereCollider(SphereCollider* collider) {

        CUP_FUNCTION();

        if (!DrawComponent<SphereCollider>("Sphere Collider", collider)) return;
        RenderCollider(collider);

        if (UI::EditFloat("Radius", &collider->m_radius)) {

            collider->SetRadius(collider->m_radius);
            SetChanges();

        }

        ImGui::PopID();

    }
    void Properties::RenderCapsuleCollider(CapsuleCollider* collider) {

        CUP_FUNCTION();

        if (!DrawComponent<CapsuleCollider>("Capsule Collider", collider)) return;
        RenderCollider(collider);

        if (UI::EditFloat("Radius", &collider->m_radius)) {

            collider->SetRadius(collider->m_radius);
            SetChanges();

        }
        if (UI::EditFloat("Height", &collider->m_height)) {

            collider->SetHeight(collider->m_height);
            SetChanges();

        }

        ImGui::PopID();

    }
    void Properties::RenderScriptComponent(ScriptComponent* scriptComponent) {

        CUP_FUNCTION();

        const Scripting::Script* script = scriptComponent->GetScript();
        if (script == nullptr) return;

        if (!DrawComponent<ScriptComponent>(script->Name(), scriptComponent)) return;

        const std::vector<Scripting::Field>& fields = script->GetFields();
        for (const Scripting::Field& field : fields) {

            switch (field.GetType()) {

            case Scripting::Field::Type::Int: { EditField(int32, UI::EditInt); break; }
            case Scripting::Field::Type::UInt: { EditField(uint32, UI::EditUInt); break; }
            case Scripting::Field::Type::Float: { EditField(float, UI::EditFloat); break; }

            case Scripting::Field::Type::Vector2: { EditField(Vector2, UI::EditVector2); break; }
            case Scripting::Field::Type::Vector3: { EditField(Vector3, UI::EditVector3); break; }

            case Scripting::Field::Type::Entity: {

                uint64 id;
                field.GetRefValue(scriptComponent, (void**) &id, (void*) INVALID_ENTITY_ID);
                InternalEntity* entity = GetEntityFromID((uint32) id);

                if (entity == nullptr && id != INVALID_ENTITY_ID) {

                    LogError("Entity field '{}' on Entity '{}' has an invalid, presumably missing value. Value (entity ID): {}", field.GetName(), *scriptComponent->GetEntity(), id);
                    field.SetValue(scriptComponent, nullptr);

                }

                if (UI::EditEntity(field.GetName(), &entity)) {

                    id = entity->GetID();
                    field.SetRefValue(scriptComponent, (void*) id);

                    SetChanges();

                }

                break;

            }
            case Scripting::Field::Type::Transform: {

                Transform* value;
                field.GetRefValue(scriptComponent, (void**) &value);

                if (value != nullptr && !value->GetEntity().IsValid() && value->GetEntity().ID() != INVALID_ENTITY_ID) {

                    LogError("Transform field '{}' on Entity '{}' has an invalid, presumably missing value. Value (entity ID): {}", field.GetName(), *scriptComponent->GetEntity(), value->GetEntity().ID());
                    field.SetValue(scriptComponent, nullptr);

                    value = nullptr;

                }

                if (UI::EditTransform(field.GetName(), &value)) {

                    field.SetRefValue(scriptComponent, value);
                    SetChanges();

                }

                break;

            }
            case Scripting::Field::Type::Material: {

                MaterialAsset asset;
                field.GetAssetValue(scriptComponent, (UUID*) &asset);

                if (UI::EditMaterial(field.GetName(), &asset)) {

                    field.SetAssetValue(scriptComponent, asset.AssetUUID());
                    SetChanges();

                }

                break;

            }
            default: break;

            }

        }

        ImGui::PopID();

    }

    template<typename T> static bool DrawComponent(const std::string& name, T* component) {

        CUP_FUNCTION();

        ImGui::PushID((uint32) (uint64) component);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 {4, 4});

        ImGui::Dummy(ImVec2(0.0f, HEADER_MARGIN));
        bool opened = ImGui::TreeNodeEx((void*) component, flags, "%s", name.c_str());

        ImGui::PopStyleVar();
        if (opened)
            ImGui::TreePop();
        else
            ImGui::PopID();

        if (ImGui::BeginPopupContextItem()) {

            if (ImGui::MenuItem("Remove Component")) {

                component->GetEntity()->template RemoveComponent<T>();

                SetChanges();
                ImGui::EndPopup();

                if (opened)
                    ImGui::PopID();

                return false;

            }

            ImGui::EndPopup();

        }

        if (opened)
            ImGui::Dummy(ImVec2(0.0f, HEADER_MARGIN));

        return opened;

    }
    static bool DrawComponent(const std::string& name, Transform* component) {

        CUP_FUNCTION();

        ImGui::PushID((uint32) (uint64) component);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });

        ImGui::Dummy(ImVec2(0.0f, HEADER_MARGIN));
        bool opened = ImGui::TreeNodeEx((void*) component, flags, "%s", name.c_str());

        ImGui::PopStyleVar();
        if (opened)
            ImGui::TreePop();
        else
            ImGui::PopID();

        if (opened)
            ImGui::Dummy(ImVec2(0.0f, HEADER_MARGIN));

        return opened;

    }

    // Assets

    void Properties::RenderMaterial() {

        CUP_FUNCTION();

        const MaterialAsset& material = ProjectAssetDatabase::GetAssetFromPath(m_selectedData.file);
        if (!material.IsValid()) {

            LogWarn("Selected File is not found in the AssetFileDatabase, try refreshing.\n\tPath: {}", GetProject().GetAssetsPath() / m_selectedData.file);
            m_selectedData.type = SelectedData::Type::None;

            return;

        }

        if (UI::EditTexture("Texture", &material->texture))
            SetChanges();

        if (UI::EditColor("Albedo", &material->albedo))
            SetChanges();
        if (UI::EditFloat("Tiling", &material->tiling))
            SetChanges();

    }

}
