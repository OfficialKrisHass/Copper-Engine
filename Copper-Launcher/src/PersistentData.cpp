#include "PersistentData.h"

#include "Dialogs.h"
#include "ProjectEntry.h"

#include <yaml-cpp/yaml.h>

#include <string>
#include <fstream>
#include <filesystem>

namespace Launcher::PersistentData {
  
  static const std::string filename = "LauncherData.cup";
#ifdef CU_WINDOWS
  static const std::string persistenFolder = std::string(getenv("appdata")) + "\\Copper-Editor\\";
#elif CU_LINUX
  static const std::string persistenFolder = std::string("/home/") + cuserid(nullptr)+ "/.config/Copper-Editor/";
#endif

	static std::string editorPath = "";

	static void LocateEditor();

	void Load(std::vector<ProjectEntry>& projectEntries) {

		YAML::Node node;
		try { node = YAML::LoadFile(persistenFolder + filename); }
		catch (YAML::Exception e) {

			Dialogs::Error("Couldn't Read LauncherData.cup", "Encountered an exception trying to Load the LauncherData.cup file.\nProvide the path to the Editor and we will create a new one");
			LocateEditor();

			Save(projectEntries);

			try { node = YAML::LoadFile(persistenFolder + filename); }
			catch (YAML::Exception e) {

				Dialogs::Error("Couldn't Read LauncherData.cup", e.msg);
				exit(-1);

			}

		}

		editorPath = node["Editor Path"].as<std::string>();

    YAML::Node entries = node["Project Entries"];
    for (uint32 i = 0; i < entries.size(); i++) {

      YAML::Node entry = entries[i];
      std::string name = entry["Name"].as<std::string>();
      std::string directory = entry["Directory"].as<std::string>();

      projectEntries.push_back(ProjectEntry(name, directory));

    }

	}
	void Save(const std::vector<ProjectEntry>& projectEntries) {

		YAML::Emitter out;

		out << YAML::BeginMap; // Main

		out << YAML::Key << "Editor Path" << YAML::Value << editorPath;

    out << YAML::Key << "Project Entries" << YAML::Value << YAML::BeginSeq; // Project Entries
    
    for (const ProjectEntry& entry : projectEntries) {

      out << YAML::BeginMap; // Entry     
      
      out << YAML::Key << "Name" << YAML::Value << entry.Name();
      out << YAML::Key << "Directory" << YAML::Value << entry.Directory();

      out << YAML::EndMap; // Entry

    }

    out << YAML::EndSeq;

		out << YAML::EndMap;

		if (!std::filesystem::exists(persistenFolder))
			std::filesystem::create_directories(persistenFolder);

		std::ofstream file(persistenFolder + filename);
		file << out.c_str();
		file.close();

	}

	const std::string& EditorPath() { return editorPath; }

	void LocateEditor() {
    
#ifdef CU_LINUX
    editorPath = Dialogs::OpenFile("Locate Copper-Editor Executable", { "Executable files", "*" }, "~");
#elif CU_WINDOWS
    editorPath = Dialogs::OpenFile("Locate Copper-Editor.exe", { "Executable files", "*.exe" }, "C:\\");
#endif
		if (editorPath == "") {

			Dialogs::Error("Invalid File", "Invalid File");
			exit(-1);

		}

	}

}
