#include "ProjectSettings.h"

#include "UI/TypeWidgets.h"

#include "Engine/Renderer/Renderer.h"

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

#define Page_General FLAG(0)
#define Page_Rendering FLAG(1)
#define Page_Physics FLAG(2)

#define Page(name) Page_##name
#define PageOption(name) if (ImGui::Button(#name, buttonSize)) currentPage = Page(name)

namespace Editor::ProjectSettings {

	using namespace Copper;

	static bool isOpen = false;

	typedef uint16 Page;
	Page currentPage = 0;

	static void ShowGeneralPage();
	static void ShowPhysicsPage();
	static void ShowRenderingPage();

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

		PageOption(General);
		PageOption(Rendering);
		PageOption(Physics);

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();

		// Page

		ImGui::TableSetColumnIndex(1);

		if (currentPage & Page(General))
			ShowGeneralPage();
		else if (currentPage & Page(Rendering))
			ShowRenderingPage();
		else if (currentPage & Page(Physics))
			ShowPhysicsPage();

		// Finish

		ImGui::EndTable();

		ImGui::PopStyleVar();
		ImGui::End();

	}

	void Open() { isOpen = true; }

	void ShowGeneralPage() {

		ImGui::Text("General Page");

	}
	void ShowRenderingPage() {

		if (ImGui::CollapsingHeader("Ambient light")) {

			UI::EditColor("Color", &Renderer::AmbientColor());
			UI::EditVector3("Direction", &Renderer::AmbientDirection());

		}

		if (ImGui::CollapsingHeader("Skybox")) {

			UI::EditColor("Skybox color", &Renderer::SkyboxColor());

		}

	}
	void ShowPhysicsPage() {

		ImGui::Text("Physics Page");

	}

}