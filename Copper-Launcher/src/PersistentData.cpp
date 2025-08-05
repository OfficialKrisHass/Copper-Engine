#include "PersistentData.h"

#include "Dialogs.h"
#include "ProjectEntry.h"

#include <yaml-cpp/yaml.h>

#include <string>
#include <fstream>

namespace Launcher::PersistentData {

    static const std::string filename = "LauncherData.cup";
#ifdef CU_WINDOWS
    static const fs::path persistenFolder = std::string(getenv("appdata")) + "\\Copper-Editor";
#elif CU_LINUX
    static const fs::path persistenFolder = std::string("/home/") + cuserid(nullptr)+ "/.config/Copper-Editor";
#endif

    static fs::path editorPath = "";

    static void LocateEditor();
#ifdef CU_DEBUG
    static fs::path editorAssetsPath = "";
    
    static void LocateEditorAssets();
#endif

    void Load(std::vector<ProjectEntry>& projectEntries) {

        YAML::Node node;
        try { node = YAML::LoadFile((persistenFolder / filename).string()); }
        catch (YAML::Exception e) {

            Dialogs::Error("Couldn't Read LauncherData.cup", "Encountered an exception trying to Load the LauncherData.cup file.\nProvide the path to the Editor and we will create a new one");
            LocateEditor();
#ifdef CU_DEBUG
            LocateEditorAssets();
#endif

            Save(projectEntries);

            try { node = YAML::LoadFile((persistenFolder / filename).string()); }
            catch (YAML::Exception e) {

                Dialogs::Error("Couldn't Read LauncherData.cup", e.msg);
                exit(-1);

            }

        }

        editorPath = node["Editor Path"].as<std::string>();
#ifdef CU_DEBUG
        if (node["Editor assets Path"])
            editorAssetsPath = node["Editor assets Path"].as<std::string>();
        else {

            Dialogs::Error("Unkown Editor assets directory", "Couldn't read the editor assets directory from persistant data storage. Please locate the editor assets directory in then next dialog");
            LocateEditorAssets();

        }
#endif

        YAML::Node entries = node["Project Entries"];
        for (uint32 i = 0; i < entries.size(); i++) {

            YAML::Node entry = entries[i];
            std::string name = entry["Name"].as<std::string>();
            std::string directory = entry["Directory"].as<std::string>();

            projectEntries.push_back(ProjectEntry(name, directory));

        }

#ifdef CU_DEBUG
        bool save = false;
#endif

        if (editorPath.empty()) {

            LocateEditor();
#ifdef CU_DEBUG
            save = true;
#elif CU_RELEASE
            Save(projectEntries);
#endif

        }
#ifdef CU_DEBUG
        if (editorAssetsPath.empty()) {

            LocateEditorAssets();
            save = true;

        }

        if (save)
            Save(projectEntries);
#endif

    }
    void Save(const std::vector<ProjectEntry>& projectEntries) {

        YAML::Emitter out;

        out << YAML::BeginMap; // Main

        out << YAML::Key << "Editor Path" << YAML::Value << editorPath.string();
#ifdef CU_DEBUG
        out << YAML::Key << "Editor assets Path" << YAML::Value << editorAssetsPath.string();
#endif 

        out << YAML::Key << "Project Entries" << YAML::Value << YAML::BeginSeq; // Project Entries

        for (const ProjectEntry& entry : projectEntries) {

            out << YAML::BeginMap; // Entry

            out << YAML::Key << "Name" << YAML::Value << entry.Name();
            out << YAML::Key << "Directory" << YAML::Value << entry.Directory().string();

            out << YAML::EndMap; // Entry

        }

        out << YAML::EndSeq;

        out << YAML::EndMap;

        if (!std::filesystem::exists(persistenFolder))
            std::filesystem::create_directories(persistenFolder);

        std::ofstream file(persistenFolder / filename);
        file << out.c_str();
        file.close();

    }

    const fs::path& EditorPath() { return editorPath; }
    const fs::path& EditorAssetsPath() {

#ifdef CU_DEBUG 
        return editorAssetsPath;
#elif CU_RELEASE
        static fs::path ret = editorPath.parent_path() / "assets";
        return ret;
#endif

    }

    void LocateEditor() {

#ifdef CU_LINUX
        editorPath = Dialogs::OpenFile("Locate Copper-Editor Executable", { "Executable files", "*" }, "~");
#elif CU_WINDOWS
        editorPath = Dialogs::OpenFile("Locate Copper-Editor.exe", { "Executable files", "*.exe" }, "C:\\");
#endif
        if (editorPath.empty()) {

            Dialogs::Error("Invalid File", "Invalid File");
            exit(-1);

        }

    }
#ifdef CU_DEBUG
    void LocateEditorAssets() {

#ifdef CU_LINUX
        editorAssetsPath = Dialogs::OpenFolder("Locate Copper-Editor Assets folder", "~");
#elif CU_WINDOWS
        editorAssetsPath = Dialogs::OpenFolder("Locate Copper-Editor Assets folder", "C:\\");
#endif

        if (editorAssetsPath.empty()) {

            Dialogs::Error("Invalid folder", "Invalid path");
            exit(-1);

        }

        if (fs::exists(editorAssetsPath / "EditorData.cu")) return;

        Dialogs::Error("Invalid folder", "Invalid Editor assets directory.");
        exit(-1);

    }
#endif

}
