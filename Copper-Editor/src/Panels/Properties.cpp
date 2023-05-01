#include "Properties.h"

#include "Engine/Scripting/ScriptingCore.h"

#include "Core/EditorApp.h"
#include "Core/SceneMeta.h"

#include "Panels/SceneHierarchy.h"

#include "Viewport/SceneCamera.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>

#include <cstring>

#define BindShowFunc(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

#define DragIntSpeed 1.0f
#define DragFloatSpeed 0.01f

using namespace Copper;

namespace Editor {

	bool Properties::dragDropTargetHovered = false;

	template<typename T> static bool DrawComponent(const std::string& name, InternalEntity* entity);

	Properties::Properties() : Panel("Properties") {

	}

	void Properties::UI() {

		if (!*selectedEntity) return;

		RenderEntity();

	}

	void Properties::RenderEntity() {

		InternalEntity* entity = *selectedEntity;

		char buffer[128] = {};
		std::strncpy(buffer, entity->name.c_str(), sizeof(buffer));

		if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {

			entity->name = buffer;
			Editor::SetChanges(true);

		}

		ImGui::SameLine();
		ImGui::VerticalSeparator();

		if(DrawComponent<Transform>("Transform", entity)) {
			
			ShowVector3("Position", &entity->GetTransform()->position);
			ShowVector3("Rotation", &entity->GetTransform()->rotation);
			ShowVector3("Scale",    &entity->GetTransform()->scale);
			
		}

		if (Light* light = entity->GetComponent<Light>()) RenderLight(light);
		if (Camera* camera = entity->GetComponent<Camera>()) RenderCamera(camera);

		if (ScriptComponent* script = entity->GetComponent<ScriptComponent>()) RenderScriptComponent(script);

		if (PhysicsBody* body = entity->GetComponent<PhysicsBody>()) RenderPhysicsBody(body);

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
				
			if (ImGui::MenuItem("Light")) {
				
				entity->AddComponent<Light>()->color.r = 0.5f;
				Editor::SetChanges(true);
			
			}
			if (ImGui::MenuItem("Mesh Renderer")) {
				
				entity->AddComponent<MeshRenderer>();
				Editor::SetChanges(true);
			
			}
			if (ImGui::MenuItem("Camera")) {
				
				entity->AddComponent<Camera>();
				Editor::SetChanges(true);
			
			}
			if (ImGui::MenuItem("Physics Body")) {

				entity->AddComponent<PhysicsBody>();
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

	void Properties::RenderLight(Light* light) {

		ImGui::PushID((int) (int64_t) light);

		if (!DrawComponent<Light>("Light", light->GetEntity())) { ImGui::PopID(); return; }

		ShowColor("Color", &light->color);
		ShowFloat("Intensity", &light->intensity);

		ImGui::PopID();

	}
	void Properties::RenderCamera(Camera* camera) {

		ImGui::PushID((int) (int64_t) camera);

		if (!DrawComponent<Camera>("Camera", camera->GetEntity())) { ImGui::PopID(); return; }

		ShowFloat("FOV", &camera->fov);
		ShowFloat("Near Plane", &camera->nearPlane);
		ShowFloat("Far Plane", &camera->farPlane);

		ImGui::PopID();

	}

	void Properties::RenderScriptComponent(ScriptComponent* script) {

		ImGui::PushID((int) (int64_t) script);

		if (!DrawComponent<ScriptComponent>(script->name, script->GetEntity())) { ImGui::PopID(); return; }
		if (!*script) {

			ImGui::Text("This Script is invalid or doesn't exist anymore");
			ImGui::PopID();

			return;

		}

		for (ScriptField& field : Scripting::GetScriptFields(script->name)) {

			switch (field.type) {

				case ScriptField::Type::Int:			RenderScriptField<int>(script, field, BindShowFunc(ShowInt)); break;
				case ScriptField::Type::UInt:			RenderScriptField<unsigned int>(script, field, BindShowFunc(ShowUInt)); break;
				case ScriptField::Type::Float:			RenderScriptField<float>(script, field, BindShowFunc(ShowFloat)); break;

				case ScriptField::Type::Vector2:		RenderScriptField<Vector2>(script, field, BindShowFunc(ShowVector2)); break;
				case ScriptField::Type::Vector3:		RenderScriptField<Vector3>(script, field, BindShowFunc(ShowVector3)); break;

				case ScriptField::Type::Entity:			RenderScriptField<InternalEntity*>(script, field, BindShowFunc(ShowEntity)); break;

			}

		}

		ImGui::PopID();

	}

	void Properties::RenderPhysicsBody(PhysicsBody* body) {

		ImGui::PushID((int) (int64_t) body);

		if (!DrawComponent<PhysicsBody>("Physics Body", body->GetEntity())) { ImGui::PopID(); return; }

		ShowFloat("Mass", &body->mass);

		ImGui::PopID();

	}

	template<typename T> static bool DrawComponent(const std::string& name, InternalEntity* entity) {

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 {4, 4});
		//ImGui::VerticalSeparator();

		bool opened = ImGui::TreeNodeEx((void*) typeid(T).hash_code(), flags, name.c_str());

		ImGui::PopStyleVar();
		if (opened) ImGui::TreePop();

		if (ImGui::BeginPopupContextItem()) {

			if (ImGui::MenuItem("Remove Component")) {
				
				entity->RemoveComponent<T>();

				SetChanges(true);
				ImGui::EndPopup();

				return false;
			
			}

			ImGui::EndPopup();

		}

		return opened;

	}

	template<typename T, typename F> void Properties::RenderScriptField(ScriptComponent* script, const ScriptField& field, F showFunc) {

		T tmp;
		script->GetFieldValue(field, &tmp);

		bool changed = showFunc(field.name, &static_cast<T>(tmp));
		if (changed) script->SetFieldValue(field, &tmp);

	}

	bool Properties::ShowBool(const std::string& name, bool* show) {

		bool ret = false;
		ret = ImGui::Checkbox(name.c_str(), show);

		if (ret) Editor::SetChanges(true);
		return ret;

	}
	bool Properties::ShowInt(const std::string& name, int* show) {

		bool ret = false;
		ret = ImGui::DragInt(name.c_str(), show, DragIntSpeed);

		if (ret) SetChanges(true);
		return ret;
		
	}
	bool Properties::ShowUInt(const std::string& name, unsigned int* show) {

		bool ret = false;
		ret = ImGui::DragInt(name.c_str(), (int*) show, DragIntSpeed);
		if (*show < 0) *show = 0;

		if (ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowFloat(const std::string& name, float* show) {

		bool ret = false;
		ret = ImGui::DragFloat(name.c_str(), show, DragFloatSpeed);

		if (ret) SetChanges(true);
		return ret;
		
	}
	bool Properties::ShowDouble(const std::string& name, double* show) {

		bool ret = false;
		ret = ImGui::DragFloat(name.c_str(), (float*) show, DragFloatSpeed);

		if (ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowString(const std::string& name, std::string* show) {

		bool ret = false;
		ret = ImGui::InputText(name.c_str(), show);

		if (ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowChar(const std::string& name, char* show) {

		bool ret = false;
		if (ImGui::InputText(name.c_str(), show, sizeof(char))) ret = true;

		if (ret) SetChanges(true);
		return ret;

	}
	
	bool Properties::ShowVector2(const std::string& name, Vector2* vec) {

		bool ret = false;

		ImGui::PushID(name.c_str());

		//Settings
		ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 {0, 0});

		//Vars
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

		//X
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("X", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &vec->x, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//Y
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("Y", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &vec->y, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();

		ImGui::VerticalSeparator();

		//End
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		if (ret) SetChanges(true);
		return ret;
		
	}
	bool Properties::ShowVector3(const std::string& name, Vector3* vec) {

		bool ret = false;
		
		ImGui::PushID(name.c_str());

		//Settings
		ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 {0, 0});

		//Vars
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = {lineHeight, lineHeight};

		//X
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("X", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &vec->x, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//Y
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("Y", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &vec->y, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//Z
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("Z", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &vec->z, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine(0.0f, 7.0f);

		//Text
		ImGui::Text(name.c_str());
		ImGui::PopItemWidth();


		//End
		ImGui::PopStyleVar();
		ImGui::PopID();

		ImGui::VerticalSeparator();

		if(ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowVector4(const std::string& name, Vector4* vec) {

		bool ret = false;
		
		ImGui::PushID(name.c_str());

		//Settings
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 {0, 0});

		//Vars
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

		//X
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("X", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &vec->x, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//Y
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("Y", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &vec->y, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//Z
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("Z", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &vec->z, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//W
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("W", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if (ImGui::DragFloat("##W", &vec->w, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();

		ImGui::VerticalSeparator();

		//End
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		if (ret) SetChanges(true);
		return ret;
		
	}
	bool Properties::ShowColor(const std::string& name, Color* col) {

		ImGui::PushID(name.c_str());

		bool ret = false;
		float colors[] {

			col->r,
			col->g,
			col->b

		};

		ret = ImGui::ColorEdit3("##Color", colors);
		ImGui::SameLine();
		ImGui::Text(name.c_str());

		if (ret) {

			col->r = colors[0];
			col->g = colors[1];
			col->b = colors[2];

		}

		ImGui::PopID();

		if (ret) SetChanges(true);
		return ret;
		
	}

	bool Properties::ShowEntity(const std::string& name, InternalEntity** entity) {

		bool ret = false;
		std::string nodeText;

		if (*entity) nodeText = (*entity)->name;
		else nodeText = "None";
		nodeText += " (Copper Object)";

		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		if (dragDropTargetHovered) ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4 {0.25f, 0.25f, 0.25f, 1.0f});

		ImGui::InputText(name.c_str(), &nodeText, ImGuiInputTextFlags_ReadOnly);
		
		ImGui::PopItemFlag();
		if (dragDropTargetHovered) ImGui::PopStyleColor();

		dragDropTargetHovered = ImGui::IsItemHovered();
		if (ImGui::BeginDragDropTarget()) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_ENTITY_NODE")) {

				dragDropTargetHovered = true;
				ret = true;
				*entity = GetEntityFromID(*(uint32_t*) payload->Data);

			}

			ImGui::EndDragDropTarget();

		}

		if (ret) SetChanges(true);
		return ret;

	}
	
}