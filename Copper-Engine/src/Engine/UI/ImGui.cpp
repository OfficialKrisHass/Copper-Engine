#include "cupch.h"
#include "ImGui.h"

#include "Engine/Core/Engine.h"

#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_opengl3.h>
#include <ImGui/backends/imgui_impl_glfw.h>

#include <ImGuizmo/ImGuizmo.h>

#include <GLFW/glfw3.h>

namespace Copper {

    static std::string iniPath = "";

    uint32 uiCount = 0;

    void UIContext::Initialize(const Window& window, bool gizmo, bool docking, bool viewports) {

        CUP_FUNCTION();
        
        if(uiCount == 0) {
            
            VERIFY_STATE(EngineState::Initialization, "Initialize the main UI context");
            CU_ASSERT(IMGUI_CHECKVERSION(), "ImGui version check failed.");

        }
        uiCount++;

        m_gizmo = gizmo;
        m_docking = docking;
        m_viewports = viewports;

        m_context = ImGui::CreateContext();
        ImGui::SetCurrentContext(m_context);
        ImGuiIO& io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NoMouseCursorChange;
        if (docking) io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        if (viewports) io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        iniPath = (ExecutableFolder() / "imgui.ini").string();
        io.IniFilename = iniPath.c_str();
        ImGui::LoadIniSettingsFromDisk(io.IniFilename);

        CU_ASSERT(ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window.GetWindowPtr()), true), "Could not initialize ImGui GLFW backend.");
        CU_ASSERT(ImGui_ImplOpenGL3_Init("#version 460"), "Could not initialize ImGui OpenGL backend.");

    }
    void UIContext::Shutdown() {

        CUP_FUNCTION();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext(m_context);

        uiCount--;

    }

    void UIContext::Begin() {

        CUP_FUNCTION();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (m_gizmo) ImGuizmo::BeginFrame();

    }
    void UIContext::End() {

        CUP_FUNCTION();

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

    void UIContext::LoadFont(const fs::path& path, float fontSize) const {

        CUP_FUNCTION();

        ImGuiIO& io = ImGui::GetIO();
        io.FontDefault = io.Fonts->AddFontFromFileTTF(path.string().c_str(), fontSize);
        CU_ASSERT(io.FontDefault != nullptr, "Could not load font at {}", path);
        
    }

    void UIContext::SetAsCurrent() const {

        CUP_FUNCTION();

        ImGui::SetCurrentContext(m_context);

    }

}
