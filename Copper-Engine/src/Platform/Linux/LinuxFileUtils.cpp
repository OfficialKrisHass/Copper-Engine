#include "cupch.h"
#include "Engine/Utilities/FileUtils.h"

#include "Engine/Renderer/Texture.h"

#include "Engine/Core/Engine.h"

#include "Engine/Filesystem/Path.h"
#include "Engine/Filesystem/Iterator.h"

#include "Engine/UI/ImGui.h"

#include <ImGui/imgui.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>

namespace Copper::Utilities {

    bool dialogOpen = false;
    std::string filterName = "";
    std::string filterExtension = "";
    Filesystem::Path currentPath = "";
    Filesystem::Path selectedFile = "";
    Filesystem::Path editingPath = "";
    Filesystem::Path result = "";

    UVector2I dialogSize;

    Texture directoryIcon;
    Texture fileIcon;

    bool CloseDialog(const Event& e);

    void RenderDockspace();
    void RenderFileView(const Window& dialogWindow, bool openMode = false);
    void RenderDirectoryTree();

    void DirectoryTreeNode(const Filesystem::Path& path);

    Filesystem::Path SaveDialog(const std::string& filter, const Filesystem::Path& initialDir) {

        if(dialogOpen) {

            LogError("Only one Dialog can be open at any given time!");
            return "";

        }
        dialogOpen = true;
        currentPath = initialDir;

        size_t extensionBegin = filter.find_first_of('\n');
        filterName = filter.substr(0, extensionBegin);
        filterExtension = filter.substr(extensionBegin + 3);

        //Setup the Dialog
        Window dialogWindow("Save Dialog", 1280, 720, false);
        UI dialogUI;

        dialogUI.Initialize(dialogWindow, false, true, false);
        dialogWindow.AddWindowCloseEventFunc(CloseDialog);

        directoryIcon = Texture("assets/Icons/DirectoryIcon.png");
        fileIcon = Texture("assets/Icons/FileIcon.png");

        while(dialogOpen) {
            
            dialogWindow.Update();
            dialogUI.Begin();

            dialogSize = dialogWindow.Size();

            RenderDockspace(); // ##Dockspace

            RenderFileView(dialogWindow);
            RenderDirectoryTree();

            ImGui::End(); // ##Dockspace

            dialogUI.End();

        }

        dialogWindow.Shutdown();
        dialogUI.Shutdown();

        GetWindow().SetAsCurrentContext();
        SetMainUIAsCurrent();

        return result;

    }
	Filesystem::Path OpenDialog(const std::string& filter, const Filesystem::Path& initialDir) {

        if(dialogOpen) {

            LogError("Only one Dialog can be open at any given time!");
            return "";

        }
        dialogOpen = true;
        currentPath = initialDir;

        size_t extensionBegin = filter.find_first_of('\n');
        filterName = filter.substr(0, extensionBegin);
        filterExtension = filter.substr(extensionBegin + 3);

        //Setup the Dialog
        Window dialogWindow("Save Dialog", 1280, 720, false);
        UI dialogUI;

        dialogUI.Initialize(dialogWindow, false, true, false);
        dialogWindow.AddWindowCloseEventFunc(CloseDialog);

        directoryIcon = Texture("assets/Icons/DirectoryIcon.png");
        fileIcon = Texture("assets/Icons/FileIcon.png");

        while(dialogOpen) {
            
            dialogWindow.Update();
            dialogUI.Begin();

            dialogSize = dialogWindow.Size();

            RenderDockspace(); // ##Dockspace

            RenderFileView(dialogWindow, true);
            RenderDirectoryTree();

            ImGui::End(); // ##Dockspace

            dialogUI.End();

        }

        dialogWindow.Shutdown();
        dialogUI.Shutdown();

        GetWindow().SetAsCurrentContext();
        SetMainUIAsCurrent();

        return result;

    }

	Filesystem::Path FolderOpenDialog() {

        LogError("Sorry I'm a big sack of lazy shit and the Save Dialog is not implemented yet :P");
        return "";

    }

    void RenderDockspace() {

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("##Dockspace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::PopStyleVar();

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

    }
    void RenderFileView(const Window& dialogWindow, bool openMode) {

        ImGui::Begin("File View");

        const float padding = 16.0f;
        const float thumbnailSize = 128.0f;
        const float cellSize = thumbnailSize + padding;
        const float panelWidth = ImGui::GetContentRegionAvail().x;

        int columns = (int) (panelWidth / cellSize);
        if(columns < 1) columns = 1;

        ImGui::Columns(columns, 0, false);

        if(ImGui::BeginPopupContextWindow("##File View")) {
                
            if(ImGui::MenuItem("New Folder")) {

                std::experimental::filesystem::create_directories((currentPath / "New Folder").String());
                editingPath = currentPath / "New Folder";

            }

            ImGui::EndPopup();
        }
        
        for(const Filesystem::DirectoryEntry& entry : Filesystem::DirectoryIterator(currentPath.String())) {

            Filesystem::Path path = entry.path().string();
            std::string name = path.File(true).String();
            bool isFile = !path.Directory();

            if (isFile && path.Extension() != filterExtension) continue;

            ImGui::PushID(name.c_str());

            uint64_t icon = isFile ? (uint64_t) fileIcon.GetID() : (uint64_t) directoryIcon.GetID();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            if(ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon), { 128.0f, 128.0f }, { 0, 1 }, { 1, 0 }) && isFile) {

                selectedFile = path;

            }
            ImGui::PopStyleColor();

            if(ImGui::BeginPopupContextItem()) {
                
                if(ImGui::MenuItem("Delete")) path.Delete();
                if(ImGui::MenuItem("Edit")) editingPath = path;

                ImGui::EndPopup();

            }

            if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {

                if(isFile) { result = path; dialogOpen = false; }
                else currentPath /= name;

            }

            if(editingPath != path) {

                ImGui::TextWrapped(name.c_str());

            } else {

                std::string fileName = name;
                if (ImGui::InputText("##Rename", &fileName) && (dialogWindow.IsKeyPressed(KeyCode::Enter) || (!ImGui::IsItemHovered() && dialogWindow.IsButtonPressed(MouseCode::Button1)))) {

                    editingPath = path.ParentPath();
                    editingPath /= fileName;

                    rename(path.String().c_str(), editingPath.String().c_str());

                    editingPath = "";

                }

            }

            ImGui::NextColumn();
            ImGui::PopID();

        }
        ImGui::Columns(1);

        ImGui::SetCursorPosY(dialogSize.y - 30.0f);

        std::string fileName = "";
        if (!selectedFile.Empty()) fileName = selectedFile.File();

        if (ImGui::InputText("Name", &fileName) && !openMode && dialogWindow.IsKeyPressed(KeyCode::Enter)) {

            result = currentPath / fileName;
            result += '.';
            result += filterExtension;
            dialogOpen = false;

        }

        ImGui::End();

    }
    void RenderDirectoryTree() {

        ImGui::Begin("Directory Tree");
        
        for(const Filesystem::DirectoryEntry& entry : Filesystem::DirectoryIterator("/home")) {

            DirectoryTreeNode(entry.path().string());

        }

        ImGui::End();

    }

    void DirectoryTreeNode(const Filesystem::Path& path) {
        
        ImGui::PushID(path.String().c_str());
        if (!path.Directory()) { ImGui::PopID(); return; }

        std::string name = path.File(true);
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx(name.c_str(), flags);

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) currentPath = path;
        if (!opened) { ImGui::PopID(); return; }

        for(const Filesystem::DirectoryEntry& entry : Filesystem::DirectoryIterator(path.String()))
            DirectoryTreeNode(entry.path().string());

        ImGui::TreePop();

        ImGui::PopID();

    }

    bool CloseDialog(const Event& e) {
        
        dialogOpen = false;
        return true;

    }

}