#include "UI.h"

#include "Window.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui_impl_opengl3.cpp>
#include <imgui_impl_glfw.cpp>

#include <ImGui/imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace Launcher::UI {

	void Initialize() {

		IMGUI_CHECKVERSION();
		ImGui::SetCurrentContext(ImGui::CreateContext());

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NoMouseCursorChange;

		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*) Window::NativePointer(), true);
		ImGui_ImplOpenGL3_Init("#version 460");

	}

	void BeginFrame() {

		// Backend

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		// Dockspace

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
										ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("##Dockspace", nullptr, window_flags); //Dockspace

		ImGui::PopStyleVar(3);

	}
	void EndFrame() {

		// Dockspace

		ImGui::End();

		// Backend

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Viewports
		
		ImGuiIO& io = ImGui::GetIO();
		GLFWwindow* backup = glfwGetCurrentContext();

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup);

	}

	void Shutdown() {

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();

		ImGui::DestroyContext();

	}
}