#include "SceneHierarchy.h"

#include "Core/EditorApp.h"
#include "Core/Utils/ModelLoader.h"

#include "Engine/Renderer/Primitives.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

namespace Editor {

	using namespace Copper;

	void SceneHierarchy::UI() {

		Scene* scene = GetScene();

		if (!scene) { selectedObj = Object(); }

		if (ImGui::BeginPopupContextWindow(0, 1, false)) {

			if(ImGui::MenuItem("Empty Object")) {
				
				selectedObj = scene->CreateObject("Object");
				SetChanges(true);
				
			}
			
			ImGui::Separator();

			if(ImGui::BeginMenu("3D Objects")) {

				if(ImGui::MenuItem("Plane")) {

					selectedObj = scene->CreateObject("Plane");
					MeshRenderer* renderer = selectedObj.AddComponent<MeshRenderer>();

					Mesh mesh;
					mesh.vertices = planeVertices;
					mesh.normals = planeNormals;
					mesh.colors = planeColors;
					mesh.indices = planeIndices;

					renderer->meshes.push_back(mesh);

					SetChanges(true);
					
				}
				if(ImGui::MenuItem("Cube")) {

					selectedObj = scene->CreateObject("Cube");
					MeshRenderer* renderer = selectedObj.AddComponent<MeshRenderer>();

					Mesh mesh;
					mesh.vertices = cubeVertices;
					mesh.normals  = cubeNormals;
					mesh.colors   = cubeColors;
					mesh.indices  = cubeIndices;

					renderer->meshes.push_back(mesh);

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
			
			std::string name = obj.name;
			ImGuiTreeNodeFlags flags = ((selectedObj == obj) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

			bool opened = ImGui::TreeNodeEx((void*) obj.GetID(), flags, name.c_str());

			if (ImGui::IsItemClicked()) selectedObj = obj;
			if (opened) {

				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
				bool opened = ImGui::TreeNodeEx((void*) 9817239, flags, name.c_str());

				if (opened) ImGui::TreePop();

				ImGui::TreePop();

			}

			if (ImGui::BeginPopupContextItem()) {

				if (ImGui::MenuItem("Delete")) {

					if (selectedObj == obj) selectedObj = Object();
					
					scene->DestroyObject(obj);
				
				}

				ImGui::EndPopup();

			}

		}

		const ImGuiPayload* payload = ImGui::GetDragDropPayload();

		if(payload && ImGui::IsWindowHovered() && ImGui::IsMouseReleased(0)) {

			std::string wantedType = "MODEL";
			std::string payloadType = payload->DataType;

			if (!(wantedType == payloadType)) return;

			std::wstring ws((wchar_t*) payload->Data);
			std::string string(ws.begin(), ws.end());

			Model model;
			model.LoadMesh(string);

			selectedObj = scene->CreateObject("Model");
			MeshRenderer* renderer = selectedObj.AddComponent<MeshRenderer>();

			renderer->meshes = model.meshes;

			Log(string);

		}

	}

}
