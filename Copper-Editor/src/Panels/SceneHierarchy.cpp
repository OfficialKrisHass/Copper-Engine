#include "SceneHierarchy.h"

#include "Core/EditorApp.h"
#include "Core/Utils/ModelLoader.h"

#include "Engine/Renderer/Primitives.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

using namespace Copper;

namespace Editor {

	MetaFile::SceneMeta sceneMeta;

	SceneHierarchy::SceneHierarchy() : Panel("Scene Hierarchy"), scene(scene) {

		AddObjectCreatedEventFunc(BindEventFunc(SceneHierarchy::OnObjectCreated));
		AddObjectDestroyedEventFunc(BindEventFunc(SceneHierarchy::OnObjectDestroyed));

	}

	void SceneHierarchy::UI() {

		if (!scene) return;
		if (ImGui::BeginPopupContextWindow(0, 1, false)) { PopupWindow(); ImGui::EndPopup(); }

		for (uint32_t i = 0; i < sceneMeta.objectIDs.size(); i++) {

			if (GetObjectFromID(sceneMeta.objectIDs[i]).transform->parent) continue;
			DrawObjectNode(i);

		}

		//Get the Window Rect through some magic math;
		ImRect windowRect;
		windowRect.Min = ImGui::GetWindowPos();
		windowRect.Max.x = windowRect.Min.x + ImGui::GetWindowWidth();
		windowRect.Max.y = windowRect.Min.y + ImGui::GetWindowHeight();
		windowRect.Min.y = ImGui::GetItemRectMax().y + 7;

		if (ImGui::BeginDragDropTargetCustom(windowRect, ImGuiID(7985548))) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_OBJECT_NODE")) RemoveParent(*(uint32_t*) payload->Data);

			ImGui::EndDragDropTarget();

		}

	}

	void SceneHierarchy::DrawObjectNode(uint32_t& objIDIndex) {

		Object& obj = GetObjectFromID(sceneMeta.objectIDs[objIDIndex]);

		ImGuiTreeNodeFlags flags = ((selectedObj == obj) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*) (uint64_t) obj.GetID(), flags, obj.tag->name.c_str());

		//The Drag and Drop stuff needs to be here, since when we open the node
		//and the object has children, the Drag Drop will only work on the last Child
		//if we try to do this after rendering the children
		if (ImGui::BeginDragDropSource()) {

			ImGui::SetDragDropPayload("SCH_OBJECT_NODE", &objIDIndex, sizeof(uint64_t), ImGuiCond_Once);
			ImGui::EndDragDropSource();

		}
		if (ImGui::BeginDragDropTarget()) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_OBJECT_NODE")) {

				uint32_t otherObjIDIndex = *(uint32_t*) payload->Data;
				Object& otherObj = GetObjectFromID(sceneMeta.objectIDs[otherObjIDIndex]);

				if (otherObj.transform->parent != obj.transform && obj.transform->parent != otherObj.transform) {

					if (otherObj.transform->parent) otherObj.transform->parent->RemoveChild(otherObj.transform);
					obj.transform->AddChild(otherObj.transform);
					
					//if (objIDIndex == objectIDs.size() - 1) objectIDs.push_back(objectIDs[otherObjIDIndex]);
					sceneMeta.objectIDs.insert(sceneMeta.objectIDs.begin() + objIDIndex + 1, sceneMeta.objectIDs[otherObjIDIndex]);
					sceneMeta.objectIDs.erase(sceneMeta.objectIDs.begin() + (objIDIndex > otherObjIDIndex ? otherObjIDIndex : otherObjIDIndex + 1));

					objIDIndex--;

					SetChanges(true);

				}

			}

			ImGui::EndDragDropTarget();

		}

		ImRect betweenNodes;
		betweenNodes.Min = ImGui::GetItemRectMin();
		betweenNodes.Max = ImGui::GetItemRectMax();
		betweenNodes.Min.y -= 4;
		betweenNodes.Max.y -= 18;

		if (ImGui::BeginDragDropTargetCustom(betweenNodes, ImGuiID(7985548))) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_OBJECT_NODE")) MoveObjectNode(objIDIndex, *(uint32_t*) payload->Data);

			ImGui::EndDragDropTarget();

		}

		if (ImGui::IsItemClicked()) selectedObj = obj;
		if (opened) {

			uint32_t childIndex;
			for (int i = 1; i <= obj.transform->numOfChildren; i++) {

				childIndex = objIDIndex + i;
				DrawObjectNode(childIndex);

			}

			ImGui::TreePop();

		}

		if (ImGui::BeginPopupContextItem()) {

			if (ImGui::MenuItem("Delete")) RemoveObjectNode(objIDIndex);

			ImGui::EndPopup();

		}
		

	}
	void SceneHierarchy::RemoveObjectNode(uint32_t objIDIndex) {

		Object& obj = GetObjectFromID(sceneMeta.objectIDs[objIDIndex]);

		if (selectedObj == obj) selectedObj = Object();

		scene->DestroyObject(obj);

		SetChanges(true);

	}
	void SceneHierarchy::MoveObjectNode(uint32_t& objIDIndex, uint32_t objToMoveIDIndex) {

		if (objIDIndex == objToMoveIDIndex || objToMoveIDIndex + 1 == objIDIndex) return;

		Object& obj = GetObjectFromID(sceneMeta.objectIDs[objIDIndex]);
		Object& objToMove = GetObjectFromID(sceneMeta.objectIDs[objToMoveIDIndex]);

		sceneMeta.objectIDs.insert(sceneMeta.objectIDs.begin() + objIDIndex, sceneMeta.objectIDs[objToMoveIDIndex]);
		sceneMeta.objectIDs.erase(sceneMeta.objectIDs.begin() + (objIDIndex > objToMoveIDIndex ? objToMoveIDIndex : objToMoveIDIndex + 1));
		objIDIndex++;

		if (objToMove.transform->parent) objToMove.transform->parent->RemoveChild(objToMove.transform);
		if (obj.transform->parent) obj.transform->parent->AddChild(objToMove.transform);

		SetChanges(true);

	}

	void SceneHierarchy::PopupWindow() {

		if (ImGui::MenuItem("Empty Object")) {

			selectedObj = scene->CreateObject("Object");
			SetChanges(true);

		}

		ImGui::Separator();

		if (ImGui::BeginMenu("3D Objects")) {

			if (ImGui::MenuItem("Plane")) {

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
			if (ImGui::MenuItem("Cube")) {

				selectedObj = scene->CreateObject("Cube");
				MeshRenderer* renderer = selectedObj.AddComponent<MeshRenderer>();
				Mesh mesh;

				mesh.vertices = cubeVertices;
				mesh.normals = cubeNormals;
				mesh.colors = cubeColors;
				mesh.indices = cubeIndices;

				renderer->meshes.push_back(mesh);
				SetChanges(true);

			}

			ImGui::EndMenu();

		}

		if (ImGui::MenuItem("Light")) {

			selectedObj = scene->CreateObject("Light");
			Light* l = selectedObj.AddComponent<Light>();

			SetChanges(true);

		}

	}

	void SceneHierarchy::RemoveParent(uint32_t objIDIndex) {

		Object& obj = GetObjectFromID(sceneMeta.objectIDs[objIDIndex]);

		if (!obj.transform->parent) return;
		
		obj.transform->parent->RemoveChild(obj.transform);
		obj.transform->parent = nullptr;

		sceneMeta.objectIDs.push_back(sceneMeta.objectIDs[objIDIndex]);
		sceneMeta.objectIDs.erase(sceneMeta.objectIDs.begin() + objIDIndex);

	}

	bool SceneHierarchy::OnObjectCreated(const Event& e) {

		ObjectEvent* event = (ObjectEvent*) &e;

		sceneMeta.objectIDs.push_back(event->obj->GetID());

		return false;

	}
	bool SceneHierarchy::OnObjectDestroyed(const Event& e) {

		ObjectEvent* event = (ObjectEvent*) &e;
		uint32_t objIDIndex;

		for(objIDIndex = 0; objIDIndex < sceneMeta.objectIDs.size() && sceneMeta.objectIDs[objIDIndex] != event->obj->GetID(); objIDIndex++) { }
		sceneMeta.objectIDs.erase(sceneMeta.objectIDs.begin() + objIDIndex);

		return false;

	}

	void SceneHierarchy::SaveSceneMeta() {

		sceneMeta.Serialize(scene);

	}
	void SceneHierarchy::LoadSceneMeta() {

		sceneMeta.Deserialize(scene);

	}

}
