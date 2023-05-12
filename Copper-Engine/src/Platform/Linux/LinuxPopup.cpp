#include "cupch.h"
#include "Engine/Input/Popup.h"

#include "Engine/Core/Engine.h"

#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_opengl3.h>
#include <ImGui/backends/imgui_impl_glfw.h>

namespace Copper::Input {

    bool popupOpen = false;
    PopupResult result = PopupResult::Invalid;

    bool ClosePopup(const Event& e);

    void SetupColors(ImVec4* colors);

    PopupResult WarningPopup(const std::string& title, const std::string& description) {
        
        ImGuiIO& io = ImGui::GetIO();
        ImFont* defaultFont = io.FontDefault;
        
        //Setting up and creating the Popup
        popupOpen = true;
        result = PopupResult::Invalid;

        Window popupWindow(title, 400, 150, false);
        popupWindow.AddWindowCloseEventFunc(ClosePopup);

        ImGuiContext* prevContext = ImGui::GetCurrentContext();
        ImGuiContext* context = ImGui::CreateContext();
        ImGui::SetCurrentContext(context);

        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*) popupWindow.GetWindowPtr(), true);
        ImGui_ImplOpenGL3_Init("#version 460");
        
        io.FontDefault = defaultFont;
        SetupColors(ImGui::GetStyle().Colors);

        //Popup Render Loop
        while(popupOpen) {

            popupWindow.Update();

            ImGui_ImplOpenGL3_NewFrame();
		    ImGui_ImplGlfw_NewFrame();
		    ImGui::NewFrame();

            //Prepare the flags, font size, etc
            ImFont* font = ImGui::GetFont();
            font->FontSize -= 2.0f;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
		    ImGui::SetNextWindowSize(viewport->Size);
		    ImGui::SetNextWindowViewport(viewport->ID);

            ImVec2 buttonSize = {(float) (popupWindow.Width() / 2.0f) - 35.0f, 20.0f};

            //Create the "Window"
		    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("##Popup", nullptr, window_flags);

            //Description
            ImGui::Text(description.c_str());

            //Buttons
            font->FontSize += 1.0f;
            ImGui::SetCursorPos({20.0f, (float) popupWindow.Height() - buttonSize.y - 20.0f});
            if(ImGui::Button("Ok", buttonSize)) { result = PopupResult::Ok; popupOpen = false; }

            ImGui::SameLine();
            ImGui::SetCursorPosX((float) popupWindow.Width() - buttonSize.x - 20.0f);
            if(ImGui::Button("Cancel", buttonSize)) { result = PopupResult::Cancel; popupOpen = false; }

            //Finish the Window
            ImGui::End();
            ImGui::PopStyleVar(3);

            font->FontSize += 1.0f;

            //Finally, Render the Popup Window
            ImGui::Render();
		    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        }

        popupWindow.Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
	    ImGui_ImplGlfw_Shutdown();
	    ImGui::DestroyContext(context);

        GetWindow().SetAsCurrentContext();
        ImGui::SetCurrentContext(prevContext);

        return result;

    }
    PopupResult ErrorPopup(const std::string& title, const std::string& description) {;

        ImGuiIO& io = ImGui::GetIO();
        ImFont* defaultFont = io.FontDefault;
        
        //Setting up and creating the Popup
        popupOpen = true;
        result = PopupResult::Invalid;

        Window popupWindow(title, 400, 150, false);
        popupWindow.AddWindowCloseEventFunc(ClosePopup);

        ImGuiContext* prevContext = ImGui::GetCurrentContext();
        ImGuiContext* context = ImGui::CreateContext();
        ImGui::SetCurrentContext(context);

        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*) popupWindow.GetWindowPtr(), true);
        ImGui_ImplOpenGL3_Init("#version 460");
        
        io.FontDefault = defaultFont;
        SetupColors(ImGui::GetStyle().Colors);

        //Popup Render Loop
        while(popupOpen) {

            popupWindow.Update();

            ImGui_ImplOpenGL3_NewFrame();
		    ImGui_ImplGlfw_NewFrame();
		    ImGui::NewFrame();

            //Prepare the flags, font size, etc
            ImFont* font = ImGui::GetFont();
            font->FontSize -= 2.0f;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
		    ImGui::SetNextWindowSize(viewport->Size);
		    ImGui::SetNextWindowViewport(viewport->ID);

            ImVec2 buttonSize = {(float) (popupWindow.Width() / 2.0f) - 35.0f, 20.0f};

            //Create the "Window"
		    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("##Popup", nullptr, window_flags);

            //Description
            ImGui::Text(description.c_str());

            //Buttons
            font->FontSize += 1.0f;
            ImGui::SetCursorPos({(float) (popupWindow.Width() - buttonSize.x) / 2.0f, (float) popupWindow.Height() - buttonSize.y - 20.0f});
            if(ImGui::Button("Ok", buttonSize)) { result = PopupResult::Ok; popupOpen = false; }

            //Finish the Window
            ImGui::End();
            ImGui::PopStyleVar(3);

            font->FontSize += 1.0f;

            //Finally, Render the Popup Window
            ImGui::Render();
		    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        }

        popupWindow.Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
	    ImGui_ImplGlfw_Shutdown();
	    ImGui::DestroyContext(context);

        GetWindow().SetAsCurrentContext();
        ImGui::SetCurrentContext(prevContext);

        return result;

    }

    bool ClosePopup(const Event& e) {
        
        popupOpen = false;
        result = PopupResult::Closed;

        return true;

    }

    void SetupColors(ImVec4* colors) {

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

    }

}