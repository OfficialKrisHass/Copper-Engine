#include "ThemeEditor.h"

#include "Dialogs.h"

#include <ImGui/imgui.h>

#include <yaml-cpp/yaml.h>

#include <fstream>

#define COLOR_EDIT(color, text) ImGui::ColorEdit3(text, (float*) &style->Colors[color])

#define WRITE_COLOR(color) out << YAML::Key << color << YAML::Value << style->Colors[color];
#define READ_COLOR(color) style->Colors[color] = colors[(int) color].as<ImVec4>()

namespace Launcher {

    ImGuiStyle* style = nullptr;

    ImVec4 disabled = ImVec4(0, 0, 0, 1);

    static void ColorEditor();

    YAML::Emitter& operator<<(YAML::Emitter& out, const ImVec4& col);

    void ThemeEditor() {

        ImGui::Begin("Theme Editor");

        if (style == nullptr)
            style = &ImGui::GetStyle();

        ImGui::DragFloat("Rounding", &style->FrameRounding);

        ImGui::Separator();

        ColorEditor();

        ImGui::End();

    }

    void ColorEditor() {

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
        ImGui::ColorEdit3("Frame background disabled", (float*) &disabled);

        ImGui::Separator();

        COLOR_EDIT(ImGuiCol_Button, "Button");
        COLOR_EDIT(ImGuiCol_ButtonHovered, "Button hovered");
        COLOR_EDIT(ImGuiCol_ButtonActive, "Button active");

        ImGui::Separator();

        COLOR_EDIT(ImGuiCol_CheckMark, "Check mark");

        ImGui::Separator();

        COLOR_EDIT(ImGuiCol_DragDropTarget, "Drag drop overlay");

    }

    void SaveTheme() {

        if (style == nullptr)
            style = &ImGui::GetStyle();

        YAML::Emitter out;
        out << YAML::BeginMap; // Main

        out << YAML::Key << "Rounding" << style->FrameRounding;

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

        out << YAML::Key << "Disabled" << YAML::Value << disabled;

        out << YAML::EndMap; // Colors

        out << YAML::EndMap; // Main

        std::ofstream file;
        file.open(DataDirectory() / "assets/Launcher.cutheme");
        file << out.c_str();
        file.close();

    }
    void LoadTheme() {

        if (style == nullptr)
            style = &ImGui::GetStyle();

        YAML::Node theme;
        try { theme = YAML::LoadFile((DataDirectory() / "assets/Launcher.cutheme").string()); }
        catch (YAML::Exception e) {

            Dialogs::Error("Theme load failed", "Could not load theme file Launcher.cutheme.\n\nError:\n" + e.msg);
            return;

        }

        style->FrameRounding = theme["Rounding"].as<float>();

        YAML::Node colors = theme["Colors"];

        READ_COLOR(ImGuiCol_WindowBg);
        READ_COLOR(ImGuiCol_ChildBg);

        READ_COLOR(ImGuiCol_Header);
        READ_COLOR(ImGuiCol_HeaderHovered);
        READ_COLOR(ImGuiCol_HeaderActive);

        READ_COLOR(ImGuiCol_TitleBg);
        READ_COLOR(ImGuiCol_TitleBgActive);
        READ_COLOR(ImGuiCol_TitleBgCollapsed);

        READ_COLOR(ImGuiCol_Tab);
        READ_COLOR(ImGuiCol_TabHovered);
        READ_COLOR(ImGuiCol_TabActive);
        READ_COLOR(ImGuiCol_TabUnfocused);
        READ_COLOR(ImGuiCol_TabUnfocusedActive);

        READ_COLOR(ImGuiCol_FrameBg);
        READ_COLOR(ImGuiCol_FrameBgHovered);
        READ_COLOR(ImGuiCol_FrameBgActive);

        READ_COLOR(ImGuiCol_Button);
        READ_COLOR(ImGuiCol_ButtonHovered);
        READ_COLOR(ImGuiCol_ButtonActive);

        READ_COLOR(ImGuiCol_CheckMark);

        READ_COLOR(ImGuiCol_DragDropTarget);

        disabled = colors["Disabled"].as<ImVec4>();

    }

    const ImVec4& ColorDisabled() { return disabled; }

    YAML::Emitter& operator<<(YAML::Emitter& out, const ImVec4& col) {

        out << YAML::Flow << YAML::BeginSeq;
        out << col.x << col.y << col.z << col.w;
        out << YAML::EndSeq;

        return out;

    }

}

namespace YAML {

    template<> struct convert<ImVec4> {

        static Node encode(const ImVec4& col) {

            Node node;
            node.push_back(col.x);
            node.push_back(col.y);
            node.push_back(col.z);
            node.push_back(col.w);
            node.SetStyle(EmitterStyle::Flow);

            return node;

        }
        static bool decode(const Node& node, ImVec4& col) {

            if (!node.IsSequence() || node.size() != 4)
                return false;

            col.x = node[0].as<float>();
            col.y = node[1].as<float>();
            col.z = node[2].as<float>();
            col.w = node[3].as<float>();

            return true;

        }

    };

}
