#include "Properties.h"

#include "Viewport/SceneCamera.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <cstring>

#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif

using namespace Copper;

namespace Editor {

	template<typename T> static void DrawComponent(const std::string& name) {

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 {4, 4});
		ImGui::VerticalSeparator();

		ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.c_str());

		ImGui::PopStyleVar();
		ImGui::TreePop();

	}

	void Properties::UI() {

		if (!selectedObj) return;

		char buffer[128];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, selectedObj.GetComponent<Name>()->name.c_str(), sizeof(buffer));

		if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {

			selectedObj.GetComponent<Name>()->name = std::string(buffer);

		}

		DrawComponent<Transform>("Transform");

		ShowVector3("Position:", selectedObj.GetComponent<Transform>()->position);
		ShowVector3("Rotation:", selectedObj.GetComponent<Transform>()->rotation, 0.1f);
		ShowVector3("Scale:", selectedObj.GetComponent<Transform>()->scale);

		if(selectedObj.HasComponent<Camera>()) {
			
			DrawComponent<Camera>("Camera");
			ShowFloat("FOV", selectedObj.GetComponent<SceneCamera>()->fov, 0.1f);
			ShowFloat("Near Plane", selectedObj.GetComponent<SceneCamera>()->nearPlane);
			ShowFloat("Far Plane", selectedObj.GetComponent<SceneCamera>()->farPlane);
			
		}

		if(selectedObj.HasComponent<SceneCamera>()) {
			
			DrawComponent<SceneCamera>("Scene Camera");
			ShowFloat("FOV", selectedObj.GetComponent<SceneCamera>()->fov, 0.1f);
			ShowFloat("Near Plane", selectedObj.GetComponent<SceneCamera>()->nearPlane);
			ShowFloat("Far Plane", selectedObj.GetComponent<SceneCamera>()->farPlane);

			ImGui::Text("Movement");

			ShowFloat("Speed", selectedObj.GetComponent<SceneCamera>()->speed);
			ShowFloat("Sensitivity", selectedObj.GetComponent<SceneCamera>()->sensitivity, 0.1f);
			
		}

		if(selectedObj.HasComponent<Mesh>()) {

			DrawComponent<Mesh>("Mesh");
			
		}

	}

	void Properties::ShowVector3(std::string name, Vector3& vec, float speed) {

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
		ImGui::DragFloat("##X", &vec.x, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//Y
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("Y", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &vec.y, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		//Z
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 {0.1f, 0.1f, 0.1f, 1.0f});
		ImGui::Button("Z", buttonSize);
		ImGui::PopItemFlag();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &vec.z, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		//End
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

	}
	void Properties::ShowFloat(std::string name, float& show, float speed) {

		ImGui::DragFloat(name.c_str(), &show, speed);
		
	}

	
}