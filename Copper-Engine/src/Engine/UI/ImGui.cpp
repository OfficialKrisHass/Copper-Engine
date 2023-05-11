#include "cupch.h"
#include "ImGui.h"

#include "Engine/Core/Engine.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <ImGui/backends/imgui_impl_opengl3.cpp>
#include <ImGui/backends/imgui_impl_glfw.cpp>

#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_opengl3.h>
#include <ImGui/backends/imgui_impl_glfw.h>

#include <ImGuizmo/ImGuizmo.h>

namespace Copper::UI {

	/*float windowBG[3];
	float Button[3];
	float ButtonH[3];
	float ButtonA[3];
	float Frame[3];
	float FrameH[3];
	float FrameA[3];
	float Tab[3];
	float TabH[3];
	float TabA[3];
	float TabU[3];
	float TabUA[3];
	float Title[3];
	float TitleA[3];
	float TitleC[3];*/

	void Initialize() {

		CHECK((GetEngineState() == EngineState::Initialization), "Cannot Initialize UI, current Engine State is: {}", EngineStateToString(GetEngineState()))

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

		colors[ImGuiCol_Header] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);

		colors[ImGuiCol_Button] = ImVec4(0.07f, 0.07f, 0.07f, 1.0f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.09f, 0.09f, 0.09f, 1.0f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);

		colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);

		colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.0f);
		colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

		colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(GetWindow().GetWindowPtr()), true);
		ImGui_ImplOpenGL3_Init("#version 460");

	}
	void Shutdown() {

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

	}

	void Begin() {

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

	}
	void End() {

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float) GetWindow().Width(), (float) GetWindow().Height());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

			GLFWwindow* backup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup);

		}

	}

	void LoadFont(const std::string& path, float fontSize) {

		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/Fonts/open-sans.regular.ttf", fontSize);

	}

	/*void ThemeEditor() {
		ImGui::Begin("Theme Editor");
		ImGui::DragFloat3("Window BG", windowBG, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat3("Button", Button, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat3("Button Hovered", ButtonH, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat3("Button Active", ButtonA, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat3("Frame BG", Frame, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat3("Frame BG Hovered", FrameH, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat3("Frame BG Active", FrameA, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat3("Tab", Tab, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat3("Tab Hovered", TabH, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat3("Tab Active", TabA, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat3("Tab Unfocused", TabU, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat3("Tab Unfocused Active", TabUA, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat3("Title BG", Title, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat3("Title BG Active", TitleA, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat3("Title BG Collapsed", TitleC, 0.001f, 0.0f, 1.0f);
		ImGui::End();
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4(windowBG[0], windowBG[1], windowBG[2], 1.0f);
		colors[ImGuiCol_Button] =        ImVec4{ Button[0], Button[1], Button[2], 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ ButtonH[0], ButtonH[1], ButtonH[2], 1.0f };
		colors[ImGuiCol_ButtonActive] =  ImVec4{ ButtonA[0], ButtonA[1], ButtonA[2], 1.0f };
		colors[ImGuiCol_FrameBg] =        ImVec4{ Frame[0], Frame[1], Frame[2], 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ FrameH[0], FrameH[1], FrameH[2], 1.0f };
		colors[ImGuiCol_FrameBgActive] =  ImVec4{ FrameA[0], FrameA[1], FrameA[2], 1.0f };
		colors[ImGuiCol_Tab] =                ImVec4{ Tab[0], Tab[1], Tab[2], 1.0f };
		colors[ImGuiCol_TabHovered] =         ImVec4{ TabH[0], TabH[1], TabH[2], 1.0f };
		colors[ImGuiCol_TabActive] =          ImVec4{ TabA[0], TabA[1], TabA[2], 1.0f };
		colors[ImGuiCol_TabUnfocused] =       ImVec4{ TabU[0], TabU[1], TabU[2], 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ TabUA[0], TabUA[1], TabUA[2], 1.0f };
		colors[ImGuiCol_TitleBg] =          ImVec4{ Title[0], Title[1], Title[2], 1.0f };
		colors[ImGuiCol_TitleBgActive] =    ImVec4{ TitleA[0], TitleA[1], TitleA[2], 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ TitleC[0], TitleC[1], TitleC[2], 1.0f };
	}*/

}