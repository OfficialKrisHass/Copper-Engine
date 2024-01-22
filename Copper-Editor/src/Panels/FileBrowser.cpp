﻿#include "FileBrowser.h"

#include "Core/EditorApp.h"

#include "Panels/Properties.h"
#include "Panels/SceneHierarchy.h"

#include <ImGui/imgui.h>

using namespace Copper;

#ifdef CU_LINUX
using std::experimental::filesystem::create_directories;
#else
using std::filesystem::create_directories;
#endif

namespace Editor {

    fs::path editingPath = "";
    fs::path FileBrowser::m_projectRelativeDir = "";

    Texture directoryIcon;
    Texture fileIcon;

    FileBrowser::FileBrowser(const fs::path& initialDir) : Panel("File Browser") {
        
        m_projectRelativeDir = initialDir;

        directoryIcon = Texture("assets/Icons/DirectoryIcon.png");
        fileIcon = Texture("assets/Icons/FileIcon.png");

    }

    void FileBrowser::UI() {
        
        ImGui::GetFont()->FontSize -= 2.0f;
        
        if(m_projectRelativeDir != "") {

            ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0));
            
            if(ImGui::Button("<-", ImVec2(30, 30))) {

                m_projectRelativeDir = m_projectRelativeDir.parent_path();
                
            }

            ImGui::PopStyleVar();
            
        }

        ImGui::SameLine();
        ImGui::Text(m_projectRelativeDir.string().c_str());
        ImGui::GetFont()->FontSize += 2.0f;

        const float padding = 16.0f;
        const float thumbnailSize = 128.0f;
        const float cellSize = thumbnailSize + padding;
        const float panelWidth = ImGui::GetContentRegionAvail().x;

        uint32 columns = (uint32) (panelWidth / cellSize);
        if(columns < 1) columns = 1;

        ImGui::Columns(columns, 0, false);

        if(ImGui::BeginPopupContextWindow("##File Browser")) {

            if(ImGui::MenuItem("Folder", 0, false, GetProject().name != "")) {

                fs::path path = GetProject().assetsPath / m_projectRelativeDir;
                path /= "New Folder";

                create_directories(path.string());

                editingPath = path;
                
            }

            ImGui::EndPopup();
            
        }

        if (!GetProject()) return;
        for(const fs::directory_entry& entry : fs::directory_iterator((GetProject().assetsPath / m_projectRelativeDir).string())) {

            fs::path fullPath = entry.path().string();
            fs::path path = fs::relative(fullPath, GetProject().assetsPath);
            std::string filename = path.filename().string();
            if(!entry.is_directory()) {  
                
                size_t index = filename.find_last_of('.');
                if (index != std::string::npos) filename.erase(index);

            }

            if (path.extension() == ".cum") continue;

            ImGui::PushID(filename.c_str());

            uint64 icon = entry.is_directory() ? (uint64) directoryIcon.GetID() : (uint64) fileIcon.GetID();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if(ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 }) && !entry.is_directory()) {

                //Properties::SetSelectedFile(path);
                
            }
            ImGui::PopStyleColor();

            if ((path.extension() == "fbx" || path.extension() == "gltf" || path.extension() == "obj") && ImGui::BeginDragDropSource()) {

                char* itemPath = (char*) path.string().c_str();

                ImGui::SetDragDropPayload("MODEL", itemPath, (path.string().size() + 1) * sizeof(char), ImGuiCond_Once);
                ImGui::EndDragDropSource();

            }
            
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                
                if (entry.is_directory()) m_projectRelativeDir /= path.filename();
                if (path.extension() == ".copper") {

                    OpenScene(GetProject().assetsPath / path);
                    
                }

            }

            if(ImGui::BeginPopupContextItem()) {
                
                if (ImGui::MenuItem("Remove")) { fs::remove_all(path); }
                if(ImGui::MenuItem("Edit")) { editingPath = fullPath; }
                
                ImGui::EndPopup();
                
            }

            if (Input::IsKey(KeyCode::Escape)) { editingPath = ""; }

            if(editingPath == fullPath) {

                char buffer[128] = {};
                std::strncpy(buffer, filename.c_str(), filename.length() * sizeof(char));

                if (ImGui::InputText("##Edit Name", buffer, sizeof(buffer)) && (Input::IsKey(KeyCode::Enter) || Input::IsButton(MouseCode::Button1))) {

                    editingPath = editingPath.parent_path();
                    editingPath /= buffer;

                    rename(fullPath.string().c_str(), editingPath.string().c_str());

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