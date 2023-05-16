#include "FileBrowser.h"

#include "Core/EditorApp.h"

#include "Panels/Properties.h"
#include "Panels/SceneHierarchy.h"

#include <ImGui/imgui.h>

using namespace Copper;

namespace Editor {

    Filesystem::Path editingPath = "";
    Filesystem::Path FileBrowser::projectRelativeDir = "";

    Texture directoryIcon;
    Texture fileIcon;

    FileBrowser::FileBrowser(const Filesystem::Path& initialDir) : Panel("File Browser") {
        
        projectRelativeDir = initialDir;

        directoryIcon = Texture("assets/Icons/DirectoryIcon.png");
        fileIcon = Texture("assets/Icons/FileIcon.png");

    }

    void FileBrowser::UI() {
        
        ImGui::GetFont()->FontSize -= 2.0f;
        
        if(projectRelativeDir != "") {

            ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0));
            
            if(ImGui::Button("<-", ImVec2(30, 30))) {

                projectRelativeDir = projectRelativeDir.ParentPath();
                
            }

            ImGui::PopStyleVar();
            
        }

        ImGui::SameLine();
        ImGui::Text(projectRelativeDir.String().c_str());
        ImGui::GetFont()->FontSize += 2.0f;

        const float padding = 16.0f;
        const float thumbnailSize = 128.0f;
        const float cellSize = thumbnailSize + padding;
        const float panelWidth = ImGui::GetContentRegionAvail().x;

        int columns = (int) (panelWidth / cellSize);
        if(columns < 1) columns = 1;

        ImGui::Columns(columns, 0, false);

        if(ImGui::BeginPopupContextWindow("##File Browser")) {

            if(ImGui::MenuItem("Folder")) {

                Filesystem::Path path = GetProject().assetsPath / projectRelativeDir;
                path /= "New Folder";

                std::experimental::filesystem::create_directories(path.String());

                editingPath = path;
                
            }

            ImGui::EndPopup();
            
        }

        for(const Filesystem::DirectoryEntry& entry : Filesystem::DirectoryIterator((GetProject().assetsPath / projectRelativeDir).String())) {

            Filesystem::Path fullPath = entry.path().string();
            Filesystem::Path path = fullPath.RelativeTo(GetProject().assetsPath);

            std::string filename = path.File().String();
            if(!fullPath.Directory()) {  
                
                size_t index = filename.find_last_of('.');
                if (index != std::string::npos) filename.erase(index);

            }

            if (path.Extension() == "cum") continue;

            ImGui::PushID(filename.c_str());

            uint64_t icon = fullPath.Directory() ? (uint64_t) directoryIcon.GetID() : (uint64_t) fileIcon.GetID();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if(ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 }) && !fullPath.Directory()) {

                //Properties::SetSelectedFile(path);
                
            }
            ImGui::PopStyleColor();

            if ((path.Extension() == "fbx" || path.Extension() == "gltf" || path.Extension() == "obj") && ImGui::BeginDragDropSource()) {

                char* itemPath = (char*) path.String().c_str();

                ImGui::SetDragDropPayload("MODEL", itemPath, (path.String().size() + 1) * sizeof(char), ImGuiCond_Once);
                ImGui::EndDragDropSource();

            }
            
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                
                if (fullPath.Directory()) projectRelativeDir /= path.File();
                if (path.Extension() == "copper") {

                    OpenScene(GetProject().assetsPath / path);
                    
                }

            }

            if(ImGui::BeginPopupContextItem()) {
                
                if(ImGui::MenuItem("Remove")) { path.Delete(); }
                if(ImGui::MenuItem("Edit")) { editingPath = fullPath; }
                
                ImGui::EndPopup();
                
            }

            if (Input::IsKey(KeyCode::Escape)) { editingPath = ""; }

            if(editingPath == fullPath) {

                char buffer[128] = {};
                std::strncpy(buffer, filename.c_str(), filename.length() * sizeof(char));

                if (ImGui::InputText("##Edit Name", buffer, sizeof(buffer)) && (Input::IsKey(KeyCode::Enter) || Input::IsButton(MouseCode::Button1))) {

                    editingPath = editingPath.ParentPath();
                    editingPath /= buffer;

                    rename(fullPath.String().c_str(), editingPath.String().c_str());

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