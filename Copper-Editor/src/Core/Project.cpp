#include "Project.h"

#include "Core/EditorApp.h"

#include "Engine/Scripting/ScriptingCore.h"

#include <yaml-cpp/yaml.h>

using namespace Copper;

namespace Editor {

	Project::Project(std::string name, std::filesystem::path path) : name(name), path(path), assetsPath(path.string() + "\\Assets") {

		sceneCam = SceneCamera(GetViewportSize());
		sceneCam.transform = new Transform(Vector3(0.0f, 0.0f, 1.0f), Vector3::zero, Vector3::one);
		sceneCam.transform->position.z = 1.0f;
		sceneCam.transform->parent = nullptr;

	}

	void Project::Save() {

		YAML::Emitter out;

		out << YAML::BeginMap; //Main

		//Main Project Stuff
		out << YAML::Key << "Name" << YAML::Value << name;
		out << YAML::Key << "Last Scene" << YAML::Value << lastOpenedScene.string();

		//Viewport
		out << YAML::Key << "Gizmo" << YAML::Value << gizmoType;

		//Scene Camera
		out << YAML::Key << "Scene Camera" << YAML::Value << YAML::BeginMap; //Scene Camera

		out << YAML::Key << "Position" << YAML::Value << sceneCam.transform->position;
		out << YAML::Key << "Rotation" << YAML::Value << sceneCam.transform->rotation;
		
		out << YAML::Key << "Fov" << YAML::Value << sceneCam.fov;
		out << YAML::Key << "Near Plane" << YAML::Value << sceneCam.nearPlane;
		out << YAML::Key << "Far Plane" << YAML::Value << sceneCam.farPlane;

		out << YAML::Key << "Speed" << YAML::Value << sceneCam.speed;
		out << YAML::Key << "Sensitivity" << YAML::Value << sceneCam.sensitivity;

		//End
		out << YAML::EndMap; //Scene Camera
		out << YAML::EndMap; //Main

		std::ofstream file(path.string() + "\\Project.cu");
		file << out.c_str();

	}
	void Project::Load() {

		YAML::Node main;
		try { main = YAML::LoadFile(path.string() + "/Project.cu"); } catch (YAML::ParserException e) {

			LogError("Failed to Read The Editor Data save file\n    {1}", e.what());
			return;

		}

		//Main Project Stuff
		name = main["Name"].as<std::string>();
		lastOpenedScene = assetsPath.string() + '\\' + main["Last Scene"].as<std::string>();

		//Viewport
		gizmoType = main["Gizmo"].as<int>();

		//Scene Camera
		YAML::Node sceneCamera = main["Scene Camera"];

		sceneCam.transform->position = sceneCamera["Position"].as<Vector3>();
		sceneCam.transform->rotation = sceneCamera["Rotation"].as<Vector3>();

		sceneCam.fov = sceneCamera["Fov"].as<float>();
		sceneCam.nearPlane = sceneCamera["Near Plane"].as<float>();
		sceneCam.farPlane = sceneCamera["Far Plane"].as<float>();

		sceneCam.speed = sceneCamera["Speed"].as<float>();
		sceneCam.sensitivity = sceneCamera["Sensitivity"].as<float>();

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