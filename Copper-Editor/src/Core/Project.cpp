#include "Project.h"

#include "Engine/Scripting/ScriptingCore.h"

#include <yaml-cpp/yaml.h>

namespace Editor {

	using namespace Copper;

	void Project::Save() {

		YAML::Emitter out;

		out << YAML::BeginMap;

		out << YAML::Key << "Name" << YAML::Value << name;
		out << YAML::Key << "Last Scene" << YAML::Value << lastOpenedScene.string();
		out << YAML::Key << "Gizmo" << YAML::Value << gizmoType;

		out << YAML::EndMap;

		std::ofstream file(path.string() + "\\Project.cu");
		file << out.c_str();

	}
	void Project::Load() {

		YAML::Node main;
		try { main = YAML::LoadFile(path.string() + "/Project.cu"); } catch (YAML::ParserException e) {

			LogError("Failed to Read The Editor Data save file\n    {1}", e.what());
			return;

		}

		name = main["Name"].as<std::string>();
		lastOpenedScene = assetsPath.string() + '\\' + main["Last Scene"].as<std::string>();
		gizmoType = main["Gizmo"].as<int>();

	}

	void Project::CreateSolution() {

		//Copy the visual studio project files
		std::ifstream templateSln("assets/Projects/EmptyTemplate/Template.sln.cut");
		std::fstream projectSln;
		projectSln.open(path.string() + "\\" + name + ".sln", std::ios::out);

		std::string line;
		while (std::getline(templateSln, line)) {

			size_t pos = line.find(':');
			while (pos != std::string::npos && line[pos + 1] == '{') {

				size_t end = line.find_first_of('}', pos);
				std::string var = line.substr(pos + 2, end - (pos + 2));

				if (var == "ProjectName") {

					line.erase(pos, (end - pos) + 1);
					line.insert(pos, name);

				}

				pos = line.find(':', pos + 1);

			}

			projectSln << line << "\n";

		}
		projectSln.close();

		std::ifstream templateCsproj("assets/Projects/EmptyTemplate/Template.csproj.cut");
		std::fstream projectCsproj;
		projectCsproj.open(path.string() + "\\" + name + ".csproj", std::ios::out);

		line = "";
		while (std::getline(templateCsproj, line)) {

			size_t pos = line.find(':');
			while (pos != std::string::npos && line[pos + 1] == '{') {

				size_t end = line.find_first_of('}', pos);
				std::string var = line.substr(pos + 2, end - (pos + 2));

				if (var == "ProjectName") {

					line.erase(pos, (end - pos) + 1);
					line.insert(pos, name);

				}

				pos = line.find(':', pos + 1);

			}

			projectCsproj << line << "\n";

		}
		projectCsproj.close();

	}
	void Project::BuildSolution(bool reloadAssembly) {

		std::string cmd = "C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\MSBuild.exe ";

		size_t pos = path.string().find_first_of(' ');
		std::string newPath = path.string();
		while (pos != std::string::npos) {

			newPath.erase(pos, 1);
			newPath.insert(pos, "\" \"");
			pos = newPath.find_first_of(' ', pos + 3);

		}

		pos = name.find_first_of(' ');
		std::string newName = name;
		while (pos != std::string::npos) {

			newName.erase(pos, 1);
			newName.insert(pos, "\" \"");
			pos = newName.find_first_of(' ', pos + 3);

		}

		cmd += newPath + "\\" + newName + ".csproj";
		cmd += " -nologo";

		system(cmd.c_str());

		if(reloadAssembly) Scripting::Reload();

	}

}