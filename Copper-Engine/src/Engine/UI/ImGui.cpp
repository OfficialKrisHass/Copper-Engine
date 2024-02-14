#include "cupch.h"
#include "ImGui.h"

#include "Engine/Core/Engine.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui_impl_opengl3.cpp>
#include <imgui_impl_glfw.cpp>

#include <ImGui/imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <ImGuizmo/ImGuizmo.h>

// TODO: Remove profiler include
#include "Engine/Debug/Profiler.h"

namespace Copper {

	std::string mainFontPath = "";
	float mainFontSize = 0.0f;

	uint32 uiCount = 0;

	void UIContext::Initialize(const Window& window, bool gizmo, bool docking, bool viewports) {

		this->gizmo = gizmo;
		this->docking = docking;
		this->viewports = viewports;
		
		if(!uiCount) {
			
			VERIFY_STATE(EngineCore::EngineState::Initialization, "Initialize the main UI");
			IMGUI_CHECKVERSION();

		}
		uiCount++;

		context = ImGui::CreateContext();
		ImGui::SetCurrentContext(context);
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NoMouseCursorChange;
		if (docking) io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		if (viewports) io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		if (!mainFontPath.empty())
			io.FontDefault = io.Fonts->AddFontFromFileTTF(mainFontPath.c_str(), mainFontSize);

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

		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window.GetWindowPtr()), true);
		ImGui_ImplOpenGL3_Init("#version 460");

	}
	void UIContext::Shutdown() {

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext(context);
		uiCount--;

	}

	void UIContext::Begin() {

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		if (gizmo) ImGuizmo::BeginFrame();

	}
	void UIContext::End() {

		//io.DisplaySize = ImVec2((float) GetWindow().Width(), (float) GetWindow().Height());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

			GLFWwindow* backup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup);

		}

	}

	void UIContext::LoadFont(const std::string& path, float fontSize) {

		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = io.Fonts->AddFontFromFileTTF(path.c_str(), fontSize);
		
		if(mainFontPath.empty()) {

			mainFontPath = path;
			mainFontSize = fontSize;

		}

	}

	void UIContext::SetAsCurrent() { ImGui::SetCurrentContext(context); }

}