#include "Properties.h"

#include "Core/EditorApp.h"

#include "Projects/Project.h"

#include "Assets/ProjectAssetDatabase.h"
#include "Assets/Serializer.h"

#include "UI/TypeWidgets.h"

#include "Engine/Scripting/ScriptingEngine.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>

#include <cstring>

#define EditField(type, func) type value; field.GetValue(scriptComponent, &value);\
                              if (func(field.GetName(), &value)) field.SetValue(scriptComponent, &value)
#define EditRefField(type, func) type value; field.GetRefValue(scriptComponent, (void**) &value);\
                                 if (func(field.GetName(), &value)) field.SetRefValue(scriptComponent, value)

#define FRAME_WIDTH 241
#define FRAME_HEIGHT 24

using namespace Copper;

namespace Editor {

    static const char* s_lightTypes[] = { "Point", "Directional" };

    Properties::SelectedData Properties::m_selectedData = { fs::path("") };
    Properties::SelectedDataType Properties::m_selectedDataType = SelectedDataType::None;

    template<typename T> static bool DrawComponent(const std::string& name, T* component);
    static bool DrawComponent(const std::string& name, Transform* component);

    void Properties::UI() {

        CUP_FUNCTION();
        CUP_START_FRAME("Properties");

        if (m_selectedDataType == SelectedDataType::Entity)
            RenderEntity();
        else if (m_selectedDataType == SelectedDataType::File)
            RenderFile();

        CUP_END_FRAME();

    }

    void Properties::RenderEntity() {

        CU_ASSERT(m_selectedDataType == SelectedDataType::Entity, "Can't render an entity when one isn't selected!");

        if (!m_selectedData.entity) return;

        InternalEntity* entity = m_selectedData.entity;

        char buffer[128] = {};
        std::strncpy(buffer, entity->name.c_str(), sizeof(buffer));

        if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {

            entity->name = buffer;
            Editor::SetChanges(true);

        }

        ImGui::SameLine();
        ImGui::Separator();

        if(DrawComponent("Transform", entity->GetTransform())) {
            
            Transform* transform = entity->GetTransform();

            Vector3 pos = transform->Position();
            if (UI::EditVector3("Position", &pos))
                transform->SetPosition(pos);

            Vector3 newRot = transform->Rotation().EulerAngles();
            if (UI::EditVector3("Rotation", &newRot))
                transform->SetRotation(Quaternion(newRot));

            Vector3 scale = transform->Scale();
            if (UI::EditVector3("Scale", &scale))
                transform->SetScale(scale);

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
                Editor::SetChanges(true);

            } else if (ImGui::MenuItem("Mesh Renderer")) {

                entity->AddComponent<MeshRenderer>();
                Editor::SetChanges(true);

            } else if (ImGui::MenuItem("Camera")) {

                entity->AddComponent<Camera>();
                Editor::SetChanges(true);

            } else if (ImGui::MenuItem("Rigid Body")) {

                entity->AddComponent<RigidBody>();
                Editor::SetChanges(true);

            } else if (ImGui::MenuItem("Box Collider") && !collider) {

                entity->AddComponent<BoxCollider>();
                Editor::SetChanges(true);

            } else if (ImGui::MenuItem("Sphere Collider") && !collider) {

                entity->AddComponent<SphereCollider>();
                Editor::SetChanges(true);

            } else if (ImGui::MenuItem("Capsule Collider") && !collider) {

                entity->AddComponent<CapsuleCollider>();
                Editor::SetChanges(true);

            }

            ImGui::Separator();

            const Scripting::ScriptMap& componentScripts = Scripting::ComponentScripts();
            for (Scripting::ScriptMap::const_iterator it = componentScripts.begin(); it != componentScripts.end(); ++it) {

                const Scripting::Script& script = it->second;
                if (!ImGui::MenuItem(it->first.c_str())) continue;

                ScriptComponent* component = entity->AddComponent<ScriptComponent>();
                component->Setup(&script);
                Editor::SetChanges(true);

                break;

            }

            ImGui::EndPopup();

        }

    }
    void Properties::RenderFile() {

        CU_ASSERT(m_selectedDataType == SelectedDataType::File, "Can't render file when a file is not selected!");
        if (m_selectedData.file.empty()) return;

        ImGui::Text(m_selectedData.file.string().c_str());
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
        if (asset == UUID::GetInvalid()) {

            LogError("Can't serialize an asset that isn't loaded! Path: {}", m_selectedData.file);
            return;

        }

        if (m_selectedData.file.extension().string() == ".mat")
            AssetFile::SerializeMaterial(GetProject().GetAssetsPath() / m_selectedData.file, asset);

    }

    // Components

    void Properties::RenderMeshRenderer(Copper::MeshRenderer* renderer) {

        if (!DrawComponent<MeshRenderer>("Mesh Renderer", renderer)) return;

        UI::EditMaterial("Material", &renderer->material);

        ImGui::PopID();

    }
    void Properties::RenderLight(Light* light) {

        if (!DrawComponent<Light>("Light", light)) return;

        UI::EditDropDown("Type", s_lightTypes, LIGHT_TYPES, (uint8*) &light->type);

        UI::EditColor("Color", &light->color);
        UI::EditFloat("Intensity", &light->intensity);

        ImGui::PopID();

    }
    void Properties::RenderCamera(Camera* camera) {

        if (!DrawComponent<Camera>("Camera", camera)) return;

        UI::EditFloat("FOV", &camera->fov);
        UI::EditFloat("Near Plane", &camera->nearPlane);
        UI::EditFloat("Far Plane", &camera->farPlane);

        ImGui::PopID();

    }

    void Properties::RenderRigidBody(RigidBody* rb) {

        if (!DrawComponent<RigidBody>("Rigid Body", rb)) return;

        if (UI::EditFloat("Mass", &rb->m_mass)) rb->SetMass(rb->m_mass);

        if (UI::EditBool("Static", &rb->m_static)) rb->SetStatic(rb->m_static);
        if (UI::EditBool("Gravity", &rb->m_gravity)) rb->SetGravity(rb->m_gravity);

        if (ImGui::TreeNode("Locks")) {

            // Position Lock

            if (UI::EditMask("Position", (uint32&) rb->m_lockMask, 3)) rb->SetLockMask(rb->m_lockMask);
            if (UI::EditMask("Rotation", (uint32&) rb->m_lockMask, 3, 3)) rb->SetLockMask(rb->m_lockMask);

            ImGui::TreePop();

        }

        ImGui::PopID();

    }

    void Properties::RenderBoxCollider(BoxCollider* collider) {

        if (!DrawComponent<BoxCollider>("Box Collider", collider)) return;

        if (UI::EditBool("Trigger", &collider->m_trigger)) collider->SetTrigger(collider->m_trigger);
        if (UI::EditVector3("Center", &collider->m_center)) collider->SetCenter(collider->m_center);

        if (UI::EditVector3("Size", &collider->m_size)) collider->SetSize(collider->m_size);

        ImGui::PopID();

    }
    void Properties::RenderSphereCollider(SphereCollider* collider) {

        if (!DrawComponent<SphereCollider>("Sphere Collider", collider)) return;

        if (UI::EditBool("Trigger", &collider->m_trigger)) collider->SetTrigger(collider->m_trigger);
        if (UI::EditVector3("Center", &collider->m_center)) collider->SetCenter(collider->m_center);

        ImGui::Separator();

        if (UI::EditFloat("Radius", &collider->m_radius)) collider->SetRadius(collider->m_radius);

        ImGui::PopID();

    }
    void Properties::RenderCapsuleCollider(CapsuleCollider* collider) {

        if (!DrawComponent<CapsuleCollider>("Capsule Collider", collider)) return;

        if (UI::EditBool("Trigger", &collider->m_trigger)) collider->SetTrigger(collider->m_trigger);
        if (UI::EditVector3("Center", &collider->m_center)) collider->SetCenter(collider->m_center);

        ImGui::Separator();

        if (UI::EditFloat("Radius", &collider->m_radius)) collider->SetRadius(collider->m_radius);
        if (UI::EditFloat("Height", &collider->m_height)) collider->SetHeight(collider->m_height);

        ImGui::PopID();

    }
    void Properties::RenderScriptComponent(ScriptComponent* scriptComponent) {

        const Scripting::Script* script = scriptComponent->GetScript();
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

                if (UI::EditEntity(field.GetName(), &entity)) {

                    id = entity->ID();
                    field.SetRefValue(scriptComponent, (void*) id);

                }

                break;

            }
            case Scripting::Field::Type::Transform: { EditRefField(Transform*, UI::EditTransform); break; }

            }

        }

        ImGui::PopID();

    }

    template<typename T> static bool DrawComponent(const std::string& name, T* component) {

        ImGui::PushID((uint32) (uint64) component);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 {4, 4});

        bool opened = ImGui::TreeNodeEx((void*) component, flags, name.c_str());

        ImGui::PopStyleVar();
        if (opened)
            ImGui::TreePop();
        else
            ImGui::PopID();

        if (ImGui::BeginPopupContextItem()) {

            if (ImGui::MenuItem("Remove Component")) {

                component->GetEntity()->template RemoveComponent<T>();

                SetChanges(true);
                ImGui::EndPopup();
                ImGui::PopID();

                return false;

            }

            ImGui::EndPopup();

        }

        return opened;

    }
    static bool DrawComponent(const std::string& name, Transform* component) {

        ImGui::PushID((uint32) (uint64) component);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });

        bool opened = ImGui::TreeNodeEx((void*) component, flags, name.c_str());

        ImGui::PopStyleVar();
        if (opened)
            ImGui::TreePop();
        else
            ImGui::PopID();

        return opened;

    }

    // Assets

    void Properties::RenderMaterial() {

        const MaterialAsset& material = ProjectAssetDatabase::GetAssetFromPath(m_selectedData.file);
        if (material == UUID::GetInvalid()) {

            LogWarn("Selected File is not found in the AssetFileDatabase, try refreshing.\n\tPath: {}", GetProject().GetAssetsPath() / m_selectedData.file);
            m_selectedDataType = SelectedDataType::None;

            return;

        }

        const std::string name = m_selectedData.file.filename().string();
        ImGui::Text(name.c_str());
        ImGui::NewLine();

        UI::EditTexture("Texture", &material->texture);
        UI::EditColor("Albedo", &material->albedo);
        UI::EditFloat("Tiling", &material->tiling);

    }

}
