#include "SceneHierarchy.h"

#include <ImGui/imgui.h>

namespace Editor {

	using namespace Copper;

	void SceneHierarchy::UIRender() {

		ImGui::Begin(name.c_str());

		if (!scene) { return; ImGui::End(); }

		for (Object obj : SceneView<>(*scene)) {

			std::string& name = obj.name->name;
			ImGuiTreeNodeFlags flags = ((selectedObj == obj) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

			bool opened = ImGui::TreeNodeEx((void*) (uint64_t) (uint32_t) obj, flags, name.c_str());

			if (ImGui::IsItemClicked()) { selectedObj = obj; }

		}

		ImGui::End();

	}

}