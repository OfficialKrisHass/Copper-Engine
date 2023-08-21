#include "cupch.h"
#include "Engine/Input/Popup.h"

#include "Engine/Core/Engine.h"

#include "Engine/UI/ImGui.h"

#include <ImGui/imgui.h>

namespace Copper::Input {

    bool popupOpen = false;
    PopupResult result = PopupResult::Invalid;

    bool ClosePopup(const Event& e);

    PopupResult WarningPopup(const std::string& title, const std::string& description) {
        
        if (popupOpen) {

            LogError("Only one Popup can be open at any given Time!");
            return PopupResult::Invalid;

        }

        //Setting Up and creating the Popup
        popupOpen = true;
        result = PopupResult::Invalid;

        Window popupWindow(title, 400, 150, false);
        popupWindow.AddWindowCloseEventFunc(ClosePopup);

        UIContext popupUI;
        popupUI.Initialize(popupWindow, false, false, false);

        //Popup Render Loop
        while(popupOpen) {

            popupWindow.Update();
            popupUI.Begin();

            //Prepare the flags, font size, etc
            ImFont* font = ImGui::GetFont();
            font->FontSize -= 2.0f;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
		    ImGui::SetNextWindowSize(viewport->Size);
		    ImGui::SetNextWindowViewport(viewport->ID);

            ImVec2 buttonSize = {(float) (popupWindow.Width() / 2.0f) - 35.0f, 30.0f};

            //Create the "Window"
		    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.0f));
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
            ImGui::PopStyleVar(4);

            font->FontSize += 1.0f;

            //Finally, Render the Popup Window
            popupUI.End();

        }

        popupWindow.Shutdown();
        popupUI.Shutdown();

        GetWindow().SetAsCurrentContext();
        SetMainUIAsCurrent();

        return result;

    }
    PopupResult ErrorPopup(const std::string& title, const std::string& description) {;

        if (popupOpen) {

            LogError("Only one Popup can be open at any given Time!");
            return PopupResult::Invalid;

        }

        //Setting Up and creating the Popup
        popupOpen = true;
        result = PopupResult::Invalid;

        Window popupWindow(title, 400, 150, false);
        popupWindow.AddWindowCloseEventFunc(ClosePopup);

        UIContext popupUI;
        popupUI.Initialize(popupWindow, false);

        //Popup Render Loop
        while(popupOpen) {

            popupWindow.Update();
            popupUI.Begin();

            //Prepare the flags, font size, etc
            ImFont* font = ImGui::GetFont();
            font->FontSize -= 2.0f;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
		    ImGui::SetNextWindowSize(viewport->Size);
		    ImGui::SetNextWindowViewport(viewport->ID);

            ImVec2 buttonSize = {(float) (popupWindow.Width() / 2.0f) - 35.0f, 30.0f};

            //Create the "Window"
		    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.0f));
            ImGui::Begin("##Popup", nullptr, window_flags);

            //Description
            ImGui::Text(description.c_str());

            //Buttons
            font->FontSize += 1.0f;
            ImGui::SetCursorPos({(float) (popupWindow.Width() - buttonSize.x) / 2.0f, (float) popupWindow.Height() - buttonSize.y - 20.0f});
            if(ImGui::Button("Ok", buttonSize)) { result = PopupResult::Ok; popupOpen = false; }

            //Finish the Window
            ImGui::End();
            ImGui::PopStyleVar(4);

            font->FontSize += 1.0f;

            //Finally, Render the Popup Window
            popupUI.End();

        }

        popupWindow.Shutdown();
        popupUI.Shutdown();

        GetWindow().SetAsCurrentContext();
        SetMainUIAsCurrent();

        return result;

    }

    bool ClosePopup(const Event& e) {
        
        popupOpen = false;
        result = PopupResult::Closed;

        return true;

    }

}