#include "Window.h"
#include "UI.h"

#include "ThemeEditor.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

#include <iostream>

namespace Launcher {

	struct Data {

		bool running = true;

	};
	static Data data;

	constexpr float WindowPadding = 10.0f;
	constexpr float ProjectTabHeight = 85.0f;

	static void Run();
	static void Shutdown();

	static void ProjectTab(const char* name, const char* dir);

	ImFont* projectTabTitleFont = nullptr;
	ImFont* projectTabDetailsFont = nullptr;

	int Entry() {

		std::cout << "Hello, World!\n";

		Window::Create(960, 540, "Copper Launcher");
		UI::Initialize();

		Run();

		return 0;

	}
	void Run() {

		constexpr ImVec2 ButtonSize = { 175, 50 };
		ImGuiIO& io = ImGui::GetIO();

		ImFont* fontB = io.Fonts->AddFontFromFileTTF("assets/open-sans.regular.ttf", 40.0f);

		projectTabTitleFont = io.Fonts->AddFontFromFileTTF("assets/open-sans.regular.ttf", 40.0f);
		projectTabDetailsFont = io.Fonts->AddFontFromFileTTF("assets/open-sans.regular.ttf", 18.0f);

		while (data.running) {

			Window::Update();

			UI::BeginFrame();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
			UI::Dockspace();
			ImGui::PopStyleVar();

			float cursorY = ImGui::GetCursorPosY() + WindowPadding;
			
			ImGui::PushFont(fontB);

			ImGui::SetCursorPosX(WindowPadding);
			ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMin().y + WindowPadding + ButtonSize.y / 2.0f - ImGui::GetTextLineHeight() / 2.0f);
			ImGui::Text("Projects");

			ImGui::PopFont();
			
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - ImGui::GetStyle().ItemInnerSpacing.x - WindowPadding - ButtonSize.x * 2.0f);
			ImGui::SetCursorPosY(cursorY);
			ImGui::Button("Add Project", ButtonSize);

			ImGui::SameLine();
			ImGui::SetCursorPosY(cursorY);
			ImGui::Button("Create Project", ButtonSize);

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + WindowPadding);
			ImGui::Separator();

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { WindowPadding, 0 });
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));
			ImGui::BeginChildFrame(ImGuiID(51515415555255), { ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - ImGui::GetCursorPosY() });
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor();

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + WindowPadding);

			ProjectTab(" ", "dir-less");
			ProjectTab("Dev Project", "C:/Programming/Copper-Engine/DevProject");
			ProjectTab("Lalalalala", "C:/Lalalala");
			ProjectTab("Half Life 3", "C:");
			ProjectTab("Portal 3", "C:/Gaben/valvesoftware");
			ProjectTab("Hellraze", "C:/Programming/Hellraze");

			ImGui::EndChildFrame();

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

	void ProjectTab(const char* name, const char* dir) {

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { WindowPadding, 0 });
		ImGui::BeginChildFrame(ImGui::GetID(name), { ImGui::GetContentRegionAvail().x, ProjectTabHeight }, ImGuiWindowFlags_NoScrollbar);
		ImGui::PopStyleVar();

		ImGui::PushFont(projectTabTitleFont);
		ImGui::Text(name);
		ImGui::PopFont();

		ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - ImGui::GetTextLineHeight());

		ImGui::PushFont(projectTabDetailsFont);
		ImGui::Text(dir);
		ImGui::PopFont();

		ImGui::EndChildFrame();

	}

	void OnWindowClose() {

		data.running = false;

	}

}