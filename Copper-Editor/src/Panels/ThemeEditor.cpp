#include "ThemeEditor.h"

#include <ImGui/imgui.h>

#include <yaml-cpp/yaml.h>

#define COLOR_EDIT(color, text) ImGui::ColorEdit3(text, (float*) &style->Colors[color])
#define WRITE_COLOR(color) out << YAML::Key << color << YAML::Value << *(Color*) &(this->style->Colors[color])

using namespace Copper;

namespace Editor {

	void ThemeEditor::UI() {

		if (!this->style)
			this->style = &ImGui::GetStyle();

		ImGui::DragFloat("Rounding", &this->style->FrameRounding);

		ImGui::Separator();

		ColorEditor();

	}

	void ThemeEditor::ColorEditor() {

		COLOR_EDIT(ImGuiCol_WindowBg, "Window background");
		COLOR_EDIT(ImGuiCol_ChildBg, "Child window background");

		ImGui::Separator();

		COLOR_EDIT(ImGuiCol_Header, "Header");
		COLOR_EDIT(ImGuiCol_HeaderHovered, "Header hovered");
		COLOR_EDIT(ImGuiCol_HeaderActive, "Header active");

		ImGui::Text("");

		COLOR_EDIT(ImGuiCol_TitleBg, "Title bar");
		COLOR_EDIT(ImGuiCol_TitleBgActive, "Title bar active");
		COLOR_EDIT(ImGuiCol_TitleBgCollapsed, "Title bar collapsed");

		ImGui::Text("");

		COLOR_EDIT(ImGuiCol_Tab, "Tab");
		COLOR_EDIT(ImGuiCol_TabHovered, "Tab hovered");
		COLOR_EDIT(ImGuiCol_TabActive, "Tab active");
		COLOR_EDIT(ImGuiCol_TabUnfocused, "Tab unfocused");
		COLOR_EDIT(ImGuiCol_TabUnfocusedActive, "Tab unfocused active");

		ImGui::Separator();

		COLOR_EDIT(ImGuiCol_FrameBg, "Frame background");
		COLOR_EDIT(ImGuiCol_FrameBgHovered, "Frame background hovered");
		COLOR_EDIT(ImGuiCol_FrameBgActive, "Frame background active");

		ImGui::Separator();

		COLOR_EDIT(ImGuiCol_Button, "Button");
		COLOR_EDIT(ImGuiCol_ButtonHovered, "Button hovered");
		COLOR_EDIT(ImGuiCol_ButtonActive, "Button active");

		ImGui::Separator();

		COLOR_EDIT(ImGuiCol_CheckMark, "Check mark");

		ImGui::Separator();

		COLOR_EDIT(ImGuiCol_DragDropTarget, "Drag drop overlay");

	}

	void ThemeEditor::SaveTheme(const fs::path& path) {

		if (!this->style)
			this->style = &ImGui::GetStyle();

		YAML::Emitter out;
		out << YAML::BeginMap; // Main

		out << YAML::Key << "Rounding" << this->style->FrameRounding;

		out << YAML::Key << "Colors" << YAML::Value << YAML::BeginMap; // Colors

		WRITE_COLOR(ImGuiCol_WindowBg);
		WRITE_COLOR(ImGuiCol_ChildBg);

		WRITE_COLOR(ImGuiCol_Header);
		WRITE_COLOR(ImGuiCol_HeaderHovered);
		WRITE_COLOR(ImGuiCol_HeaderActive);

		WRITE_COLOR(ImGuiCol_TitleBg);
		WRITE_COLOR(ImGuiCol_TitleBgActive);
		WRITE_COLOR(ImGuiCol_TitleBgCollapsed);

		WRITE_COLOR(ImGuiCol_Tab);
		WRITE_COLOR(ImGuiCol_TabHovered);
		WRITE_COLOR(ImGuiCol_TabActive);
		WRITE_COLOR(ImGuiCol_TabUnfocused);
		WRITE_COLOR(ImGuiCol_TabUnfocusedActive);

		WRITE_COLOR(ImGuiCol_FrameBg);
		WRITE_COLOR(ImGuiCol_FrameBgHovered);
		WRITE_COLOR(ImGuiCol_FrameBgActive);

		WRITE_COLOR(ImGuiCol_Button);
		WRITE_COLOR(ImGuiCol_ButtonHovered);
		WRITE_COLOR(ImGuiCol_ButtonActive);

		WRITE_COLOR(ImGuiCol_CheckMark);

		WRITE_COLOR(ImGuiCol_DragDropTarget);

		out << YAML::EndMap; // Colors

		out << YAML::EndMap; // Main

		std::ofstream file;
		file.open(path.string() + ".cutheme");
		file << out.c_str();
		file.close();

	}
	void ThemeEditor::LoadTheme(const fs::path& path) {

		if (!this->style)
			this->style = &ImGui::GetStyle();

		YAML::Node main;
		try {

			main = YAML::LoadFile(path.string());

		} catch (YAML::ParserException e) {

			LogError("Failed to Read The Scene Meta Data file ({})\n    {}", path.string(), e.what());
			return;

		}

		this->style->FrameRounding = main["Rounding"].as<float>();

		YAML::Node colors = main["Colors"];

		for (YAML::const_iterator it = colors.begin(); it != colors.end(); ++it) {

			int col = it->first.as<int>();
			Color value = it->second.as<Color>();
			this->style->Colors[col] = *(ImVec4*) &value;

		}

	}

}