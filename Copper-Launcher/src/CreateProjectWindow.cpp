#include "CreateProjectWindow.h"

#include "Fonts.h"

#include "Dialogs.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

#include <string>

#define INPUT_TEXT_WIDTH 350.0f

#define CREATE_BUTTON_SIZE_X 150.0f
#define CREATE_BUTTON_SIZE_Y 50.0f
#define CREATE_BUTTON_Y_OFFSET 10.0f

namespace Launcher::CreateProjectWindow {

  char projectName[64];
  char projectPath[256];

  void Render(bool *open) {

    if (!*open) return;

    ImGui::Begin("Create Project", open, ImGuiWindowFlags_NoResize);

    // Name and path

    float xOff = ImGui::CalcTextSize("Name:").x + WindowPadding + ImGui::GetStyle().ItemInnerSpacing.x;

    ImGui::Text("Name:");
    ImGui::SameLine();
    ImGui::SetCursorPosX(xOff);
    ImGui::SetNextItemWidth(INPUT_TEXT_WIDTH);
    ImGui::InputText("##ProjectName", projectName, sizeof(projectName));

    ImGui::SetCursorPosX(xOff - ImGui::CalcTextSize("Path:").x - ImGui::GetStyle().ItemInnerSpacing.x - 2);
    ImGui::Text("Path:");
    ImGui::SameLine();
    ImGui::SetCursorPosX(xOff);
    ImGui::SetNextItemWidth(INPUT_TEXT_WIDTH);
    ImGui::InputText("##ProjectPath", projectPath, sizeof(projectPath));

    ImGui::SameLine();
    ImGui::PushFont(Fonts::TextFont());
    if (ImGui::Button("Browse")) {

      std::string path = Dialogs::OpenFolder("Select path", BaseProjectDir);
      if (path != "")
        path.copy(projectPath, sizeof(projectPath));

    }
    ImGui::PopFont();

    // Create Button
    
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().ItemInnerSpacing.y * CREATE_BUTTON_Y_OFFSET);
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f - CREATE_BUTTON_SIZE_X / 2.0f);
    ImGui::PushFont(Fonts::BigTextFont());
    if (ImGui::Button("Create", { CREATE_BUTTON_SIZE_X, CREATE_BUTTON_SIZE_Y })) {

      //

    }
    ImGui::PopFont();

    ImGui::End();

  }

}
