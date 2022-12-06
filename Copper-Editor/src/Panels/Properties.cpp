#include "Properties.h"

#include "Core/EditorApp.h"

#include "Viewport/SceneCamera.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>

#include <cstring>

using namespace Copper;

namespace Editor {

	std::filesystem::path Properties::selectedFile = "";
	bool Properties::wasFileLast = false;

	template<typename T> static bool DrawComponent(const std::string& name) {

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 {4, 4});
		//ImGui::VerticalSeparator();

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

		char buffer[128] = {};
		std::strncpy(buffer, selectedObj.tag->name.c_str(), sizeof(buffer));

		if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {

			selectedObj.tag->name = buffer;

			SetChanges(true);

		}

		ImGui::SameLine();
		ImGui::VerticalSeparator();

		if(DrawComponent<Transform>("Transform")) {
			
			if (ShowVector3("Position:", selectedObj.GetComponent<Transform>()->position)) SetChanges(true);
			if (ShowVector3("Rotation:", selectedObj.GetComponent<Transform>()->rotation, 0.1f)) SetChanges(true);
			if (ShowVector3("Scale:", selectedObj.GetComponent<Transform>()->scale)) SetChanges(true);

			ImGui::VerticalSeparator();
			
		}

		if(selectedObj.HasComponent<Camera>() && DrawComponent<Camera>("Camera")) {
			
			if (ShowFloat("FOV", selectedObj.GetComponent<Camera>()->fov, 0.1f)) SetChanges(true);
			if (ShowFloat("Near Plane", selectedObj.GetComponent<Camera>()->nearPlane)) SetChanges(true);
			if (ShowFloat("Far Plane", selectedObj.GetComponent<Camera>()->farPlane)) SetChanges(true);
			
		}
		if (selectedObj.HasComponent<Light>() && DrawComponent<Light>("Light")) {

			if (ShowColor("Color", selectedObj.GetComponent<Light>()->color, false)) SetChanges(true);
			if (ShowFloat("Intensity", selectedObj.GetComponent<Light>()->intensity)) SetChanges(true);

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
				
			if (ImGui::MenuItem("Light"))			{ selectedObj.AddComponent<Light>(); Editor::SetChanges(true); }
			if (ImGui::MenuItem("Mesh Renderer"))	{ selectedObj.AddComponent<MeshRenderer>(); Editor::SetChanges(true); }
			if (ImGui::MenuItem("Camera"))			{ selectedObj.AddComponent<Camera>(); Editor::SetChanges(true); }

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

		return ret;
		
	}
	bool Properties::ShowInt(std::string name, int& show, int speed) {

		bool ret = false;
		if (ImGui::DragInt(name.c_str(), &show, speed)) ret = true;

		return ret;
		
	}
	bool Properties::ShowUInt(std::string name, unsigned int& show, int speed) {

		bool ret = false;
		if (ImGui::DragInt(name.c_str(), (int*) &show, speed)) ret = true;
		if (show < 0) show = 0;

		return ret;

	}
	bool Properties::ShowFloat(std::string name, float& show, float speed) {

		bool ret = false;
		if (ImGui::DragFloat(name.c_str(), &show, speed)) ret = true;

		return ret;
		
	}
	bool Properties::ShowDouble(std::string name, double& show, float speed) {

		bool ret = false;
		if (ImGui::DragFloat(name.c_str(), (float*) &show, speed)) ret = true;

		return ret;

	}
	bool Properties::ShowString(std::string name, std::string& show) {

		bool ret = false;
		if (ImGui::InputText(name.c_str(), &show)) ret = true;

		return ret;

	}
	bool Properties::ShowChar(std::string name, char& show) {

		bool ret;
		if (ImGui::InputText(name.c_str(), &show, sizeof(char))) ret = true;

		return ret;

	}
	
}