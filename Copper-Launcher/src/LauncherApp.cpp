#include "Window.h"
#include "UI.h"

#include "ProjectEntry.h"

#include "ThemeEditor.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

#include <iostream>
#include <vector>

namespace Launcher {

	struct Data {

		bool running = true;

		std::vector<ProjectEntry> projectEntries = {	ProjectEntry("", "dir-less"),
														ProjectEntry("Dev Project", "C:/Programming/Copper-Engine/DevProject"),
														ProjectEntry("Lalalalala", "C:/Lalalala"),
														ProjectEntry("Half Life 3", "C:"),
														ProjectEntry("Portal 3", "C:/Gaben/valvesoftware"),
														ProjectEntry("Hellraze", "C:/Programming/Hellraze"), };

		ImFont* titleFont = nullptr;

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

		Window::Create(960, 540, "Copper Launcher");
		UI::Initialize();

		ProjectEntry::InitializeFonts();
		data.titleFont = UI::AddFont(MainFontPath, 40.0f);

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
			
			ImGui::Separator();

			ProjectEntries();

			//ThemeEditor();
			UI::EndFrame();


		}

		SaveTheme();

		Shutdown();

	}
	void Shutdown() {

		UI::Shutdown();
		Window::Shutdown();

	}

	void TitleText() {

		ImGui::PushFont(data.titleFont);

		ImGui::SetCursorPosX(WindowPadding);
		ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMin().y + WindowPadding + ButtonSize.y / 2.0f - ImGui::GetTextLineHeight() / 2.0f);
		ImGui::Text("Copper Launcher");

		ImGui::PopFont();

	}
	void AddProjectButton(float cursorY) {

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::GetStyle().ItemInnerSpacing.x - WindowPadding - ButtonSize.x * 2.0f);
		ImGui::SetCursorPosY(cursorY);
		ImGui::Button("Add Project", ButtonSize);

	}
	void CreateProjectButton(float cursorY) {

		ImGui::SameLine();
		ImGui::SetCursorPosY(cursorY);
		ImGui::Button("Create Project", ButtonSize);

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