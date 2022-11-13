#include "SceneHierarchy.h"

#include "Core/EditorApp.h"
#include "Core/Utils/ModelLoader.h"

#include "Engine/Renderer/Primitives.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

namespace Editor {

	using namespace Copper;

	void SceneHierarchy::UI() {

		scene = GetScene();
		anyNodeHovered = false;

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

		for (Object& obj : SceneView<>(scene)) {

			if (obj.transform->parent) continue;
			
			DrawObjectNode(obj);

		}

		const ImGuiPayload* payload = ImGui::GetDragDropPayload();

		if(payload && ImGui::IsWindowHovered() && ImGui::IsMouseReleased(0)) {

			if (CheckDropType("MODEL", payload->DataType)) DropModel(payload->Data);
			if (CheckDropType("OBJECT", payload->DataType)) DropObject(payload->Data);

		}

	}

	void SceneHierarchy::DrawObjectNode(Object obj) {

		std::string name = obj.tag->name;
		ImGuiTreeNodeFlags flags = ((selectedObj == obj) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		bool opened = ImGui::TreeNodeEx((void*) (uint64_t) obj.GetID(), flags, name.c_str());

		if (ImGui::BeginDragDropSource()) {

			int32_t data = obj.GetID();

			ImGui::SetDragDropPayload("OBJECT", &data, sizeof(int32_t), ImGuiCond_Once);
			ImGui::EndDragDropSource();

		}
		if (ImGui::BeginDragDropTarget()) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OBJECT")) {

				Object& child = GetObjectFromID(*(int32_t*) (payload->Data));

				if (child.transform->parent != obj.transform && obj.transform->parent != child.transform) {

					if (child.transform->parent) {

						child.transform->parent->RemoveChild(child.transform);
						child.transform->position += child.transform->parent->GlobalPosition();

					}

					child.transform->position -= obj.transform->GlobalPosition();
					child.transform->parent = obj.transform;
					obj.transform->AddChild(child.transform);


				}

			}
			
			ImGui::EndDragDropTarget();

		}

		if (ImGui::IsItemClicked()) selectedObj = obj;
		if (!anyNodeHovered && ImGui::IsItemHovered()) anyNodeHovered = true;
		if (opened) {

			for (int i = 0; i < obj.transform->numOfChildren; i++) {
				
				DrawObjectNode(*obj.transform->GetChild(i)->object);
			
			}

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
	
	void SceneHierarchy::DropModel(void* data) {

		std::string string = (const char*) data;

		Model model;
		model.LoadMesh(string);

		selectedObj = scene->CreateObject("Model");
		MeshRenderer* renderer = selectedObj.AddComponent<MeshRenderer>();

		renderer->meshes = model.meshes;

	}
	void SceneHierarchy::DropObject(void* data) {

		if (anyNodeHovered) return;

		Object& child = GetObjectFromID(*(int32_t*) data);

		if (child.transform->parent) {

			child.transform->parent->RemoveChild(child.transform);
			child.transform->position += child.transform->parent->GlobalPosition();

		}
		child.transform->parent = nullptr;

	}

}
