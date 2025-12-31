#include "ThemeEditor.h"

#include <ImGui/imgui.h>

#include <yaml-cpp/yaml.h>

#include <fstream>

#define COLOR_EDIT(color, text) ImGui::ColorEdit3(text, (float*) &m_style->Colors[color])
#define WRITE_COLOR(color) out << YAML::Key << color << YAML::Value << *(Color*) &(m_style->Colors[color])

using namespace Copper;

namespace Editor {

    void ThemeEditor::UI() {

        CUP_FUNCTION();
        CU_ASSERT(m_style, "Theme Editor style is nullptr, make sure you called Load or SaveTheme at least once");

        ImGui::DragFloat("Rounding", &m_style->FrameRounding);

        ImGui::Separator();

        ColorEditor();

    }

    void ThemeEditor::ColorEditor() {

        CUP_FUNCTION();

        COLOR_EDIT(ImGuiCol_WindowBg, "Window background");
        COLOR_EDIT(ImGuiCol_ChildBg, "Child window background");

        ImGui::Separator();

        COLOR_EDIT(ImGuiCol_Header, "Header");
        COLOR_EDIT(ImGuiCol_HeaderHovered, "Header hovered");
        COLOR_EDIT(ImGuiCol_HeaderActive, "Header active");

        ImGui::NewLine();

        COLOR_EDIT(ImGuiCol_TitleBg, "Title bar");
        COLOR_EDIT(ImGuiCol_TitleBgActive, "Title bar active");
        COLOR_EDIT(ImGuiCol_TitleBgCollapsed, "Title bar collapsed");

        ImGui::NewLine();

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

        COLOR_EDIT(ImGuiCol_Border, "Border");

        ImGui::Separator();

        COLOR_EDIT(ImGuiCol_CheckMark, "Check mark");

        ImGui::Separator();

        COLOR_EDIT(ImGuiCol_DragDropTarget, "Drag drop overlay");

    }

    void ThemeEditor::SaveTheme(const fs::path& path) {

        CUP_FUNCTION();

        if (!m_style)
            m_style = &ImGui::GetStyle();

        YAML::Emitter out;
        out << YAML::BeginMap; // Main

        out << YAML::Key << "Rounding" << this->m_style->FrameRounding;

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

        WRITE_COLOR(ImGuiCol_Border);

        WRITE_COLOR(ImGuiCol_CheckMark);

        WRITE_COLOR(ImGuiCol_DragDropTarget);

        out << YAML::EndMap; // Colors

        out << YAML::EndMap; // Main

        fs::path finalPath = path;
        finalPath.replace_extension("cutheme");

        std::ofstream file;
        file.open(finalPath.string());
        file << out.c_str();
        file.close();

    }
    void ThemeEditor::LoadTheme(const fs::path& path) {

        CUP_FUNCTION();

        if (m_style == nullptr)
            m_style = &ImGui::GetStyle();

        YAML::Node main;
        try {

            main = YAML::LoadFile(path.string());

        } catch (YAML::ParserException e) {

            LogError("Failed to read the editor theme file at {}. Error: {}.", path, e.what());
            return;

        }

        m_style->FrameRounding = main["Rounding"].as<float>();

        YAML::Node colors = main["Colors"];

        for (YAML::const_iterator it = colors.begin(); it != colors.end(); ++it) {

            uint32 col = it->first.as<uint32>();
            Color value = it->second.as<Color>();

            m_style->Colors[col] = *(ImVec4*) &value;

        }

        if (GetEngineState() == EngineState::PostInitialization)
            LogStatus("\tEditor theme '{}' loaded.", path.filename().string());
        else
            LogStatus("Editor theme '{}' loaded.", path.filename().string());

    }

}
