#include "Properties.h"

#include "Core/EditorApp.h"

#include "Viewport/SceneCamera.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <cstring>

#define _CRT_SECURE_NO_WARNINGS

using namespace Copper;

namespace Editor {

	std::filesystem::path Properties::selectedFile = "";
	bool Properties::wasFileLast = false;

	template<typename T> static bool DrawComponent(const std::string& name) {

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 {4, 4});
		ImGui::VerticalSeparator();

		bool opened = ImGui::TreeNodeEx((void*) typeid(T).hash_code(), flags, name.c_str());

		ImGui::PopStyleVar();
		if(opened) ImGui::TreePop();

		return opened;

	}

	void Properties::UI() {

		if ( wasFileLast && selectedFile != "") RenderFile();
		if (!wasFileLast && selectedObj) RenderObject();

	}

	void Properties::RenderObject() {

		char buffer[128];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, selectedObj.GetComponent<Name>()->name.c_str(), sizeof(buffer));

		if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {

			selectedObj.GetComponent<Name>()->name = std::string(buffer);

			SetChanges(true);

		}

		ImGui::SameLine();

		if(DrawComponent<Transform>("Transform")) {
			
			if (ShowVector3("Position:", selectedObj.GetComponent<Transform>()->position)) SetChanges(true);
			if (ShowVector3("Rotation:", selectedObj.GetComponent<Transform>()->rotation, 0.1f)) SetChanges(true);
			if (ShowVector3("Scale:", selectedObj.GetComponent<Transform>()->scale)) SetChanges(true);
			
		}

		if(selectedObj.HasComponent<Camera>() && DrawComponent<Camera>("Camera")) {
			
			if (ShowFloat("FOV", selectedObj.GetComponent<SceneCamera>()->fov, 0.1f)) SetChanges(true);
			if (ShowFloat("Near Plane", selectedObj.GetComponent<SceneCamera>()->nearPlane)) SetChanges(true);
			if (ShowFloat("Far Plane", selectedObj.GetComponent<SceneCamera>()->farPlane)) SetChanges(true);
			
		}
		
	}
	
	void Properties::RenderFile() {

		if(selectedFile.extension() == ".mat") {

			ImGui::Text("Material File");
			
		} else if(selectedFile.extension() == ".txt") {

			ImGui::Text("Script File");
			
		}
		
	}

	bool Properties::ShowVector2(std::string name, Vector2& vec, float speed) {

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

		return ret;
		
	}
	bool Properties::ShowVector3(std::string name, Vector3& vec, float speed) {

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

		return ret;

	}
	bool Properties::ShowVector4(std::string name, Vector4& vec, float speed) {

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

		return ret;
		
	}
	bool Properties::ShowColor(std::string name, Copper::Color& col, bool showAlpha) {

		bool ret = false;
		
		ImGui::PushID(name.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 75.0f);
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		
		if (ImGui::ColorEdit3("##Color", &col.r)) ret = true;

		ImGui::PopID();

		return ret;
		
	}
	bool Properties::ShowInt(std::string name, int& show, bool uint) {

		return true;
		
	}
	bool Properties::ShowFloat(std::string name, float& show, float speed) {

		bool ret = false;
		
		if (ImGui::DragFloat(name.c_str(), &show, speed)) ret = true;

		return ret;
		
	}

	
}