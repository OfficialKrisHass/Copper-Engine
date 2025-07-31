#include "Project.h"

#include "Core/EditorApp.h"

#include "Projects/ProjectChecker.h"
#include "Projects/ProjectTemplate.h"

#include "Assets/AssetType.h"
#include "Assets/ProjectAssetDatabase.h"

#include "Panels/FileBrowser.h"

#include <Engine/Core/Window.h>

#include <Engine/Scripting/ScriptingEngine.h>

#include <Engine/Input/Input.h>
#include <Engine/Input/Popup.h>

#include <Engine/Utilities/FileUtils.h>

#include <cstdlib>
#include <yaml-cpp/yaml.h>

#include <fstream>

using namespace Copper;

namespace Editor {

    static void CreateFileAndReplace(const fs::path& original, const fs::path& out, const std::string& what, const std::string& argument);

    Project::Project(const fs::path& path) {

        CUP_FUNCTION();
        Create(path);

    }

    void Project::Create(const fs::path& path) {

        CUP_FUNCTION();

        m_path = path;
        m_name = path.filename().string();

        if (!fs::exists(m_path))
            fs::create_directories(m_path);

        CreateProjectFromTemplate("Empty", *this);

        bool opened = false;
        try { opened = LoadFile(path); }
        catch (YAML::Exception& e) {

            Input::ErrorPopup("Failed to open project", "Could not open Project.cu file.\n\nPath: " + path.string() + "\n\nError: " + e.msg);
            exit(-1);

        }

    }

    void Project::Open(const fs::path& path) {

        CUP_FUNCTION();

#ifdef CU_LOG_STATUS
        if (GetEngineState() == EngineState::PostInitialization)
            LogStatus("\tOpening project '{}'.", path.filename().string());
        else
#endif
            Log("Opening project '{}'.", path.filename().string());

        try { 

            if (!LoadFile(path)) return; 

        }
        catch (YAML::Exception e) {

            Input::ErrorPopup("Project open error", "Could not open Project.cu file.\n\nPath: " + path.string() + "\n\nError: " + e.msg);
            return;

        }

        if (uint16_t issueFlags = ProjectChecker::CheckProject(*this)) {

            std::string description = "This project is missing some of the core folders and/or files that are required by the Editor to function properly.\n\n";
            ProjectChecker::PrintIssues(*this, issueFlags, description);
            description += "\nDo you want the editor to attempt to fix the project ?";

            switch (Input::WarningPopup("Corrupted Project", description)) {

                case Input::PopupResult::Yes: ProjectChecker::FixProject(*this, issueFlags); break;
                case Input::PopupResult::No: return;
                default: return; 

            }

        }

        if (m_valid)
            ProjectAssetDatabase::Unload();

        m_valid = true;

        FileBrowser::Refresh();
        ProjectAssetDatabase::Initialize();

        if (Scripting::GameAssembly().IsValid())
            Scripting::Unload();

        Scripting::Load((path / "Binaries/" / (m_name + ".dll")).string());

        m_assetWatch.Start(GetAssetsPath());
        m_assetWatch.SetCallback(std::bind(&Project::FileChangeCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        if (m_lastOpenedScenePath.empty() || !fs::exists(GetAssetsPath() / m_lastOpenedScenePath)) return;

        OpenScene(GetAssetsPath() / m_lastOpenedScenePath);


    }
    void Project::Open() {

        CUP_FUNCTION();

        fs::path path = Utilities::FolderOpenDialog("Open Project", m_path.empty() ? ROOT_DIR : m_path.parent_path());
        if (path.empty()) return;

        Open(path);

    }
    void Project::Save(bool saveScene) const {

        CUP_FUNCTION();

        if (m_path.empty()) {

            LogError("Can't save project at empty path");
            return;

        }

        YAML::Emitter out;
        out << YAML::BeginMap; // Main

        out << YAML::Key << "Name" << YAML::Value << m_name;
        out << YAML::Key << "Last scene" << YAML::Value << m_lastOpenedScenePath;

        out << YAML::EndMap; // Main
        std::ofstream file(m_path / "Project.cu");
        file << out.c_str(); 

        if (saveScene)
            SaveScene();

        LogDebug("Project saved.");

    }
    void Project::SaveAs() {

        CUP_FUNCTION();

        m_path = Utilities::FolderOpenDialog("Save Project As", m_path.empty() ? ROOT_DIR : m_path.parent_path());
        if (m_path.empty()) return;

        Save();

    }

    void Project::Update() {

        CUP_FUNCTION();

        m_assetWatch.Update();
        ProjectAssetDatabase::Update();

        // Most rebuilding will be handled by the editors OnWindowFocus event handler
        // However, there could be an edge case, so for pure safety
        if (m_shouldRebuild && GetWindow().IsFocused())
            Build();

    }

    bool Project::LoadFile(const fs::path& path) {

        CUP_FUNCTION();

        if (path.empty()) {

            LogError("Can't open project at empty path");
            return false;

        }
        if (!fs::exists(path / "Project.cu")) {

            Input::ErrorPopup("Invalid project", "This folder is not a valid project (Project.cu could not be found).");
            return false;

        }

        m_path = path;

        YAML::Node main;
        try { main = YAML::LoadFile((path / "Project.cu").string()); } catch (YAML::Exception e) {

            Input::ErrorPopup("Project open error", "Something went wrong during loading the project file (Project.cu).\n\nProject Path: " + path.string() + "\n\nError: " + e.what());
            return false; 

        }

        m_name = main["Name"].as<std::string>();
        m_lastOpenedScenePath = main["Last scene"].as<fs::path>();

        return true;

    }

    bool Project::Build() {

        CUP_FUNCTION();

        if (!BuildScripts()) {

            LogError("Failed to Build the project scripts.");
            return false;

        }
        if (!Scripting::Reload()) {

            LogError("Failed to reload the Scripting Engine.");
            return false;

        }

        m_shouldRebuild = false;

        return true;

    }
    bool Project::BuildScripts() const {

        CUP_FUNCTION();

#ifdef CU_WINDOWS
        std::string cmd = "C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\MSBuild.exe ";

        size_t pos = m_path.string().find_first_of(' ');
        std::string newPath = m_path.string();
        while (pos != std::string::npos) {

            newPath.erase(pos, 1);
            newPath.insert(pos, "\" \"");
            pos = newPath.find_first_of(' ', pos + 3);

        }

        pos = m_name.find_first_of(' ');
        std::string newName = m_name;
        while (pos != std::string::npos) {

            newName.erase(pos, 1);
            newName.insert(pos, "\" \"");
            pos = newName.find_first_of(' ', pos + 3);

        }

        cmd += newPath + "\\" + newName + ".csproj";
        cmd += " -nologo";

        system(cmd.c_str());
#else
        const std::string cmd = "make --no-print-directory -C \"" + m_path.string() + "\" -f Makefile";
        system(cmd.c_str());
#endif

        return true;

    }

    void Project::RegenerateProjectFiles() const {

        CUP_FUNCTION();

        CreateFileAndReplace("assets/Templates/Project Files/Project.cu.cut", m_path / "Project.cu", ":{ProjectName}", m_name);

    }
    void Project::RegenerateBuildFiles() const {

        CUP_FUNCTION();

#ifdef CU_WINDOWS
        CreateFileAndReplace(ExecutableFolder() / "assets/Templates/Project Files/Template.sln.cut", m_path / (m_name + ".sln"), ":{ProjectName}", m_name);
        CreateFileAndReplace(ExecutableFolder() / "assets/Templates/Project Files/Template.csproj.cut", m_path / (m_name + ".csproj"), ":{ProjectName}", m_name);
#elif CU_LINUX
        CreateFileAndReplace(ExecutableFolder() / "assets/Templates/Project Files/premake5.lua.cut", m_path / "premake5.lua", ":{ProjectName}", m_name);
#endif

    }

#ifdef CU_LINUX
    void Project::RunPremake() const {

        CUP_FUNCTION();

        if (m_path.empty()) {

            LogError("Project has an empty path, can't run premake");
            return;

        }

        // It hurts my eyes, but there is no other solution that I know of
        //system(("cd \"" + data.project.path.string() + "\" ; ./premake/premake5 gmake2").c_str());

        // Turns out there is :)
        system(((ExecutableFolder() / "util/premake/premake5 --file=\"").string() + (m_path / "premake5.lua").string() + "\" gmake2 > /dev/null").c_str());

    }
#endif

    void Project::FileChangeCallback(const fs::path& path, FileChangeType changeType, uint32 cookie) {

        CUP_FUNCTION();

        AssetType type = GetAssetTypeFromExtension(path.extension().string());

        ProjectAssetDatabase::OnAssetChange(path, changeType, type, cookie);
        FileBrowser::OnAssetChange(path, changeType, type, cookie);

        // TODO: Move this into a separate file that handles the build system

        if (path.extension().string() != ".cs") return;

        m_shouldRebuild = true;

#ifdef CU_LINUX
        if (changeType != FileChangeType::Changed)
            RunPremake();
#endif

    }

    static void CreateFileAndReplace(const fs::path& original, const fs::path& out, const std::string& what, const std::string& argument) {

        CUP_FUNCTION();

        if (!fs::exists(out.parent_path()))
            fs::create_directories(out.parent_path());

        std::ifstream originalFile(original);
        std::ofstream templateFile(out);

        std::string line;
        while (std::getline(originalFile, line)) {

            size_t pos = line.find(what);
            while (pos != std::string::npos) {

                line.replace(pos, what.size(), argument);
                pos = line.find(what);

            }

            templateFile << line << "\n";

        }

        originalFile.close();
        templateFile.close();

    }

}
