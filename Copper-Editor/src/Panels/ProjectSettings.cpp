#include "ProjectSettings.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

#define Page_General FLAG(0)
#define Page_Physics FLAG(1)

namespace Editor::ProjectSettings {

	using namespace Copper;

	static bool isOpen = false;

	typedef uint16 Page;
	Page currentPage = 0;

	static void GeneralPage();
	static void PhysicsPage();

	void UIRender() {

		if (!isOpen) return;

		// Setup window and tables

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Project Settings", &isOpen, ImGuiWindowFlags_NoDocking);
		
		ImGui::BeginTable("ProjectSettingsTable", 2, ImGuiTableFlags_Resizable);

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

		// Setup Buttons

		const ImVec2 buttonSize = { ImGui::GetContentRegionAvail().x, ImGui::GetFontSize() + 5.0f };

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0.0f, 0.5f });

		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));

		// Page select

		if (ImGui::Button("General", buttonSize))
			currentPage = Page_General;

		if (ImGui::Button("Physics", buttonSize))
			currentPage = Page_Physics;

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();

		// Page

		ImGui::TableSetColumnIndex(1);

		if (currentPage & Page_General)
			GeneralPage();
		else if (currentPage & Page_Physics)
			PhysicsPage();

		// Finish

		ImGui::EndTable();

		ImGui::PopStyleVar();
		ImGui::End();

	}

	void Open() { isOpen = true; }

	void GeneralPage() {

		ImGui::Text("General Page");

	}
	void PhysicsPage() {

		ImGui::Text("Physics Page");

	}

}