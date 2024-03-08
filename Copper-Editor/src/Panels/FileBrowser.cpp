#include "FileBrowser.h"

#include "Core/EditorApp.h"

#include "AssetFile/AssetFileDatabase.h"
#include "AssetFile/Serializer.h"
#include "AssetFile/MetaFile.h"

#include "Panels/Properties.h"
#include "Panels/SceneHierarchy.h"

#include "Engine/AssetStorage/AssetMap.h"
#include "Engine/AssetStorage/AssetStorage.h"

#include "Engine/Utilities/FileTemplate.h"

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

    TextureData directoryIcon;
    TextureData fileIcon;

    FileBrowser::FileBrowser(const fs::path& initialDir) : Panel("File Browser") {
        
        m_projectRelativeDir = initialDir;

        directoryIcon.Create("assets/Icons/DirectoryIcon.png", TextureData::Format::RGBA);
        fileIcon.Create("assets/Icons/FileIcon.png", TextureData::Format::RGBA);

    }

    void FileBrowser::UI() {

        CUP_START_FRAME("File browser");

        if (!GetProject()) {

            ImGui::Text("No project is open!");
            return;

        }
        
        RelativeDirHeader();
        WindowPopup();

        // Setup collumns

        const float panelWidth = ImGui::GetContentRegionAvail().x;

        uint32 columns = (uint32) (panelWidth / CellSize);
        if(columns < 1) columns = 1;

        ImGui::Columns(columns, 0, false);

        // Display Items

        for(const fs::directory_entry& entry : fs::directory_iterator((GetProject().assetsPath / m_projectRelativeDir).string())) {

            // Setup

            const bool directory = entry.is_directory();
            const fs::path path = fs::relative(entry.path(), GetProject().assetsPath);
            const std::string extension = path.extension().string();
            if (extension == ".cum") continue;

            std::string filename = path.filename().string();
            if (!directory)
                filename = filename.substr(0, filename.find_last_of('.'));

            ImGui::PushID(path.string().c_str());
            EntryIcon(directory);

            // Functionality

            if (directory)
                DirectoryEntry(path, filename);
            else
                FileEntry(path, filename, extension);

            EntryPopup(path);

            // Finalize

            if (editingPath == path)
                EditName(path, filename);
            else
                ImGui::TextWrapped(filename.c_str());

            ImGui::NextColumn();
            ImGui::PopID();
            
        }

        ImGui::Columns(1);

        CUP_END_FRAME();
        
    }

    void FileBrowser::RelativeDirHeader() {

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

        if (!ImGui::BeginPopupContextWindow("##File Browser")) return;

        if (ImGui::BeginMenu("New")) {

            if (ImGui::MenuItem("Folder", 0, false, GetProject().name != "")) {

                fs::path path = GetProject().assetsPath / m_projectRelativeDir;
                path /= "New Folder";

                fs::create_directories(path.string());

                editingPath = path;

            }
            ImGui::Separator();
            if (ImGui::MenuItem("Script")) {

                Utils::FileFromTemplate("Script.cs", (GetProject().assetsPath / m_projectRelativeDir / "Script.cs").string(), {{"ScriptName", "Script"}});
                editingPath = GetProject().assetsPath / m_projectRelativeDir / "Script.cs";

            }
            if (ImGui::MenuItem("Material")) {

                fs::path path = m_projectRelativeDir / "Material.mat";

                Material mat = AssetStorage::CreateAsset<MaterialData>();
                editingPath = path;

                AssetFile::SerializeMaterial(GetProject().assetsPath / path, mat);
                MetaFile::Serialize((GetProject().assetsPath / path).string() + ".cum", mat.AssetUUID());

                AssetFileDatabase::Refresh();

            }

            ImGui::EndMenu();

        }
        
        ImGui::EndPopup();

    }
    void FileBrowser::EntryPopup(const Copper::fs::path& path) {

        if (!ImGui::BeginPopupContextItem()) return;

        if (ImGui::MenuItem("Remove")) {

            fs::remove_all(GetProject().assetsPath / path);
            if (fs::exists((GetProject().assetsPath / path).string() + ".cum"))
                fs::remove((GetProject().assetsPath / path).string() + ".cum");

            AssetFileDatabase::Refresh();

        }
        if (ImGui::MenuItem("Edit"))
            editingPath = path;

        ImGui::EndPopup();

    }

    void FileBrowser::EntryIcon(bool directory) {

        uint32_t iconID = directory ? directoryIcon.ID() : fileIcon.ID();

        ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
        ImGui::ImageButton(reinterpret_cast<ImTextureID>((uint64) iconID), { THUMBNAIL_SIZE, THUMBNAIL_SIZE }, { 0, 1 }, { 1, 0 });
        ImGui::PopStyleColor();

    }

    void FileBrowser::DirectoryEntry(const fs::path& path, const std::string& filename) {

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
            m_projectRelativeDir /= filename;

    }
    void FileBrowser::FileEntry(const fs::path& path, const std::string& filename, const std::string& extension) {

        if (ImGui::IsItemClicked())
            clickedFile = path;
        if (clickedFile == path && ImGui::IsItemHovered() && ImGui::IsMouseReleased(0))
            Properties::SetSelectedFile(path);

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {

            if (extension == ".copper")
                OpenScene(GetProject().assetsPath / path);

        }

        if ((extension == ".fbx" || extension == ".gltf" || extension == ".obj") && ImGui::BeginDragDropSource()) {

            char* itemPath = (char*) path.string().c_str();

            ImGui::SetDragDropPayload("MODEL", itemPath, (path.string().size() + 1) * sizeof(char), ImGuiCond_Once);
            ImGui::EndDragDropSource();

        }
        if ((extension == ".png" || extension == ".jpg") && ImGui::BeginDragDropSource()) {

            Texture& texture = AssetFileDatabase::GetAssetFromPath<Texture>(GetProject().assetsPath / path);

            ImGui::SetDragDropPayload("FB_TEXTURE", &texture, sizeof(Texture), ImGuiCond_Once);
            ImGui::EndDragDropSource();

        }

    }

    void FileBrowser::EditName(const Copper::fs::path& path, const std::string& filename) {

        const std::string fullPath = (GetProject().assetsPath / path).string();

        char buffer[128] = {};
        std::strncpy(buffer, filename.c_str(), filename.length() * sizeof(char));

        if (ImGui::InputText("##Edit Name", buffer, sizeof(buffer)) && (Input::IsKey(KeyCode::Enter) || Input::IsButton(MouseCode::Button1))) {

            editingPath = editingPath.parent_path();
            editingPath /= buffer;
            editingPath += path.extension();

            const std::string newFullPath = (GetProject().assetsPath / editingPath).string();
            CU_ASSERT(!rename(fullPath.c_str(), newFullPath.c_str()), "An error occured trying to rename a file!\n\tFile: {}\n\tNew name: {}", path.string(), buffer);
            
            std::string extension = path.extension().string();
            if (extension == ".copper" ||
                extension == ".png" || extension == ".jpg" ||
                extension == ".mat")
                CU_ASSERT(!rename((fullPath + ".cum").c_str(), (newFullPath + ".cum").c_str()), "An error occured trying to rename a file!\n\tFile: {}\n\tNew name: {}", path.string(), buffer);

            if (Properties::GetSelectedFile() == path)
                Properties::SetSelectedFile(editingPath);

            AssetFileDatabase::Refresh();

            editingPath = "";

        }

    }

    void FileBrowser::NewScript(const Copper::fs::path& path) {

        std::ifstream templ("assets/Templates/Script.cs.cut");
        std::ofstream out(path / "Script.cs");

        out << templ.rdbuf();

    }

}