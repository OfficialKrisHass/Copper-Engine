#include "NewModal.h"
#include "NewModalData.h"

#include "Core/EditorApp.h"

#include "Projects/Project.h"

#include "Assets/Serializer.h"

#include "Panels/FileBrowser.h"

#include <Engine/Renderer/Material.h>

#include <Engine/AssetStorage/AssetStorage.h>
#include <Engine/AssetStorage/AssetPtr.h>

#include <Engine/Input/Input.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <ImGui/imgui.h>

#include <fstream>

#define NAME_MAX_LENGTH 64

#define BUTTON_HEIGHT 40

using namespace Copper;

namespace Editor::NewModal {

    static bool open = false;
    static uint32 selectedOption = 0;

    static char nameInput[NAME_MAX_LENGTH];

    static fs::path directory;

    void Options();
    void Details();

    void Create();

    void CreateFolder();
    void CreateScript();
    void CreateMaterial();

    bool CreateDisabled();

    void Open() {

        CUP_FUNCTION();
        open = true;

        ImGui::OpenPopup("New");

        directory = GetProject().GetAssetsPath() / FileBrowser::GetRelativeDir();

    }
    void Close() {

        CUP_FUNCTION();
        open = false;

        nameInput[0] = '\0';

    }

    void UIRender() {

        CUP_FUNCTION();

        if (!open) return;

        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowBgAlpha(1.0f);

        if (!ImGui::BeginPopupModal("New", &open)) return;

        const ImGuiStyle& style = ImGui::GetStyle();

        Options();
        ImGui::SameLine();
        Details();

        float tmp = ImGui::GetContentRegionAvail().y;

        // Button
        
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { style.FramePadding.x * 3.0f, style.FramePadding.y * 3.0f });

        const ImVec2 size = ImVec2(ImGui::CalcTextSize("Create").x + style.FramePadding.x * 8.0f, BUTTON_HEIGHT);
        ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionMax().x - size.x - style.WindowPadding.x,
                                   ImGui::GetContentRegionMax().y - size.y - style.ItemSpacing.y));

        bool disabled = CreateDisabled();

        ImGui::BeginDisabled(disabled);
        if ((ImGui::Button("Create", size) || Input::GetKeyState(KeyCode::Enter) == KeyState::Pressed) && !disabled)
            Create();
        ImGui::EndDisabled();

        ImGui::PopStyleVar();
        ImGui::SameLine();

        // Name input

        float cursorY = ImGui::GetCursorPosY();

        ImGui::SetCursorPos(ImVec2(style.WindowPadding.x, cursorY + style.FramePadding.y));
        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::SetCursorPosY(cursorY + tmp / 4.0f);
        ImGui::InputText("##Name", nameInput, NAME_MAX_LENGTH);

        if (Input::GetKeyState(KeyCode::Escape) == KeyState::Pressed)
            Close();

        ImGui::EndPopup();

    }

    void Options() {

        CUP_FUNCTION();
        ImGui::BeginChild("##Options", ImVec2(70.0f * ImGui::GetContentRegionAvail().x / 100.0f, ImGui::GetContentRegionAvail().y - BUTTON_HEIGHT * 1.5f), true);

        ImGui::PushItemWidth(-1);

        ImVec2 size = ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y);
        if (ImGui::BeginListBox("##OptionsList", size)) {

            for (int i = 0; i < Data::optionsCount; i++) {

                ImGui::PushID(i);

                const bool selected = i == selectedOption;
                if (ImGui::Selectable(Data::options[i], selected))
                    selectedOption = i;

                if (selected)
                    ImGui::SetItemDefaultFocus();

                ImGui::PopID();

            }

            ImGui::EndListBox();

        }

        ImGui::PopItemWidth();

        ImGui::EndChild();

    }
    void Details() {

        CUP_FUNCTION();

        ImGui::BeginChild("##Details", ImVec2(0, 90.0f * ImGui::GetContentRegionAvail().y / 100.0f));

        ImGui::TextWrapped(Data::optionsDetails[selectedOption]);

        ImGui::EndChild();

    }

    void Create() {

        CUP_FUNCTION();

        switch (selectedOption) {

            case 0: CreateFolder(); break;
            case 1: CreateScript(); break;
            case 2: CreateMaterial(); break;

        }

        Close();

    }

    void CreateFolder() {

        CUP_FUNCTION();
        fs::create_directories(directory / nameInput);

    }
    void CreateScript() {

        CUP_FUNCTION();

        std::ifstream templ;
        std::ofstream file;

        templ.open(ExecutableFolder() / "assets/Templates/Script.cs.cut");
        file.open((directory / nameInput).replace_extension(".cs"));

        std::string line;
        while (std::getline(templ, line)) {

            std::size_t pos = line.find(":{ScriptName}");
            while (pos != std::string::npos) {

                line.erase(pos, strlen(":{ScriptName}"));
                line.insert(pos, nameInput);

                pos = line.find(":{ScriptName}");

            }

            file << line << "\n";

        }

        file.close();
        templ.close();

    }
    void CreateMaterial() {

        CUP_FUNCTION();

        MaterialAsset material = AssetStorage::CreateAsset<Material>();
        material->texture = Texture::WhiteTexture();

        AssetFile::SerializeMaterial((directory / nameInput).replace_extension(".mat"), material);

    }

    bool CreateDisabled() {

        CUP_FUNCTION();

        fs::path path = directory / nameInput;

        switch (selectedOption) {

            case 0: break;
            case 1: path.replace_extension(".cs"); break;
            case 2: path.replace_extension(".mat"); break;
            default: break;

        }

        return fs::exists(path);

    }

}
