#include "FileBrowser.h"

#include "Core/EditorApp.h"

#include "Projects/Project.h"

#include "Assets/ProjectAssetDatabase.h"
#include "Assets/Model.h"

#include "Panels/Properties.h"

#include "UI/NewItemModal.h"

#include <Engine/Renderer/Texture.h>

#include <Engine/Input/Input.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

#include <map>

#define PADDING 16.0f
#define THUMBNAIL_SIZE 128.0f
#define ENTRY_SIZE (THUMBNAIL_SIZE + PADDING)

using namespace Copper;

namespace Editor {

    FileBrowser::DirectoryEntry FileBrowser::m_rootEntry;
    const FileBrowser::DirectoryEntry* FileBrowser::m_currEntry = &m_rootEntry;

    fs::path FileBrowser::m_projectRelativeDir;

    fs::path editingPath = "";
    fs::path clickedFile = "";

    Texture directoryIcon;
    Texture fileIcon;
    
    bool openNewItemModal = false;

    void FileBrowser::Initialize() {

        CUP_FUNCTION();

        directoryIcon.Create(ExecutableFolder() / "assets/Icons/DirectoryIcon.png", Texture::Format::RGBA);
        fileIcon.Create(ExecutableFolder() / "assets/Icons/FileIcon.png", Texture::Format::RGBA);

    }
    void FileBrowser::Refresh() {

        CUP_FUNCTION();

        m_currEntry = &m_rootEntry;
        SetRelativeDir("");

        m_rootEntry.folders.clear();
        m_rootEntry.files.clear();

        RefreshDirectoryTree(m_rootEntry, "");

    }

    void FileBrowser::UI() {

        CUP_FUNCTION();
        CUP_START_FRAME("File browser");

        if (!GetProject().IsValid()) {

            ImGui::Text("No project is open!");
            return;

        }

        // Window stuff
        
        RelativeDirHeader();
        WindowPopup();

        RenderDirectoryEntry(*m_currEntry);

        // Delete key

        if (Properties::GetSelectedData().type == SelectedData::Type::File && ImGui::IsWindowFocused() && Input::GetKeyState(KeyCode::Delete) == KeyState::Pressed) {

            fs::remove_all(GetProject().GetAssetsPath() / Properties::GetSelectedData().file);
            Properties::ClearSelectedData();

        }
        if (m_focused && Input::GetKeyState(KeyCode::Mouse3) == KeyState::Pressed)
            NavigateUp();

        // New modal

        if (openNewItemModal) {

            NewItemModal::Open();
            openNewItemModal = false;

        }
        NewItemModal::UIRender();

        CUP_END_FRAME();
        
    }

    void FileBrowser::RefreshDirectoryTree(DirectoryEntry& root, fs::path path) {

        CUP_FUNCTION();

        root.files.clear();
        for (const fs::directory_entry& entry : fs::directory_iterator(GetProject().GetAssetsPath() / path)) {

            std::string name = entry.path().filename().string();

            if (entry.is_directory()) {

                // This will either create, or get the entry. I am so smart

                DirectoryEntry& subEntry = root.folders[name];
                subEntry.parent = &root;
                RefreshDirectoryTree(subEntry, path / name);

            } else if (entry.is_regular_file())
                root.files.push_back(name);

        }

        std::sort(root.files.begin(), root.files.end());

    }
    void FileBrowser::OnAssetChange(const fs::path& path, FileChangeType changeType, AssetType type, uint32 cookie) {

        CUP_FUNCTION();

        const fs::path fullPath = GetProject().GetAssetsPath() / path;
        if (!fs::exists(fullPath.parent_path())) return;

        switch (changeType) {

            case FileChangeType::Created: {

                if (fs::is_regular_file(fullPath)) {

                    DirectoryEntry* parent = CreateDirectoryEntry(path.parent_path());
                    parent->AddFile(path.filename().string());

                } else if (fs::is_directory(fullPath))
                    CreateDirectoryEntry(path); // This will create the entry if it does not exist

                break;

            }
            case FileChangeType::RenamedOld:
            case FileChangeType::Deleted: {

                const std::string name = path.filename().string();

                DirectoryEntry* parent = GetDirectoryEntry(path.parent_path());
                if (parent == nullptr) break;

                const auto it = parent->folders.find(name);
                if (it != parent->folders.end())
                    parent->folders.erase(it);
                else {

                    uint32 i;
                    for (i = 0; i < parent->files.size(); i++) {

                        if (parent->files[i] != name) continue;

                        parent->files.erase(parent->files.begin() + i);
                        break;

                    }

                } 

                break;


            }
            case FileChangeType::RenamedNew: {

                // TODO: Replace the creation of entries and files with DirectoryEntry functions and a constructor

                if (fs::is_directory(fullPath)) {

                    DirectoryEntry* entry = CreateDirectoryEntry(path);
                    RefreshDirectoryTree(*entry, path);

                } else if (fs::is_regular_file(fullPath)) {

                    DirectoryEntry* parent = CreateDirectoryEntry(path.parent_path());
                    parent->AddFile(path.filename().string());

                }

                break;

            }
            default: break;

        }

    }
    FileBrowser::DirectoryEntry* FileBrowser::GetDirectoryEntry(const fs::path& path) {

        CUP_FUNCTION();

        DirectoryEntry* tmp = &m_rootEntry;
        for (const fs::path& it : path) {

            const auto folder = tmp->folders.find(it.string());
            if (folder == tmp->folders.end()) return nullptr;

            tmp = &folder->second;

        }

        return tmp;

    }
    FileBrowser::DirectoryEntry* FileBrowser::CreateDirectoryEntry(const fs::path& path) {

        CUP_FUNCTION();

        DirectoryEntry* entry = &m_rootEntry;
        for (const fs::path& it : path) {

            const std::string name = it.string();
            DirectoryEntry* tmp = entry;

            entry = &entry->folders[name];
            if (entry->parent == nullptr)
                entry->parent = tmp;

        }

        return entry;

    }

    void FileBrowser::RenderDirectoryEntry(const DirectoryEntry& entry) {

        CUP_FUNCTION();

        uint32 columns = (uint32) (ImGui::GetContentRegionAvail().x / ENTRY_SIZE);
        if (columns < 1)
            columns = 1;

        if (!ImGui::BeginTable("##FileTable", columns)) return;

        for (const auto& it : entry.folders) {

            // Setup

            const std::string& name = it.first;
            const DirectoryEntry& folder = it.second;
            const fs::path& path = m_projectRelativeDir / name;

            ImGui::PushID(name.c_str());
            RenderEntryIcon(directoryIcon, path);

            // Functionality

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {

                m_currEntry = &folder;
                m_projectRelativeDir /= name;

            }

            // Finish

            EditName(path, name);
            ImGui::PopID();

        }
        for (const std::string& file : entry.files) {

            // Setup

            const fs::path& path = m_projectRelativeDir / file;

            ImGui::PushID(file.c_str());
            RenderEntryIcon(fileIcon, path);

            // Functionality

            const bool hovered = ImGui::IsItemHovered();
            const AssetType type = GetAssetTypeFromExtension(path.extension().string());

            if (ImGui::IsItemClicked())
                clickedFile = path;
            if (clickedFile == path && hovered && ImGui::IsMouseReleased(0))
                Properties::SetSelectedFile(path);

            if (hovered && ImGui::IsMouseDoubleClicked(0)) {

                switch (type) {

                    case AssetType::Scene: OpenSceneNext(GetProject().GetAssetsPath() / path); break;
                    default: break;

                }

            }

            if (IsDatabaseAsset(type) && ImGui::BeginDragDropSource()) {

                const UUID& uuid = ProjectAssetDatabase::GetAssetFromPath(path);
                CU_ASSERT(uuid.IsValid(), "Invalid UUID retrieved from GetAssetFromPath(), asset path: '{}'", path);

                // TODO: Possibly replace this by passing the type directly into the DragDropPayload, instead of a string
                //       may require a custom Drag Drop system ffs.
                static const std::unordered_map<AssetType, const char*> dragDropType = {

                    { AssetType::Texture, "FB_TEXTURE" },
                    { AssetType::Material, "FB_MATERIAL" },
                    { AssetType::Model, "FB_MODEL" },

                };
                ImGui::SetDragDropPayload(dragDropType.at(type), &uuid, sizeof(UUID), ImGuiCond_Once);

                ImGui::EndDragDropSource();

            }

            // Finish

            EditName(path, file);
            ImGui::PopID();

        }

        ImGui::EndTable();

    }
    void FileBrowser::RenderEntryIcon(const Texture& icon, const fs::path& path) {

        CUP_FUNCTION();

        if (Properties::GetSelectedData().type == SelectedData::Type::File && Properties::GetSelectedData().file == path)
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_TabSelected]);
        else
            ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });

        ImGui::TableNextColumn();
        ImGui::ImageButton("##Entry", static_cast<ImTextureID>(icon.GetID()), { THUMBNAIL_SIZE, THUMBNAIL_SIZE }, { 0, 1 }, { 1, 0 });
        ImGui::PopStyleColor();

        EntryPopup(path);

    }

    void FileBrowser::RelativeDirHeader() {

        CUP_FUNCTION();

        ImGui::GetFont()->FontSize -= 2.0f;

        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0));

        if (ImGui::Button("<-", ImVec2(30, 30)) && m_projectRelativeDir != "")
            NavigateUp();

        ImGui::PopStyleVar();

        ImGui::SameLine();

        std::string path = "/";
        if (!m_projectRelativeDir.empty())
            path += m_projectRelativeDir.string() + '/';

        ImGui::Text(path.c_str());

        ImGui::GetFont()->FontSize += 2.0f;

    }
    void FileBrowser::WindowPopup() {

        CUP_FUNCTION();

        if (!ImGui::BeginPopupContextWindow("##File Browser")) return;

        if (ImGui::MenuItem("New item", nullptr, false, GetProject().IsValid()))
            openNewItemModal = true;
        
        ImGui::EndPopup();

    }
    void FileBrowser::EntryPopup(const Copper::fs::path& path) {

        CUP_FUNCTION();

        if (!ImGui::BeginPopupContextItem()) return;

        if (ImGui::MenuItem("Delete"))
            fs::remove_all(GetProject().GetAssetsPath() / path);
        if (ImGui::MenuItem("Rename"))
            editingPath = path;

        ImGui::EndPopup();

    }

    void FileBrowser::EditName(const Copper::fs::path& path, const std::string& filename) {

        CUP_FUNCTION();

        if (editingPath != path) {
            
            ImGui::TextWrapped(filename.c_str());
            return;

        }

        const fs::path fullPath = (GetProject().GetAssetsPath() / path).string();

        char buffer[128] = {};
        std::strncpy(buffer, filename.c_str(), filename.length() * sizeof(char));

        ImGui::InputText("##Edit Name", buffer, sizeof(buffer));
        if (Input::GetKeyState(KeyCode::Enter) == KeyState::Pressed || (Input::GetKeyState(KeyCode::Mouse0) == KeyState::Pressed && !ImGui::IsItemHovered())) {

            editingPath = editingPath.parent_path();
            editingPath /= buffer;

            CU_ASSERT(fs::exists(fullPath), "Tried to renamed a file that doesn't exist. Path: '{}', new path: '{}'", fullPath.string(), editingPath.string());
            fs::rename(fullPath, GetProject().GetAssetsPath() / editingPath);
            Log("Renamed '{}' to '{}'", fullPath.string(), editingPath.string());
            
            if (Properties::GetSelectedData().type == SelectedData::Type::File && Properties::GetSelectedData().file == path)
                Properties::SetSelectedFile(editingPath);

            editingPath = "";

        }

    }

    void FileBrowser::NavigateUp() {

        CUP_FUNCTION();

        m_projectRelativeDir = m_projectRelativeDir.parent_path();
        m_currEntry = m_currEntry->parent;

    }

}
