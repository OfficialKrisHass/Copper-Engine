#include "Project.h"

#include "Core/EditorApp.h"

#include "Engine/Scripting/ScriptingCore.h"

#include <yaml-cpp/yaml.h>
#include "Engine/YAMLOverloads/YAMLOverloads.h"

using namespace Copper;

namespace Editor {

	Project::Project(const std::string& name, const Filesystem::Path& path) : name(name), path(path), assetsPath(path / "Assets") {}

	void Project::Save() {

		YAML::Emitter out;

		out << YAML::BeginMap; //Main

		//Main Project Stuff
		out << YAML::Key << "Name" << YAML::Value << name;
		out << YAML::Key << "Last Scene" << YAML::Value << lastOpenedScene;

		//Viewport
		out << YAML::Key << "Gizmo" << YAML::Value << gizmoType;

		//Scene Camera
		out << YAML::Key << "Scene Camera" << YAML::Value << YAML::BeginMap; //Scene Camera

		out << YAML::Key << "Position" << YAML::Value << sceneCam.GetTransform()->position;
		out << YAML::Key << "Rotation" << YAML::Value << sceneCam.GetTransform()->rotation;
		
		out << YAML::Key << "Fov" << YAML::Value << sceneCam.fov;
		out << YAML::Key << "Near Plane" << YAML::Value << sceneCam.nearPlane;
		out << YAML::Key << "Far Plane" << YAML::Value << sceneCam.farPlane;

		out << YAML::Key << "Speed" << YAML::Value << sceneCam.speed;
		out << YAML::Key << "Sensitivity" << YAML::Value << sceneCam.sensitivity;

		//End
		out << YAML::EndMap; //Scene Camera
		out << YAML::EndMap; //Main

		std::ofstream file(path + "/Project.cu");
		file << out.c_str();

	}
	void Project::Load(const Filesystem::Path& path) {

		sceneCam = SceneCamera(GetViewportSize());

		if(!path.Empty()) {

			this->path = path;
			this->assetsPath = path / "Assets";

		}

		YAML::Node main;
		try { main = YAML::LoadFile(path / "Project.cu"); } catch (YAML::ParserException e) {

			LogError("Failed to Read The Editor Data save file\n    {1}", e.what());
			return;

		}

		//Main Project Stuff
		name = main["Name"].as<std::string>();
		lastOpenedScene = assetsPath / main["Last Scene"].as<std::string>();

		//Viewport
		gizmoType = main["Gizmo"].as<int>();

		//Scene Camera
		YAML::Node sceneCamera = main["Scene Camera"];
		sceneCam.transform = new Transform(sceneCamera["Position"].as<Vector3>(), sceneCamera["Rotation"].as<Quaternion>(), Vector3::one);

		sceneCam.fov = sceneCamera["Fov"].as<float>();
		sceneCam.nearPlane = sceneCamera["Near Plane"].as<float>();
		sceneCam.farPlane = sceneCamera["Far Plane"].as<float>();

		sceneCam.speed = sceneCamera["Speed"].as<float>();
		sceneCam.sensitivity = sceneCamera["Sensitivity"].as<float>();

	}

	void Project::BuildSolution(bool firstBuild) {

		std::string cmd = "C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\MSBuild.exe ";

		size_t pos = path.String().find_first_of(' ');
		std::string newPath = path.String();
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

		if (firstBuild)
			Scripting::Reload(path / "Binaries" / (name + ".dll"));
		else
			Scripting::Reload();

	}

}