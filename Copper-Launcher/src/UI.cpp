#include "UI.h"

#include "Window.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <ImGui/examples/imgui_impl_opengl3.cpp>
#include <ImGui/examples/imgui_impl_glfw.cpp>

#include <ImGui/imgui.h>
#include <ImGui/examples/imgui_impl_opengl3.h>
#include <ImGui/examples/imgui_impl_glfw.h>

namespace Launcher::UI {

	void Initialize() {

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.IniFilename = "Launcher-imgui.ini";

		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Window::Get()), true);
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

	}
	void End() {

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float) 1280, (float) 720);

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

}