#include "Properties.h"

#include "Engine/Scripting/ScriptingCore.h"

#include "Core/EditorApp.h"
#include "Core/SceneMeta.h"

#include "UI/TypeWidgets.h"

#include "Panels/SceneHierarchy.h"

#include "Viewport/SceneCamera.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>

#include <cstring>

#define BindShowFunc(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

#define FRAME_WIDTH 241
#define FRAME_HEIGHT 24

using namespace Copper;

namespace Editor {

	static const char* s_lightTypes[] = { "Point", "Directional" };

	template<typename T> static bool DrawComponent(const std::string& name, T* component);
	static bool DrawComponent(const std::string& name, Transform* component);

	Properties::Properties() : Panel("Properties") {

	}

	void Properties::UI() {

		if (!*m_selectedEntity) return;
		CUP_START_FRAME("Properties");

		RenderEntity();

		CUP_END_FRAME();

	}

	void Properties::RenderEntity() {

		InternalEntity* entity = *m_selectedEntity;

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

		if (ScriptComponent* script = entity->GetComponent<ScriptComponent>()) RenderScriptComponent(script);

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

			for (std::string scriptName : Scripting::GetScriptComponents()) {

				if (ImGui::MenuItem(scriptName.c_str())) {
					
					ScriptComponent* script = entity->AddComponent<ScriptComponent>();

					script->Init(scriptName);
					Editor::SetChanges(true);
						
				}

			}

			ImGui::EndPopup();
				
		}
		
	}

	void Properties::RenderMeshRenderer(Copper::MeshRenderer* renderer) {

		if (!DrawComponent<MeshRenderer>("Mesh Renderer", renderer)) return;

		Mesh& mesh = renderer->meshes[0];

		UI::EditTexture("Texture", &mesh.material.texture);
		UI::EditColor("Albedo", &mesh.material.albedo);
		UI::EditFloat("Tiling", &mesh.material.tiling);

		ImGui::PopID();

	}
	void Properties::RenderLight(Light* light) {

		if (!DrawComponent<Light>("Light", light)) return;

		UI::EditDropDown("Type", s_lightTypes, LIGHT_TYPES, (uint32*) &light->type);

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

		bool changed = false;

		if (UI::EditBool("Static", &rb->isStatic) && !changed) changed = true;
		if (UI::EditBool("Gravity", &rb->gravity) && !changed) changed = true;

		if (UI::EditFloat("Mass", &rb->mass) && !changed) changed = true;

		if (ImGui::TreeNode("Locks")) {

			// Position Lock

			if (UI::EditMask("Position", (uint32&) rb->m_lockMask, 3) && !changed) changed = true;
			if (UI::EditMask("Rotation", (uint32&) rb->m_lockMask, 3, 3) && !changed) changed = true;

			ImGui::TreePop();
				
		}

		if (changed && IsSceneRuntimeRunning()) rb->Setup();

		ImGui::PopID();

	}
	
	void Properties::RenderBoxCollider(BoxCollider* collider) {

		if (!DrawComponent<BoxCollider>("Box Collider", collider)) return;

		UI::EditBool("Trigger", &collider->trigger);
		UI::EditVector3("Center", &collider->center);

		UI::EditVector3("Size", &collider->size);

		ImGui::PopID();

	}
	void Properties::RenderSphereCollider(SphereCollider* collider) {

		if (!DrawComponent<SphereCollider>("Sphere Collider", collider)) return;

		UI::EditBool("Trigger", &collider->trigger);
		UI::EditVector3("Center", &collider->center);

		ImGui::Separator();

		UI::EditFloat("Radius", &collider->radius);

		ImGui::PopID();

	}
	void Properties::RenderCapsuleCollider(CapsuleCollider* collider) {

		if (!DrawComponent<CapsuleCollider>("Capsule Collider", collider)) return;

		UI::EditBool("Trigger", &collider->trigger);
		UI::EditVector3("Center", &collider->center);

		ImGui::Separator();

		UI::EditFloat("Radius", &collider->radius);
		UI::EditFloat("Height", &collider->height);

		ImGui::PopID();

	}

	void Properties::RenderScriptComponent(ScriptComponent* script) {

		if (!DrawComponent<ScriptComponent>(script->name, script)) return;
		if (!*script) {

			ImGui::Text("This Script is invalid or doesn't exist anymore");
			ImGui::PopID();

			return;

		}

		for (ScriptField& field : Scripting::GetScriptFields(script->name)) {

			switch (field.type) {

				case ScriptField::Type::Int:			RenderScriptField<int32>(script, field, UI::EditInt); break;
				case ScriptField::Type::UInt:			RenderScriptField<uint32>(script, field, UI::EditUInt); break;
				case ScriptField::Type::Float:			RenderScriptField<float>(script, field, UI::EditFloat); break;

				case ScriptField::Type::Vector2:		RenderScriptField<Vector2>(script, field, UI::EditVector2); break;
				case ScriptField::Type::Vector3:		RenderScriptField<Vector3>(script, field, UI::EditVector3); break;

				case ScriptField::Type::Entity:			RenderScriptField<InternalEntity*>(script, field, UI::EditEntity); break;
				case ScriptField::Type::Transform:		RenderScriptField<Transform*>(script, field, UI::EditTransform); break;

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
				
				component->GetEntity()->RemoveComponent<T>();

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

	template<typename T, typename F> void Properties::RenderScriptField(ScriptComponent* script, const ScriptField& field, F showFunc) {

		T tmp;
		script->GetFieldValue(field, &tmp);

		bool changed = showFunc(field.name, &tmp);
		if (changed) script->SetFieldValue(field, &tmp);

	}
	
}