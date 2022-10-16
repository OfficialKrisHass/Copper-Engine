#include "SceneHierarchy.h"

#include "Core/EditorApp.h"

#include "Engine/Renderer/Primitives.h"

#include <ImGui/imgui.h>

namespace Editor {

	using namespace Copper;

	void SceneHierarchy::UI() {

		Scene* scene = GetScene();

		if (!scene) { selectedObj = Object::Null(); }

		if (ImGui::BeginPopupContextWindow(0, 1, false)) {

			if(ImGui::MenuItem("Empty Object")) {
				
				selectedObj = scene->CreateObject("Object");
				SetChanges(true);
				
			}
			
			ImGui::Separator();

			if(ImGui::BeginMenu("3D Objects")) {

				if(ImGui::MenuItem("Plane")) {

					selectedObj = scene->CreateObject("Plane");
					Mesh* m = selectedObj.AddComponent<Mesh>();

					m->vertices = planeVertices;
					m->indices  = planeIndices;
					m->material = Material::Default();
					m->Regenerate();

					SetChanges(true);
					
				}
				if(ImGui::MenuItem("Cube")) {

					selectedObj = scene->CreateObject("Cube");
					Mesh* m = selectedObj.AddComponent<Mesh>();

					m->vertices = cubeVertices;
					m->indices  = cubeIndices;
					m->material = Material::Default();
					m->Regenerate();

					SetChanges(true);
					
				}
				
				ImGui::EndMenu();
				
			}

			if(ImGui::MenuItem("Light")) {

				selectedObj = scene->CreateObject("Light");
				Light* l = selectedObj.AddComponent<Light>();

				SetChanges(true);
				
			}
			
			ImGui::EndPopup();
				
		}

		for (Object obj : SceneView<>(*scene)) {
			
			ImGui::PushID(obj.id);

			std::string& name = obj.name->name;
			ImGuiTreeNodeFlags flags = ((selectedObj == obj) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*) (uint64_t) (uint32_t) obj, flags, name.c_str());

			if (ImGui::IsItemClicked()) { selectedObj = obj; }
			if (opened) { ImGui::TreePop(); }

			ImGui::PopID();

		}

	}

}
