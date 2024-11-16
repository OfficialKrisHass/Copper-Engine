#include "Base.h"
#include "Window.h"
#include "UI.h"
#include "Fonts.h"

#include "Dialogs.h"
#include "PersistentData.h"

#include "ProjectEntry.h"

#include "ThemeEditor.h"

#include "CreateProjectWindow.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

#include <iostream>
#include <vector>

#include <filesystem>

namespace Launcher {
  
	struct Data {

		bool running = true;

		std::vector<ProjectEntry> projectEntries = {};

    bool createProjectWinOpen = false;

	};
	static Data data;

	constexpr ImVec2 ButtonSize = { 175, 50 };

	static void Run();
	static void Shutdown();

	static void TitleText();
	static void AddProjectButton(float cursorY);
	static void CreateProjectButton(float cursorY);

	static void ProjectEntries();

	int Entry() {

		std::cout << "Hello, World!\n";

		PersistentData::Load(data.projectEntries);

		Window::Create(960, 540, "Copper Launcher");
		UI::Initialize();
    Fonts::Initialize();

		Run();

		return 0;

	}
	void Run() {

		while (data.running) {

			Window::Update();
			UI::BeginFrame();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
			UI::Dockspace();
			ImGui::PopStyleVar();

			float cursorY = ImGui::GetCursorPosY() + WindowPadding;

			TitleText();
			AddProjectButton(cursorY);
			CreateProjectButton(cursorY);

      CreateProjectWindow::Render(&data.createProjectWinOpen);

			ImGui::Separator();

			ProjectEntries();

			UI::EndFrame();

		}

		SaveTheme();
    PersistentData::Save(data.projectEntries);

		Shutdown();

	}
	void Shutdown() {

		UI::Shutdown();
		Window::Shutdown();

	}

	void TitleText() {

		ImGui::PushFont(Fonts::TitleFont());

    // Bro I wrote this code like a year ago, and I dont know what the fuck is going on in here
    // Ah the wonders of working with ImGui

		ImGui::SetCursorPosX(WindowPadding);
		ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMin().y + WindowPadding + ButtonSize.y / 2.0f - ImGui::GetTextLineHeight() / 2.0f);
		ImGui::Text("Copper Launcher");

		ImGui::PopFont();

	}
	void AddProjectButton(float cursorY) {

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::GetStyle().ItemInnerSpacing.x - WindowPadding - ButtonSize.x * 2.0f);
		ImGui::SetCursorPosY(cursorY);
		if (!ImGui::Button("Add Project", ButtonSize)) return;

		std::string path = Dialogs::OpenFolder("Select Project Folder", BaseProjectDir);
		if (path == "") return;
		if (!std::filesystem::exists(path + "/Project.cu")) {

			Dialogs::Error("Invalid Project", "The folder you have selected does not contain a Project.cu file! Check if you have selected the correct folder");
			return;

		}

		data.projectEntries.push_back(ProjectEntry(std::filesystem::path(path).filename().string(), path));

	}
	void CreateProjectButton(float cursorY) {

		ImGui::SameLine();
		ImGui::SetCursorPosY(cursorY);
		if (ImGui::Button("Create Project", ButtonSize))
      data.createProjectWinOpen = true;

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + WindowPadding);

	}

	void ProjectEntries() {

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { WindowPadding, 0 });
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));

		ImGui::BeginChildFrame(ImGuiID(51515415555255), { ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - ImGui::GetCursorPosY() });

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + WindowPadding);

		for (const ProjectEntry& entry : data.projectEntries)
			entry.Render();

		ImGui::EndChildFrame();

	}

	void OnWindowClose() {

		data.running = false;

	}

}
