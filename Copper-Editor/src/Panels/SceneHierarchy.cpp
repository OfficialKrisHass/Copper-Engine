#include "SceneHierarchy.h"

#include "Core/EditorApp.h"
#include "Core/Utils/ModelLoader.h"

#include "Panels/Properties.h"

#include "Engine/Renderer/Primitives.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

#define MOVE_RECT_HEIGHT 4.0f

using namespace Copper;

namespace Editor {

	uint32_t clickedEntityID = INVALID_ENTITY_ID;

	SceneHierarchy::SceneHierarchy() : Panel("Scene Hierarchy") { }

	void SceneHierarchy::UI() {

		if (ImGui::BeginPopupContextWindow("##Scene Hierarchy")) {

			PopupWindow();
			ImGui::EndPopup();

		}

		for (currentNodeIndex = 0; currentNodeIndex < GetSceneMeta()->objectIDs.size(); currentNodeIndex++) {

			InternalEntity* entity = GetSceneMeta()->GetEntity(currentNodeIndex);
			if (!entity) continue;
			if (entity->GetTransform()->Parent()) continue;

			DrawEntityNode(entity);

		}

		RemoveParentTarget();
		MoveEntityLastTarget();

		currentNodeIndex = -1;

	}

	void SceneHierarchy::DrawEntityNode(InternalEntity* entity) {

		ImGui::PushID((int) (uint64_t) entity);

		ImGuiTreeNodeFlags flags = ((selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx(entity, flags, entity->name.c_str());

		if (ImGui::IsItemClicked()) clickedEntityID = entity->ID();
		if (ImGui::IsMouseReleased(0) && entity->ID() == clickedEntityID) {

			if (Properties::IsDragDropTargetHovered()) clickedEntityID = INVALID_ENTITY_ID;
			else selectedEntity = entity;

		}

		if (ImGui::BeginDragDropSource()) {

			uint32_t data = currentNodeIndex;
			ImGui::SetDragDropPayload("SCH_ENTITY_NODE", &data, sizeof(uint32_t), ImGuiCond_Once);
			ImGui::EndDragDropSource();

		}
		if (ImGui::BeginDragDropTarget()) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_ENTITY_NODE"))
				entity->GetTransform()->AddChild(GetSceneMeta()->GetEntity(*(uint32_t*) payload->Data)->GetTransform());

			ImGui::EndDragDropTarget();

		}

		const ImVec2 itemMin = ImGui::GetItemRectMin();
		const ImRect reorderRect = ImRect({ itemMin.x, itemMin.y - MOVE_RECT_HEIGHT }, { ImGui::GetItemRectMax().x, itemMin.y });
		if (ImGui::BeginDragDropTargetCustom(reorderRect, (int64_t) entity)) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_ENTITY_NODE"))
				MoveEntity(*(uint32_t*) payload->Data);

			ImGui::EndDragDropTarget();

		}

		if (ImGui::BeginPopupContextItem()) {

			if (ImGui::MenuItem("Remove")) {

				scene->RemoveEntity(entity);
				selectedEntity = nullptr;

				ImGui::EndPopup();
				ImGui::PopID();

				return;

			}

			ImGui::EndPopup();

		}

		if (opened) {

			for (int i = 0; i < entity->GetTransform()->NumOfChildren(); i++) {

				DrawEntityNode(entity->GetTransform()->GetChild(i)->GetEntity());

			}

			ImGui::TreePop();

		}

		ImGui::PopID();

	}
	void SceneHierarchy::PopupWindow() {

		if (ImGui::MenuItem("Entity", 0, false, scene)) {

			selectedEntity = scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one);
			SetChanges(true);

		}

		ImGui::Separator();

		if (ImGui::BeginMenu("3D Objects")) {

			if (ImGui::MenuItem("Plane", 0, false, scene)) {

				selectedEntity = scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one, "Plane");
				MeshRenderer* renderer = selectedEntity->AddComponent<MeshRenderer>();
				Mesh mesh;

				mesh.vertices = planeVertices;
				mesh.normals = planeNormals;
				mesh.colors = planeColors;
				mesh.indices = planeIndices;

				renderer->meshes.push_back(mesh);
				SetChanges(true);

			}
			if (ImGui::MenuItem("Cube", 0, false, scene)) {

				selectedEntity = scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one, "Cube");
				MeshRenderer* renderer = selectedEntity->AddComponent<MeshRenderer>();
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

		if (ImGui::MenuItem("Light", 0, false, scene)) {

			selectedEntity = scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one, "Light");
			Light* l = selectedEntity->AddComponent<Light>();

			SetChanges(true);

		}
		if (ImGui::MenuItem("Camera", 0, false, scene)) {

			selectedEntity = scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one, "Camera");
			Camera* c = selectedEntity->AddComponent<Camera>();

			SetChanges(true);

		}

	}

	void SceneHierarchy::RemoveParentTarget() {

		const ImVec2 regionMax = ImGui::GetWindowContentRegionMax();
		const ImRect windowRect{ { ImGui::GetWindowContentRegionMin().x + 1, ImGui::GetItemRectMax().y + 2 }, { regionMax.x, regionMax.y + 80 } };
		if (!ImGui::BeginDragDropTargetCustom(windowRect, ImGuiID(310320231753))) return;

		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_ENTITY_NODE"))
			GetSceneMeta()->GetEntity(*(uint32_t*) payload->Data)->GetTransform()->SetParent(nullptr);

		ImGui::EndDragDropTarget();

	}
	void SceneHierarchy::MoveEntityLastTarget() {

		const float itemMinX = ImGui::GetItemRectMin().x;
		const ImVec2 itemMax = ImGui::GetItemRectMax();
		const ImRect reorderRect = { { itemMinX, itemMax.y }, { itemMax.x, itemMax.y + MOVE_RECT_HEIGHT } };
		if (!ImGui::BeginDragDropTargetCustom(reorderRect, ImGuiID(679875643556))) return;

		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_ENTITY_NODE");
		if (!payload || *(uint32_t*) payload->Data == currentNodeIndex - 1) {

			ImGui::EndDragDropTarget();
			return;

		}

		uint32_t nodeIndex = *(uint32_t*) payload->Data;
		std::vector<uint32_t>& objectIDs = GetSceneMeta()->objectIDs;

		objectIDs.push_back(objectIDs[nodeIndex]);
		objectIDs.erase(objectIDs.begin() + nodeIndex);

		ImGui::EndDragDropTarget();

	}

	void SceneHierarchy::MoveEntity(uint32_t nodeIndex) const {

		if (nodeIndex == currentNodeIndex || nodeIndex == currentNodeIndex - 1) return;

		std::vector<uint32_t>& objectIDs = GetSceneMeta()->objectIDs;

		objectIDs.insert(objectIDs.begin() + currentNodeIndex, objectIDs[nodeIndex]);
		objectIDs.erase(objectIDs.begin() + (nodeIndex > currentNodeIndex ? nodeIndex + 1 : nodeIndex));

	}

}