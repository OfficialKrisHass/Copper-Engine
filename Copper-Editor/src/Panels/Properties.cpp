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

using namespace Copper;

namespace Editor {

	std::filesystem::path Properties::selectedFile = "";
	bool Properties::wasFileLast = false;
	bool Properties::dragDropTargetHovered = false;

	MetaFile::SceneMeta* sceneMeta;

	template<typename T> static bool DrawComponent(const std::string& name, bool& removed) {

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 {4, 4});
		//ImGui::VerticalSeparator();

		bool opened = ImGui::TreeNodeEx((void*) typeid(T).hash_code(), flags, name.c_str());

		if (opened && ImGui::BeginPopupContextItem()) {

			if (ImGui::MenuItem("Remove Component")) removed = true;

			ImGui::EndPopup();

		}

		ImGui::PopStyleVar();
		if(opened) ImGui::TreePop();

		return opened;

	}

	Properties::Properties() : Panel("Properties") {

		sceneMeta = SceneHierarchy::GetSceneMetaPointer();

	}

	void Properties::UI() {

		if ( wasFileLast && selectedFile != "") RenderFile();
		if (!wasFileLast && selectedObj) RenderObject();

	}

	void Properties::RenderObject() {

		char buffer[128] = {};
		std::strncpy(buffer, selectedObj.tag->name.c_str(), sizeof(buffer));

		if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {

			selectedObj.tag->name = buffer;

			SetChanges(true);

		}

		ImGui::SameLine();
		ImGui::VerticalSeparator();

		bool removed = false;
		if(DrawComponent<Transform>("Transform", removed)) {
			
			ShowVector3("Position:", selectedObj.transform->position);
			ShowVector3("Rotation:", selectedObj.transform->rotation, 0.1f);
			ShowVector3("Scale:", selectedObj.transform->scale);

			ImGui::VerticalSeparator();

			removed = false;
			
		}

		for (ScriptComponent* script : selectedObj.GetComponents<ScriptComponent>()) {

			ImGui::PushID((int) (int64_t) script);

			if (!DrawComponent<ScriptComponent>(script->name, removed)) { ImGui::PopID(); continue; }
			if (removed) {

				selectedObj.RemoveComponent<ScriptComponent>(script->index);

				removed = false;
				SetChanges(true);

				ImGui::PopID();
				continue;

			}

			for (ScriptField& field : Scripting::GetScriptFields(script->name)) {

				switch (field.type) {

					case ScriptField::Type::Int: { RenderScriptField<int>(script, field, BindShowFunc(ShowInt)); break; }
					case ScriptField::Type::UInt: { RenderScriptField<unsigned int>(script, field, BindShowFunc(ShowUInt)); break; }
					case ScriptField::Type::Float: { RenderScriptField<float>(script, field, BindShowFunc(ShowFloat)); break; }
					case ScriptField::Type::CopperObject: { RenderScriptField<Object>(script, field, BindShowFunc(ShowObject)); break; }

				}

			}

			ImGui::PopID();

		}
		for (Light* light : selectedObj.GetComponents<Light>()) {

			ImGui::PushID((int) (int64_t) light);

			if (!DrawComponent<Light>("Light", removed)) { ImGui::PopID(); continue; }
			if (removed) {
				
				selectedObj.RemoveComponent<Light>(light->index);

				removed = false;
				SetChanges(true);

				ImGui::PopID();
				continue;
			
			}

			ShowColor("Color", light->color, false);
			ShowFloat("Intensity", light->intensity);

			ImGui::PopID();

		}
		for (Camera* camera : selectedObj.GetComponents<Camera>()) {

			ImGui::PushID((int) (int64_t) camera);

			if (!DrawComponent<Camera>("Camera", removed)) { ImGui::PopID(); continue; }
			if (removed) {

				selectedObj.RemoveComponent<Camera>(camera->index);

				removed = false;
				SetChanges(true);

				ImGui::PopID();
				continue;

			}

			ShowFloat("FOV", camera->fov, 0.1f);
			ShowFloat("Near Plane", camera->nearPlane);
			ShowFloat("Far Plane", camera->farPlane);

			ImGui::PopID();

		}

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
				
			if (ImGui::MenuItem("Light"))			{ selectedObj.AddComponent<Light>()->color.r = 0.5f; Editor::SetChanges(true); }
			if (ImGui::MenuItem("Mesh Renderer"))	{ selectedObj.AddComponent<MeshRenderer>(); Editor::SetChanges(true); }
			if (ImGui::MenuItem("Camera"))			{ selectedObj.AddComponent<Camera>(); Editor::SetChanges(true); }

			for (std::string scriptName : Scripting::GetScriptComponents()) {

				if (ImGui::MenuItem(scriptName.c_str())) {
					
					ScriptComponent* script = selectedObj.AddComponent<ScriptComponent>();

					script->Init(selectedObj.GetID(), scriptName);
						
				}

			}

			ImGui::EndPopup();
				
		}
		
	}
	void Properties::RenderFile() {

		if(selectedFile.extension() == ".mat") {

			ImGui::Text("Material File");
			
		} else if(selectedFile.extension() == ".txt") {

			ImGui::Text("Script File");
			
		}
		
	}

	template<typename T, typename F> void Properties::RenderScriptField(ScriptComponent* script, const ScriptField& field, F showFunc) {

		T tmp;
		script->GetFieldValue(field, &tmp);

		bool changed = showFunc(field.name, static_cast<T&>(tmp));
		if (changed) script->SetFieldValue(field, &tmp);

	}

	bool Properties::ShowInt(const std::string& name, int& show, float speed) {

		bool ret = false;
		if (ImGui::DragInt(name.c_str(), &show, speed)) ret = true;

		if (ret) SetChanges(true);
		return ret;
		
	}
	bool Properties::ShowUInt(const std::string& name, unsigned int& show, float speed) {

		bool ret = false;
		if (ImGui::DragInt(name.c_str(), (int*) &show, speed)) ret = true;
		if (show < 0) show = 0;

		if (ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowFloat(const std::string& name, float& show, float speed) {

		bool ret = false;
		if (ImGui::DragFloat(name.c_str(), &show, speed)) ret = true;

		if (ret) SetChanges(true);
		return ret;
		
	}
	bool Properties::ShowDouble(const std::string& name, double& show, float speed) {

		bool ret = false;
		if (ImGui::DragFloat(name.c_str(), (float*) &show, speed)) ret = true;

		if (ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowString(const std::string& name, std::string& show) {

		bool ret = false;
		if (ImGui::InputText(name.c_str(), &show)) ret = true;

		if (ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowChar(const std::string& name, char& show) {

		bool ret = false;
		if (ImGui::InputText(name.c_str(), &show, sizeof(char))) ret = true;

		if (ret) SetChanges(true);
		return ret;

	}
	
	bool Properties::ShowVector2(const std::string& name, Vector2& vec, float speed) {

		bool ret = false;

		ImGui::PushID(name.c_str());

		//Text
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text(name.c_str());
		ImGui::NextColumn();

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
		if (ImGui::DragFloat("##X", &vec.x, speed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//Y
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("Y", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &vec.y, speed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();

		//End
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		if (ret) SetChanges(true);
		return ret;
		
	}
	bool Properties::ShowVector3(const std::string& name, Vector3& vec, float speed) {

		bool ret = false;
		
		ImGui::PushID(name.c_str());

		//Text
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text(name.c_str());
		ImGui::NextColumn();

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
		if (ImGui::DragFloat("##X", &vec.x, speed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//Y
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("Y", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &vec.y, speed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//Z
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("Z", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &vec.z, speed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();

		//End
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		if(ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowVector4(const std::string& name, Vector4& vec, float speed) {

		bool ret = false;
		
		ImGui::PushID(name.c_str());

		//Text
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text(name.c_str());
		ImGui::NextColumn();

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
		if (ImGui::DragFloat("##X", &vec.x, speed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//Y
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("Y", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &vec.y, speed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//Z
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("Z", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &vec.z, speed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//W
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("W", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if (ImGui::DragFloat("##W", &vec.w, speed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();

		//End
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		if (ret) SetChanges(true);
		return ret;
		
	}
	bool Properties::ShowColor(const std::string& name, Color& col, float speed) {

		ImGui::PushID(name.c_str());

		bool ret = false;
		float colors[] {

			col.r,
			col.g,
			col.b

		};

		if (ImGui::ColorEdit3("##Color", colors)) ret = true;
		ImGui::SameLine();
		ImGui::Text(name.c_str());

		if (ret) {

			col.r = colors[0];
			col.g = colors[1];
			col.b = colors[2];

		}

		ImGui::PopID();

		if (ret) SetChanges(true);
		return ret;
		
	}

	bool Properties::ShowObject(const std::string& name, Object& obj) {

		bool ret = false;
		std::string test;

		if (obj) test = obj.tag->name;
		else test = "None";
		test += " (Copper Object)";

		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		if (dragDropTargetHovered) ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4 {0.25f, 0.25f, 0.25f, 1.0f});

		ImGui::InputText(name.c_str(), &test, ImGuiInputTextFlags_ReadOnly);
		
		ImGui::PopItemFlag();
		if (dragDropTargetHovered) ImGui::PopStyleColor();

		dragDropTargetHovered = ImGui::IsItemHovered();
		if (ImGui::BeginDragDropTarget()) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_OBJECT_NODE")) {

				dragDropTargetHovered = true;
				ret = true;
				obj = GetObjectFromID(sceneMeta->objectIDs[*(int32_t*) payload->Data]);

			}

			ImGui::EndDragDropTarget();

		}

		if (ret) SetChanges(true);
		return ret;

	}

}