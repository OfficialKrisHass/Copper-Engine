#include "Properties.h"

#include "Engine/Scripting/ScriptingCore.h"

#include "Core/EditorApp.h"
#include "Core/MetaFileSerialization.h"

#include "Panels/SceneHierarchy.h"

#include "Viewport/SceneCamera.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>

#include <cstring>

#define BindShowFunc(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }
//#define BindShowFunc(func) std::bind(&func, this, std::placeholders::_1)

#define DragIntSpeed 1.0f
#define DragFloatSpeed 0.01f

using namespace Copper;

namespace Editor {

	bool Properties::dragDropTargetHovered = false;

	MetaFile::SceneMeta* sceneMeta;

	template<typename T> static bool DrawComponent(const std::string& name) {

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 {4, 4});
		//ImGui::VerticalSeparator();

		bool opened = ImGui::TreeNodeEx((void*) typeid(T).hash_code(), flags, name.c_str());

		ImGui::PopStyleVar();
		if(opened) ImGui::TreePop();

		return opened;

	}

	Properties::Properties() : Panel("Properties") {

		sceneMeta = SceneHierarchy::GetSceneMetaPointer();

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

			SetChanges(true);

		}

		ImGui::SameLine();
		ImGui::VerticalSeparator();

		if(DrawComponent<Transform>("Transform")) {
			
			ShowVector3("Position", &entity->GetTransform()->position);
			ShowVector3("Rotation", &entity->GetTransform()->rotation);
			ShowVector3("Scale",    &entity->GetTransform()->scale);
			
		}

		if (ScriptComponent* script = entity->GetComponent<ScriptComponent>()) RenderScriptComponent(script);
		if (Light* light = entity->GetComponent<Light>()) RenderLight(light);
		if (Camera* camera = entity->GetComponent<Camera>()) RenderCamera(camera);

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
				
			if (ImGui::MenuItem("Light"))			{ entity->AddComponent<Light>()->color.r = 0.5f; Editor::SetChanges(true); }
			if (ImGui::MenuItem("Mesh Renderer"))	{ entity->AddComponent<MeshRenderer>(); Editor::SetChanges(true); }
			if (ImGui::MenuItem("Camera"))			{ entity->AddComponent<Camera>(); Editor::SetChanges(true); }

			for (std::string scriptName : Scripting::GetScriptComponents()) {

				if (ImGui::MenuItem(scriptName.c_str())) {
					
					ScriptComponent* script = entity->AddComponent<ScriptComponent>();

					script->Init(entity->ID(), scriptName);
						
				}

			}

			ImGui::EndPopup();
				
		}
		
	}

	void Properties::RenderScriptComponent(ScriptComponent* script) {

		ImGui::PushID((int) (int64_t) script);

		if (!DrawComponent<ScriptComponent>(script->name)) { ImGui::PopID(); return; }

		for (ScriptField& field : Scripting::GetScriptFields(script->name)) {

			switch (field.type) {

				case ScriptField::Type::Int:			RenderScriptField<int>(script, field, BindShowFunc(ShowInt)); break;
				case ScriptField::Type::UInt:			RenderScriptField<unsigned int>(script, field, BindShowFunc(ShowUInt)); break;
				case ScriptField::Type::Float:			RenderScriptField<float>(script, field, BindShowFunc(ShowFloat)); break;

				case ScriptField::Type::Vector2:		RenderScriptField<Vector2>(script, field, BindShowFunc(ShowVector2)); break;
				case ScriptField::Type::Vector3:		RenderScriptField<Vector3>(script, field, BindShowFunc(ShowVector3)); break;

				case ScriptField::Type::Entity:			RenderScriptField<InternalEntity*>(script, field, BindShowFunc(ShowEntity)); break;
				case ScriptField::Type::Component:
				{

					ComponentScriptField* componentField = (ComponentScriptField*) &field;
					if (componentField->isBuiltinComponent) { break; } //Not Implemented yet

					ScriptComponent* tmp = nullptr;
					script->GetFieldValue(field, tmp);

					break;

				}

			}

		}

		ImGui::PopID();

	}
	void Properties::RenderLight(Copper::Light* light) {

		ImGui::PushID((int) (int64_t) light);

		if (!DrawComponent<Light>("Light")) { ImGui::PopID(); return; }

		ShowColor("Color", &light->color);
		ShowFloat("Intensity", &light->intensity);

		ImGui::PopID();

	}
	void Properties::RenderCamera(Copper::Camera* camera) {

		ImGui::PushID((int) (int64_t) camera);

		if (!DrawComponent<Camera>("Camera")) { ImGui::PopID(); return; }

		ShowFloat("FOV", &camera->fov);
		ShowFloat("Near Plane", &camera->nearPlane);
		ShowFloat("Far Plane", &camera->farPlane);

		ImGui::PopID();

	}

	template<typename T, typename F> void Properties::RenderScriptField(ScriptComponent* script, const ScriptField& field, F showFunc) {

		T tmp;
		script->GetFieldValue(field, &tmp);

		bool changed = showFunc(field.name, &static_cast<T>(tmp));
		if (changed) script->SetFieldValue(field, &tmp);

	}

	bool Properties::ShowInt(const std::string& name, int* show) {

		bool ret = false;
		if (ImGui::DragInt(name.c_str(), show, DragIntSpeed)) ret = true;

		if (ret) SetChanges(true);
		return ret;
		
	}
	bool Properties::ShowUInt(const std::string& name, unsigned int* show) {

		bool ret = false;
		if (ImGui::DragInt(name.c_str(), (int*) show, DragIntSpeed)) ret = true;
		if (*show < 0) *show = 0;

		if (ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowFloat(const std::string& name, float* show) {

		bool ret = false;
		if (ImGui::DragFloat(name.c_str(), show, DragFloatSpeed)) ret = true;

		if (ret) SetChanges(true);
		return ret;
		
	}
	bool Properties::ShowDouble(const std::string& name, double* show) {

		bool ret = false;
		if (ImGui::DragFloat(name.c_str(), (float*) show, DragFloatSpeed)) ret = true;

		if (ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowString(const std::string& name, std::string* show) {

		bool ret = false;
		if (ImGui::InputText(name.c_str(), show)) ret = true;

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

		if (ImGui::ColorEdit3("##Color", colors)) ret = true;
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
				*entity = GetEntityFromID(sceneMeta->objectIDs[*(uint32_t*) payload->Data]);

			}

			ImGui::EndDragDropTarget();

		}

		if (ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowComponent(const std::string& name, Component* component) {

		bool ret = false;
		std::string nodeText;

		if (component) nodeText = component->GetEntity()->name;
		else nodeText = "None";
		nodeText += " (Component)";

		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		if (dragDropTargetHovered) ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4 {0.25f, 0.25f, 0.25f, 1.0f});

		ImGui::InputText(name.c_str(), &nodeText, ImGuiInputTextFlags_ReadOnly);

		ImGui::PopItemFlag();
		if (dragDropTargetHovered) ImGui::PopStyleColor();

		return ret;

	}

}