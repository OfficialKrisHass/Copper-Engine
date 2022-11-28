﻿#include "FileBrowser.h"

#include "Core/EditorApp.h"

#include "Panels/Properties.h"
#include "Panels/SceneHierarchy.h"

#include <ImGui/imgui.h>

using namespace Copper;

namespace Editor {

    std::filesystem::path editingPath = "";

    FileBrowser::FileBrowser(std::filesystem::path initialDir) : Panel("File Browser"), currentDir(initialDir) {
        
        directoryIcon = Texture("assets/Icons/FileBrowser/DirectoryIcon.png");
        fileIcon = Texture("assets/Icons/FileBrowser/FileIcon.png");
        
    }


    void FileBrowser::UI() {
        
        ImGui::GetFont()->FontSize -= 2.0f;
        
        if(currentDir != std::filesystem::path("assets/TestProject")) {

            ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0));
            
            if(ImGui::Button("<-", ImVec2(30, 30))) {

                currentDir = currentDir.parent_path();
                
            }

            ImGui::PopStyleVar();
            
        }

        ImGui::SameLine();
        ImGui::Text(std::filesystem::relative(currentDir, Editor::GetProjectPath()).string().c_str());
        ImGui::GetFont()->FontSize += 2.0f;

        const float padding = 16.0f;
        const float thumbnailSize = 128.0f;
        const float cellSize = thumbnailSize + padding;
        const float panelWidth = ImGui::GetContentRegionAvail().x;

        int columns = (int) (panelWidth / cellSize);
        if(columns < 1) columns = 1;

        ImGui::Columns(columns, 0, false);

        if(ImGui::BeginPopupContextWindow(0, 1, false)) {

            if(ImGui::MenuItem("Folder")) {

                std::filesystem::path path = currentDir;
                path += "/New Folder";

                std::filesystem::create_directories(path);

                editingPath = path;
                
            }

            ImGui::EndPopup();
            
        }

        for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(currentDir)) {

            std::filesystem::path path = entry.path();
            std::string filename = path.filename().string();

            ImGui::PushID(filename.c_str());

            Texture icon = entry.is_directory() ? directoryIcon : fileIcon;
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if(ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64_t) icon.GetID()), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 }) && !entry.is_directory()) {

                Properties::SetSelectedFile(path);
                
            }
            ImGui::PopStyleColor();

            if ((path.extension() == ".fbx" || path.extension() == ".gltf" || path.extension() == ".obj") && ImGui::BeginDragDropSource()) {

                char* itemPath = (char*) path.c_str();

                ImGui::SetDragDropPayload("MODEL", itemPath, (path.string().size() + 1) * sizeof(char), ImGuiCond_Once);
                ImGui::EndDragDropSource();

            }
            
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                
                if (entry.is_directory()) currentDir /= path.filename();
                if (path.extension() == ".copper") {

                    OpenScene(path);
                    
                }

            }

            if(ImGui::BeginPopupContextItem()) {
                
                if(ImGui::MenuItem("Remove")) { std::filesystem::remove_all(path); }
                if(ImGui::MenuItem("Edit")) { editingPath = path; }
                
                ImGui::EndPopup();
                
            }

            if (Input::IsKey(KeyCode::Escape)) { editingPath = ""; }

            if(editingPath == path) {

                char buffer[128] = {};
                std::strncpy(buffer, path.filename().string().c_str(), sizeof(buffer));

                if (ImGui::InputText("##Edit Name", buffer, sizeof(buffer)) && (Input::IsKey(KeyCode::Enter) || Input::IsButton(Input::Button1))) {

                    editingPath = editingPath.parent_path();
                    editingPath += "/";
                    editingPath += buffer;

                    rename(path, editingPath);

                    editingPath = "";

                }
                
            } else {
                
                ImGui::TextWrapped(filename.c_str());
                
            }
                
            ImGui::NextColumn();
            ImGui::PopID();
            
        }

        ImGui::Columns(1);
        
    }

    
}