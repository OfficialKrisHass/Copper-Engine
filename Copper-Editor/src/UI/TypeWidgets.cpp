#include "TypeWidgets.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>

static inline ImVec2  operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }

#define DISABLED_BUTTON(label, size) ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);\
									 ImGui::Button(label, size);\
									 ImGui::PopItemFlag()

namespace Editor::UI {

	using namespace Copper;

	bool EditBool(const std::string& name, bool* show) {

		return ImGui::Checkbox(name.c_str(), show);;

	}
	bool EditInt(const std::string& name, int32* show) {

		return ImGui::DragInt(name.c_str(), show, DragIntSpeed);

	}
	bool EditUInt(const std::string& name, uint32* show) {

		return ImGui::DragInt(name.c_str(), (int*) show, DragIntSpeed, 0);;

	}
	bool EditFloat(const std::string& name, float* show) {

		return ImGui::DragFloat(name.c_str(), show, DragFloatSpeed);

	}
	bool EditDouble(const std::string& name, double* show) {

		return ImGui::DragFloat(name.c_str(), (float*) show, DragFloatSpeed);

	}
	bool EditString(const std::string& name, std::string* show) {

		return ImGui::InputText(name.c_str(), show);

	}
	bool EditChar(const std::string& name, char* show) {

		return ImGui::InputText(name.c_str(), show, sizeof(char));

	}

	bool EditVector2(const std::string& name, Vector2* vec) {

		bool ret = false;

		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// Init

		ImGui::PushID(name.c_str());

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));

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

		return ret;

	}
	bool EditVector3(const std::string& name, Vector3* vec) {

		bool ret = false;

		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		const ImVec2 buttonSize = { lineHeight, lineHeight };

		// Init

		ImGui::PushID(name.c_str());

		ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));

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

		return ret;

	}
	bool EditVector4(const std::string& name, Vector4* vec) {

		bool ret = false;

		const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// Init

		ImGui::PushID(name.c_str());

		ImGui::PushMultiItemsWidths(5, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));

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

		return ret;

	}
	bool EditColor(const std::string& name, Color* col) {

		bool ret = false;

		ImGui::PushID(name.c_str());

		ret = ImGui::ColorEdit3("##Color", &col->r);
		ImGui::SameLine();
		ImGui::Text(name.c_str());

		ImGui::PopID();

		return ret;

	}

	bool EditEntity(const std::string& name, InternalEntity** entity) {

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

		return ret;

	}
	bool EditTransform(const std::string& name, Transform** transform) {

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

		return ret;

	}

	bool EditTexture(const std::string& name, Copper::Texture* texture) {

		ImGuiID id = ImGuiID((uint32) (uint64) name.c_str());
		ImGui::PushID(id);

		bool ret = false;
		std::string nodeText;

		if (*texture) {

			fs::path tmp = texture->Path();
			nodeText = tmp.filename().string();

		} else
			nodeText = "None";
		nodeText += " (Texture)";

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

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FB_TEXTURE_PATH")) {

				ret = true;
				texture->Create((const char*) payload->Data);

			}

			ImGui::EndDragDropTarget();

		}

		ImGui::PopID();

		return ret;

	}

	bool EditDropDown(const std::string& name, const char* items[], uint32 count, uint32* selected) {

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
	bool EditMask(const std::string& name, uint32& mask, uint32 num, uint32 maskOffset, char startLabel) {

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