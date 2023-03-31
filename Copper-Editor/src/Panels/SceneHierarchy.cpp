#include "SceneHierarchy.h"

#include "Core/EditorApp.h"
#include "Core/Utils/ModelLoader.h"

#include "Panels/Properties.h"

#include "Engine/Renderer/Primitives.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

using namespace Copper;

namespace Editor {

	uint32_t clickedEntityID = invalidID;

	SceneHierarchy::SceneHierarchy() : Panel("Scene Hierarchy") { }

	void SceneHierarchy::UI() {

		CU_ASSERT(scene, "SceneHierarchy::scene is nullptr... HOW ???!!??!!");

		if (ImGui::BeginPopupContextWindow(0, 1, false)) {

			PopupWindow();
			ImGui::EndPopup();

		}

		for (uint32_t eID : GetSceneMeta()->objectIDs) {

			InternalEntity* entity = GetEntityFromID(eID);
			if (entity->GetTransform()->Parent()) continue;

			DrawEntityNode(entity);

		}

		ImRect windowRect;
		windowRect.Min = ImGui::GetWindowPos();

		windowRect.Max.x = windowRect.Min.x + ImGui::GetWindowWidth();
		windowRect.Max.y = windowRect.Min.y + ImGui::GetWindowHeight();
		windowRect.Min.y = ImGui::GetItemRectMax().y + 7;

		if (ImGui::BeginDragDropTargetCustom(windowRect, ImGuiID(310320231753))) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_ENTITY_NODE"))
				GetEntityFromID(*(uint32_t*) payload->Data)->GetTransform()->SetParent(nullptr);

			ImGui::EndDragDropTarget();

		}

	}

	void SceneHierarchy::DrawEntityNode(InternalEntity* entity) {

		ImGui::PushID((int) (uint64_t) entity);

		ImGuiTreeNodeFlags flags = ((selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx(entity, flags, entity->name.c_str());

		if (ImGui::IsItemClicked()) clickedEntityID = entity->ID();
		if (ImGui::IsMouseReleased(0) && entity->ID() == clickedEntityID) {

			if (Properties::IsDragDropTargetHovered()) clickedEntityID = invalidID;
			else selectedEntity = entity;

		}

		if (ImGui::BeginDragDropSource()) {

			uint32_t id = entity->ID();
			ImGui::SetDragDropPayload("SCH_ENTITY_NODE", &id, sizeof(uint64_t), ImGuiCond_Once);
			ImGui::EndDragDropSource();

		}
		if (ImGui::BeginDragDropTarget()) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_ENTITY_NODE"))
				entity->GetTransform()->AddChild(GetEntityFromID(*(uint32_t*) payload->Data)->GetTransform());

			ImGui::EndDragDropTarget();

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

		if (ImGui::MenuItem("Entity")) {

			selectedEntity = scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one);
			SetChanges(true);

		}

		ImGui::Separator();

		if (ImGui::BeginMenu("3D Objects")) {

			if (ImGui::MenuItem("Plane")) {

				selectedEntity = scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one, "Plane");
				MeshRenderer* renderer = selectedEntity->AddComponent<MeshRenderer>();
				Mesh mesh;

				mesh.vertices = GetPlaneVertices();
				mesh.normals = GetPlaneNormals();
				mesh.colors = GetPlaneColors();
				mesh.indices = GetPlaneIndices();

				renderer->meshes.push_back(mesh);
				SetChanges(true);

			}
			if (ImGui::MenuItem("Cube")) {

				selectedEntity = scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one, "Cube");
				MeshRenderer* renderer = selectedEntity->AddComponent<MeshRenderer>();
				Mesh mesh;

				mesh.vertices = GetCubeVertices();
				mesh.normals = GetCubeNormals();
				mesh.colors = GetCubeColors();
				mesh.indices = GetCubeIndices();

				renderer->meshes.push_back(mesh);
				SetChanges(true);

			}

			ImGui::EndMenu();

		}

		if (ImGui::MenuItem("Light")) {

			selectedEntity = scene->CreateEntity(Vector3::zero, Vector3::zero, Vector3::one, "Light");
			Light* l = selectedEntity->AddComponent<Light>();

			SetChanges(true);

		}

	}

}