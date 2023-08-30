#include "ProjectTemplate.h"

namespace Editor {

	void CreateFileAndReplace(const Filesystem::Path& original, const Filesystem::Path& out, const std::string& what, const std::string& replace);

	void CopyFileTo(const Filesystem::Path& original, const Filesystem::Path& where, bool binary = false);

	void CreateTemplateFromProject(const Project& project) {

		const Filesystem::Path path = "assets/Templates/" + project.name;

		std::experimental::filesystem::create_directories(path.String() + "/Assets/Scenes");

		CreateFileAndReplace(project.path / "Project.cu", path / "Project.cu.cut", project.name, ":{ProjectName}");

		CopyFileTo(project.assetsPath / project.lastOpenedScene, path / "Assets" / project.lastOpenedScene);
		CopyFileTo(project.assetsPath / (project.lastOpenedScene.String() + ".cum"), path / "Assets" / (project.lastOpenedScene.String() + ".cum"));

	}
	void CreateProjectFromTemplate(const Filesystem::Path& templatePath, Project& project) {

		std::experimental::filesystem::create_directories(project.path.String() + "/Assets/Scenes");
		std::experimental::filesystem::create_directories(project.path.String() + "/Binaries");
		std::experimental::filesystem::create_directories(project.path.String() + "/Objs");

		CreateFileAndReplace(templatePath / "Project.cu.cut", project.path / "Project.cu", ":{ProjectName}", project.name);

	#ifdef CU_WINDOWS
		CreateFileAndReplace("assets/Templates/Template.sln.cut", project.path / (project.name + ".sln"), ":{ProjectName}", project.name);
		CreateFileAndReplace("assets/Templates/Template.csproj.cut", project.path / (project.name + ".csproj"), ":{ProjectName}", project.name);
	#else
		CreateFileAndReplace("assets/Templates/premake5.lua.cut", project.path / "premake5.lua", ":{ProjectName}", project.name);

		// Since we have to use premake for Linux, we also have to copy the premake executable
		std::experimental::filesystem::copy("assets/premake", project.path.String() + "/premake", std::experimental::filesystem::copy_options::recursive);
	#endif

		//Copy the Template Scene
		CopyFileTo(templatePath / "Assets/Scenes/EmptyTemplate.copper", project.assetsPath / "Scenes/EmptyTemplate.copper");
		CopyFileTo(templatePath / "Assets/Scenes/EmptyTemplate.copper.cum", project.assetsPath / "Scenes/EmptyTemplate.copper.cum");

		project.lastOpenedScene = "Scenes/EmptyTemplate.copper";

		CopyFileTo("assets/ScriptAPI/Copper-ScriptingAPI.dll", project.path / "Binaries/Copper-ScriptingAPI.dll", true);

	}
	
	void CreateFileAndReplace(const Filesystem::Path& original, const Filesystem::Path& out, const std::string& what, const std::string& argument) {

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

	void CopyFileTo(const Filesystem::Path& original, const Filesystem::Path& where, bool binary) {
		
		std::ios_base::openmode inFlags = std::ios_base::in;
		std::ios_base::openmode outFlags = std::ios_base::out;
		if(binary) {

			inFlags |= std::ios_base::binary;
			outFlags |= std::ios_base::binary;

		}

		std::ifstream originalFile;
		std::ofstream newFile;

		originalFile.open(original.String(), inFlags);
		newFile.open(where.String(), outFlags);

		newFile << originalFile.rdbuf();

		originalFile.close();
		newFile.close();

	}

}