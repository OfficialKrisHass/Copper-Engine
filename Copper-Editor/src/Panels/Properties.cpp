#include "Properties.h"

#include "Engine/Scripting/ScriptingCore.h"

#include "Core/EditorApp.h"
#include "Core/SceneMeta.h"

#include "Panels/SceneHierarchy.h"

#include "Viewport/SceneCamera.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>

#include <cstring>

static inline ImVec2  operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }

#define BindShowFunc(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

#define DISABLED_BUTTON(label, size) ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);\
									 ImGui::Button(label, size);\
									 ImGui::PopItemFlag()

#define DragIntSpeed 1.0f
#define DragFloatSpeed 0.01f

#define FRAME_WIDTH 241
#define FRAME_HEIGHT 24

using namespace Copper;

namespace Editor {

	static const char* s_lightTypes[] = { "Point", "Directional" };

	template<typename T> static bool DrawComponent(const std::string& name, T* component);
	static bool DrawComponent(const std::string& name, Transform* component);

	Properties::Properties() : Panel("Properties") {

	}

	void Properties::UI() {

		if (!*m_selectedEntity) return;

		RenderEntity();

	}

	void Properties::RenderEntity() {

		InternalEntity* entity = *m_selectedEntity;

		char buffer[128] = {};
		std::strncpy(buffer, entity->name.c_str(), sizeof(buffer));

		if (ImGui::InputText("##Name", buffer, sizeof(buffer))) {

			entity->name = buffer;
			Editor::SetChanges(true);

		}

		ImGui::SameLine();
		ImGui::Separator();

		if(DrawComponent("Transform", entity->GetTransform())) {
			
			Transform* transform = entity->GetTransform();

			Vector3 pos = transform->Position();
			if (ShowVector3("Position", &pos))
				transform->SetPosition(pos);

			Vector3 newRot = transform->Rotation().EulerAngles();
			if (ShowVector3("Rotation", &newRot))
				transform->SetRotation(Quaternion(newRot));

			Vector3 scale = transform->Scale();
			if (ShowVector3("Scale", &scale))
				transform->SetScale(scale);

			ImGui::PopID();
			
		}

		if (Light* light = entity->GetComponent<Light>()) RenderLight(light);
		if (Camera* camera = entity->GetComponent<Camera>()) RenderCamera(camera);

		if (RigidBody* rb = entity->GetComponent<RigidBody>()) RenderRigidBody(rb);

		if (BoxCollider* collider = entity->GetComponent<BoxCollider>()) RenderBoxCollider(collider);
		if (SphereCollider* collider = entity->GetComponent<SphereCollider>()) RenderSphereCollider(collider);
		if (CapsuleCollider* collider = entity->GetComponent<CapsuleCollider>()) RenderCapsuleCollider(collider);

		if (ScriptComponent* script = entity->GetComponent<ScriptComponent>()) RenderScriptComponent(script);

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

			Collider* collider = entity->GetComponent<Collider>();
				
			if (ImGui::MenuItem("Light")) {
				
				entity->AddComponent<Light>()->color.r = 0.5f;
				Editor::SetChanges(true);
			
			} else if (ImGui::MenuItem("Mesh Renderer")) {
				
				entity->AddComponent<MeshRenderer>();
				Editor::SetChanges(true);
			
			} else if (ImGui::MenuItem("Camera")) {
				
				entity->AddComponent<Camera>();
				Editor::SetChanges(true);
			
			} else if (ImGui::MenuItem("Rigid Body")) {

				entity->AddComponent<RigidBody>();
				Editor::SetChanges(true);

			} else if (ImGui::MenuItem("Box Collider") && !collider) {

				entity->AddComponent<BoxCollider>();
				Editor::SetChanges(true);

			} else if (ImGui::MenuItem("Sphere Collider") && !collider) {

				entity->AddComponent<SphereCollider>();
				Editor::SetChanges(true);

			} else if (ImGui::MenuItem("Capsule Collider") && !collider) {

				entity->AddComponent<CapsuleCollider>();
				Editor::SetChanges(true);

			}

			ImGui::Separator();

			for (std::string scriptName : Scripting::GetScriptComponents()) {

				if (ImGui::MenuItem(scriptName.c_str())) {
					
					ScriptComponent* script = entity->AddComponent<ScriptComponent>();

					script->Init(scriptName);
					Editor::SetChanges(true);
						
				}

			}

			ImGui::EndPopup();
				
		}
		
	}

	void Properties::RenderLight(Light* light) {

		if (!DrawComponent<Light>("Light", light)) return;

		ShowDropDown("Type", s_lightTypes, LIGHT_TYPES, (uint32*) &light->type);

		ShowColor("Color", &light->color);
		ShowFloat("Intensity", &light->intensity);

		ImGui::PopID();

	}
	void Properties::RenderCamera(Camera* camera) {

		if (!DrawComponent<Camera>("Camera", camera)) return;

		ShowFloat("FOV", &camera->fov);
		ShowFloat("Near Plane", &camera->nearPlane);
		ShowFloat("Far Plane", &camera->farPlane);

		ImGui::PopID();

	}

	void Properties::RenderRigidBody(RigidBody* rb) {

		if (!DrawComponent<RigidBody>("Rigid Body", rb)) return;

		bool changed = false;

		if (ShowBool("Static", &rb->isStatic) && !changed) changed = true;
		if (ShowBool("Gravity", &rb->gravity) && !changed) changed = true;

		if (ShowFloat("Mass", &rb->mass) && !changed) changed = true;

		ImGui::Text("Locks");
		ImGui::Separator();

		// Position Lock
		if (ShowMask("Position", (uint32&) rb->m_lockMask, 3) && !changed) changed = true;
		if (ShowMask("Rotation", (uint32&) rb->m_lockMask, 3, 3) && !changed) changed = true;

		if (changed && IsSceneRuntimeRunning()) rb->Setup();

		ImGui::PopID();

	}
	
	void Properties::RenderBoxCollider(BoxCollider* collider) {

		if (!DrawComponent<BoxCollider>("Box Collider", collider)) return;

		ShowBool("Trigger", &collider->trigger);
		ShowVector3("Center", &collider->center);

		ImGui::Separator();

		ShowVector3("Size", &collider->size);

		ImGui::PopID();

	}
	void Properties::RenderSphereCollider(SphereCollider* collider) {

		if (!DrawComponent<SphereCollider>("Sphere Collider", collider)) return;

		ShowBool("Trigger", &collider->trigger);
		ShowVector3("Center", &collider->center);

		ImGui::Separator();

		ShowFloat("Radius", &collider->radius);

		ImGui::PopID();

	}
	void Properties::RenderCapsuleCollider(CapsuleCollider* collider) {

		if (!DrawComponent<CapsuleCollider>("Capsule Collider", collider)) return;

		ShowBool("Trigger", &collider->trigger);
		ShowVector3("Center", &collider->center);

		ImGui::Separator();

		ShowFloat("Radius", &collider->radius);
		ShowFloat("Height", &collider->height);

		ImGui::PopID();

	}

	void Properties::RenderScriptComponent(ScriptComponent* script) {

		if (!DrawComponent<ScriptComponent>(script->name, script)) return;
		if (!*script) {

			ImGui::Text("This Script is invalid or doesn't exist anymore");
			ImGui::PopID();

			return;

		}

		for (ScriptField& field : Scripting::GetScriptFields(script->name)) {

			switch (field.type) {

				case ScriptField::Type::Int:			RenderScriptField<int32>(script, field, BindShowFunc(ShowInt)); break;
				case ScriptField::Type::UInt:			RenderScriptField<uint32>(script, field, BindShowFunc(ShowUInt)); break;
				case ScriptField::Type::Float:			RenderScriptField<float>(script, field, BindShowFunc(ShowFloat)); break;

				case ScriptField::Type::Vector2:		RenderScriptField<Vector2>(script, field, BindShowFunc(ShowVector2)); break;
				case ScriptField::Type::Vector3:		RenderScriptField<Vector3>(script, field, BindShowFunc(ShowVector3)); break;

				case ScriptField::Type::Entity:			RenderScriptField<InternalEntity*>(script, field, BindShowFunc(ShowEntity)); break;
				case ScriptField::Type::Transform:		RenderScriptField<Transform*>(script, field, BindShowFunc(ShowTransform)); break;

			}

		}

		ImGui::PopID();

	}

	template<typename T> static bool DrawComponent(const std::string& name, T* component) {

		ImGui::PushID((uint32) (uint64) component);

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 {4, 4});

		bool opened = ImGui::TreeNodeEx((void*) component, flags, name.c_str());

		ImGui::PopStyleVar();
		if (opened)
			ImGui::TreePop();
		else
			ImGui::PopID();

		if (ImGui::BeginPopupContextItem()) {

			if (ImGui::MenuItem("Remove Component")) {
				
				component->GetEntity()->RemoveComponent<T>();

				SetChanges(true);
				ImGui::EndPopup();
				ImGui::PopID();

				return false;
			
			}

			ImGui::EndPopup();

		}

		return opened;

	}
	static bool DrawComponent(const std::string& name, Transform* component) {

		ImGui::PushID((uint32) (uint64) component);

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });

		bool opened = ImGui::TreeNodeEx((void*) component, flags, name.c_str());

		ImGui::PopStyleVar();
		if (opened)
			ImGui::TreePop();
		else
			ImGui::PopID();

		return opened;

	}

	template<typename T, typename F> void Properties::RenderScriptField(ScriptComponent* script, const ScriptField& field, F showFunc) {

		T tmp;
		script->GetFieldValue(field, &tmp);

		bool changed = showFunc(field.name, &tmp);
		if (changed) script->SetFieldValue(field, &tmp);

	}

	bool Properties::ShowBool(const std::string& name, bool* show) {

		bool ret = false;
		ret = ImGui::Checkbox(name.c_str(), show);

		if (ret) Editor::SetChanges(true);
		return ret;

	}
	bool Properties::ShowInt(const std::string& name, int32* show) {

		bool ret = false;
		ret = ImGui::DragInt(name.c_str(), show, DragIntSpeed);

		if (ret) SetChanges(true);
		return ret;
		
	}
	bool Properties::ShowUInt(const std::string& name, uint32* show) {

		bool ret = false;
		ret = ImGui::DragInt(name.c_str(), (int*) show, DragIntSpeed);
		if (*show < 0) *show = 0;

		if (ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowFloat(const std::string& name, float* show) {

		bool ret = false;
		ret = ImGui::DragFloat(name.c_str(), show, DragFloatSpeed);

		if (ret) SetChanges(true);
		return ret;
		
	}
	bool Properties::ShowDouble(const std::string& name, double* show) {

		bool ret = false;
		ret = ImGui::DragFloat(name.c_str(), (float*) show, DragFloatSpeed);

		if (ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowString(const std::string& name, std::string* show) {

		bool ret = false;
		ret = ImGui::InputText(name.c_str(), show);

		if (ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowChar(const std::string& name, char* show) {

		bool ret = false;
		if (ImGui::InputText(name.c_str(), show, sizeof(char))) ret = true;

		if (ret) SetChanges(true);
		return ret;

	}
	
	bool Properties::ShowVector2(const std::string& name, Vector2* vec) {

		bool ret = false;

		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

		const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
		const ImVec4 buttonColor = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

		// Init

		ImGui::PushID(name.c_str());

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 {0, 0});
		ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);

		// X
		
		DISABLED_BUTTON("X", buttonSize);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &vec->x, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Y
		
		DISABLED_BUTTON("Y", buttonSize);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &vec->y, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine(0.0f, 7.0f);

		// Text

		ImGui::Text(name.c_str());
		ImGui::PopItemWidth();


		// Cleanup
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::PopID();


		if (ret) SetChanges(true);
		return ret;
		
	}
	bool Properties::ShowVector3(const std::string& name, Vector3* vec) {

		bool ret = false;

		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

		const ImVec2 buttonSize = { lineHeight, lineHeight };
		const ImVec4 buttonColor = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

		// Init

		ImGui::PushID(name.c_str());

		ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 {0, 0});
		ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);

		// X

		DISABLED_BUTTON("X", buttonSize);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &vec->x, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Y

		DISABLED_BUTTON("Y", buttonSize);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &vec->y, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Z

		DISABLED_BUTTON("Z", buttonSize);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &vec->z, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine(0.0f, 7.0f);

		// Text

		ImGui::Text(name.c_str());
		ImGui::PopItemWidth();

		// Cleanup

		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::PopID();

		if(ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowVector4(const std::string& name, Vector4* vec) {

		bool ret = false;

		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		
		const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
		const ImVec4 buttonColor = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

		// Init

		ImGui::PushID(name.c_str());

		ImGui::PushMultiItemsWidths(5, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 {0, 0});
		ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);

		// X

		DISABLED_BUTTON("X", buttonSize);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &vec->x, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Y
		
		DISABLED_BUTTON("Y", buttonSize);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &vec->y, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// Z
		
		DISABLED_BUTTON("Z", buttonSize);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &vec->z, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// W
		
		DISABLED_BUTTON("W", buttonSize);

		ImGui::SameLine();
		if (ImGui::DragFloat("##W", &vec->w, DragFloatSpeed, 0.0f, 0.0f, "%.2f")) ret = true;
		ImGui::PopItemWidth();
		ImGui::SameLine(0.0f, 7.0f);

		// Text

		ImGui::Text(name.c_str());
		ImGui::PopItemWidth();

		// Cleanup

		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::PopID();

		if (ret) SetChanges(true);
		return ret;
		
	}
	bool Properties::ShowColor(const std::string& name, Color* col) {

		bool ret = false;

		ImGui::PushID(name.c_str());

		ret = ImGui::ColorEdit3("##Color", &col->r);
		ImGui::SameLine();
		ImGui::Text(name.c_str());

		ImGui::PopID();

		if (ret) SetChanges(true);
		return ret;
		
	}

	bool Properties::ShowEntity(const std::string& name, InternalEntity** entity) {

		ImGuiID id = ImGuiID((uint32) (uint64) name.c_str());
		ImGui::PushID(id);

		bool ret = false;
		std::string nodeText;

		if (*entity)
			nodeText = (*entity)->name;
		else
			nodeText = "None";
		nodeText += " (Copper Entity)";

		const ImGuiStyle& style = ImGui::GetStyle();
		const ImVec2 cursorPos = ImGui::GetCurrentWindow()->DC.CursorPos;

		const float textSizeY = ImGui::CalcTextSize(name.c_str(), nullptr, true).y;
		const ImVec2 frameSize = ImGui::CalcItemSize({ 0, 0 }, ImGui::CalcItemWidth(), textSizeY + style.FramePadding.y * 2.0f);

		const bool hovered = ImGui::IsMouseHoveringRect(cursorPos, cursorPos + frameSize);

		// Frame

		if (hovered)
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetColorU32(ImGuiCol_FrameBgHovered));

		ImGui::BeginChildFrame(id, frameSize);
		ImGui::Text(nodeText.c_str());
		ImGui::EndChildFrame();

		if (hovered)
			ImGui::PopStyleColor();

		// Name

		ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
		ImGui::Text(name.c_str());

		// Drag Drop

		// For some reason the rect sometimes flickers, and it seems to be based entirely on randomness
		// sometimes it does, sometimes not, Love it :)))))))))))
		// TODO: Fix

		if (ImGui::BeginDragDropTargetCustom({ cursorPos, cursorPos + frameSize }, ImGuiID((uint32) (uint64) name.c_str()))) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_ENTITY_NODE")) {

				ret = true;
				*entity = (InternalEntity*) payload->Data;

			}

			ImGui::EndDragDropTarget();

		}

		ImGui::PopID();

		if (ret) SetChanges(true);
		return ret;

	}
	bool Properties::ShowTransform(const std::string& name, Transform** transform) {

		bool ret = false;

		ImGuiID id = ImGuiID((uint32) (uint64) name.c_str());
		ImGui::PushID(id);

		std::string nodeText;

		if (*transform)
			nodeText = (*transform)->GetEntity()->name;
		else
			nodeText = "None";
		nodeText += " (Transform)";

		const ImGuiStyle& style = ImGui::GetStyle();
		const ImVec2 cursorPos = ImGui::GetCurrentWindow()->DC.CursorPos;

		const float textSizeY = ImGui::CalcTextSize(name.c_str(), nullptr, true).y;
		const ImVec2 frameSize = ImGui::CalcItemSize({ 0, 0 }, ImGui::CalcItemWidth(), textSizeY + style.FramePadding.y * 2.0f);

		const bool hovered = ImGui::IsMouseHoveringRect(cursorPos, cursorPos + frameSize);

		// Frame

		if (hovered)
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetColorU32(ImGuiCol_FrameBgHovered));

		ImGui::BeginChildFrame(id, frameSize);
		ImGui::Text(nodeText.c_str());
		ImGui::EndChildFrame();

		if (hovered)
			ImGui::PopStyleColor();

		// Name

		ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
		ImGui::Text(name.c_str());

		// Drag Drop

		// For some reason the rect sometimes flickers, and it seems to be based entirely on randomness
		// sometimes it does, sometimes not, Love it :)))))))))))
		// TODO: Fix

		if (ImGui::BeginDragDropTargetCustom({ cursorPos, cursorPos + frameSize }, ImGuiID((uint32) (uint64) name.c_str()))) {

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCH_ENTITY_NODE")) {

				ret = true;
				*transform = ((InternalEntity*) payload->Data)->GetTransform();

			}

			ImGui::EndDragDropTarget();

		}

		ImGui::PopID();

		if (ret) SetChanges(true);
		return ret;

	}

	bool Properties::ShowDropDown(const std::string& name, const char* items[], uint32 count, uint32* selected) {

		bool ret = false;

		if (ImGui::BeginCombo(name.c_str(), items[*selected])) {

			for (int i = 0; i < count; i++) {

				const bool isSelected = (i == *selected);
				if (ImGui::Selectable(items[i], isSelected))
					*selected = i;

				if (isSelected)
					ImGui::SetItemDefaultFocus();

			}
			ImGui::EndCombo();

		}

		return ret;

	}
	bool Properties::ShowMask(const std::string& name, uint32& mask, uint32 num, uint32 maskOffset, char startLabel) {

		ImGui::Text(name.c_str());

		bool tmp;
		bool ret = false;
		std::string label = "";

		for (uint32 i = 0, bit = maskOffset; i < num; i++, maskOffset++) {

			ImGui::SameLine();

			tmp = mask & 1 << maskOffset;
			label = (char) (startLabel + i);
			label += "##" + name;

			if (!ImGui::Checkbox(label.c_str(), &tmp))
				continue;

			ret = true;

			if (tmp)
				mask |= 1 << maskOffset;
			else
				mask &= ~(1 << maskOffset);

		}

		return ret;

	}
	
}