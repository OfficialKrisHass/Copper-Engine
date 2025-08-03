#include "CreateProjectWindow.h"

#include "ProjectTemplate.h"

#include "Fonts.h"

#include "Dialogs.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>

#include <string>

#include <iostream>

#define INPUT_TEXT_WIDTH 350.0f

#define CREATE_BUTTON_SIZE_X 150.0f
#define CREATE_BUTTON_SIZE_Y 50.0f
#define CREATE_BUTTON_Y_OFFSET 10.0f

namespace Launcher::CreateProjectWindow {

    std::string projectName;
    std::string projectPath;

    void Render(bool *open) {

        if (!*open) return;

        ImGuiStyle& style = ImGui::GetStyle();

        ImGui::Begin("Create Project", open, ImGuiWindowFlags_NoResize);

        // Name and path

        float xOff = ImGui::CalcTextSize("Name:").x + style.WindowPadding.x + style.ItemInnerSpacing.x;

        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::SetCursorPosX(xOff);
        ImGui::SetNextItemWidth(INPUT_TEXT_WIDTH);
        ImGui::InputText("##ProjectName", &projectName);

        ImGui::SetCursorPosX(xOff - ImGui::CalcTextSize("Path:").x - style.ItemInnerSpacing.x - 2);
        ImGui::Text("Path:");
        ImGui::SameLine();
        ImGui::SetCursorPosX(xOff);
        ImGui::SetNextItemWidth(INPUT_TEXT_WIDTH);
        ImGui::InputText("##ProjectPath", &projectPath);

        ImGui::SameLine();
        ImGui::PushFont(Fonts::TextFont());
        if (ImGui::Button("Browse")) {

            projectPath = Dialogs::OpenFolder("Select path", BaseProjectDir);

        }
        ImGui::PopFont();

        float winWidth = ImGui::GetWindowWidth() - style.WindowPadding.x * 2.0f;

        static const std::vector<ProjectTemplate>& templates = ProjectTemplate::GetTemplates();
        static int selectedTemplate = 0;

        ImGui::Text("Template:");
        if (ImGui::BeginListBox("##Template", ImVec2(winWidth / 2.0f, 0.0f))) {

            for (int i = 0; i < templates.size(); i++) {

                bool selected = (selectedTemplate == i);
                if (ImGui::Selectable(templates[i].GetName().c_str(), selected))
                    selectedTemplate = i;

                if (selected)
                    ImGui::SetItemDefaultFocus();

            }

            ImGui::EndListBox();

        }
        ImGui::SameLine();

        // ImGui is retarded and I hate it why the fuck does it exist ????????
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 240.0f);
        ImGui::Text(templates[selectedTemplate].GetDescription().c_str());
        ImGui::PopTextWrapPos();

        // Create Button

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().ItemInnerSpacing.y * CREATE_BUTTON_Y_OFFSET);
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f - CREATE_BUTTON_SIZE_X / 2.0f);
        ImGui::PushFont(Fonts::BigTextFont());

        ImGui::BeginDisabled(projectPath.empty() || projectName.empty());
        if (ImGui::Button("Create", { CREATE_BUTTON_SIZE_X, CREATE_BUTTON_SIZE_Y }) &&
            templates[selectedTemplate].CreateProject(projectName, projectPath))
            LaunchEditor(projectPath + "/" + projectName);
        ImGui::EndDisabled();

        ImGui::PopFont();

        ImGui::End();

    }

}
