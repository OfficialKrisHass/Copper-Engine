#include "ProjectTemplate.h"

namespace Editor {

	void CreateFileAndReplace(const std::filesystem::path& originalPath, const std::filesystem::path& outPath, const std::string& what, const std::string& replace);

	void CopyFileTo(const std::filesystem::path& original, const std::filesystem::path& where, bool binary = false);

	void CreateTemplateFromProject(const Project& project) {

		//Create The Directories
		std::filesystem::create_directories("assets\\Templates\\" + project.name + "\\Assets\\Scenes");
		std::filesystem::create_directories("assets\\Templates\\" + project.name + "\\Binaries");
		std::filesystem::create_directories("assets\\Templates\\" + project.name + "\\Objs");

		//Create the .cu, .sln, .csproj template files
		CreateFileAndReplace(project.path / "Project.cu", "assets\\Templates\\" + project.name + "\\Project.cu.cut", project.name, ":{ProjectName}");
		CreateFileAndReplace(project.path / "DevProject.sln", "assets\\Templates\\" + project.name + "\\Template.sln.cut", project.name, ":{ProjectName}");
		CreateFileAndReplace(project.path / "DevProject.csproj", "assets\\Templates\\" + project.name + "\\Template.csproj.cut", project.name, ":{ProjectName}");

		//Copy the Last opened Scene
		CopyFileTo(project.assetsPath / project.lastOpenedScene, "assets\\Templates\\" + project.name + "\\Assets\\" + project.lastOpenedScene.string());
		CopyFileTo(project.assetsPath / (project.lastOpenedScene.string() + ".cum"), "assets\\Templates\\" + project.name + "\\Assets\\" + project.lastOpenedScene.string() + ".cum");

		//Copy the Scripting API dll
		CopyFileTo("assets\\ScriptAPI\\Copper-ScriptingAPI.dll", "assets\\Templates\\" + project.name + "\\Binaries\\Copper-ScriptingAPI.dll", true);

	}
	void CreateProjectFromTemplate(const std::filesystem::path& templatePath, Project& project) {

		//Create the Directories
		std::filesystem::create_directories(project.path / "Assets\\Scenes");
		std::filesystem::create_directories(project.path / "Binaries");
		std::filesystem::create_directories(project.path / "Objs");

		//Create the Project files from the tempalte
		CreateFileAndReplace(templatePath / "Project.cu.cut", project.path / "Project.cu", ":{ProjectName}", project.name);
		CreateFileAndReplace(templatePath / "Template.sln.cut", project.path / (project.name + ".sln"), ":{ProjectName}", project.name);
		CreateFileAndReplace(templatePath / "Template.csproj.cut", project.path / (project.name + ".csproj"), ":{ProjectName}", project.name);

		//Copy the Template Scene
		CopyFileTo(templatePath / "Assets\\Scenes\\EmptyTemplate.copper", project.assetsPath / "Scenes\\EmptyTemplate.copper");
		CopyFileTo(templatePath / "Assets\\Scenes\\EmptyTemplate.copper.cum", project.assetsPath / "Scenes\\EmptyTemplate.copper.cum");

		project.lastOpenedScene = "Scenes\\EmptyTemplate.copper";

		////Copy the Scripting API dll
		CopyFileTo("assets\\ScriptAPI\\Copper-ScriptingAPI.dll", project.path / "Binaries\\Copper-ScriptingAPI.dll", true);

	}
	
	void CreateFileAndReplace(const std::filesystem::path& originalPath, const std::filesystem::path& outPath, const std::string& what, const std::string& argument) {

		std::ifstream originalFile(originalPath);
		std::ofstream templateFile(outPath);

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

	void CopyFileTo(const std::filesystem::path& original, const std::filesystem::path& where, bool binary) {

		std::ifstream originalFile(original, std::ios::in | binary ? std::ios::binary : 0);
		std::ofstream newFile(where, std::ios::out | binary ? std::ios::binary : 0);

		newFile << originalFile.rdbuf();

		originalFile.close();
		newFile.close();

	}

}