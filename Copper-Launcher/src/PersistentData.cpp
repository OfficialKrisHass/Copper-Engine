#include "PersistentData.h"

#include "Dialogs.h"

#include <yaml-cpp/yaml.h>

#include <iostream>

#include <string>
#include <fstream>
#include <filesystem>

namespace Launcher::PersistentData {

	static std::string editorPath = "";

	static void LocateEditor();

	void Load() {

		YAML::Node node;
		try { node = YAML::LoadFile(std::string(getenv("appdata")) + "\\Copper-Editor\\LauncherData.cup"); }
		catch (YAML::Exception e) {

			Dialogs::Error("Couldn't Read LauncherData.cup", "Encountered an exception trying to Load the LauncherData.cup file.\nProvide the path to the Editor and we will create a new one");
			LocateEditor();

			Save();

			try { node = YAML::LoadFile(std::string(getenv("appdata")) + "\\Copper-Editor\\LauncherData.cup"); }
			catch (YAML::Exception e) {

				Dialogs::Error("Couldn't Read LauncherData.cup", e.msg);
				exit(-1);

			}

		}

		editorPath = node["Editor Path"].as<std::string>();

	}
	void Save() {

		YAML::Emitter out;

		out << YAML::BeginMap; // Main

		out << YAML::Key << "Editor Path" << YAML::Value << editorPath;

		out << YAML::EndMap;

		std::string path = std::string(getenv("appdata")) + "\\Copper-Editor";
		if (!std::filesystem::exists(path))
			std::filesystem::create_directories(path);

		path += "\\LauncherData.cup";

		std::ofstream file(path.c_str());
		file << out.c_str();
		file.close();

	}

	const std::string& EditorPath() { return editorPath; }

	void LocateEditor() {

		editorPath = Dialogs::OpenFile("Locate Copper-Editor.exe", { "Executable files", "*.exe" }, "C:\\");
		if (editorPath == "") {

			Dialogs::Error("Invalid File", "Invalid File");
			exit(-1);

		}

	}

}