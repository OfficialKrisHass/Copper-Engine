#include "Project.h"

#include "Core/EditorApp.h"

#include "Engine/Scripting/ScriptingCore.h"

#include <yaml-cpp/yaml.h>
#include "Engine/YAMLOverloads/Everything.h"

using namespace Copper;

namespace Editor {

	static void CreateFileAndReplace(const fs::path& original, const fs::path& out, const std::string& what, const std::string& argument);

	Project::Project(const std::string& name, const fs::path& path) : name(name), path(path), assetsPath(path / "Assets") {}

	void Project::Save() const {

		YAML::Emitter out;

		out << YAML::BeginMap; //Main

		//Main Project Stuff
		out << YAML::Key << "Name" << YAML::Value << name;
		out << YAML::Key << "Last Scene" << YAML::Value << lastOpenedScene;

		//Viewport
		out << YAML::Key << "Gizmo" << YAML::Value << gizmoType;

		//Scene Camera
		out << YAML::Key << "Scene Camera" << YAML::Value << YAML::BeginMap; //Scene Camera

		out << YAML::Key << "Position" << YAML::Value << GetSceneCam().GetTransform()->position;
		out << YAML::Key << "Rotation" << YAML::Value << GetSceneCam().GetTransform()->rotation;

		out << YAML::Key << "Speed" << YAML::Value << GetSceneCam().speed;
		out << YAML::Key << "Sensitivity" << YAML::Value << GetSceneCam().sensitivity;

		//End
		out << YAML::EndMap; //Scene Camera
		out << YAML::EndMap; //Main

		std::ofstream file(path / "Project.cu");
		file << out.c_str();

	}
	void Project::Load(const fs::path& path) {

		if(!path.empty()) {

			this->path = path;
			this->assetsPath = path / "Assets";

		}

		YAML::Node main;
		try { main = YAML::LoadFile((path / "Project.cu").string()); } catch (YAML::ParserException e) {

			LogError("Failed to Read The Editor Data save file\n    {1}", e.what());
			return;

		}

		try {

		//Main Project Stuff
		name = main["Name"].as<std::string>();
		lastOpenedScene = assetsPath / main["Last Scene"].as<std::string>();

		//Viewport
		gizmoType = main["Gizmo"].as<int>();

		//Scene Camera
		YAML::Node sceneCamera = main["Scene Camera"];
		GetSceneCam().transform = new Transform(sceneCamera["Position"].as<Vector3>(), sceneCamera["Rotation"].as<Quaternion>(), Vector3::one);

		GetSceneCam().speed = sceneCamera["Speed"].as<float>();
		GetSceneCam().sensitivity = sceneCamera["Sensitivity"].as<float>();

		} catch(YAML::Exception e) {

			LogError("Encountered an exception when loading a project with path {}: {}", path, e.msg);
			return;

			// TODO: Make a Project browser panel to open a project when an invalid project was found

		}

	}

	bool Project::BuildSolution(bool firstBuild) const {

	#ifdef CU_WINDOWS
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
	#else
		const std::string cmd = "make --no-print-directory -C \"" + path.String() + "\" -f Makefile";
		system(cmd.c_str());
	#endif

		if (firstBuild)
			return Scripting::Reload(path / "Binaries" / (name + ".dll"));
		else
			return Scripting::Reload();

	}

	void Project::RegenerateProjectFile() const {

		CreateFileAndReplace("assets/Templates/Project.cu.cut", path / "Project.cu", ":{ProjectName}", name);

	}
	void Project::RegenerateIDEFiles() const {

	#ifdef CU_WINDOWS
		CreateFileAndReplace("assets/Templates/Template.sln.cut", path / (name + ".sln"), ":{ProjectName}", name);
		CreateFileAndReplace("assets/Templates/Template.csproj.cut", path / (name + ".csproj"), ":{ProjectName}", name);
	#elif CU_LINUX
		CreateFileAndReplace("assets/Templates/premake5.lua.cut", path / "premake5.lua", ":{ProjectName}", name);
	#endif

	}

#ifdef CU_LINUX
	void Project::RunPremake() const {

		const std::string p = path.string();

		// It hurts my eyes, but there is no other solution that I know of
		//system(("cd \"" + data.project.path.string() + "\" ; ./premake/premake5 gmake2").c_str());

		// Turns out there is :)
		system(("./util/premake/premake5 --file=\"" + p + "premake5.lua\" gmake2").c_str());

	}
#endif

	static void CreateFileAndReplace(const fs::path& original, const fs::path& out, const std::string& what, const std::string& argument) {

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