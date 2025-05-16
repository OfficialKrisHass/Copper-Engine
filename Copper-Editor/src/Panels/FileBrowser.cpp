#include "FileBrowser.h"

#include "Core/EditorApp.h"

#include "Projects/Project.h"

#include "Assets/ProjectAssetDatabase.h"
#include "Assets/Model.h"

#include "Panels/Properties.h"

#include "UI/NewModal.h"

#include <Engine/Renderer/Texture.h>

#include <Engine/Input/Input.h>

#include <ImGui/imgui.h>

#include <fstream>

#define PADDING 16.0f
#define THUMBNAIL_SIZE 128.0f

#ifdef CU_WINDOWS
#define PATH_SEPARATOR "\\"
#elif CU_LINUX
#define PATH_SEPARATOR "/"
#endif

using namespace Copper;

namespace Editor {

    static constexpr float CellSize = THUMBNAIL_SIZE + PADDING;

    fs::path FileBrowser::m_projectRelativeDir = "";

    fs::path editingPath = "";
    fs::path clickedFile = "";

    Texture directoryIcon;
    Texture fileIcon;
    
    bool openNewModal = false;

    void FileBrowser::Initialize() {

        CUP_FUNCTION();

        directoryIcon.Create(ExecutableFolder() / "assets/Icons/DirectoryIcon.png", Texture::Format::RGBA);
        fileIcon.Create(ExecutableFolder() / "assets/Icons/FileIcon.png", Texture::Format::RGBA);

    }

    void FileBrowser::UI() {

        CUP_FUNCTION();
        CUP_START_FRAME("File browser");

        if (!GetProject().IsValid()) {

            ImGui::Text("No project is open!");
            return;

        }
        
        RelativeDirHeader();
        WindowPopup();

        // Setup collumns

        const float panelWidth = ImGui::GetContentRegionAvail().x;

        uint32 columns = (uint32) (panelWidth / CellSize);
        if (columns < 1)
            columns = 1;

        ImGui::Columns(columns, 0, false);

        // Display Items

        for(const fs::directory_entry& entry : fs::directory_iterator((GetProject().GetAssetsPath() / m_projectRelativeDir).string())) {

            // Setup

            const bool directory = entry.is_directory();
            const fs::path path = fs::relative(entry.path(), GetProject().GetAssetsPath());
            const std::string extension = path.extension().string();
            const std::string filename = path.filename().replace_extension().string();

            ImGui::PushID(filename.c_str());

            if (Properties::GetSelectedData().type == SelectedData::Type::File && Properties::GetSelectedData().file == path)
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_TabSelected]);
            else
                ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });

            // Functionality
            
            if (directory)
                DirectoryEntry(path, filename);
            else
                FileEntry(path, filename, extension);
            ImGui::PopStyleColor();

            EntryPopup(path);

            if (editingPath == path)
                EditName(path, filename);
            else
                ImGui::TextWrapped(filename.c_str());

            // Finalize

            ImGui::NextColumn();
            ImGui::PopID();
            
        }

        if (Properties::GetSelectedData().type == SelectedData::Type::File && ImGui::IsWindowFocused() && Input::GetKeyState(KeyCode::Delete) == KeyState::Pressed) {

            fs::remove_all(GetProject().GetAssetsPath() / Properties::GetSelectedData().file);
            Properties::ClearSelectedData();

        }

        ImGui::Columns(1);

        // New modal

        if (openNewModal) {

            NewModal::Open();
            openNewModal = false;

        }
        NewModal::UIRender();

        CUP_END_FRAME();
        
    }

    void FileBrowser::RelativeDirHeader() {

        CUP_FUNCTION();

        ImGui::GetFont()->FontSize -= 2.0f;

        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0));
        if (ImGui::Button("<-", ImVec2(30, 30)) && m_projectRelativeDir != "")
            m_projectRelativeDir = m_projectRelativeDir.parent_path();
        ImGui::PopStyleVar();

        ImGui::SameLine();

        std::string path = PATH_SEPARATOR;
        if (!m_projectRelativeDir.empty())
            path += m_projectRelativeDir.string() + PATH_SEPARATOR;

        ImGui::Text(path.c_str());

        ImGui::GetFont()->FontSize += 2.0f;

    }
    void FileBrowser::WindowPopup() {

        CUP_FUNCTION();

        if (!ImGui::BeginPopupContextWindow("##File Browser")) return;

        if (ImGui::MenuItem("New", nullptr, false, GetProject().IsValid()))
            openNewModal = true;
        
        ImGui::EndPopup();

    }
    void FileBrowser::EntryPopup(const Copper::fs::path& path) {

        CUP_FUNCTION();

        if (!ImGui::BeginPopupContextItem()) return;

        if (ImGui::MenuItem("Remove"))
            fs::remove_all(GetProject().GetAssetsPath() / path);
        if (ImGui::MenuItem("Edit"))
            editingPath = path;

        ImGui::EndPopup();

    }

    void FileBrowser::DirectoryEntry(const fs::path& path, const std::string& filename) {

        CUP_FUNCTION();

        ImGui::ImageButton("##Entry", static_cast<ImTextureID>((uint64) directoryIcon.GetID()), { THUMBNAIL_SIZE, THUMBNAIL_SIZE }, { 0, 1 }, { 1, 0 });

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
            m_projectRelativeDir /= filename;

    }
    void FileBrowser::FileEntry(const fs::path& path, const std::string& filename, const std::string& extension) {

        CUP_FUNCTION();

        ImGui::ImageButton("##Entry", static_cast<ImTextureID>((uint64) fileIcon.GetID()), { THUMBNAIL_SIZE, THUMBNAIL_SIZE }, { 0, 1 }, { 1, 0 });

        if (ImGui::IsItemClicked())
            clickedFile = path;
        if (clickedFile == path && ImGui::IsItemHovered() && ImGui::IsMouseReleased(0))
            Properties::SetSelectedFile(path);

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {

            if (extension == ".copper")
                OpenSceneNext(GetProject().GetAssetsPath() / path);

        }

        if (extension == ".fbx" && ImGui::BeginDragDropSource()) {

            ModelAsset& model = ProjectAssetDatabase::GetAssetFromPath<ModelAsset>(path);

            ImGui::SetDragDropPayload("FB_MODEL", &model, sizeof(ModelAsset), ImGuiCond_Once);
            ImGui::EndDragDropSource();

        }
        if ((extension == ".png" || extension == ".jpg") && ImGui::BeginDragDropSource()) {

            TextureAsset& texture = ProjectAssetDatabase::GetAssetFromPath<TextureAsset>(path);

            ImGui::SetDragDropPayload("FB_TEXTURE", &texture, sizeof(TextureAsset), ImGuiCond_Once);
            ImGui::EndDragDropSource();

        }
        if (extension == ".mat" && ImGui::BeginDragDropSource()) {

            MaterialAsset& material = ProjectAssetDatabase::GetAssetFromPath<MaterialAsset>(path);

            ImGui::SetDragDropPayload("FB_MATERIAL", &material, sizeof(MaterialAsset), ImGuiCond_Once);
            ImGui::EndDragDropSource();

        }

    }

    void FileBrowser::EditName(const Copper::fs::path& path, const std::string& filename) {

        CUP_FUNCTION();

        const std::string fullPath = (GetProject().GetAssetsPath() / path).string();

        char buffer[128] = {};
        std::strncpy(buffer, filename.c_str(), filename.length() * sizeof(char));

        ImGui::InputText("##Edit Name", buffer, sizeof(buffer));
        if (Input::GetKeyState(KeyCode::Enter) == KeyState::Pressed || (Input::GetKeyState(KeyCode::Mouse0) == KeyState::Pressed && !ImGui::IsItemHovered())) {

            editingPath = editingPath.parent_path();
            editingPath /= buffer;
            editingPath += path.extension();

            fs::rename(fullPath, GetProject().GetAssetsPath() / editingPath);
            
            if (Properties::GetSelectedData().type == SelectedData::Type::File && Properties::GetSelectedData().file == path)
                Properties::SetSelectedFile(editingPath);

            editingPath = "";

        }

    }

}
