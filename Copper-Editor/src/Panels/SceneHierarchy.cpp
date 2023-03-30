#include "SceneHierarchy.h"

#include "Core/EditorApp.h"
#include "Core/Utils/ModelLoader.h"

#include "Panels/Properties.h"

#include "Engine/Renderer/Primitives.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

using namespace Copper;

namespace Editor {

	MetaFile::SceneMeta sceneMeta;
	uint32_t clickedEntityID = invalidID;

	SceneHierarchy::SceneHierarchy() : Panel("Scene Hierarchy") {

		/*AddEntityCreatedEventFunc(BindEventFunc(SceneHierarchy::OnObjectCreated));
		AddEntityDestroyedEventFunc(BindEventFunc(SceneHierarchy::OnObjectDestroyed));*/

	}

	void SceneHierarchy::UI() {

		CU_ASSERT(scene, "SceneHierarchy::scene is nullptr... HOW ???!!??!!");

		for (uint32_t eID : sceneMeta.objectIDs) {

			InternalEntity* entity = GetEntityFromID(eID);
			if (entity->GetTransform()->Parent()) continue;

			DrawEntityNode(entity);

		}

		//if (ImGui::BeginPopupContextWindow(0, 1, false)) { PopupWindow(); ImGui::EndPopup(); }

		//for (uint32_t i = 0; i < sceneMeta.objectIDs.size(); i++) {

		//	if (GetObjectFromID(sceneMeta.objectIDs[i]).TransformGet()->Parent()) continue;
		//	DrawObjectNode(i);

		//}

		////Get the Window Rect through some magic math;
		//ImRect windowRect;
		//windowRect.Min = ImGui::GetWindowPos();
		//windowRect.Max.x = windowRect.Min.x + ImGui::GetWindowWidth();
		//windowRect.Max.y = windowRect.Min.y + ImGui::GetWindowHeight();
		//windowRect.Min.y = ImGui::GetItemRectMax().y + 7;

		//if (ImGui::BeginDragDropTargetCustom(windowRect, ImGuiID(7985548))) {

		//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_OBJECT_NODE")) RemoveParent(*(uint32_t*) payload->Data);

		//	ImGui::EndDragDropTarget();

		//}

	}

	void SceneHierarchy::DrawEntityNode(InternalEntity* entity) {

		ImGui::PushID((int) (uint64_t) entity);

		ImGuiTreeNodeFlags flags = ((selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx(entity, flags, entity->name.c_str());

		if (ImGui::BeginDragDropSource()) {

			uint32_t id = entity->ID();
			ImGui::SetDragDropPayload("SCH_ENTITY_NODE", &id, sizeof(uint64_t), ImGuiCond_Once);
			ImGui::EndDragDropSource();

		}

		if (ImGui::IsItemClicked()) clickedEntityID = entity->ID();
		if (ImGui::IsMouseReleased(0) && entity->ID() == clickedEntityID) {

			if (Properties::IsDragDropTargetHovered()) clickedEntityID = invalidID;
			else selectedEntity = entity;

		}
		if (opened) {

			for (int i = 0; i < entity->GetTransform()->NumOfChildren(); i++) {

				DrawEntityNode(entity->GetTransform()->GetChild(i)->GetEntity());

			}

			ImGui::TreePop();

		}

		ImGui::PopID();

	}

	//void SceneHierarchy::DrawObjectNode(uint32_t& objIDIndex) {

	//	Object* obj = &GetObjectFromID(sceneMeta.objectIDs[objIDIndex]);

	//	ImGuiTreeNodeFlags flags = ((selectedObj == obj) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	//	bool opened = ImGui::TreeNodeEx((void*) (uint64_t) obj->GetID(), flags, obj->TagGet()->name.c_str());

	//	//The Drag and Drop stuff needs to be here, since when we open the node
	//	//and the object has children, the Drag Drop will only work on the last Child
	//	//if we try to do this after rendering the children
	//	if (ImGui::BeginDragDropSource()) {

	//		ImGui::SetDragDropPayload("SCH_OBJECT_NODE", &objIDIndex, sizeof(uint64_t), ImGuiCond_Once);
	//		ImGui::EndDragDropSource();

	//	}
	//	if (ImGui::BeginDragDropTarget()) {

	//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_OBJECT_NODE")) ChangeObjectParent(*(uint32_t*) payload->Data, objIDIndex);
	//		ImGui::EndDragDropTarget();

	//	}

	//	ImRect betweenNodes;
	//	betweenNodes.Min = ImGui::GetItemRectMin();
	//	betweenNodes.Max = ImGui::GetItemRectMax();
	//	betweenNodes.Min.y -= 4;
	//	betweenNodes.Max.y -= 18;

	//	if (ImGui::BeginDragDropTargetCustom(betweenNodes, ImGuiID(7985548))) {

	//		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_OBJECT_NODE")) MoveObjectNode(objIDIndex, *(uint32_t*) payload->Data);

	//		ImGui::EndDragDropTarget();

	//	}

	//	if (ImGui::IsItemClicked()) clickedObjID = obj->GetID();
	//	if (ImGui::IsMouseReleased(0) && obj->GetID() == clickedObjID) {

	//		if (Properties::IsDragDropTargetHovered()) clickedObjID = -1;
	//		else  selectedObj = obj;

	//	}

	//	if (opened) {

	//		uint32_t childIndex;
	//		for (int i = 1; i <= obj->TransformGet()->NumOfChildren(); i++) {

	//			childIndex = objIDIndex + i;
	//			DrawObjectNode(childIndex);

	//		}

	//		ImGui::TreePop();

	//	}

	//	if (ImGui::BeginPopupContextItem()) {

	//		if (ImGui::MenuItem("Delete")) RemoveObjectNode(objIDIndex);

	//		ImGui::EndPopup();

	//	}
	//	

	//}
	//void SceneHierarchy::RemoveObjectNode(uint32_t objIDIndex) {

	//	Object* obj = &GetObjectFromID(sceneMeta.objectIDs[objIDIndex]);
	//	if (selectedObj == obj) selectedObj = nullptr;

	//	scene->DestroyObject(*obj);

	//	SetChanges(true);

	//}
	//void SceneHierarchy::MoveObjectNode(uint32_t& objIDIndex, uint32_t objToMoveIDIndex) {

	//	if (objIDIndex == objToMoveIDIndex || objToMoveIDIndex + 1 == objIDIndex) return;

	//	Object& obj = GetObjectFromID(sceneMeta.objectIDs[objIDIndex]);
	//	Object& objToMove = GetObjectFromID(sceneMeta.objectIDs[objToMoveIDIndex]);

	//	sceneMeta.objectIDs.insert(sceneMeta.objectIDs.begin() + objIDIndex, sceneMeta.objectIDs[objToMoveIDIndex]);
	//	sceneMeta.objectIDs.erase(sceneMeta.objectIDs.begin() + (objIDIndex > objToMoveIDIndex ? objToMoveIDIndex : objToMoveIDIndex + 1));
	//	objIDIndex++;

	//	if (objToMove.TransformGet()->Parent()) objToMove.TransformGet()->Parent()->RemoveChild(objToMove.TransformGet());
	//	if (obj.TransformGet()->Parent()) obj.TransformGet()->Parent()->AddChild(objToMove.TransformGet());

	//	SetChanges(true);

	//}

	//void SceneHierarchy::ChangeObjectParent(uint32_t& objIDIndex, uint32_t& newParentIDIndex) {

	//	Object* obj = &GetObjectFromID(sceneMeta.objectIDs[objIDIndex]);
	//	Transform* parent = GetObjectFromID(sceneMeta.objectIDs[newParentIDIndex]).TransformGet();

	//	if (obj->TransformGet()->Parent() != parent && parent->Parent() != obj->TransformGet()) {

	//		if (obj->TransformGet()->Parent()) obj->TransformGet()->Parent()->RemoveChild(obj->TransformGet());
	//		parent->AddChild(obj->TransformGet());

	//		sceneMeta.objectIDs.insert(sceneMeta.objectIDs.begin() + newParentIDIndex + 1, sceneMeta.objectIDs[objIDIndex]);
	//		sceneMeta.objectIDs.erase(sceneMeta.objectIDs.begin() + (newParentIDIndex > objIDIndex ? objIDIndex : objIDIndex + 1));

	//		newParentIDIndex--;

	//		SetChanges(true);

	//	}

	//}

	//void SceneHierarchy::PopupWindow() {

	//	if (ImGui::MenuItem("Empty Object")) {

	//		selectedObj = &scene->CreateObject("Object");
	//		SetChanges(true);

	//	}

	//	ImGui::Separator();

	//	if (ImGui::BeginMenu("3D Objects")) {

	//		if (ImGui::MenuItem("Plane")) {

	//			selectedObj = &scene->CreateObject("Plane");
	//			MeshRenderer* renderer = selectedObj->AddComponent<MeshRenderer>();
	//			Mesh mesh;

	//			mesh.vertices = GetPlaneVertices();
	//			mesh.normals = GetPlaneNormals();
	//			mesh.colors = GetPlaneColors();
	//			mesh.indices = GetPlaneIndices();

	//			renderer->meshes.push_back(mesh);
	//			SetChanges(true);

	//		}
	//		if (ImGui::MenuItem("Cube")) {

	//			selectedObj = &scene->CreateObject("Cube");
	//			MeshRenderer* renderer = selectedObj->AddComponent<MeshRenderer>();
	//			Mesh mesh;

	//			mesh.vertices = GetCubeVertices();
	//			mesh.normals = GetCubeNormals();
	//			mesh.colors = GetCubeColors();
	//			mesh.indices = GetCubeIndices();

	//			renderer->meshes.push_back(mesh);
	//			SetChanges(true);

	//		}

	//		ImGui::EndMenu();

	//	}

	//	if (ImGui::MenuItem("Light")) {

	//		selectedObj = &scene->CreateObject("Light");
	//		Light* l = selectedObj->AddComponent<Light>();

	//		SetChanges(true);

	//	}

	//}

	//void SceneHierarchy::RemoveParent(uint32_t objIDIndex) {

	//	Object& obj = GetObjectFromID(sceneMeta.objectIDs[objIDIndex]);

	//	if (!obj.TransformGet()->Parent()) return;
	//	
	//	obj.TransformGet()->Parent()->RemoveChild(obj.TransformGet());

	//	sceneMeta.objectIDs.push_back(sceneMeta.objectIDs[objIDIndex]);
	//	sceneMeta.objectIDs.erase(sceneMeta.objectIDs.begin() + objIDIndex);

	//}

	//bool SceneHierarchy::OnObjectCreated(const Event& e) {

	//	ObjectEvent* event = (ObjectEvent*) &e;

	//	sceneMeta.objectIDs.push_back(event->obj->GetID());

	//	return false;

	//}
	//bool SceneHierarchy::OnObjectDestroyed(const Event& e) {

	//	ObjectEvent* event = (ObjectEvent*) &e;
	//	uint32_t objIDIndex;

	//	for(objIDIndex = 0; objIDIndex < sceneMeta.objectIDs.size() && sceneMeta.objectIDs[objIDIndex] != event->obj->GetID(); objIDIndex++) { }
	//	sceneMeta.objectIDs.erase(sceneMeta.objectIDs.begin() + objIDIndex);

	//	return false;

	//}

	void SceneHierarchy::SaveSceneMeta() {

		sceneMeta.Serialize(scene);

	}
	void SceneHierarchy::LoadSceneMeta() {

		sceneMeta.Deserialize(scene);

	}

	MetaFile::SceneMeta* SceneHierarchy::GetSceneMetaPointer() {

		return &sceneMeta;

	}

}
